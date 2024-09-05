#include "Telemetry.h"
#include "IMU.h"
#include "gps.h"
#include "reciever.h"
#include <TinyGPS++.h>
#include <Servo.h>

Servo steeringOne;
Servo steeringTwo;
Servo motor;

Telemetry telemetry;
IMU imu;
GPS gps_setup;
TinyGPSPlus gps;
Reciever reciever;

double heading;
double bearing;

double destinationLatitude = 45.942483;
double destinationLongitude = -66.696411;

double P, I, D;
double pValue, iValue, dValue = 0;
double maxPidValue = 360;
double pidOutput;
double error;
double previousError = 0;
bool startPid = false;

int servoOutput = 0;
int speedOutput = 0;
int servoOffset = 0;

long long timer = micros();

void setup()
{
    Serial.begin(115200);
    imu.begin();
    telemetry.begin();

}
void loop()
{
   heading = imu.getHeading();
   bearing = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), destinationLatitude, destinationLongitude);
    //telemetry.sendTelemetry(69, gps.location.lat(), gps.location.lng(), bearing, heading);
  
    telemetry.update();
    switch(telemetry.latestData.identifier){
    case 0:
      break;
    case 1:
      P = telemetry.latestData.data;
      break;
    case 2:
      I = telemetry.latestData.data;
      break;
    case 3:
      D = telemetry.latestData.data;
      break;
    case 4:
      if (telemetry.latestData.data == 1){
        startPid = true;
      }
      else{
        startPid = false;
      }
      break;
    case 5:
      destinationLatitude = telemetry.latestData.data;
      break;
    case 6:
      destinationLongitude = telemetry.latestData.data;
      break;
    case 7:
      telemetry.sendBasicTransmitStruct(1, pValue);
      telemetry.sendBasicTransmitStruct(2, iValue);
      telemetry.sendBasicTransmitStruct(3, dValue);
      telemetry.sendBasicTransmitStruct(4, pidOutput);
      telemetry.sendBasicTransmitStruct(5, gps.location.lat());
      telemetry.sendBasicTransmitStruct(6, gps.location.lng());
      telemetry.sendBasicTransmitStruct(7, heading);
      telemetry.sendBasicTransmitStruct(8, bearing);
      break;
    case 8:
      bearing = telemetry.latestData.data;
      break;
    case 9:
      servoOffset = telemetry.latestData.data;
      break;
  }

  telemetry.clear();
    
    if (startPid){
      error = heading - bearing;
      if (abs(error) > 180){
        int temp = 360 - abs(error);
        if (error > 0){
          error = -1 * temp;
        }
        else{
          error = temp;
        }
      }

      iValue += I * error;
      if (iValue > maxPidValue) iValue = maxPidValue;
      else if (iValue < maxPidValue * -1) iValue = maxPidValue * -1;

      pValue = P * error;
      dValue = D * (error - previousError);

      pidOutput = pValue + iValue + dValue;
      if (pidOutput > maxPidValue) pidOutput = maxPidValue;
      else if (pidOutput < maxPidValue * -1) pidOutput = maxPidValue * -1;

      previousError = error;
    }
    

    
}

void setup1(){
  steeringOne.attach(A0);
  steeringTwo.attach(A1);
  motor.attach(A2);
  reciever.begin();
  gps_setup.begin();
}

void loop1(){
    long long loopTimer = millis();
  if (millis() - timer > 1000){
    timer = millis();
    telemetry.sendBasicTransmitStruct(1, pValue);
    telemetry.sendBasicTransmitStruct(2, iValue);
    telemetry.sendBasicTransmitStruct(3, dValue);
    telemetry.sendBasicTransmitStruct(4, pidOutput);
    telemetry.sendBasicTransmitStruct(5, gps.location.lat());
    telemetry.sendBasicTransmitStruct(6, gps.location.lng());
    telemetry.sendBasicTransmitStruct(7, heading);
    telemetry.sendBasicTransmitStruct(8, bearing);
  Serial.println(millis() - loopTimer);
  }
  reciever.read();
    while (Serial2.available())
    {
    gps.encode(Serial2.read());
    }
    if (reciever.rxFrameDone){
      if (reciever.channelValues[9] > 1100){
        servoOutput = map(pidOutput, -360, 360, 170, 10);
        servoOutput -= servoOffset;
      }
      else{
        servoOutput = map(reciever.channelValues[0], 900, 2100, 0, 180);
      }
      speedOutput = map(reciever.channelValues[1], 900, 2100, 0, 180);
    }
    steeringOne.write(servoOutput);
    steeringTwo.write(servoOutput);
    motor.write(speedOutput);
}