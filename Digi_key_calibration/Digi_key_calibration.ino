/**
 * Compass Demo
 * 
 * Print heading (in degrees) to attached I2C OLED display. Demonstrate
 * how to use magnetometer calibration data and convert magnetic heading
 * to geographic heading.
 * 
 * Author: Shawn Hymel
 * Date: May 5, 14
 * 
 * License: 0BSD (https://opensource.org/licenses/0BSD)
 */
#include <Wire.h>

#include <Adafruit_Sensor.h>
const int pin_reset = 8;

// Hard-iron calibration settings
const float hard_iron[3] = {
  2.26,  -6.29,  -4.97
};

// Soft-iron calibration settings
const float soft_iron[3][3] = {
  {  0.981,  0.010, 0.028},
  {  0.010,  1.010, -0.000  },
  { 0.028, -0.000,  1.010  }
};

// Magnetic declination from magnetic-declination.com
// East is positive ( ), west is negative (-)
// mag_decl = ( /-)(deg   min/60   sec/3600)
// Set to 0 to get magnetic heading instead of geo heading
const float mag_decl = 0;//-16.35;

const int hmcRegister = 0x1E;
const int hmcModeRegister = 0x02;
const int hmcMagRegister = 0x03;

float xOff, yOff, zOff;
float xCal, yCal, zCal;
float hmcX, hmcY, hmcZ;

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
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(hmcRegister);
  Wire.write(hmcModeRegister);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop() {

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

  // Calculate angle for heading, assuming board is parallel to
  // the ground and  Y points toward heading.
  heading = -1 * (atan2(mag_data[0], mag_data[1]) * 180) / M_PI;

  // Apply magnetic declination to convert magnetic heading
  // to geographic heading  
  heading  += mag_decl;

  // Convert heading to 0..360 degrees
  if (heading < 0) {
    heading  += 360;
  }

  // Print calibrated results
  // Serial.print("[");
  // Serial.print(mag_data[0], 1);
  // Serial.print("\t");
  // Serial.print(mag_data[1], 1);
  // Serial.print("\t");
  // Serial.print(mag_data[2], 1);
  // Serial.print("] Heading: ");
  Serial.println(heading, 0);

  // hmcX = mag_data[0];
  // hmcY = mag_data[1];
  // hmcZ = mag_data[2];

  // Serial.print("Raw:0,0,0,0,0,0,");
  // Serial.print(int(hmcX*10)); Serial.print(","); Serial.print(int(hmcY*10)); Serial.print(","); Serial.println(int(hmcZ*10));
  // Serial.print("Uni:0.00,0.00,0.00,0.0000,0.0000,0.0000,");
  // Serial.print(hmcX); Serial.print(","); Serial.print(hmcY); Serial.print(","); Serial.println(hmcZ);

  //delay(100); 
}