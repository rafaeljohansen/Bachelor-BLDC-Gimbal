/* This sketch receives a JSON string
  over Serial0, stores the string, parsing it
  and print the parsed values on the serial monitor
 */

#include <ArduinoJson.h>


String jsonReceived = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int jsonSetpoint1;
int jsonSetpoint2;
int jsonP;
int jsonI;
int jsonD;

int Acc1Axis0;
int Acc1Axis1;
int Acc1Axis2;
int Acc2Axis0;
int Acc2Axis1;
int Acc2Axis2;




void setup() {
  // initialize serial:
  Serial.begin(250000);
  // reserve 300 bytes for the inputString:
  jsonReceived.reserve(300);
  Serial.println("Started!");
}


void loop() 
{  

  //If the received string is complete:
  if (stringComplete) { 
    
    StaticJsonBuffer<300> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(jsonReceived);

    //parsing OK?
    if (!root.success()) 
      Serial.println("parseObject() failed");
      
    

    //Store data to variables
    jsonSetpoint1 = root["Setpoint1"];
    jsonSetpoint2 = root["Setpoint2"];
    jsonP = root["P"];
    jsonI = root["I"];
    jsonD = root["D"];
    Acc1Axis0 = root["Accelerometer1"][0];
    Acc1Axis1 = root["Accelerometer1"][1];
    Acc1Axis2 = root["Accelerometer1"][2];
    Acc2Axis0 = root["Accelerometer2"][0];
    Acc2Axis1 = root["Accelerometer2"][1];
    Acc2Axis2 = root["Accelerometer2"][2];

    //Print the JSON string and all parsed data
    Serial.println("Received JSON string:");
    Serial.println(jsonReceived);
    Serial.println("Parsed data:");
    Serial.print("Setpoint 1: ");
    Serial.println(jsonSetpoint1);
    Serial.print("Setpoint 2: ");
    Serial.println(jsonSetpoint2);
    Serial.print("P: ");
    Serial.println(jsonP);
    Serial.print("I: ");
    Serial.println(jsonI);
    Serial.print("D: ");
    Serial.println(jsonD);
    Serial.print("Acc1Axis0: ");
    Serial.println(Acc1Axis0);
    Serial.print("Acc1Axis1: ");
    Serial.println(Acc1Axis1);
    Serial.print("Acc1Axis2: ");
    Serial.println(Acc1Axis2);
    Serial.print("Acc2Axis0: ");
    Serial.println(Acc2Axis0);
    Serial.print("Acc2Axis1: ");
    Serial.println(Acc2Axis1);
    Serial.print("Acc2Axis2: ");
    Serial.println(Acc2Axis2);
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
