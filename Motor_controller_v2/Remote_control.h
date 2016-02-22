#include "Arduino.h"

#ifndef Remote_control_h
#define Remote_control_h

/***** Definitions *****/
#define INT32_MAX 0x7fffffffL
#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)
#define INT64_MAX 0x7fffffffffffffffLL
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)


class Remote_control
{
  public:

  /***** Constructor and methods *****/
    Remote_control(uint32_t RemoteInterval);
    void readRemote(uint64_t currentMicros);
    void readBlynk(uint64_t currentMicros);
    int16_t setPoint(uint64_t currentMicros);
    
  
    uint16_t readRemoteInterval;
    uint32_t updateSetpointInterval;
    int16_t currentSetPoint;


    uint64_t counterHigh; 
    uint64_t pulseWidthHigh; 
    uint64_t pulseWidthLow; 
    uint8_t dutyCycle; 
    uint8_t previousDutyCycle;
    
    
  private:

  /***** Update interval *****/
    uint32_t m_RemoteInterval;
    uint32_t m_CWintervalRC;
    uint32_t m_CCWintervalRC;
    uint32_t m_CWintervalBlynk;
    uint32_t m_CCWintervalBlynk;
   
    
    uint64_t previousMicrosSetpoint = 0;
    uint64_t previousMicrosReadRemote = 0;
    
     
   
};

#endif
