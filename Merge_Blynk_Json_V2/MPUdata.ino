#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"





/* Initialisation without parameters */
void mpuInit(MPU6050 &MpuObject)
{
  Wire.begin();

  MpuObject.initialize();
  MpuObject.testConnection();
  MpuObject.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  MpuObject.setXGyroOffset(70);
  MpuObject.setYGyroOffset(60);
  MpuObject.setZGyroOffset(-85);
  MpuObject.setZAccelOffset(2088); // 1688 factory default
  MpuObject.setDMPEnabled(true);

  /* Serial.println("MPU6050 delay started (15 sec)");
  delay(15000); // Needed for stabilisation
  Serial.println("initialization finished"); */ 
}



/* Initialisation with parameters */
void mpuInit(MPU6050 &MpuObject, int16_t X_gyroOffset, int16_t Y_gyroOffset, int16_t Z_gyroOffset, int16_t Z_AccelOffset)            
{
  Wire.begin();

  MpuObject.initialize();
  MpuObject.testConnection();
  MpuObject.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  MpuObject.setXGyroOffset(X_gyroOffset);
  MpuObject.setYGyroOffset(Y_gyroOffset);
  MpuObject.setZGyroOffset(Z_gyroOffset);
  MpuObject.setZAccelOffset(Z_AccelOffset); // 1688 factory default
  MpuObject.setDMPEnabled(true);

  /* Serial.println("MPU6050 delay started (15 sec)");
  delay(15000); // Needed for stabilisation
  Serial.println("initialization finished"); */
}

void updateAcc1()
{
  mpuGetValues(mpu1);
}

void updateAcc2()
{
  mpuGetValues(mpu2);
}


void mpuGetValues(MPU6050 &MpuObject)
{
  Quaternion q;           // [w, x, y, z] quaternion container
  VectorFloat gravity;    // [x, y, z]    gravity vector
  
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  
  
  packetSize = MpuObject.dmpGetFIFOPacketSize();
  mpuIntStatus = MpuObject.getIntStatus();
  fifoCount = MpuObject.getFIFOCount();
  if((mpuIntStatus & 0x10) || fifoCount == 1024) 
    MpuObject.resetFIFO(); // reset FIFO so we can continue cleanly
     
  else if (mpuIntStatus & 0x02) 
  {
    while (fifoCount < packetSize) 
      fifoCount = MpuObject.getFIFOCount();
      
    MpuObject.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
       
     // display Euler angles in degrees
    MpuObject.dmpGetQuaternion(&q, fifoBuffer);
    MpuObject.dmpGetGravity(&gravity, &q);
    MpuObject.dmpGetYawPitchRoll(ypr, &q, &gravity);

    MpuObject.yaw = ypr[0] * 180/M_PI;
    MpuObject.pitch = ypr[1] * 180/M_PI;
    MpuObject.roll = ypr[2] * 180/M_PI;
    //ypr[n] * 18000/M_PI
  }
}

