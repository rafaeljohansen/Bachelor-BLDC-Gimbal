/* The motors are controlled by to signal lines; step and direction.
 The direction pin is either pulled high or low. The step pin will
 trig a step on the motor on a rising edge.

         MOTOR1          MOTOR2
PWM sig: PD2/D2(/INT)    PC2/A2 
Enable:  PC3/A3(/M_P)    PC3/A3(/M_P)
*/

/* Because I've changed the PWM frequency by manipulating all three
 timers, functions such as delay, millis and micros are way out of 
 range. in order to get the correct values out of these fuctions
 you'll have to follow these steps:

 MODIFIED TIMERS     UNMODIFIED TIMERS 
 micros()/64     ->  micros()
 millis()/64     ->  millis()
 delay(64)       ->  delay(1)
*/

//Use cli() and sei() to turn ISR on and off!

/***** Included libraries *****/
#include "BLDC.h"
#include "Remote_control.h"
#include <ArduinoJson.h>


/***** Pin definitions *****/
const uint8_t M1_phase1 = 9;
const uint8_t M1_phase2 = 10;
const uint8_t M1_phase3 = 11;
const uint8_t M2_phase1 = 3;
const uint8_t M2_phase2 = 5;
const uint8_t M2_phase3 = 6;



/***** Variables *****/
uint64_t currentMicros;
uint32_t RemoteUpdateInterval = 200000;
int32_t previousYaw = 0;
int32_t previousPitch = 0;



/***** Objects *****/
BLDC Motor1(M1_phase1, M1_phase2, M1_phase3);
BLDC Motor2(M2_phase1, M2_phase2, M2_phase3);
Remote_control Yaw(RemoteUpdateInterval);
Remote_control Pitch(RemoteUpdateInterval);



void setup() 
{
  Serial.begin(115200);
  Motor1.begin();
  Motor2.begin();

  /***** ATmega328p register settings *****/
  DDRC &= ~0x0E; // 0b00001110 -> 11110001 & XXXXXXXX = XXXX000X  set PC1, PC2 and PC3 as input (0 is input, 1 is output)
  // DDRC = DDRC & ~0x0E;
  DDRD &= ~0x04; // 0b00000100 -> set PD2 as input
  PORTC |= _BV(PC3);
  PORTC |= 0x0C; // 0b00001100 -> enable input pullup for PC3 and PC2
  //PORTD |= 0x04; // 0b00000100 -> enable input pullup for PD2
  
  // Pin Change Interrupt enable on PCINT1 vector (PCINT8 - PCINT14)
  PCICR |= _BV(PCIE1);    // Enable PCIE1 (PCINT8 - 14)
  PCICR |= _BV(PCIE2);    // Enable PCIE2 (PCINT16 - 23) 
  PCMSK1 |= _BV(PCINT10); // Enable trig PCINT10 (PC2)
  PCMSK2 |= _BV(PCINT18); // Enable trig PCINT18 (PD2)
  /***************************************/
  
  sei(); // Turn global interrupt on.
}


void loop() 
{
   
  while(1) 
  {
    /***** Store current micros *****/
    currentMicros = micros()/64;
    /********************************/
    
  
    /***** Update remote input and setpoint on Yaw axis *****/  
    //Yaw.readRemote(currentMicros);
    //Yaw.setPoint(currentMicros);


    /***** Update remote input and setpoint on Pitch axis *****/  
    //Pitch.readRemote(currentMicros);
    //Pitch.setPoint(currentMicros);
    





 
    
  }

}






/**************************************************************
This section uses PCINT1 and 2 vector for detecting a change in
the pin state. The on and off time are measured and stored, and 
together we're able to measure the duty cycle in percentage for
each PWM signal. The duty cycle are stored in the struct
variables  motor1_input.dutyCycle  and  motor2_input.dutyCycle
**************************************************************/


/***** MOTOR 1 *****/
ISR(PCINT2_vect) // Trigs when PD2 detects a change
{
  if(!(PINC & _BV(PC3))) // PWM read enable pin M/P
  {
    // Turn off global interrupts
    cli();
    
    if(PIND & _BV(PD2)) // PWM rising
      Yaw.counterHigh = micros()/64; // Start high counter  

    else // PWM falling
      Yaw.pulseWidthHigh = micros()/64 - Yaw.counterHigh; // Stop high counter 

    
    // Calculate positive duty cycle (38 - 62%). Should be 50% as standard
    Yaw.previousDutyCycle = Yaw.dutyCycle;
    Yaw.dutyCycle = (Yaw.pulseWidthHigh - 900)/36 + 36;
    
    // Set dutycycle = 50 if out of range (less than 38 or more than 62)
    (Yaw.dutyCycle > 62 || Yaw.dutyCycle < 38) ? Yaw.dutyCycle = Yaw.previousDutyCycle : 0;
   
    // Turn on global interrupts
    sei();  
  }
  
  //Remote control not enabled
  else
    Yaw.dutyCycle = 50;
}


/***** MOTOR 2 *****/
ISR(PCINT1_vect) // Trigs when PC2 detects a change
{ 
  if(!(PINC & _BV(PC3))) // PWM read enable pin M/P
  {
    // Turn off global interrupts
    cli();
    
    if(PINC & _BV(PC2)) // PWM rising
      Pitch.counterHigh = micros()/64; // Start high counter
    
    else // PWM falling
      Pitch.pulseWidthHigh = micros()/64 - Pitch.counterHigh; // Stop high counter
    
    
    // Calculate positive duty cycle (38 - 62%). Should be 50% as standard
    Pitch.previousDutyCycle = Pitch.dutyCycle;
    Pitch.dutyCycle = (Pitch.pulseWidthHigh - 900)/36 + 36;
    
    // Set dutycycle = 50 if out of range (less than 38 or more than 62)
    (Pitch.dutyCycle > 62 || Pitch.dutyCycle < 38) ? Pitch.dutyCycle = Pitch.previousDutyCycle : 0;

    // Turn on global interrupts  
    sei();
  }

  // Remote control not enabled
  else
    Pitch.dutyCycle = 50; 
}
