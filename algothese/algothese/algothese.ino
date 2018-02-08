#include <Wire.h>
#include <Kalman.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define ATTENTE 500

#define RESTRICT_PITCH
Kalman kalmanX;
Kalman kalmanY;
Servo servoX;
Servo servoY;
Servo servoPince;
int compteur;

//#define RESTRIC_PITCH // Comment out to restrist roll to +/- 90 degrees.

//IMU Data
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw; //TempRaw is a 16-bit integer

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
int frequencyX[freqNumReadingsX];
int freqAverageX;
float valueDifX;
int rawValue;
int readings[numReadings];
int readingsX[numReadings]; //The readings from analog input
int readIndexX= 0; //The index of the current reading
int totalX = 0; //The running total
int avgReadIndexX = 0;
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
int frequencyY[freqNumReadingsY];
int freqAverageY;
float valueDifY;
int readingsY[numReadings]; //The readings from analog input
int readIndexY= 0; //The index of the current reading
int totalY = 0; //The running total
int avgReadIndexY = 0;
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
int servoXvalue;
int servoYvalue;
int cali = 0;

int pinBouton;
int pos;

uint32_t timer; //unsigned 32-bit integer
uint8_t i2cData[14]; //Buffer for I2C data. Total array of 14 Registers. Unsigned 8-bit integer


void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000UL); //Set I2C frequency to 400kHz
  Serial.println("Arduino is ready");
  Serial1.begin(9600);  


  for(int thisReading =0; thisReading< numReadings; thisReading++){
    readings[thisReading]=0;
    readingsX[thisReading]=0;
    readingsY[thisReading]=0;
  }

  //SERVO TESTING
<<<<<<< HEAD
  servoX.attach(8);
  servoY.attach(9);
  servoPince.attach(13);
  pinMode(4,INPUT);
=======
  servoX.attach(9);
  servoY.attach(8);

>>>>>>> a6bb552d7eb9e292a46b4f2ad61add4959f88466
  i2cData[0] =7; //Set the sample rate to 1000Hz - 8kHz/(7+1) =1000Hz
  i2cData[1] = 0x00; //Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro Filtering, 8kHz sampling;
  //the fsync() function is intended to force a physical write of data from the buffer cache, and to assure that after a system crash or other failure that all data up to the time of the fsync() call is recorded on the disk
  i2cData[2] = 0x00; //Set Gyro Full Scale Range to +250 deg/s
  i2cData[3] = 0x00; //Set Accelerometer Full Scale Range to +2g
  while(i2cWrite(0x19, i2cData, 4,false));//Write to all four register at once
  while(i2cWrite(0x6B, 0x01, true)); //PLL with x axis gyroscope reference and disable sleep mode

  while(i2cRead(0x75, i2cData, 1));
  if(i2cData[0] != 0x68){
    Serial.print(F("Error reading sensor"));
    while(1);
  }

  delay(100); //Wait for sensor to stabilize

  //Set kalmen and gyriscope starting angle
  while(i2cRead(0x3B, i2cData, 6));
  accX = (i2cData[0] << 8 | i2cData[1]); //Bitewise operators *|*, (Bitewise OR) and <<, (Shift Value Left).
  accY = (i2cData[2] << 8 | i2cData[3]); //Reference http://playground.arduino.cc/Code/BitMath
  accZ = (i2cData[4] << 8 | i2cData[5]);

  #ifdef RESTRICT_PITCH
  double roll = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  #else
  double roll = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ))*RAD_TO_DEG;
  #endif

  kalmanX.setAngle(roll); //Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;

  timer=micros();
    
}

void loop() {
  // update all values
  while(i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = ((i2cData[6] << 8) | i2cData[7]);
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);

  double dt = (double)(micros()-timer)/100000; //Calculates the delta time
  timer = micros();

  #ifdef RESTRICT_PITCH
    double roll = atan2(accY, accZ)* RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  #else
    double pitch = atan2(-accX, -accZ)* RAD_TO_DEG;
    double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  #endif

    double gyroXrate = gyroX/131.0;
    double gyroYrate = gyroY/131.0;

    #ifdef RESTRICT_PITCH
      if((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)){
        kalmanX.setAngle(roll);
        kalAngleX=roll;
        gyroXangle = roll;
      }
      else{
      kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); //Calculate angle using Kalman filter
      }

      if(abs(kalAngleX > 90)){
        gyroYrate = - gyroYrate; //Invert gyro rate to fit the restricted accelerometer reading
      }
      kalAngleY=kalmanY.getAngle(pitch, gyroYrate, dt);

    #else
    if((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)){
      kalmanY.setAngle(pitch);
      compAngleY = pitch;
      kalAngleY = pitch;
      gyroYangle = pitch;
    }
    else
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); //Calculate the angle using Kalman filter

   if(abs(kalAngleY) > 90)
      gyroXrate = -gyroXrate;

   kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); //Calculate angle using Kalman filter
   #endif

   gyroXangle += gyroXrate * dt; //Calculate gyro angle without any filter
   gyroYangle += gyroYrate * dt;

   //Reset the gyroscope angle when it drifts too far
   if (gyroXangle < -180 || gyroXangle >180)
    gyroXangle = kalAngleX;
   if( gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;

   //FREQUENCY CALCULATIONS KALMAN Y

   //Change in value
   valueDifY=( averageY[1] - averageY[2]);

   //substract the last reading:
   // rawValue = anologRead(inputPin);
   totalY = totalY - readingsY[readIndexY];
   readingsY[readIndexY] = kalAngleY;
   //add the reading to the total
   totalY = totalY + readings[readIndexY];
   readIndexY = readIndexY + 1;

   //if we're at the end
   if(readIndexY >= numReadings) {
    readIndexY = 0;
   }

   //calculate the average:
   averageY[avgReadIndexY] = totalY/numReadings;
   avgReadIndexY = avgReadIndexY +1;
   if(avgReadIndexY >= avgReadingsY){
    avgReadIndexY =0;
   }

   //Detecting Change
   unsigned long trueTime = millis();
   elapsedTimeY = trueTime - previousTimeY;

   if(valueDifY > movementValue1Y || valueDifY <movementValue2Y){
    movementY =true;
    previousTimeY = trueTime;
   }
   else (movementY = false);

   //AVERAGE FREQUENCY

   //substract the last reading
   freqTotalY = freqTotalY - freqReadingsY[freqReadIndexY];
   //get frequency values
   freqReadingsY[freqReadIndexY]=interval /elapsedTimeY;
   //Add the reading to the total
   freqTotalY = freqTotalY + freqReadingsY[freqReadIndexY];
   //advance to the next position in the array
   freqReadIndexY= freqReadIndexY +1;
   //if at the end of the array
   if(freqReadIndexY >= freqNumReadingsY){
    freqReadIndexY =0;
   }

   //Calculate the average frequency
   freqAverageY = freqTotalY / freqNumReadingsY;
   
   //FREQUENCY KALMANX
   valueDifX = (averageX[1] - averageX[2]);
   totalX = totalX - readingsX[readIndexX];
   readingsX[readIndexX] = kalAngleX;
   totalX = totalX + readingsX[readIndexX];
   readIndexX = readIndexX +1;

   if (readIndexX >= numReadings){
    readIndexX=0;
   }

   averageX[avgReadIndexX] = totalX /numReadings;
   avgReadIndexX = avgReadIndexX +1;
   if(avgReadIndexX >= avgReadingsX){
    avgReadIndexX=0;
   }

   //Detecting Ch ange
   elapsedTimeX = trueTime - previousTimeX;

   if(valueDifX > movementValue1X || valueDifX <movementValue2X){
    movementX =true;
    previousTimeX = trueTime;
   }
   else (movementX = false);

   //AVERAGE FREQUENCY

   //substract the last reading
   freqTotalX = freqTotalX - freqReadingsX[freqReadIndexX];
   //get frequency values
   freqReadingsX[freqReadIndexX]=interval /elapsedTimeX;
   //Add the reading to the total
   freqTotalX = freqTotalX + freqReadingsX[freqReadIndexX];
   //advance to the next position in the array
   freqReadIndexX= freqReadIndexX +1;
   //if at the end of the array
   if(freqReadIndexX >= freqNumReadingsX){
    freqReadIndexX =0;
   }
    //Calculate the average frequency
   freqAverageX = freqTotalX / freqNumReadingsX;

   int freqTotAvg = (freqAverageX + freqAverageY )/2;

<<<<<<< HEAD
   String envoie;
   if(freqTotAvg >= 12){
=======
   if(freqTotAvg >=12){
>>>>>>> a6bb552d7eb9e292a46b4f2ad61add4959f88466
    servoXvalue = map(kalAngleX, -180,180, 50,130);
    servoX.write(servoXvalue );
    servoYvalue = map(kalAngleY, 180,-180,50,130);
    servoY.write(servoYvalue );
    if(compteur == 30){
    envoie = (String)servoYvalue+"/"+(String)servoXvalue+"/"+"1/";
    Serial1.print(envoie);
    Serial1.print("#");
    compteur = 0;
    }
}
  else{
    if(compteur == 30){
    envoie = (String)servoYvalue+"/"+(String)servoXvalue+"/"+"0/";
    Serial1.print(envoie);
    Serial1.print("#");
    compteur = 0;
    }
  }
  compteur++;

  boolean etatBouton=digitalRead(pinBouton);
   pos = 0;
   
   if(etatBouton == 1){
      //Ferme la pince
      monServomoteur.write(65);
  }
 if(etatBouton==0){
  monServomoteur.write(0);
  }
}
