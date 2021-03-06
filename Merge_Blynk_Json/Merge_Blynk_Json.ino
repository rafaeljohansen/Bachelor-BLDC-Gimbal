#include <FS.h>
#include <ArduinoJson.h>
#include <Ticker.h>

/***** Definitions *****/
#define INT32_MAX 0x7fffffffL
#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)
#define INT64_MAX 0x7fffffffffffffffLL
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)

/***** Variables *****/
double stdP = 1.6;
double stdI = 2.8;
double stdD = 0.1;
int i = 0;

/***** Objects *****/
StaticJsonBuffer<300> jsonDataBuffer; // 300 bytes JSON buffer for UART communication
JsonObject& jsonSerial = jsonDataBuffer.createObject(); // A reference to the JsonObject, the actual bytes are inside the JsonDataBuffer
//JsonArray& Accelerometer1 = jsonSerial.createNestedArray("Accelerometer1"); // Object to create an array for Accelerometer 1
//JsonArray& Accelerometer2 = jsonSerial.createNestedArray("Accelerometer2"); // Object to create an array for Accelerometer 2
Ticker BrugiTransmitData;

/***** Blynk *****/
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
#include <FS.h>
char auth[] = "b7b58db178cc41f3a16337431b8b3e15"; //Blynk key

//For connection to WLAN//
const char* ssid = "Rafael's iPhone";
const char* password = "hb2xodz5i0ed";

#include "Timer.h" //For replacement of delay
Timer t;

int P;
int I;
int D;
int x;
int y;
bool autoTuneBtn;

uint32_t CWintervalBlynkX;
uint32_t CCWintervalBlynkX;
uint32_t CWintervalBlynkY;
uint32_t CCWintervalBlynkY;
/*****************/

void setup() {
  Serial.begin(115200); // For debugging
  Serial1.begin(250000); // Json data to Brugi
  Serial.println();
  
  /***** Blynk *****/
  Blynk.begin(auth, ssid, password); //Connecting to blynk
  t.every(200, task, 0); //every 1000 ms do task
  /*****************/
  
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

  // Run jsonSend every 50th millisecond
  BrugiTransmitData.attach_ms(1000, jsonSend); //(milliseconds, method to run);
}



void loop() 
{
  /***** Blynk *****/
  Blynk.run();
  t.update();
  /*****************/
}


/* This method will send the jsonSerial
 string over Serial1 with 250 kBaud */
void jsonSend()
{
  jsonSerial.printTo(Serial1);
  Serial1.print("\n");
}

/***** Blynk *****/
void task(void* val) //For replacement of delay
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
  Serial.println(autoTuneBtn);

/**** X-axis joystick ****/
  if(x > 50) // x more than 50
      {
        CWintervalBlynkX = map(x, 100, 51,10000,40000);
        CCWintervalBlynkX = UINT32_MAX;
      }
      
      else if(x < 50) // x less than 50
      {
        CWintervalBlynkX = UINT32_MAX;
        CCWintervalBlynkX = map(x, 0, 49,10000,40000);
      }

      else // x equals 50
      {
        CWintervalBlynkX = UINT32_MAX; // Freeze clock wise
        CCWintervalBlynkX = UINT32_MAX; // Freeze counter clock wise
      }
/*************************/

/**** Y-axis joystick ****/
  if(y > 50) // y more than 50
      {
        CWintervalBlynkY = map(y, 100, 51,10000,40000);
        CCWintervalBlynkY = UINT32_MAX;
      }
      
      else if(y < 50) // y less than 50
      {
        CWintervalBlynkY = UINT32_MAX;
        CCWintervalBlynkY = map(y, 0, 49,10000,40000);
      }

      else // y equals 50
      {
        CWintervalBlynkY = UINT32_MAX; // Freeze clock wise
        CCWintervalBlynkY = UINT32_MAX; // Freeze counter clock wise
      }
/*************************/
/* On behalf of the joystick, CWintervalBlynk and CCWintervalBlynk gives a value 
on how much the setpoint on the brugi-board shall change in both X- and Y-axis. */

 /***** Json *****/
 //getPIDFromSPIFFS(); //Only necessary on startup
  
  //Will be done in blynk
  jsonSerial["CWintervalBlynkX"] = CWintervalBlynkX;
  jsonSerial["CCWintervalBlynkX"] = CCWintervalBlynkX;
  jsonSerial["CWintervalBlynkY"] = CWintervalBlynkY;
  jsonSerial["CCWintervalBlynkY"] = CCWintervalBlynkY;
  jsonSerial["P"] = P;
  jsonSerial["I"] = I;
  jsonSerial["D"] = D;
  jsonSerial["autoTuneBtn"] = autoTuneBtn;

  //Only necessary when permanently storing
  //Shold not run in a loop
  //writePIDtoSPIFFS(); 
  

  jsonSerial.prettyPrintTo(Serial);

  i++;  
  /****************/
}
/*****************/

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
  autoTuneBtn = param.asInt(); //Reads from button for auto-tune
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
  Blynk.virtualWrite(V5, autoTuneBtn); //writes to auto tune button
}
/********************************************************/


