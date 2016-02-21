/* The motors are controlled by to signal lines; step and direction.
 The direction pin is either pulled high or low. The step pin will
 trig a step on the motor on a rising edge.

       MOTOR1          MOTOR2
Step:  PD2/D2(/INT)    PC2/A2 
Dir:   PC1/A1          PC3/A3(/M_P)
*/

#include "BLDC.h"

// Motors connected to pin 3,5,6 and 9,10,11
BLDC Motor1(9,10,11);
BLDC Motor2(3,5,6);


void setup() 
{
 // Serial.begin(115200);
  Motor1.begin();
  Motor2.begin();
  
  DDRC &= ~0x0E; // 0b00001110 -> set PC1, PC2 and PC3 as input
  DDRD &= ~0x04; // 0b00000100 -> set PD2 as input
  PORTC |= 0x0E; // 0b00001110 -> enable input pullup for PC3, PC2 and PC1
  PORTD |= 0x04; // 0b00000100 -> enable input pullup for PD2
  
  // Pin Change Interrupt enable on PCINT1 vector (PCINT8 - PCINT14)
  PCICR |= _BV(PCIE1);    // Enable PCIE1 (PCINT8 - 14)
  PCICR |= _BV(PCIE2);    // Enable PCIE2 (PCINT16 - 23) 
  PCMSK1 |= _BV(PCINT10);  // Enable PCINT10 (PC2)
  PCMSK2 |= _BV(PCINT18); // Enable PCINT18 (PD2)

  sei(); // Turn global interrupt on.
}



void loop() 
{
  while(1); // Let the interrupt handle everything
}



// Trigs when PC1 (PCINT9) detects a rising edge
ISR(PCINT1_vect) 
{ 
  if(PINC & _BV(PC2)) // if(PC2.read == true)
  {   
    ((PINC & _BV(PC3)) ? Motor2.step(true) : Motor2.step(false));
    // If PC3.read == HIGH -> Motor2.step(true), else -> Motor2.step(false)
    
  }  
}



// Trigs when PD2 (PCINT18) detects a rising edge
ISR(PCINT2_vect) 
{
  if(PIND & _BV(PD2)) // if(PD2.read == true)
  {    
    ((PINC & _BV(PC1)) ? Motor1.step(true) : Motor1.step(false));
   // If PC1.read == HIGH -> Motor1.step(true), else -> Motor1.step(false)
  }
}

