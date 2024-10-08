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



void readHmc(){
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcMagRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(hmcRegister, 6, true);
  hmcX = Wire.read() << 8| Wire.read();
  hmcZ = Wire.read() << 8| Wire.read();
  hmcY = Wire.read() << 8| Wire.read();
}


void setup() {
   Wire.setSDA(20);
   Wire.setSCL(21);
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcModeRegister);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop() {
  readHmc();
  
  if (hmcX > 32767)
  hmcX = hmcX - 65536;
  if (hmcY > 32767)
  hmcY = hmcY - 65536;
  if (hmcZ > 32767)
  hmcZ = hmcZ - 65536;
  hmcX /= 10.90;
  hmcZ /= 10.90;
  hmcY /= 10.90;
  xOff = hmcX - 3.800202;
  yOff = hmcY + 6.923689;
  zOff = hmcZ + 7.969065;

  xCal =  1.013272*xOff + 0.028993*yOff + 0.005559*zOff;
  zCal =  0.028993*xOff + 1.038330*yOff + 0.002060*zOff;
  yCal =  0.005559*xOff + 0.002060*yOff + 1.025920*zOff;
  

  //Serial.print(xCal); Serial.print(","); Serial.print(yCal); Serial.print(","); Serial.println(zCal);

  heading = (atan2(hmcY, hmcX)*(180/pi));
  heading = 360 + heading + 16.41667;
  if (heading > 360){
    heading -= 360;
  }

  Serial.print(hmcX); Serial.print(", "); Serial.print(hmcY); Serial.print(", "); Serial.print(hmcZ); Serial.print(", "); Serial.println(heading);
  delay(100);
}
