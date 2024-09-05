#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "config.h"
#ifdef MPU6050_IMU
#include "MPU6050.h"
#endif

#ifdef HMC5883L_IMU
#include "HMC5883L.h"
#endif

class IMU{
  public:
    IMU();
    void begin();
    double getHeading();
    double getPitch();
    double getRoll();

  private:
    #ifdef MPU6050_IMU
    MPU_6050 mpu;
    #endif

    #ifdef HMC5883L_IMU
    HMC5883L hmc;
    #endif
};

#endif