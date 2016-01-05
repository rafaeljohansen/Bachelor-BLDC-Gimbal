/*
 * TODO: Add external control using step and direction for each motor
 * could be done using PCINT, since digital pin 3 is occupied
 * http://www.avrfreaks.net/forum/rising-and-falling-edges-pcint-interrupt
 * http://www.avrfreaks.net/forum/whats-functional-difference-between-int-and-pcint
 */

#include "BLDC.h"

//Motor1 connected to pin 3, 5 and 6
BLDC Motor1(3,5,6);

//Motor2 connected to pin 9, 10 and 11
BLDC Motor2(9,10,11);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  Motor1.step(true);
  Motor2.step(false);
  delay(500);
}
