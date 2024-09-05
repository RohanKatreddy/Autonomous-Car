#include <Wire.h>

void setup() {
  Wire.begin(8);                // join i2c bus as slave with address 8
  Wire.onReceive(receiveData);      // register receiveData function as callback
  Serial.begin(115200);           // initialize serial communication
}

void loop() {
  // nothing to do here
}

void receiveData(int byteCount) {
  if (byteCount == 2) {          // make sure we received two bytes of data
    byte byte1 = Wire.read();    // read the first byte
    byte byte2 = Wire.read();    // read the second byte
    int data = ((int)byte1 << 8) | byte2;  // combine the bytes into a 16-bit integer value
    Serial.println(data);        // print the result to the serial monitor
  }
}