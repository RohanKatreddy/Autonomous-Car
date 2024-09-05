#include "HMC5883L.h"

HMC5883L::HMC5883L(){
}

void HMC5883L::begin(){
  Wire.beginTransmission(HMC_REGISTER);
  Wire.write(HMC_MODE_REGISTER);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void HMC5883L::read(){
  double hmcX, hmcY, hmcZ;
  Wire.beginTransmission(HMC_REGISTER);
  Wire.write(HMC_MAG_REGISTER);
  Wire.endTransmission(false);
  Wire.requestFrom(HMC_REGISTER, 6, true);
  hmcX = Wire.read() << 8 | Wire.read();
  hmcZ = Wire.read() << 8 | Wire.read();
  hmcY = Wire.read() << 8 | Wire.read();
  hmcX = fixValue(hmcX);
  hmcY = fixValue(hmcY);
  hmcZ = fixValue(hmcZ);


  static double hi_cal[3];
  hmcX /= 10.90;
  hmcZ /= 10.90;
  hmcY /= 10.90;

  double mag_data[] = {hmcX,
                      hmcY,
                      hmcZ};

  // Apply hard-iron offsets
  for (uint8_t i = 0; i < 3; i++)
  {
    hi_cal[i] = mag_data[i] - HARD_IRON[i];
  }

  // Apply soft-iron scaling
  for (uint8_t i = 0; i < 3; i++)
  {
    mag_data[i] = (SOFT_IRON[i][0] * hi_cal[0]) + (SOFT_IRON[i][1] * hi_cal[1]) - (SOFT_IRON[i][2] * hi_cal[2]);
  }

  xCal = mag_data[0];
  yCal = mag_data[1];
  zCal = mag_data[2];
}

double HMC5883L::getX(){
  return xCal;
}

double HMC5883L::getY(){
  return yCal;
}

double HMC5883L::getZ(){
  return zCal;
}

int HMC5883L::fixValue(int value)
{
  if (value > 32767)
    value = value - 65536;
  return value;
}