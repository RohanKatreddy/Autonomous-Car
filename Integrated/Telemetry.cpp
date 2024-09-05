#include "Telemetry.h"

Telemetry::Telemetry():radio()
{

}

void Telemetry::begin()
{
  radio.begin();
}

void Telemetry::sendString(String text)
{
    char* charArray = const_cast<char*>(text.c_str());
    radio.sendCharArray(charArray, sizeof(charArray)+1);
}

void Telemetry::sendTelemetry(int identifier, double latitude, double longitude, int bearing, int heading){
  radio.sendTelemetry(identifier, latitude, longitude, bearing, heading);
}

void Telemetry::sendBasicTransmitStruct(int identifier, double value){
  radio.sendBasicTransmitStruct(identifier, value);
}

void Telemetry::update(){
  latestData = NRF24::latestData;
}

void Telemetry::clear(){
  latestData.identifier = 0;
  latestData.data = 0;
  radio.clear();
}