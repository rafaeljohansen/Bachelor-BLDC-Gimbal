uint64_t prevMicrosStepYaw;
uint64_t prevMicrosGetGain;
uint32_t intervl;
uint32_t yawCWinterval;
uint32_t yawCCWinterval;

void stepYaw(uint64_t currentMicros, uint32_t stepInterval, bool direction)
{
  if(currentMicros - prevMicrosStepYaw >= stepInterval)
  {
    Motor1.step(direction);
  }
  
}

 

void getGain(uint64_t currentMicros, int16_t pidGain)
{

  if(currentMicros - prevMicrosGetGain >= 100000) //100 ms
  {
    prevMicrosGetGain = currentMicros;

      if(pidGain > 0) // Dutycycle more than 50
      {
        yawCWinterval = map(pidGain, 62, 51,10000,40000);
        yawCCWinterval = UINT32_MAX;
      }
      
      else if(pidGain < 0) // Dutycycle less than 50
      {
        yawCWinterval = UINT32_MAX;
        yawCCWinterval = map(pidGain, 38, 49,10000,40000);
      }

      else
      {
        yawCWinterval = UINT32_MAX;
        yawCCWinterval = UINT32_MAX;
      }

  }
}
  


 
 /* Code for steppping motor using remote
    if(previousYaw > Yaw.currentSetPoint)
    {
      previousYaw = Yaw.currentSetPoint;
      Motor1.step(false);
    }

    else if(previousYaw < Yaw.currentSetPoint)
    {
      previousYaw = Yaw.currentSetPoint;
      Motor1.step(true);
    }

      if(previousPitch > Pitch.currentSetPoint)
    {
      previousPitch = Pitch.currentSetPoint;
      Motor2.step(false);
    }

    else if(previousPitch < Pitch.currentSetPoint)
    {
      previousPitch = Pitch.currentSetPoint;
      Motor2.step(true);   
    }
    */
