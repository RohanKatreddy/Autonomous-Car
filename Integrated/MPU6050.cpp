#include "MPU6050.h"

MPU_6050::MPU_6050(){

}

void MPU_6050::begin(){
  #ifdef CUSTOM_I2C
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();
  Wire.setClock(400000);
  #endif
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(GYRO_OFFSET[0]);
  mpu.setYGyroOffset(GYRO_OFFSET[1]);
  mpu.setZGyroOffset(GYRO_OFFSET[2]);
  mpu.setXAccelOffset(ACCEL_OFFSET[0]);
  mpu.setYAccelOffset(ACCEL_OFFSET[1]);
  mpu.setZAccelOffset(ACCEL_OFFSET[2]);
  if (devStatus == 0)
  {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
}

void MPU_6050::read(){
  if (!dmpReady)
    return;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  }
}

double MPU_6050::getPitch(){
  return ypr[1];
}

double MPU_6050::getRoll(){
  return ypr[2];
}

double MPU_6050::getYaw(){
  return ypr[0];
}
