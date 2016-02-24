#include <FS.h>
#include <ArduinoJson.h>
#include <Ticker.h>


/***** Variables *****/
double stdP = 1.6;
double stdI = 2.8;
double stdD = 0.1;
int i = 0;


/***** Objects *****/
StaticJsonBuffer<300> jsonDataBuffer; // 300 bytes JSON buffer for UART communication
JsonObject& jsonSerial = jsonDataBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
JsonArray& Accelerometer1 = jsonSerial.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
JsonArray& Accelerometer2 = jsonSerial.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2
Ticker BrugiTransmitData;



void setup() {
  Serial.begin(115200); // For debugging
  Serial1.begin(250000); // Json data to Brugi
  Serial.println();
  
  //clean FS, for testing
  //SPIFFS.format();

  

  Accelerometer1.add(1); // Accelerometer 1; X-axis
  Accelerometer1.add(2); // Accelerometer 1; Y-axis
  Accelerometer1.add(3); // Accelerometer 1; Z-axis
  Accelerometer2.add(1); // Accelerometer 2; X-axis
  Accelerometer2.add(2); // Accelerometer 2; Y-axis
  Accelerometer2.add(3); // Accelerometer 2; Z-axis

  jsonSerial["Setpoint1"] = 0;
  jsonSerial["Setpoint2"] = 0;
  jsonSerial["P"] = 0;
  jsonSerial["I"] = 0;
  jsonSerial["D"] = 0;

  /*** Init MPU6050 with delay here **/

  // Run jsonSend every 50th millisecond
  BrugiTransmitData.attach_ms(1000, jsonSend); //(milliseconds, method to run);
}



void loop() {
  getPIDFromSPIFFS(); //Only necessary on startup. Run in setup();
  
  //Will be done in blynk
  jsonSerial["Setpoint1"] = i;
  jsonSerial["Setpoint2"] = i+1;
  jsonSerial["P"] = i + 2;
  jsonSerial["I"] = i + 3;
  jsonSerial["D"] = i + 4;


  Accelerometer1[0] = i;
  Accelerometer1[1] = i+1;
  Accelerometer1[2] = i+2;
  Accelerometer2[0] = i+3;
  Accelerometer2[1] = i+4;
  Accelerometer2[2] = i+5;  

  //Only necessary when permanently storing
  //Shold not run in a loop, but as an event
  writePIDtoSPIFFS(); 
  

  jsonSerial.prettyPrintTo(Serial);

  i++;  
  
  delay(5000); 
}



/* This method will send the jsonSerial
 string over Serial1 with 250 kBaud */
void jsonSend()
{
  jsonSerial.printTo(Serial1);
  Serial1.print("\n");
}




