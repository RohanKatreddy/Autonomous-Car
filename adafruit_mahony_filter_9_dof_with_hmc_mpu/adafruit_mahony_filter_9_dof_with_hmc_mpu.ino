#include <Adafruit_AHRS.h>
#include <Adafruit_AHRS_FusionInterface.h>
#include <Adafruit_AHRS_Madgwick.h>
#include <Adafruit_AHRS_Mahony.h>
#include <Adafruit_AHRS_NXPFusion.h>
#include <Adafruit_Sensor_Set.h>
#include <Adafruit_Simple_AHRS.h>

#include <MahonyAHRS.h>

#include <Wire.h>
const int pin_reset = 8;
const float hard_iron[3] = {
  2.26,  -6.29,  -4.97
};

// Soft-iron calibration settings
const float soft_iron[3][3] = {
  {  0.981,  0.010, 0.028},
  {  0.010,  1.010, -0.000  },
  { 0.028, -0.000,  1.010  }
};
// THE ONE THAT WORKS NOT THE OTHER 
const int mpuRegister = 0x68;
const int mpuResetRegister = 0x6B;
const int mpuGyroConfigRegister = 0x1B;
const int mpuAccConfigRegister = 0x1C;
const int mpuGyroRegister = 0x43;
const int mpuAccRegister = 0x3B;
const int hmcRegister = 0x1E;
const int hmcModeRegister = 0x02;
const int hmcMagRegister = 0x03;
const float mag_decl = 0;//-16.35;
const float factor = 0.0000611; //0.00010687022 7000 serial else 0.0000611 4000 no serial

float xOff, yOff, zOff;
float xCal, yCal, zCal;
float hmcX, hmcY, hmcZ;
float xComp, yComp;
float heading;
//float  test_heading;

long accX, accY, accZ;
long accVector;
int gyroX, gyroY, gyroZ;
float gyroXOffset, gyroYOffset, gyroZOffset;
float pitch, roll;
float accPitch, accRoll;
bool isGyroCalibrated = false;
bool isStart = true;
float filteredPitch, filteredRoll;
#define FILTER_UPDATE_RATE_HZ 100
unsigned long loopTimer;
const float pi = 3.14159265358979323;
Adafruit_Mahony filter; 
void readHmc(){
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcMagRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(hmcRegister, 6, true);
  hmcX = Wire.read() << 8| Wire.read();
  hmcZ = Wire.read() << 8| Wire.read();
  hmcY = Wire.read() << 8| Wire.read();
}


void readGyro(){
  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuGyroRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuRegister, 6, true);
  if (!isGyroCalibrated){
    gyroX = (Wire.read() << 8|Wire.read());
    gyroY = (Wire.read() << 8|Wire.read());
    gyroZ = (Wire.read() << 8|Wire.read());
  }
  else{
    gyroX = ((Wire.read() << 8|Wire.read()) - gyroXOffset);
    gyroY = ((Wire.read() << 8|Wire.read()) - gyroYOffset);
    gyroZ = ((Wire.read() << 8|Wire.read()) - gyroZOffset);    
  }
}


void readAcc(){
  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuAccRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuRegister, 6, true);
  accX = (Wire.read() << 8|Wire.read());
  accY = (Wire.read() << 8|Wire.read());
  accZ = (Wire.read() << 8|Wire.read());
}


void calibrateGyro(){
  long int start = millis();
  int count = 0;
  while(millis() < start+5000){
    readGyro();
    gyroXOffset += gyroX;
    gyroYOffset += gyroY;
    gyroZOffset += gyroZ;
    count += 1;
  }
  gyroXOffset /= count;
  gyroYOffset /= count;
  gyroZOffset /= count;
  isGyroCalibrated = true;
}


void setup(){
 Serial.begin(115200);
 filter.begin(FILTER_UPDATE_RATE_HZ);
  Wire.begin();
  Wire.beginTransmission(mpuRegister);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuGyroConfigRegister);
  Wire.write(0x01);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuAccConfigRegister);
  Wire.write(0x10);
  Wire.endTransmission(true);

  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcModeRegister);
  Wire.write(0x00);
  Wire.endTransmission(true);

  calibrateGyro();
  loopTimer = micros();
}

void loop(){


  
  static float hi_cal[3];
  static float heading = 0;

 readHmc();
  
  hmcX /= 10.90;
  hmcZ /= 10.90;
  hmcY /= 10.90;
  
  // Put raw magnetometer readings into an array
  float mag_data[] = {hmcX,
                      hmcY,
                      hmcZ};

  // Apply hard-iron offsets
  for (uint8_t i = 0; i < 3; i++) {
    hi_cal[i] = mag_data[i] - hard_iron[i];
  }

  // Apply soft-iron scaling
  for (uint8_t i = 0; i < 3; i++) {
    mag_data[i] = (soft_iron[i][0] * hi_cal[0])+
                  (soft_iron[i][1] * hi_cal[1])+ 
                  (soft_iron[i][2] * hi_cal[2]);
  }

  xCal = mag_data[0];
  yCal = mag_data[1];
  zCal = mag_data[2];
    readGyro();
  readAcc();
filter.update(gyroX, gyroY, gyroZ, 
                accX, accY, accZ, 
                xCal, yCal, zCal);
  byte byte1 = (int) heading >> 8;
  byte byte2 = (int) heading & 0xff;
  Wire.beginTransmission(8);
  Wire.write(byte1);
  Wire.write(byte2);
  Wire.endTransmission(true);

 Serial.print("Raw: ");
  Serial.print(accX, 4); Serial.print(", ");
  Serial.print(accY, 4); Serial.print(", ");
  Serial.print(accZ, 4); Serial.print(", ");
  Serial.print(gyroX, 4); Serial.print(", ");
  Serial.print(gyroY, 4); Serial.print(", ");
  Serial.print(gyroZ, 4); Serial.print(", ");
  Serial.print(xCal, 4); Serial.print(", ");
  Serial.print(yCal, 4); Serial.print(", ");
  Serial.print(zCal, 4); Serial.println("");

  while(micros() - loopTimer < 4000);
  loopTimer = micros();
}
