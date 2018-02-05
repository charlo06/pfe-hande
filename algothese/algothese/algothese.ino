#include <Wire.h>
#include <Kalman.h>
#include <Servo.h>

#define RESTRICT_PITCH
Kalman kalmanX;
Kalman kalmanY;
Servo myServo;

//#define RESTRIC_PITCH // Comment out to restrist roll to +/- 90 degrees.

//IMU Data
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int 16_t tempRaw; //TempRaw is a 16-bit integer

double gyroXangle, gyroYangle; //Angle calculated using Gyroscope only
double kalAngleX, kalAngleY; //Angle calculated averages

//From pot frequency average
const int numReadings = 12; //Number of raw value readings to take
const int avgReadingsX = 3; //Stores calculated averages
const int avgReadingsY = 3;
const int freqNumReadingsX = 12; //Number of frequency to take
const int freqNumReadingsY = 12;

//Kalman X angle
int freqReadingsX[freqNumReadingsX];
int freqReadIndexX = 0;
int freqTotalX = 0;
int frequencyX[freqnumberReadingsX];
int freqAverageX;
float valueDifX;
int rawValue;
int readings[numReadings];
int readingsX[numReadings]; //The readings from analog input
int readIndexX= 0; //The index of the current reading
int totalX = 0; //The running total
int averageReadIndexX = 0;
int averageX[avgReadingsX];
float movementValue1X = 0.85;
float movementValue2X = -0.85;
int movementX = false;
unsigned long trueTime;
unsigned long previousTimeX = 0;
unsigned long elapsedTimeX = 0;
double interval = 1000;

// KALMAN Y ANGLE
int freqReadingsY[freqNumReadingsY];
int freqReadIndexY = 0;
int freqTotalY = 0;
int frequencyY[freqnumberReadingsY];
int freqAverageY;
float valueDifY;
int readingsY[numReadings]; //The readings from analog input
int readIndexY= 0; //The index of the current reading
int totalY = 0; //The running total
int averageReadIndexY = 0;
int averageY[avgReadingsY];
float movementValue1Y = 0.85;
float movementValue2Y = -0.85;
int movementY = false;
unsigned long previousTimeY = 0;
unsigned long elapsedTimeY = 0;

//LED TESTING
int ledPin = 3;
int ledPinValue = 0;
int led2Pin = 5;
int led2PinValue = 0;

//SERVO TESTING
int servoValue;
int servoValue2;
int cali = 0;

uint32_t timer; //unsigned 32-bit integer
uint8_t i2cData[14]; //Buffer for I2C data. Total array of 14 Registers. Unsigned 8-bit integer


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
