#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
String a;
RF24 radio(6, 7);  // CE, CSN

const byte address[6] = "00001";

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial2.setRX(5);

  Serial2.begin(115200); 
  Serial.begin(115200); 
}

void loop() {
    while(Serial2.available()) {

  a= Serial2.readStringUntil('\n');// read the incoming data as string
  a.trim();
  if (a.startsWith("$GNGLL")){
  Serial.println(a.substring(7, 17));
  Serial.println(a.substring(20, 31));
  String temp = a.substring(7, 17);
  char text[11];
  temp.toCharArray(text, 11);
  radio.write(&text, sizeof(text));
  }
}
}