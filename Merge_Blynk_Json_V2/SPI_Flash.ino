/* 
 This method will get the values of

 jsonSPIFFS["SetpointYaw"]
 jsonSPIFFS["SetpointPitch"]
 jsonSPIFFS["P_yaw"]
 jsonSPIFFS["I_yaw"]
 jsonSPIFFS["D_yaw"]
 jsonSPIFFS["P_pitch"]
 jsonSPIFFS["I_pitch"]
 jsonSPIFFS["D_pitch"]

 in the Brugi_data.json that's stored in the SPIFFS, and
 save it to the global json namespaces
 
 jsonSerial["SetpointYaw"]
 jsonSerial["SetpointPitch"]
 jsonSerial["P_yaw"]
 jsonSerial["I_yaw"]
 jsonSerial["D_yaw"]
 jsonSerial["P_pitch"]
 jsonSerial["I_pitch"]
 jsonSerial["D_pitch"]
*/
void getDataFromSPIFFS()
{  
  //read configuration from FS json
  /***** Start SPIFFS and read data from Json *****/
  if (SPIFFS.begin()) 
  {
    /***** IF Brugi_data.json exist *****/
    if (SPIFFS.exists("/Brugi_data.json")) 
    {
      //file exists, reading and loading
      Serial.println("reading Brugi data file from SPIFFS:");
      File dataFile = SPIFFS.open("/Brugi_data.json", "r");
      if (dataFile) 
      {
        size_t size = dataFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        dataFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& jsonSPIFFS = jsonBuffer.parseObject(buf.get());
        if (jsonSPIFFS.success()) 
        {
          Serial.println("Storing values to jsonSerial");

          /***** Copy the Json values to the global variables *****/
          jsonSerial["SetpointYaw"] = jsonSPIFFS["SetpointYaw"];
          jsonSerial["SetpointPitch"] = jsonSPIFFS["SetpointPitch"];
          jsonSerial["P_yaw"] = jsonSPIFFS["P_yaw"];
          jsonSerial["I_yaw"] = jsonSPIFFS["I_yaw"];
          jsonSerial["D_yaw"] = jsonSPIFFS["D_yaw"];
          jsonSerial["P_pitch"] = jsonSPIFFS["P_pitch"];
          jsonSerial["I_pitch"] = jsonSPIFFS["I_pitch"];
          jsonSerial["D_pitch"] = jsonSPIFFS["D_pitch"];
        } 
        else 
          Serial.println("failed to load json config");
      }
    } 

    /***** Else if Brugi_data.json doesn't exist *****/
    else 
    {  Serial.println("Brugi_data.json not found. Creating file");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& jsonSPIFFS = jsonBuffer.createObject();
      jsonSPIFFS["SetpointYaw"] = 0;
      jsonSPIFFS["SetpointPitch"] = 0;
      jsonSPIFFS["P_yaw"] = 1.6;
      jsonSPIFFS["I_yaw"] = 2.8;
      jsonSPIFFS["D_yaw"] = 0.1;
      jsonSPIFFS["P_pitch"] = 1.6;
      jsonSPIFFS["I_pitch"] = 2.8;
      jsonSPIFFS["D_pitch"] = 0.1;

  
      File dataFile = SPIFFS.open("/Brugi_data.json", "w");
      if (!dataFile) {
        Serial.println(F("failed to open Brugi_data.json file for writing"));
      }
  
      //This saves content into the Json file
      jsonSPIFFS.printTo(dataFile);
      dataFile.close();
      Serial.println("Saving finished! Saved values:");
      jsonSPIFFS.printTo(Serial);
      Serial.print("\n");
    }
  }
}




/* 
 This method will store the values of
 
 jsonSerial["SetpointYaw"]
 jsonSerial["SetpointPitch"]
 jsonSerial["P_yaw"]
 jsonSerial["I_yaw"]
 jsonSerial["D_yaw"]
 jsonSerial["P_pitch"]
 jsonSerial["I_pitch"]
 jsonSerial["D_pitch"]

 Into Brugi_data.json that's located in the SPIFFS 
*/
void writeDatatoSPIFFS()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& jsonSPIFFS = jsonBuffer.createObject();
  
  jsonSPIFFS["SetpointYaw"] = jsonSerial["SetpointYaw"]; 
  jsonSPIFFS["SetpointPitch"] = jsonSerial["SetpointPitch"];
  jsonSPIFFS["P_yaw"] = jsonSerial["P_yaw"];
  jsonSPIFFS["I_yaw"] = jsonSerial["I_yaw"];
  jsonSPIFFS["D_yaw"] = jsonSerial["D_yaw"];
  jsonSPIFFS["P_pitch"] = jsonSerial["P_pitch"];
  jsonSPIFFS["I_pitch"] = jsonSerial["I_pitch"];
  jsonSPIFFS["D_pitch"] = jsonSerial["D_pitch"];

  Serial.println("Ready to write to Brugi_data.json");
  File dataFile = SPIFFS.open("/Brugi_data.json", "w");
  if (!dataFile) {
    Serial.println(F("failed to open Brugi_data.json file for writing"));
  }

  //This saves content into the Json file
  jsonSPIFFS.printTo(dataFile);
  //json.printTo(Serial);
  Serial.println("Data write finished!");
  dataFile.close();
}
