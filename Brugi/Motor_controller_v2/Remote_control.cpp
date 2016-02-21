
#include "Arduino.h"
#include "Remote_control.h"


Remote_control::Remote_control(uint32_t t_interval)
{
  m_RemoteInterval = t_interval;
}


void Remote_control::readRemote(uint64_t currentMicros)
{

  if(currentMicros - previousMicrosReadRemote >= m_RemoteInterval) // Run this if-query every 'readInterval' microseconds
  {
    previousMicrosReadRemote = currentMicros;


  /****** If remote is enabled ******/
    if(!(PINC & _BV(PC3))) 
    {
      if(dutyCycle > 50) // Dutycycle more than 50
      {
        m_CWinterval = map(dutyCycle, 62, 51,10000,40000);
        m_CCWinterval = UINT32_MAX;
      }
      
      else if(dutyCycle < 50) // Dutycycle less than 50
      {
        m_CWinterval = UINT32_MAX;
        m_CCWinterval = map(dutyCycle, 38, 49,10000,40000);
      }

      else // Dutycycle equals 50
      {
        m_CWinterval = UINT32_MAX; // Freeze clock wise
        m_CCWinterval = UINT32_MAX; // Freeze counter clock wise
      }
    }
  /**********************************/


  /****** Else if remote is disabled ******/
    else
    {
      m_CWinterval = UINT32_MAX; // Freeze clock wise
      m_CCWinterval = UINT32_MAX; // Freeze counter clock wise
    }
  /****************************************/    


  }
}



int16_t Remote_control::setPoint(uint64_t currentMicros)
{
  if(currentMicros - previousMicrosSetpoint >= m_CWinterval)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint++;
  }

  if(currentMicros - previousMicrosSetpoint >= m_CCWinterval)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint--;
  }
    
  return currentSetPoint;
}




