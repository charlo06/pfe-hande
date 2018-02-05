#include<Wire.h>
 /* Inclut la lib Servo pour manipuler le servomoteur */
#include <Servo.h>

/* Créer un objet Servo pour contrôler le servomoteur */
Servo servoX, servoY, servoZ;
    const int MPU_addr=0x68;  // I2C address of the MPU-6050
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    int initX, initY,initZ;
    void setup(){
      Wire.begin();
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0);     // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      Serial.begin(9600);

        // Attache le servomoteur à la broche D9
        servoX.attach(9);
        servoY.attach(10);
        servoZ.attach(8);
       // monServomoteur.attach(9);
       // servo2.attach(10);

        Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
      initX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
      initY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      initZ=Wire.read()<<8|Wire.read();

      servoX.write(90);
      servoY.write(90);
      servoZ.write(90);
    }
    void loop(){
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
      AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
      AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   //   Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

      //AcX= AcX -initX;
      AcY= AcY -300;
      AcZ= AcZ+1500;

      GyX =(GyX + 3380)/10;
      GyY = (GyY - 990)/10;
      GyZ = (GyZ  +210)/10;
      Serial.print("AcX = "); Serial.print(AcX);
      Serial.print(" | AcY = "); Serial.print(AcY);
      Serial.print(" | AcZ = "); Serial.print(AcZ);
      Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
      Serial.print(" | GyX = "); Serial.print(GyX);
      Serial.print(" | GyY = "); Serial.print(GyY);
      Serial.print(" | GyZ = "); Serial.println(GyZ);
      int value1 = constrain(GyY,-180,180);
      value1= map (value1,-200,200,60,120);
      servoY.write(value1);
      int value2 = constrain(GyZ,-180,180);
      value2=map(value2,200,-200,60,120);
      servoX.write(value2);

      delay(50);
      
        // Fait bouger le bras de 0° à 180°
     /* for (int position = 0; position <= 180; position++) {
        monServomoteur.write(position);
        servo2.write(position);
        delay(10);
      }
      
      // Fait bouger le bras de 180° à 10°
      for (int position = 180; position >= 0; position--) {
        monServomoteur.write(position);
        servo2.write(position);
        delay(10);
      }*/
    }
