#include <Wire.h>


const int hmcRegister = 0x1E;
const int hmcModeRegister = 0x02;
const int hmcMagRegister = 0x03;

float xOff, yOff, zOff;
float xCal, yCal, zCal;
float hmcX, hmcY, hmcZ;
float heading;

float temp;

float xComp, yComp;

float pi = 3.14159265358979323;



int fixValue(int value) {
  if (value > 32767)
    value = value - 65536;
  return value;
}

void readHmc() {
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcMagRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(hmcRegister, 6, true);
  hmcX = Wire.read() << 8 | Wire.read();
  hmcZ = Wire.read() << 8 | Wire.read();
  hmcY = Wire.read() << 8 | Wire.read();
  hmcX = fixValue(hmcX);
  hmcY = fixValue(hmcY);
  hmcZ = fixValue(hmcZ);
}


void setup() {
  Serial.begin(115200);
   Wire.setSDA(8);
    Wire.setSCL(9);
  Wire.begin();
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcModeRegister);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop() {
  readHmc();
  
  hmcX /= 10.90;
  hmcZ /= 10.90;
  hmcY /= 10.90;
 Serial.print("Raw:0,0,0,0,0,0,");
 Serial.print(int(hmcX*10)); Serial.print(","); Serial.print(int(hmcY*10)); Serial.print(","); Serial.println(int(hmcZ*10));
 Serial.print("Uni:0.00,0.00,0.00,0.0000,0.0000,0.0000,");
  Serial.print(hmcX); Serial.print(","); Serial.print(hmcY); Serial.print(","); Serial.println(hmcZ);
}
