/***** Included libraries *****/
#include "Remote_control.h"
//<Wire.h>
//PID.h
//Blynk
#include "Timer.h"


/***** Pin definitions *****/
volatile int altitudeInterrupt = 5;
//volatile int azimuthInterrupt = 4;


/***** Variable declarations *****/
uint32_t counter;


/***** Objects *****/
Remote_control Angle(altitudeInterrupt);
//Remote_control Azimuth(azimuthInterrupt);
Timer t;



void setup() 
{
  Serial.begin(250000);
  pinMode(4, OUTPUT);
  Angle.begin();
//  Azimuth.begin();
  t.every(100, step, (void*)0);
}



void loop() 
{
  t.update();
 // delay(100);
 // Serial.println(Azimuth.pulseWidth);
 // Serial.println(Angle.pulseWidth);
  yield();
}

void step(void* context)
{
  uint32_t pulse = Angle.pulseWidth;
  counter++;
  pulse = map(pulse, 1000, 1950, 1000, 10);

  //(pulse > 1850) ? pulse = 4294967295 : 0 ;
 
  if((counter % pulse) == 0)
  {
    digitalWrite(4, HIGH);
    delayMicroseconds(20);
    digitalWrite(4, LOW);
  }
  
}






