#include "Json_TxRx.h"
#include "Arduino.h"
#include <ArduinoJson.h>

Json_TxRx::Json_TxRx(void)
{
  
}

void Json_TxRx::jsonTx(double t_P, double t_I, double t_D, bool t_autoTuneEnable, int16_t t_positionYaw, int16_t t_positionPitch, int16_t t_setpointYaw, int16_t t_setpointPitch)
{
  StaticJsonBuffer<300> JsonBuffer; // 300 bytes JSON buffer for UART communication
  JsonObject& JsonTx = JsonBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
  JsonArray& Accelerometer1 = JsonTx.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
  JsonArray& Accelerometer2 = JsonTx.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2

  Accelerometer1.add(0); // Accelerometer 1; X-axis
  Accelerometer1.add(1); // Accelerometer 1; Y-axis
  Accelerometer1.add(2); // Accelerometer 1; Z-axis
  Accelerometer2.add(0); // Accelerometer 2; X-axis
  Accelerometer2.add(1); // Accelerometer 2; Y-axis
  Accelerometer2.add(2); // Accelerometer 2; Z-axis

  JsonTx["P"] = t_P;
  JsonTx["I"] = t_I;
  JsonTx["D"] = t_D;
  JsonTx["Autotune button"] = t_autoTuneEnable;
  JsonTx["Position Yaw"] = t_positionYaw;
  JsonTx["Position Pitch"] = t_positionPitch;
  JsonTx["Setpoint Yaw"] = t_setpointYaw;
  JsonTx["Setpoint Pitch"] = t_setpointPitch;
  
  JsonTx.printTo(Serial);
  Serial.print("\n");
  
}


/* This method parses the jsonReceived string if the string is completely received
   It stores the parsed string in separate local variables */
void Json_TxRx::parseRx()
{
  if(rxComplete == true)
  {
    StaticJsonBuffer<300> JsonBuffer;
    JsonObject& JsonRx = JsonBuffer.parseObject(jsonReceived);

    if (JsonRx.success()) // Parsing succeeded
    {
       intervalYaw = JsonRx["Interval Yaw"];
       intervalPitch = JsonRx["Interval Pitch"];
       P = JsonRx["P"];
       I = JsonRx["I"];
       D = JsonRx["D"];
       JsonRx["Autotune button"] = autoTuneEnable;
       Acc1_AxisX = JsonRx["Accelerometer1"][0];
       Acc1_AxisY = JsonRx["Accelerometer1"][1];
       Acc1_AxisZ = JsonRx["Accelerometer1"][2];
       Acc2_AxisX = JsonRx["Accelerometer2"][0];
       Acc2_AxisY = JsonRx["Accelerometer2"][1];
       Acc2_AxisZ = JsonRx["Accelerometer2"][2];
    }
    
    jsonReceived = ""; // Empty received string
    rxComplete = false;
  }
}




/* This method receives the separate characters from the serial port
   and stores it to the jsonReceived string */
void Json_TxRx::serialRxEvent()
{
  while(Serial.available())
  {
    char inChar = (char)Serial.read();
    jsonReceived += inChar;
    if (inChar == '\n') // If received newline
      rxComplete = true; 
  }
}

