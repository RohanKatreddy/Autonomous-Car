#include "NRF24.h"

NRF24::recieverStruct NRF24::latestData;
RF24 NRF24::radio(NRF24L01_CE, NRF24L01_CSN);
NRF24::NRF24()
{
}

void NRF24::begin(){
  #ifdef CUSTOM_SPI
    SPI.setRX(SPI_RX);
    SPI.setTX(SPI_TX);
    SPI.setSCK(SPI_CLK);
#endif
    radio.begin();
    radio.maskIRQ(1, 1, 0);
    pinMode(NRF24L01_INTERRUPT_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(NRF24L01_INTERRUPT_PIN), nrfIsr, FALLING);
    radio.openWritingPipe(ADDRESS);
    radio.openReadingPipe(0, ADDRESS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_1MBPS);
    radio.setRetries(3,5);
   radio.startListening();
}

void NRF24::sendCharArray(char array[], int size)
{
  radio.stopListening();
  radio.write(array, size);
radio.startListening();
}

void NRF24::sendTelemetry(int identifier, double latitude, double longitude, int bearing, int heading){
  radio.stopListening();
  struct combinedData{
    int identifier;
    double latitude;
    double longitude;
    int bearing;
    int heading;
  };

  combinedData message = {
    identifier,
    latitude,
    longitude,
    bearing,
    heading
  };
  radio.write(&message, sizeof(message));
  radio.startListening();
}

void NRF24::sendBasicTransmitStruct(int identifier, double value){
  radio.stopListening();
  transmitStruct transmitData;
  transmitData.identifier = identifier;
  transmitData.data = value;
  radio.write(&transmitData, sizeof(transmitData));
  radio.startListening();
}

void NRF24::clear(){
    latestData.identifier = 0;
  latestData.data = 0;
}

void NRF24::nrfIsr(){
  if (radio.available()) {
  radio.read(&latestData, sizeof(latestData));
  Serial.println("hi");
  }
}