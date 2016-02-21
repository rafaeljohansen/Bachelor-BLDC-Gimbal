/* BLDC - BrushLess DC Motor */

#include "BLDC.h"
#include "Arduino.h"

    
BLDC::BLDC(uint8_t t_phase1, uint8_t t_phase2, uint8_t t_phase3)
{
  //Store arguments to local variables
  m_phase1 = t_phase1;
  m_phase2 = t_phase2;
  m_phase3 = t_phase3;

  //set to output
  pinMode(m_phase1, OUTPUT);
  pinMode(m_phase2, OUTPUT);
  pinMode(m_phase3, OUTPUT);
}



void BLDC::begin() // Needed to permanently set the timer registers
{
  // Set PWM frequency: 63500 Hz -> timer0, ~31250 Hz -> timer1 and timer2 
  TCCR0B = TCCR0B & 0b11111000 | 0x01; // pin 5 and 6
  TCCR1B = TCCR1B & 0b11111000 | 0x01; // pin 9 and 10
  TCCR2B = TCCR2B & 0b11111000 | 0x01; // pins 3 and 11   
  ICR1 = 0xFF; // 8-bit PWM resolution
   
  step(true); //enable PWM on the three phases
}


void BLDC::step(bool t_direction)
{  
  if (t_direction == true) // clockwise or counter clockwise
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
  if(currentStepPhase1 > maxArrayPlace) currentStepPhase1 = 0; 
  if(currentStepPhase1 < 0) currentStepPhase1 = maxArrayPlace;
    
  if(currentStepPhase2 > maxArrayPlace) currentStepPhase2 = 0;
  if(currentStepPhase2 < 0) currentStepPhase2 = maxArrayPlace;
       
  if(currentStepPhase3 > maxArrayPlace) currentStepPhase3 = 0;
  if(currentStepPhase3 < 0) currentStepPhase3 = maxArrayPlace;

  
  
  analogWrite(m_phase1, pwmSin[currentStepPhase1]);
  analogWrite(m_phase2, pwmSin[currentStepPhase2]);
  analogWrite(m_phase3, pwmSin[currentStepPhase3]);
  
}
