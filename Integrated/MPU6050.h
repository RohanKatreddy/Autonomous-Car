#ifndef MPU_6050_H
#define MPU_6050_H

#include <Arduino.h>
#include "config.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"
#include "Wire.h"

class MPU_6050{
  public:
    MPU_6050();
    void begin();
    void read();
    double getPitch();
    double getRoll();
    double getYaw();

  private:
    MPU6050 mpu;
    bool dmpReady = false;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];
    Quaternion q;
    VectorFloat gravity; 
    float ypr[3];
};

#endif