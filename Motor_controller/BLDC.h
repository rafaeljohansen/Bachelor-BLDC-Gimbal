#include "Arduino.h"

#ifndef BLDC_h
#define BLDC_h

class BLDC
{
  public:
    BLDC(uint8_t phase1, uint8_t phase2, uint8_t phase3);
    void step(bool direction);
    
  private:
    int8_t _phase1;
    int8_t _phase2;
    int8_t _phase3;
    uint8_t currentStepPhase1 = 0;
    uint8_t currentStepPhase2 = 16;
    uint8_t currentStepPhase3 = 32; 
 
    const uint8_t pwmSin[48] = {127,110,94,78,64,50,37,26,17,10,4,1,0,1,4,10,17,26,37,50,64,78,94,110,127,144,160,176,191,204,217,228,237,244,250,253,254,253,250,244,237,228,217,204,191,176,160,144};
};


#endif
