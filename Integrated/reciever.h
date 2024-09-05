#ifndef RECIEVER_H
#define RECIEVER_H

#include <Arduino.h>
#include "config.h"

class Reciever
{
public:
  Reciever();
  void begin();
  void read();
  int channelValues[IBUS_MAXCHANNELS] = {0};
  boolean rxFrameDone;

private:
  uint8_t ibusIndex = 0;
  uint8_t ibus[IBUS_BUFFSIZE] = {0};
  uint16_t ibusValue[IBUS_MAXCHANNELS];
  void formatRx();
};

#endif