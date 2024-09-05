#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "config.h"

class GPS
{
public:
  GPS();
  void begin();
  void read();
  double getLatitude();
  double getLongitude();

private:
  double latitude;
  double longitude;
};

#endif