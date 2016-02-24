#include "Arduino.h"

#ifndef Json_TxRx_h
#define Json_TxRx_h

class Json_TxRx
{
  public:
    Json_TxRx(void);
    void jsonTx(double t_P, double t_I, double t_D, bool t_autoTuneEnable, int16_t t_positionYaw, int16_t t_positionPitch, int16_t t_setpointYaw, int16_t t_setpointPitch);
    void parseRx(void);
    void serialRxEvent(void);


    int intervalYaw;
    int intervalPitch;
    double P;
    double I;
    double D;
    bool autoTuneEnable;
    int Acc1_AxisX;
    int Acc1_AxisY;
    int Acc1_AxisZ;
    int Acc2_AxisX;
    int Acc2_AxisY;
    int Acc2_AxisZ;

    

  private:
    String jsonReceived;
    bool rxComplete = false;
};


#endif
