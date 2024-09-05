#ifndef CONFIG_H
#define CONFIG_H

/**************************
 * TELEMETRY MODULE SETUP *
 **************************/

/******* NRF24L01 module setup *******/

#define NRF24L01
const int NRF24L01_CE = 6;
const int NRF24L01_CSN = 7;
const int NRF24L01_INTERRUPT_PIN = 2;
const byte ADDRESS[6] = "00001";
const byte ADDRESS2[6] = "00002";

/******* IMU setup *******/

/******* MPU setup *******/
#define MPU6050_IMU

const float HARD_IRON[3] = {
    0.09, -7.90, -17.92};
const float SOFT_IRON[3][3] = {
    {1.007, 0.007, 0.028},
    {0.007, 0.988, -0.005},
    {0.028, -0.005, 1.006}};
const int GYRO_OFFSET[3] = {
    -24, 12, 47};
const int ACCEL_OFFSET[3] = {
    -487, 1502, 1003};

/******* HMC setup *******/
#define HMC5883L_IMU
const int DECLINATION = -16.32;
const int HMC_REGISTER = 0x1E;
const int HMC_MODE_REGISTER = 0x02;
const int HMC_MAG_REGISTER = 0x03;

/******* GPS setup *******/

/******* RECIEVER setup *******/
#define IBUS_BUFFSIZE 32
#define IBUS_MAXCHANNELS 10

/*********************************
 * MODIFY SPI INTERFACE SETTINGS *
 * -> Intended for RP2040        *
 *********************************/

#define CUSTOM_SPI
const int SPI_CLK = 18; // SCK
const int SPI_TX = 19;  // MOSI
const int SPI_RX = 20;  // MISO

#define CUSTOM_I2C
const int SCL_PIN = 9;
const int SDA_PIN = 8;

#define CUSTOM_SERIAL_GPS
const int GPS_TX_PIN = 4;
const int GPS_RX_PIN = 5;

#define CUSTOM_SERIAL_RECIEVER
const int RECIEVER_TX_PIN = 0;
const int RECIEVER_RX_PIN = 1;

#endif