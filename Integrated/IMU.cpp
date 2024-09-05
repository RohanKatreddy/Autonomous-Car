#include "IMU.h"

IMU::IMU(){
}

void IMU::begin(){
  mpu.begin();
  hmc.begin();
}

double IMU::getHeading(){
  mpu.read();
  hmc.read();
  double xComp, yComp, heading;
  xComp = hmc.getX() * cos(mpu.getPitch() * -1) - hmc.getY() * sin(mpu.getRoll() * 1) * sin(mpu.getPitch() * -1) + hmc.getZ() * cos(mpu.getRoll() * 1) * sin(mpu.getPitch() * -1);
  yComp = hmc.getY() * cos(mpu.getRoll() * 1) + hmc.getZ() * sin(mpu.getRoll() * 1);
  heading = 1 * (atan2(xComp, yComp) * 180) / M_PI;
  heading += DECLINATION;
  if (heading < 0)
  {
    heading += 360;
  }
  return heading;
}

double IMU::getPitch(){
  mpu.read();
  return mpu.getPitch() * 180 / M_PI;
}

double IMU::getRoll(){
  mpu.read();
  return mpu.getRoll() * 180 / M_PI;
}