#include <Wire.h>
const int pin_reset = 8;
const float hard_iron[3] = {
  6,  -9.79,  -12.5
};

// Soft-iron calibration settings
const float soft_iron[3][3] = {
  {  1.002,  0.008, 0.024},
  {  0.008,  0.988, 0.002  },
  { 0.024, 0.002,  1.010  }
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
const float mag_decl = 0;        //-16.35;
const float factor = 0.0000611;  //0.00010687022 7000 serial else 0.0000611 4000 no serial

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

unsigned long loopTimer;
const float pi = 3.14159265358979323;

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


void readGyro() {
  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuGyroRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuRegister, 6, true);
  if (!isGyroCalibrated) {
    gyroX = fixValue((Wire.read() << 8 | Wire.read()));
    gyroY = fixValue((Wire.read() << 8 | Wire.read()));
    gyroZ = fixValue((Wire.read() << 8 | Wire.read()));
  } else {
    gyroX = fixValue((Wire.read() << 8 | Wire.read())) - gyroXOffset;
    gyroY = fixValue((Wire.read() << 8 | Wire.read())) - gyroYOffset;
    gyroZ = fixValue((Wire.read() << 8 | Wire.read())) - gyroZOffset;
  }
}


void readAcc() {
  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuAccRegister);
  Wire.endTransmission(false);
  Wire.requestFrom(mpuRegister, 6, true);
  accX = fixValue((Wire.read() << 8 | Wire.read()));
  accY = fixValue((Wire.read() << 8 | Wire.read()));
  accZ = fixValue((Wire.read() << 8 | Wire.read()));
}


void calibrateGyro() {
  long int start = millis();
  int count = 0;
  while (millis() < start + 5000) {
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


void setup() {
  Serial.begin(115200);
   Wire.setSDA(20);
    Wire.setSCL(21);
  Wire.begin();
  Wire.beginTransmission(mpuRegister);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Wire.beginTransmission(mpuRegister);
  Wire.write(mpuGyroConfigRegister);
  Wire.write(0x08);
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

void loop() {
  readGyro();

  pitch += gyroX * factor;
  roll += gyroY * factor;

  pitch += roll * sin(gyroZ * 0.00000106639);  //0.00000932618 old 7000 value else new (seems to be working) 0.00000186523 7000 else 0.00000106639 4000 by joop brooking (1/(1000/4)/65.5)
  roll -= pitch * sin(gyroZ * 0.00000106639);

  readAcc();

  accVector = sqrt((accX * accX) + (accY * accY) + (accZ * accZ));
  accPitch = asin((float)accY / accVector) * 57.296;
  accRoll = asin((float)accX / accVector) * -57.296;

  accPitch += 1.25;
  accRoll += 6.4;

  if (isStart) {
    pitch = accPitch;
    roll = accRoll;
    isStart = false;
  } else {
    pitch = pitch * 0.9996 + accPitch * 0.0004;
    roll = roll * 0.9996 + accRoll * 0.0004;
  }

  filteredPitch = filteredPitch * 0.9 + pitch * 0.1;
  filteredRoll = filteredRoll * 0.9 + roll * 0.1;

  static float hi_cal[3];
  static float heading = 0;

  readHmc();

  hmcX /= 10.90;
  hmcZ /= 10.90;
  hmcY /= 10.90;

  // Put raw magnetometer readings into an array
  float mag_data[] = { hmcX,
                       hmcY,
                       hmcZ };

  // Apply hard-iron offsets
  for (uint8_t i = 0; i < 3; i++) {
    hi_cal[i] = mag_data[i] - hard_iron[i];
  }

  // Apply soft-iron scaling
  for (uint8_t i = 0; i < 3; i++) {
    mag_data[i] = (soft_iron[i][0] * hi_cal[0]) + (soft_iron[i][1] * hi_cal[1]) + (soft_iron[i][2] * hi_cal[2]);
  }

  xCal = mag_data[0];
  yCal = mag_data[1];
  zCal = mag_data[2];


  xComp = xCal * cos(pitch * 0.0174533) + yCal * sin(roll * 0.0174533) * sin(pitch * 0.0174533) + zCal * cos(roll * 0.0174533) * sin(pitch * 0.0174533);
  yComp = yCal * cos(roll * 0.0174533) + zCal * sin(roll * 0.0174533);

  heading = 1 * (atan2(xComp, yComp) * 180) / M_PI;
  heading += mag_decl;
  if (heading < 0) {
    heading += 360;
  }

  /*
  test_heading = atan2(yCal, xCal) * (180/pi); // non compensated
  test_heading += 360;
  if (test_heading > 360){
    test_heading -= 360;
  }
  */
  byte byte1 = (int)heading >> 8;
  byte byte2 = (int)heading & 0xff;
  // rp2040.fifo.push(heading);
  // Wire.beginTransmission(8);
  // Wire.write(byte1);
  // Wire.write(byte2);
  // Wire.endTransmission(true);
  Serial.println(heading);

  while (micros() - loopTimer < 4000);
  Serial.println(micros() - loopTimer);
  loopTimer = micros();
}

// void setup1(){
//   Serial.begin(115200);
// }

// void loop1(){
//   if(rp2040.fifo.available()){
//     Serial.begin(rp2040.fifo.pop());
//   }
// }
