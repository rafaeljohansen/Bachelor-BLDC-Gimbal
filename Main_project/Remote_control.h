#include "Arduino.h"

#ifndef Remote_control_h
#define Remote_control_h

class Remote_control
{
  public:
    Remote_control(uint8_t t_interruptPin);
    void begin();
    static void risingEdge();
    static void fallingEdge();
    void startCounter();
    void stopCounter();

    uint32_t pulseWidth;
    

  private:
    uint8_t m_interruptPin;
    static Remote_control *instance_;
    uint64_t counter;
      
};

#endif
