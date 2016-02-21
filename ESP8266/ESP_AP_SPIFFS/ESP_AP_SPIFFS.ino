
/***** Included libraries *****/
#include <FS.h>                   
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          
#include <ArduinoJson.h>
//#include MPU6050.h
//#include blynk.h


/***** Pin definitions *****/



/***** Variables *****/
char token[32]; // Blynk token will be stored here
bool shouldSaveConfig = false; // Flag for saving data in the web page text fields



/***** Objects *****/
StaticJsonBuffer<300> jsonDataBuffer; // 300 bytes JSON buffer for UART communication
JsonObject& jsonSerial = jsonDataBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
JsonArray& Accelerometer1 = jsonSerial.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
JsonArray& Accelerometer2 = jsonSerial.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2



/***** JSON string build *****/
//Accelerometer1.add(0); // Accelerometer 1; X-axis
//Accelerometer1.add(0); // Accelerometer 1; Y-axis
//Accelerometer1.add(0); // Accelerometer 1; Z-axis
//Accelerometer2.add(0); // Accelerometer 2; X-axis
//Accelerometer2.add(0); // Accelerometer 2; Y-axis
//Accelerometer2.add(0); // Accelerometer 2; Z-axis
//jsonSerial["Setpoint"] = 0;
//jsonSerial["P"] = 0;
//jsonSerial["I"] = 0;
//jsonSerial["D"] = 0;





//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}


void setup() {
  Serial.begin(115200);
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

/***** Start SPIFFS and read data from Json *****/
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
    Serial.println("failed to mount FS");
  //end read

  
  /** The data that goes in the custom text fields goes here. 
  The parameters are id/name, placeholder/prompt, default length **/
  WiFiManagerParameter custom_token("token", "token", token, 32);
 

  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  /****** Custom fields in the log on window goes here! ******/
  wifiManager.addParameter(&custom_token);

  //Clears all wifi settings
  //wifiManager.resetSettings();
 

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "ESP8266"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("ESP8266")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }


  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");



  /****** Read updated values ******/
  strcpy(token, custom_token.getValue());

  /****** Save the custom parameters to SPIFFS file system ******/
  if (shouldSaveConfig) {
    Serial.println("saving config");
    
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["token"] = token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println(F("failed to open config file for writing"));
    }

    //json["token"].printTo(Serial); --> will print "storedValue" (with quotes)
    //json["storedValue"].printTo(Serial); 


    //This saves content into the Json file
    json.printTo(configFile);
    configFile.close();
    Serial.println("Blynk token stored!");
  }


  Serial.print("Stored token: ");
  Serial.print(token);
  
  Serial.print("\nLocal IP: ");
  Serial.println(WiFi.localIP());

  //Rest of the setup goes below:

  

  

}

void loop() {
  // put your main code here, to run repeatedly:
  

}



