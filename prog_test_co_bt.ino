#include <SoftwareSerial.h>



SoftwareSerial mySerial(11, 10); //Mettre le num des pin où sont branchés Rx et Tx

void setup() {
  // put your setup code here, to run once:
  pinMode(11, INPUT);
  pinMode(10, OUTPUT);
 
  mySerial.begin(38400);// démarre la voie série à la vitesse 9600 baud
  Serial.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly
  int i = 0;
  char someChar[32]={0};

  if(Serial.available()){
    do{
      someChar[i++]= Serial.read();
      delay(3);
    }while (Serial.available() > 0);
    mySerial.println(someChar);
    Serial.println(someChar);
  }
  while(mySerial.available()){
    Serial.print((char)mySerial.read());
  }

}
