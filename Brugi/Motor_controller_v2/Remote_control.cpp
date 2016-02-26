
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
    if(!(PINC & _BV(PC3))) // 00001111 & 00001000 = 00001000 -> 11110111
    {
      if(dutyCycle > 50) // Dutycycle more than 50
      {
        m_CWintervalRC = map(dutyCycle, 62, 51,10000,40000);
        m_CCWintervalRC = UINT32_MAX;
      }
      
      else if(dutyCycle < 50) // Dutycycle less than 50
      {
        m_CWintervalRC = UINT32_MAX;
        m_CCWintervalRC = map(dutyCycle, 38, 49,10000,40000);
      }

      else // Dutycycle equals 50
      {
        m_CWintervalRC = UINT32_MAX; // Freeze clock wise
        m_CCWintervalRC = UINT32_MAX; // Freeze counter clock wise
      }
    }
  /**********************************/

  /****** Else if remote is disabled ******/
    else
    {
      m_CWintervalRC = UINT32_MAX; // Freeze clock wise
      m_CCWintervalRC = UINT32_MAX; // Freeze counter clock wise
    }
  /****************************************/    

  }
}

void Remote_control::readBlynk(uint64_t currentMicros)
{
  if(interval < 0)
  {
    m_CCWintervalBlynk = abs(interval);
    m_CWintervalBlynk = UINT32_MAX;
  }
  else
  { 
    m_CCWintervalBlynk = UINT32_MAX;
    m_CWintervalBlynk = interval;
  }
}


int16_t Remote_control::setPoint(uint64_t currentMicros)
{
  /****** RC changing setpoint ******/
  if(currentMicros - previousMicrosSetpoint >= m_CWintervalRC)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint++;
  }

  if(currentMicros - previousMicrosSetpoint >= m_CCWintervalRC)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint--;
  }
  /**********************************/
  
  /****** Blynk changing setpoint ******/
  if(currentMicros - previousMicrosSetpoint >= m_CWintervalBlynk)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint++;
  }

  if(currentMicros - previousMicrosSetpoint >= m_CCWintervalBlynk)
  {
    previousMicrosSetpoint = currentMicros;
    currentSetPoint--;
  }
  /*************************************/   
  return currentSetPoint;
}




