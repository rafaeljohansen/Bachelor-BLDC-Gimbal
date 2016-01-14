#include "Remote_control.h"
#include "Arduino.h"


Remote_control *Remote_control::instance_;


Remote_control::Remote_control(uint8_t t_interruptPin)
{
  m_interruptPin = t_interruptPin;
}


void Remote_control::begin()
{
  attachInterrupt(m_interruptPin, risingEdge, RISING);
  instance_ = this;
}


void Remote_control::risingEdge()
{
  instance_ -> startCounter();
}


void Remote_control::fallingEdge()
{
  instance_ -> stopCounter();
}


void Remote_control::startCounter() 
{
  counter = micros();
  detachInterrupt(m_interruptPin);
  attachInterrupt(m_interruptPin, fallingEdge, FALLING);
  instance_ = this;
}


void Remote_control::stopCounter() 
{
  pulseWidth = micros() - counter;
  detachInterrupt(m_interruptPin);
  attachInterrupt(m_interruptPin, risingEdge, RISING);
  instance_ = this;
}
