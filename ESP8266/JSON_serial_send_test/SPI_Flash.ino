/* 
 This method will get the values of
 
 json["Setpoint"]
 json["P"]
 json["I"]
 json["D"]

 in the PID.json that's located in the SPIFFS, and
 save it to the global json namespaces
 
 jsonSerial["Setpoint"]
 jsonSerial["P"]
 jsonSerial["I"]
 jsonSerial["D"] 
*/
void getPIDFromSPIFFS()
{  
  //read configuration from FS json
  /***** Start SPIFFS and read data from Json *****/
  if (SPIFFS.begin()) 
  {
    /***** IF PID.json exist *****/
    if (SPIFFS.exists("/PID.json")) 
    {
      //file exists, reading and loading
      Serial.println("reading PID file from SPIFFS:");
      File configFile = SPIFFS.open("/PID.json", "r");
      if (configFile) 
      {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (json.success()) 
        {
          Serial.println("Storing values to jsonSerial");

          /***** Copy the Json values to the global variables *****/
          jsonSerial["Setpoint"] = json["Setpoint"];
          jsonSerial["P"] = json["P"];
          jsonSerial["I"] = json["I"];
          jsonSerial["D"] = json["D"];
        } 
        else 
          Serial.println("failed to load json config");
      }
    } 

    /***** Else if PID.json doesn't exist *****/
    else 
    {  Serial.println("PID.json not found. Creating file");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      //Stock PID values
      json["Setpoint"] = 0;
      json["P"] = stdP;
      json["I"] = stdI;
      json["D"] = stdD;
  
      File PIDFile = SPIFFS.open("/PID.json", "w");
      if (!PIDFile) {
        Serial.println(F("failed to open PID.json file for writing"));
      }
  
      //This saves content into the Json file
      json.printTo(PIDFile);
      PIDFile.close();
      Serial.println("Saving finished! Saved PID values:");
      json.printTo(Serial);
      Serial.print("\n");
    }
  }
}




/* 
 This method will store the values of
 
 jsonSerial["Setpoint"]
 jsonSerial["P"]
 jsonSerial["I"]
 jsonSerial["D"]

 Into PID.json that's located in the SPIFFS 
*/
void writePIDtoSPIFFS()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  
  json["Setpoint"] = jsonSerial["Setpoint"];  
  json["P"] = jsonSerial["P"];
  json["I"] = jsonSerial["I"];
  json["D"] = jsonSerial["D"];

  Serial.println("Ready to write to PID.json");
  File PIDFile = SPIFFS.open("/PID.json", "w");
  if (!PIDFile) {
    Serial.println(F("failed to open PID file for writing"));
  }

  //This saves content into the Json file
  json.printTo(PIDFile);
  //json.printTo(Serial);
  Serial.println("PID write finished!");
  PIDFile.close();
}
