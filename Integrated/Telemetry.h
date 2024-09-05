#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "config.h"
#include "NRF24.h"

class Telemetry
{
public:
    NRF24::recieverStruct latestData;
    Telemetry();
    void begin();
    void sendString(String text);
    void sendTelemetry(int identifier, double latitude, double longitude, int bearing, int heading);
    void sendBasicTransmitStruct(int identifier, double value);
    void update();
    void clear();

private:
#ifdef NRF24L01
    NRF24 radio;
#endif
};

#endif