// Needed for Wifi and access point
#include <FS.h>                   
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          
#include <ArduinoJson.h>   

//define your default values here, if there are different values in config.json, they are overwritten.
char token[32];


//flag for saving data
bool shouldSaveConfig = false;

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
  //here  "AutoConnectAP"
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

    // copy Json content to 'token' and 'storedValue'
    strcpy(token, json["token"]);

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
