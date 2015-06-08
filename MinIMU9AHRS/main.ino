#include <Wire.h>
#include <Servo.h>
#include <math.h>



const int mCount = 4;
Servo motors[mCount];

int ports [mCount] = {6, 7, 8, 9}; //Port of each servo
byte motorPos [mCount];

void setup() {
  
  
  for (int i = 0; i < mCount; ++i) {
    motors [i].attach (ports [i]);
    motorPos [i] = 0;
    motors [i].write (motorPos [i]);
  }
  //Setup serial connection
  Serial.begin(115200);
  Serial.setTimeout(2);
  
  delay(3000);
  
  for (int i = 0; i < mCount; ++i) {
    motorPos [i] = 50;
    motors [i].write (motorPos[i]);
  }

  
  //pololuSetup();
  //attachRPMSensors();
  
  delay(2000);

}

//Read commands and set the motors
void loop() {
  
  //noInterrupts();
  receiveData();
  receiveData();

  //interrupts();
  sensors();
}

void sensors() {
  
  distanceSensor();
  getVoltage();
  //getRPM();
  //pololuLoop();
  
  for (int i = 0; i < 4; ++i) {
  Serial.write(0);
  Serial.write(motors [i].read());
  receiveData();
  }
  delay(2);
  
  sendTelemetry();
  
  
  if (cm >= 150)
  {
   delay(16); 
  }
  else if (cm >= 50)
  {
   delay(22); 
  }
  else if (cm >= 1)
  {
   delay(32); 
  }
  else
  {
   delay(16); 
  }
}

void receiveData() {
  if (Serial.available()) {      
    for (int i = 0; i < mCount; ++i) {
            motorPos [i] = Serial.read();
            motors [i].write (motorPos[i]);
    }
  lastSignalTime = millis();
  }       
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (motorPos [i]);
  }
  
  if (millis() - lastSignalTime > 3000) {
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (0); 
    }
  }
}


