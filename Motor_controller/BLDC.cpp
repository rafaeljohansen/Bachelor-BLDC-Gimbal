/* BLDC - BrushLess DC Motor */

#include "BLDC.h"
#include "Arduino.h"

    
BLDC::BLDC(uint8_t phase1, uint8_t phase2, uint8_t phase3)
{
  //Store arguments to local variables
  _phase1 = phase1;
  _phase2 = phase2;
  _phase3 = phase3;

  //set to output
  pinMode(_phase1, OUTPUT);
  pinMode(_phase2, OUTPUT);
  pinMode(_phase3, OUTPUT);
}


void BLDC::begin() // Needed to permanently set the timer registers
{
  // Set PWM frequency to 31250 Hz
  TCCR0B = TCCR0B & 0b11111000 | 0x01; // pin 5 and 6
  TCCR1B = TCCR1B & 0b11111000 | 0x01; // pin 9 and 10
  TCCR2B = TCCR2B & 0b11111000 | 0x01; // pins 3 and 11   
  ICR1 = 0xFF; // 8-bit PWM resolution
  
  step(true); //enable PWM on the three phases
}


void BLDC::step(bool direction)
{  
  if (direction == true) // clockwise or counter clockwise
  {
    currentStepPhase1++;
    currentStepPhase2++;
    currentStepPhase3++;
  }
  else 
  {
    currentStepPhase1--;
    currentStepPhase2--;
    currentStepPhase3--;  
  }

  //Start at the beginning or start at the end 
  if(currentStepPhase1 > 47) currentStepPhase1 = 0; 
  if(currentStepPhase1 < 0) currentStepPhase1 = 47;
    
  if(currentStepPhase2 > 47) currentStepPhase2 = 0;
  if(currentStepPhase2 < 0) currentStepPhase2 = 47;
       
  if(currentStepPhase3 > 47) currentStepPhase3 = 0;
  if(currentStepPhase3 < 0) currentStepPhase3 = 47;
     
  analogWrite(_phase1, pwmSin[currentStepPhase1]);
  analogWrite(_phase2, pwmSin[currentStepPhase2]);
  analogWrite(_phase3, pwmSin[currentStepPhase3]);
}
