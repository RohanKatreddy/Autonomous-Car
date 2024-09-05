#ifndef NRF24_H
#define NRF24_H

#include <Arduino.h>
#include "config.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

class NRF24
{
public:
    NRF24();
    struct recieverStruct{
      int identifier;
      double data;
    };
    static recieverStruct latestData;
    static RF24 radio;
    void begin();
    void sendCharArray(char arr[], int size);
    void sendBasicTransmitStruct(int identifier, double value);
    void sendTelemetry(int identifier, double latitude, double longitude, int bearing, int heading);
    void clear();
private:
    static void nrfIsr();
      struct transmitStruct{
    int identifier;
    double data;
  };
};


#endif