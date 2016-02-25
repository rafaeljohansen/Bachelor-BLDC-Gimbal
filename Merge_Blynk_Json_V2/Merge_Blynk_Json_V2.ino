/***** Includements *****/
#include <FS.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          
#include <DNSServer.h>
#include <Wire.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

/***** Definitions *****/
#define INT32_MAX 0x7fffffffL
#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)
#define INT64_MAX 0x7fffffffffffffffLL
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

/***** Variables *****/
// Access point
char token[32]; // Blynk token will be stored here
bool shouldSaveConfig = false; // Flag for saving data in the web page text fields

/* Moved to JsonData.h
double P_yaw;
double I_yaw;
double D_yaw;
double P_pitch;
double I_pitch;
double D_pitch;
int32_t setpointYaw;
int32_t setpointPitch;
*/


struct JsonData
{
  int16_t setpoint;
  double P;
  double I;
  double D;
};

int x;
int y;
int graph;
bool autoTuneEnable;
String jsonReceived = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int jsonP;
int jsonI;
int jsonD;


/***** Objects *****/
MPU6050 mpu1(0x68);
MPU6050 mpu2(0x69);
StaticJsonBuffer<300> jsonDataBuffer; // 300 bytes JSON buffer for UART communication
JsonObject& jsonSerial = jsonDataBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
JsonArray& Accelerometer1 = jsonSerial.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
JsonArray& Accelerometer2 = jsonSerial.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2

Ticker TickerMPU1;
Ticker TickerMPU2;

Ticker TransmitData;
Ticker ReceiveData;
Ticker TickerStoreToSPIFFS;

Ticker Task;

JsonData Yaw;
JsonData Pitch;


/***** Blynk Connection *****/
//char auth[] = "b7b58db178cc41f3a16337431b8b3e15"; //Blynk key
//For connection to WLAN//
//const char* ssid = "Rafael's iPhone";
//const char* password = "hb2xodz5i0ed";



void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}



void setup() {
  Serial.begin(115200); // For debugging
  Serial1.begin(250000); // Json data to Brugi

  Serial.println("Initializing MPUs");
  mpuInit(mpu1);
  mpuInit(mpu2);

  //clean FS, for testing
  //SPIFFS.format();

/***** Start Wifi accesspoint initializing *****/
  Serial.println("mounting SPIFFS...");
  
  if (SPIFFS.begin()) 
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) 
    {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) 
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) 
        {
          Serial.println("\nParsed json");

          /****** Copy the Json values to the global variables ******/
          strcpy(token, json["token"]);
        } 
        else 
          Serial.println("failed to load json config");
      }
    }
  } 
  else 
    Serial.println("failed to mount SPIFFS");
  //end read

  /** The data that goes in the custom text fields goes here. 
  The parameters are id/name, placeholder/prompt, default length **/
  WiFiManagerParameter custom_token("token", "token", token, 32);

  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //Set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  //Set static IP
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // Add token text field on the webpage
  wifiManager.addParameter(&custom_token);

  /* Fetches SSID and password and tries to connect if it
  does not connect it starts an access point with the 
  specified name ("ESP8266") and goes into a blocking loop
  awaiting configuration */
  if (!wifiManager.autoConnect("ESP8266")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //Store data to config.json
  if (shouldSaveConfig) 
  {
    Serial.println("saving config");
    
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["token"] = token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println(F("failed to open config file for writing"));
    }

    //This saves content into the Json file
    json.printTo(configFile);
    configFile.close();
    Serial.println("Blynk token stored!");
  }
  
  //if you get here you have connected to the WiFi
  Serial.println("connected to Wifi!");

  //Read updated values
  strcpy(token, custom_token.getValue());

  Serial.print("Stored token: ");
  Serial.println(token);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
/***** End Wifi accesspoint initializing *****/


  //Add two arrays in the jsonSerial string
  //One for each accelerometer
  Accelerometer1.add(0); // Accelerometer 1; X-axis
  Accelerometer1.add(1); // Accelerometer 1; Y-axis
  Accelerometer1.add(2); // Accelerometer 1; Z-axis
  Accelerometer2.add(0); // Accelerometer 2; X-axis
  Accelerometer2.add(1); // Accelerometer 2; Y-axis
  Accelerometer2.add(2); // Accelerometer 2; Z-axis

  
/*Get data from SPIFFS and store them to the jsonSerial object 
  This will also create these current Json namespaces:
  jsonSerial["SetpointYaw"]
  jsonSerial["SetpointPitch"]
  jsonSerial["P_yaw"]
  jsonSerial["I_yaw"]
  jsonSerial["D_yaw"]
  jsonSerial["P_pitch"]
  jsonSerial["I_pitch"]
  jsonSerial["D_pitch"]
*/  
  getDataFromSPIFFS();


  //Delay needed in order to let the MPU6050 stabilize
  Serial.println("10 sec delay for MPU6050");
  delay(10000);
    

  //Connect to blynk
  Blynk.begin(token, WiFi.SSID().c_str(), WiFi.psk().c_str());  // (token, ssid, password);


  /***** Initialize tickers *****/
  //Update accelerometers
  TickerMPU1.attach_ms(20, updateAcc1); //(milliseconds, method)
  TickerMPU2.attach_ms(20, updateAcc2);

  //Json related
  TransmitData.attach_ms(20, jsonSerialSend); //(milliseconds, method to run);
  ReceiveData.attach_ms(100, receivedSerialData);
  TickerStoreToSPIFFS.attach(120, writeDatatoSPIFFS); //(seconds, method)
  

  
  //Run task every 1000 millisecond
  Task.attach_ms(200, task);
  
}


void loop() 
{
  Blynk.run();
  serialEvent();
}


/* This method will send the jsonSerial
 string over Serial1 with 250 kBaud
 The rest of the string will be built
 in the [blynk method]
 */
void jsonSerialSend()
{
  
  
  //Set PID values here
  jsonSerial["SetpointYaw"] = Yaw.setpoint; 
  jsonSerial["SetpointPitch"] = Pitch.setpoint;
  jsonSerial["P_yaw"] = Yaw.P;
  jsonSerial["I_yaw"] = Yaw.I;
  jsonSerial["D_yaw"] = Yaw.D;
  jsonSerial["P_pitch"] = Pitch.P;
  jsonSerial["I_pitch"] = Pitch.I;
  jsonSerial["D_pitch"] = Pitch.D;
  Accelerometer1[0] = mpu1.yaw;
  Accelerometer1[1] = mpu1.pitch;
  Accelerometer1[2] = mpu1.roll;
  Accelerometer2[0] = mpu2.yaw;
  Accelerometer2[1] = mpu2.pitch;
  Accelerometer2[2] = mpu2.roll;
  jsonSerial.prettyPrintTo(Serial);
  jsonSerial.printTo(Serial1);
  Serial1.print("\n");
}


/***** Blynk *****/
void task()
{
 /* Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(P);
  Serial.print("\t");
  Serial.print(I);
  Serial.print("\t");
  Serial.print(D);
  Serial.print("\t");
  Serial.println(autoTuneEnable); */

/**** X-axis joystick ****/
  if(x > 50) // x more than 50
      {
        intervalYaw = map(x, 100, 51,10000,40000);
      }
      
      else if(x < 50) // x less than 50
      {

        intervalYaw = -map(x, 0, 49,10000,40000);
      }

      else // x equals 50
      {
        intervalYaw = UINT32_MAX; // Freeze rotation around z-axis
      }
/*************************/

/**** Y-axis joystick ****/
  if(y > 50) // y more than 50
      {
        intervalPitch = map(y, 100, 51,10000,40000);
      }
      
      else if(y < 50) // y less than 50
      {
        intervalPitch = map(y, 0, 49,10000,40000);
      }

      else // y equals 50
      {
        intervalPitch = UINT32_MAX; // Freeze rotation around y-axis
      }

/* On behalf of the joystick, CWintervalBlynk and CCWintervalBlynk gives a value 
on how much the setpoint on the brugi-board shall change in both X- and Y-axis. */


}


void receivedSerialData()
{
    //If the received string is complete:
  if (stringComplete) { 
    
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(jsonReceived);

    //parsing OK?
    if (!root.success()) 
      Serial.println("parseObject() failed");

    //Store data to variables
    jsonP = root["P"];
    jsonI = root["I"];
    jsonD = root["D"];
    autoTuneEnable = root["Autotune"];

    //Print the JSON string and all parsed data
    Serial.println("Received JSON string:");
    Serial.println(jsonReceived);
    Serial.print("P: ");
    Serial.println(jsonP);
    Serial.print("I: ");
    Serial.println(jsonI);
    Serial.print("D: ");
    Serial.println(jsonD);
    Serial.print("autotune button: ");
    Serial.println(autoTuneEnable);
    Serial.println("Done!\n");
    
    // clear the string
    jsonReceived = "";
    stringComplete = false;
  }
}

/*
 SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() 
{
  while (Serial1.available()) 
  {
    char inChar = (char)Serial1.read();
    jsonReceived += inChar;

    if (inChar == '\n') // If received newline
      stringComplete = true;
  }
}

/******** Blynk - Read changes on virtual pins ********/
BLYNK_WRITE(V0)
{
  Yaw.P = param.asInt(); //Reads from slider for Propotional gain
}

BLYNK_WRITE(V1)
{
  Yaw.I = param.asInt(); //Reads from slider for Integral gain
}

BLYNK_WRITE(V2)
{
  Yaw.D = param.asInt(); //Reads from slider for Derivative gain
}

BLYNK_WRITE(V3)
{
  Pitch.P = param.asInt(); //Reads from slider for Propotional gain
}

BLYNK_WRITE(V4)
{
  Pitch.I = param.asInt(); //Reads from slider for Integral gain
}

BLYNK_WRITE(V5)
{
  Pitch.D = param.asInt(); //Reads from slider for Derivative gain
}

BLYNK_WRITE(V6)
{
  x = param.asInt(); //Reads from joystick horisontal value 
}

BLYNK_WRITE(V7)
{
  y = param.asInt(); //Reads from joystick vertical value
}

BLYNK_WRITE(V8)
{
  autoTuneEnable = param.asInt(); //Reads from button for auto-tune
}
/******************************************************/

/******** Blynk - Writes changes on virtual pins ********/
BLYNK_READ(V0)
{
  Blynk.virtualWrite(V0, Yaw.P); //writes to P-slider
}

BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, Yaw.I); //writes to I-slider
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, Yaw.D); //writes to D-slider
}

BLYNK_READ(V3)
{
  Blynk.virtualWrite(V0, Pitch.P); //writes to P-slider
}

BLYNK_READ(V4)
{
  Blynk.virtualWrite(V1, Pitch.I); //writes to I-slider
}

BLYNK_READ(V5)
{
  Blynk.virtualWrite(V2, Pitch.D); //writes to D-slider
}

BLYNK_READ(V8)
{
  Blynk.virtualWrite(V5, autoTuneEnable); //writes to auto tune button
}
/********************************************************/


