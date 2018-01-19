#include<Wire.h>
 /* Inclut la lib Servo pour manipuler le servomoteur */
#include <Servo.h>
/* Créer un objet Servo pour contrôler le servomoteur */
Servo monServomoteur, servo2;
    const int MPU_addr=0x68;  // I2C address of the MPU-6050
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    void setup(){
      Wire.begin();
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0);     // set to zero (wakes up the MPU-6050)
      Wire.endTransmission(true);
      Serial.begin(9600);
      //pinMode(9,OUTPUT);
      //digitalWrite(9,HIGH);
      Serial1.begin(9640);// démarre la voie série à la vitesse 9600 baud

        // Attache le servomoteur à la broche D9
        monServomoteur.attach(9);
        servo2.attach(10);
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
    //  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    //  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
     // GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
      /*Serial.print("AcX = "); Serial.print(AcX);
      Serial.print(" | AcY = "); Serial.print(AcY);
      Serial.print(" | AcZ = "); Serial.print(AcZ);
      Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
      Serial.print(" | GyX = "); Serial.print(GyX);
      Serial.print(" | GyY = "); Serial.print(GyY);
      Serial.print(" | GyZ = "); Serial.println(GyZ);*/
      sendData(AcX,AcY,AcZ);
      int value1 = constrain(AcX,-180,180);
      value1= map (value1,-180,180,0,180);
      monServomoteur.write(value1);
      int value2 = constrain(AcY,-180,180);
      value2=map(value2,-180,180,0,180);
      servo2.write(value2);

      delay(5);
      
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
    void sendData(int16_t X, int16_t Y, int16_t Z){
      String data;
      if(Serial.available()){
        data = (String)X;
        data += '-'+(String)Y;
        data += '-'+(String)Z;
        data += '\n';
        Serial1.println(data);
        Serial.println(data);
      }
      while(Serial1.available()){
        Serial.print((char)Serial1.read());
      }
    }
