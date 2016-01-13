#include "Arduino.h"

#ifndef BLDC_h
#define BLDC_h

class BLDC
{
  public:
    BLDC(uint8_t t_phase1, uint8_t t_phase2, uint8_t t_phase3);
    void begin();
    void step(bool direction);
    
    
  private:
    uint8_t m_phase1;
    uint8_t m_phase2;
    uint8_t m_phase3;
    int16_t currentStepPhase1 = 0;
    int16_t currentStepPhase2 = sizeof(pwmSin) / 3;
    int16_t currentStepPhase3 = sizeof(pwmSin) / 3 * 2 ; 
    int16_t maxArrayPlace = sizeof(pwmSin) - 1;
     
    // f(x) = 1 - sin(x)
    //const uint8_t pwmSin[48] = {127, 110, 94, 78, 64, 50, 37, 26, 17, 10, 4, 1, 0, 1, 4, 10, 17, 26, 37, 50, 64, 78, 94, 110, 127, 144, 160, 176, 191, 204, 217, 228, 237, 244, 250, 253, 254, 253, 250, 244, 237, 228, 217, 204, 191, 176, 160, 144};
    //const uint8_t pwmSin[96] = {127, 119, 110, 102, 94, 86, 78, 71, 64, 56, 50, 43, 37, 32, 26, 21, 17, 13, 10, 7, 4, 2, 1, 0, 0, 0, 1, 2, 4, 7, 10, 13, 17, 21, 26, 32, 37, 43, 50, 56, 64, 71, 78, 86, 94, 102, 110, 119, 127, 135, 144, 152, 160, 168, 176, 183, 190, 198, 204, 211, 217, 222, 228, 233, 237, 241, 244, 247, 250, 252, 253, 254, 254, 254, 253, 252, 250, 247, 244, 241, 237, 233, 228, 222, 217, 211, 204, 198, 191, 183, 176, 168, 160, 152, 144, 135};
    const uint8_t pwmSin[189] = {126, 119, 115, 110, 106, 102, 98, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 50, 46, 43, 40, 37, 34, 32, 29, 26, 24, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15, 17, 19, 21, 24, 26, 29, 32, 34, 37, 40, 43, 46, 50, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 98, 102, 106, 110, 115, 119, 126, 135, 139, 144, 148, 152, 156, 160, 164, 168, 172, 176, 179, 183, 187, 190, 194, 198, 201, 204, 208, 211, 214, 217, 220, 222, 225, 228, 230, 233, 235, 237, 239, 241, 243, 244, 246, 247, 249, 250, 251, 252, 252, 253, 253, 254, 254, 254, 254, 254, 254, 253, 253, 252, 252, 251, 250, 249, 247, 246, 244, 243, 241, 239, 237, 235, 233, 230, 228, 225, 222, 220, 217, 214, 211, 208, 204, 201, 198, 194, 191, 187, 183, 179, 176, 172, 168, 164, 160, 156, 152, 148, 144, 139, 135};//, 131};
    //const uint8_t pwmSin[192] = {127, 123, 119, 115, 110, 106, 102, 98, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 50, 46, 43, 40, 37, 34, 32, 29, 26, 24, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15, 17, 19, 21, 24, 26, 29, 32, 34, 37, 40, 43, 46, 50, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 98, 102, 106, 110, 115, 119, 123, 127, 131, 135, 139, 144, 148, 152, 156, 160, 164, 168, 172, 176, 179, 183, 187, 190, 194, 198, 201, 204, 208, 211, 214, 217, 220, 222, 225, 228, 230, 233, 235, 237, 239, 241, 243, 244, 246, 247, 249, 250, 251, 252, 252, 253, 253, 254, 254, 254, 254, 254, 253, 253, 252, 252, 251, 250, 249, 247, 246, 244, 243, 241, 239, 237, 235, 233, 230, 228, 225, 222, 220, 217, 214, 211, 208, 204, 201, 198, 194, 191, 187, 183, 179, 176, 172, 168, 164, 160, 156, 152, 148, 144, 139, 135, 131};
    //const uint8_t pwmSin[384] = {127, 125, 123, 121, 119, 117, 115, 112, 110, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 56, 55, 53, 51, 50, 48, 46, 45, 43, 42, 40, 39, 37, 36, 34, 33, 32, 30, 29, 28, 26, 25, 24, 23, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 10, 9, 8, 7, 7, 6, 5, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 26, 28, 29, 30, 32, 33, 34, 36, 37, 39, 40, 42, 43, 45, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 64, 65, 67, 69, 71, 73, 75, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 115, 117, 119, 121, 123, 125, 127, 129, 131, 133, 135, 137, 139, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 179, 181, 183, 185, 187, 189, 190, 192, 194, 196, 198, 199, 201, 203, 204, 206, 208, 209, 211, 212, 214, 215, 217, 218, 220, 221, 222, 224, 225, 226, 228, 229, 230, 231, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 244, 245, 246, 247, 247, 248, 249, 249, 250, 250, 251, 251, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 253, 253, 252, 252, 252, 251, 251, 250, 250, 249, 249, 248, 247, 247, 246, 245, 244, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 231, 230, 229, 228, 226, 225, 224, 222, 221, 220, 218, 217, 215, 214, 212, 211, 209, 208, 206, 204, 203, 201, 199, 198, 196, 194, 192, 191, 189, 187, 185, 183, 181, 179, 178, 176, 174, 172, 170, 168, 166, 164, 162, 160, 158, 156, 154, 152, 150, 148, 146, 144, 142, 139, 137, 135, 133, 131, 129};
    //const uint8_t pwmSin[192] = {127, 123, 119, 115, 110, 106, 102, 98, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 50, 46, 43, 40, 37, 34, 32, 29, 26, 24, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15, 17, 19, 21, 24, 26, 29, 32, 34, 37, 40, 43, 46, 50, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 98, 102, 106, 110, 115, 119, 123, 127, 131, 135, 139, 144, 148, 152, 156, 160, 164, 168, 172, 176, 179, 183, 187, 190, 194, 198, 201, 204, 208, 211, 214, 217, 220, 222, 225, 228, 230, 233, 235, 237, 239, 241, 243, 244, 246, 247, 249, 250, 251, 252, 252, 253, 253, 254, 254, 254, 254, 254, 253, 253, 252, 252, 251, 250, 249, 247, 246, 244, 243, 241, 239, 237, 235, 233, 230, 228, 225, 222, 220, 217, 214, 211, 208, 204, 201, 198, 194, 191, 187, 183, 179, 176, 172, 168, 164, 160, 156, 152, 148, 144, 139, 135, 131};

    

  
};

#endif
