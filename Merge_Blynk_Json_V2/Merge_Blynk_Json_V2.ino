/***** Includements *****/
#include <FS.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

/***** Definitions *****/
#define INT32_MAX 0x7fffffffL
#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)
#define INT64_MAX 0x7fffffffffffffffLL
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

/***** Variables *****/
double stdP = 1.6;
double stdI = 2.8;
double stdD = 0.1;
int i = 0;
int P;
int I;
int D;
int x;
int y;
int graph;
bool autoTuneEnable;
int32_t intervalYaw;
int32_t intervalPitch;
String jsonReceived = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int jsonP;
int jsonI;
int jsonD;


/***** Objects *****/
StaticJsonBuffer<300> jsonDataBuffer; // 300 bytes JSON buffer for UART communication
JsonObject& jsonSerial = jsonDataBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
//JsonArray& Accelerometer1 = jsonSerial.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
//JsonArray& Accelerometer2 = jsonSerial.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2
Ticker BrugiTransmitData;
Ticker Task;
Ticker ReceivedData;

/***** Blynk Connection *****/
char auth[] = "b7b58db178cc41f3a16337431b8b3e15"; //Blynk key
//For connection to WLAN//
const char* ssid = "Rafael's iPhone";
const char* password = "hb2xodz5i0ed";


void setup() {
  Serial.begin(115200); // For debugging
  Serial1.begin(250000); // Json data to Brugi
  Serial.println();
  
  Blynk.begin(auth, ssid, password); //Connecting to blynk
  
  //clean FS, for testing
  //SPIFFS.format();

  jsonSerial["CWintervalBlynkX"] = 0;
  jsonSerial["CCWintervalBlynkX"] = 0;
  jsonSerial["CWintervalBlynkY"] = 0;
  jsonSerial["CCWintervalBlynkY"] = 0;
  jsonSerial["P"] = 0;
  jsonSerial["I"] = 0;
  jsonSerial["D"] = 0;
  jsonSerial["autoTuneBtn"] = 0;

  /*** Init MPU6050 with delay here **/

  //Run jsonSend every 50th millisecond
  BrugiTransmitData.attach_ms(1000, jsonSend); //(milliseconds, method to run);
  //Run task every 1000 millisecond
  Task.attach_ms(200, task);
  ReceivedData.attach_ms(200, receivedData);
}


void loop() 
{
  Blynk.run();
}


/* This method will send the jsonSerial
 string over Serial1 with 250 kBaud */
void jsonSend()
{
  jsonSerial.printTo(Serial1);
  Serial1.print("\n");
}


/***** Blynk *****/
void task()
{
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(P);
  Serial.print("\t");
  Serial.print(I);
  Serial.print("\t");
  Serial.print(D);
  Serial.print("\t");
  Serial.println(autoTuneEnable);

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
/*************************/
/* On behalf of the joystick, CWintervalBlynk and CCWintervalBlynk gives a value 
on how much the setpoint on the brugi-board shall change in both X- and Y-axis. */

 /***** Json *****/
 //getPIDFromSPIFFS(); //Only necessary on startup
  
  //Will be done in blynk
  jsonSerial["Interval Yaw"] = intervalYaw;
  jsonSerial["Interval Pitch"] = intervalPitch;
  jsonSerial["P"] = P;
  jsonSerial["I"] = I;
  jsonSerial["D"] = D;
  jsonSerial["autoTuneEnable"] = autoTuneEnable;

  //Only necessary when permanently storing
  //Shold not run in a loop
  //writePIDtoSPIFFS(); 
  

  jsonSerial.prettyPrintTo(Serial);

  i++;  
  /****************/
}
/*****************/

void receivedData()
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
    graph = root["graph"];
    autoTuneEnable = root["autoTuneBtn"];

    //Print the JSON string and all parsed data
    Serial.println("Received JSON string:");
    Serial.println(jsonReceived);
    Serial.print("P: ");
    Serial.println(jsonP);
    Serial.print("I: ");
    Serial.println(jsonI);
    Serial.print("D: ");
    Serial.println(jsonD);
    Serial.print("graph: ");
    Serial.println(graph);
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
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read();
    jsonReceived += inChar;

    if (inChar == '\n') // If received newline
      stringComplete = true;
  }
}

/******** Blynk - Read changes on virtual pins ********/
BLYNK_WRITE(V0)
{
  P = param.asInt(); //Reads from slider for Propotional gain
}

BLYNK_WRITE(V1)
{
  I = param.asInt(); //Reads from slider for Integral gain
}

BLYNK_WRITE(V2)
{
  D = param.asInt(); //Reads from slider for Derivative gain
}

BLYNK_WRITE(V3)
{
  x = param.asInt(); //Reads from joystick horisontal value 
}

BLYNK_WRITE(V4)
{
  y = param.asInt(); //Reads from joystick vertical value
}

BLYNK_WRITE(V5)
{
  autoTuneEnable = param.asInt(); //Reads from button for auto-tune
}
/******************************************************/

/******** Blynk - Writes changes on virtual pins ********/
BLYNK_READ(V0)
{
  Blynk.virtualWrite(V0, P); //writes to P-slider
}

BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1, I); //writes to I-slider
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, P); //writes to D-slider
}

BLYNK_READ(V5)
{
  Blynk.virtualWrite(V5, autoTuneEnable); //writes to auto tune button
}

BLYNK_READ(V6)
{
  Blynk.virtualWrite(V6, graph); //writes to auto tune button
}
/********************************************************/


