#ifndef HMC5883L_H
#define HMC5883L_H

#include <Arduino.h>
#include "config.h"
#include "Wire.h"


class HMC5883L{
  public:
    HMC5883L();
    void begin();
    void read();
    double getX();
    double getY();
    double getZ();
  
  private:
    double xCal, yCal, zCal;
    int fixValue(int value);

};

#endif