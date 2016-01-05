/* The motors are controlled by to signal lines; step and direction.
 The direction pin is either pulled high or low. The step pin will
 trig a step on the motor on a rising edge.

       MOTOR1          MOTOR2
Step:  A1/PC1          A2/PC2 
Dir:   D2/PD2(/INT)    A3/PC3(/M_P)
*/

#include "BLDC.h"

BLDC Motor1(9,10,11);
BLDC Motor2(3,5,6);



void setup() 
{
  Motor1.begin();
  Motor2.begin();

  DDRC &= ~0x0E; //0b00001110 -> set PC1, PC2 and PC3 as input
  DDRD &= ~0x04; //0b00000100 -> set PD2 as input
  PORTC |= 0x08; //0b00001000 -> enable input pullup for PC3
  PORTD |= 0x04; //0b00000100 -> enable input pullup for PD2
  
  // Pin Change Interrupt enable on PCINT1 vector (PCINT8 - PCINT14)
  PCICR |= _BV(PCIE1);    // PCINT8 - 14 enable
  PCMSK1 |= _BV(PCINT9);  // PCINT9 - PC1
  PCMSK1 |= _BV(PCINT10); // PCINT10 - PC2

  sei(); // Turn global interrupt on.
}



void loop() 
{
  while(1); //let the interrupt handle everything
}



ISR(PCINT1_vect) //PCINT8 - 14
{ 
  if(PINC & _BV(PC1)) //PCINT9 trig on positive edge
  { 
    ((PIND & 0x04) ? Motor1.step(true) : Motor1.step(false));
   // If PD2 = HIGH -> Motor1.step(true), else -> Motor1.step(false)
  }

  if(PINC & _BV(PC2)) //PCINT10 trig on positive edge
  {
    ((PINC & 0x08) ? Motor2.step(true) : Motor2.step(false));
    // If PC3 = HIGH -> Motor2.step(true), else -> Motor2.step(false)
  }
   
}
