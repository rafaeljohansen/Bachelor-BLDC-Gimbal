/***** Included libraries *****/
#include "Remote_control.h"
//<Wire.h>
//PID.h
//Blynk
//"Timer.h"


/***** Pin definitions *****/
volatile int altitudeInterrupt = 5;
volatile int azimuthInterrupt = 4;


/***** Variable declarations *****/



/***** Objects *****/
Remote_control Angle(altitudeInterrupt);
Remote_control Azimuth(azimuthInterrupt);




void setup() 
{
  Serial.begin(115200);
  Angle.begin();
  Azimuth.begin();

}



void loop() 
{
    delay(100);
    Serial.println(Azimuth.pulseWidth);
    Serial.println(Angle.pulseWidth);
}







