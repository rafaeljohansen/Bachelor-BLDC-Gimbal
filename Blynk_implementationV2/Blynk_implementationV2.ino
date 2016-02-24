
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>
#include <FS.h>
char auth[] = "b7b58db178cc41f3a16337431b8b3e15"; //Blynk key

//For connection to WLAN//
const char* ssid = "AndroidAP";
const char* password = "ncts6496";

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

void setup() 
{
  SPIFFS.format();
  Blynk.begin(auth, ssid, password); //Connecting to blynk
  Serial.begin(115200);
  t.every(200, task, 0); //every 1000 ms do task
}

/******** Read changes on virtual pins ********/
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
/**********************************************/

/******** Writes changes on virtual pins ********/
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
/************************************************/

void loop() 
{
  Blynk.run();
  t.update();
}

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

      else // Dutycycle equals 50
      {
        CWintervalBlynkX = UINT32_MAX; // Freeze clock wise
        CCWintervalBlynkX = UINT32_MAX; // Freeze counter clock wise
      }
/*************************/

/**** Y-axis joystick ****/
  if(y > 50) // x more than 50
      {
        CWintervalBlynkY = map(y, 100, 51,10000,40000);
        CCWintervalBlynkY = UINT32_MAX;
      }
      
      else if(y < 50) // x less than 50
      {
        CWintervalBlynkY = UINT32_MAX;
        CCWintervalBlynkY = map(y, 0, 49,10000,40000);
      }

      else // Dutycycle equals 50
      {
        CWintervalBlynkY = UINT32_MAX; // Freeze clock wise
        CCWintervalBlynkY = UINT32_MAX; // Freeze counter clock wise
      }
/*************************/
/* On behalf of the joystick, CWintervalBlynk and CCWintervalBlynk gives a value 
on how much the setpoint on the brugi-board shall change in both X- and Y-axis. */
}

