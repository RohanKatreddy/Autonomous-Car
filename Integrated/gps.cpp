#include "gps.h"

GPS::GPS(){

}

void GPS::begin(){
  #ifdef CUSTOM_SERIAL_GPS
  Serial2.setRX(GPS_RX_PIN);
  Serial2.setTX(GPS_TX_PIN);
  Serial2.begin(115200);
  #endif
}

void GPS::read(){
  while(Serial2.available()) {
  String rawData = Serial2.readStringUntil('\n');// read the incoming data as string
  rawData.trim();
  if (rawData.startsWith("$GNGLL")){
    latitude = rawData.substring(7, 17).toDouble();
    longitude = rawData.substring(20, 31).toDouble();
  }
}
}

double GPS::getLatitude(){
  return latitude/100;
}

double GPS::getLongitude(){
  return longitude/100;
}