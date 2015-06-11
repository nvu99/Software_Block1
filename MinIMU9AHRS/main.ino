#include <Wire.h>
#include <Servo.h>
#include <math.h>



const int mCount = 4;
Servo motors[mCount];

unsigned int ports [mCount] = {6, 7, 8, 9}; //Port of each servo
unsigned int motorPos [mCount];

void setup() {
  
  
  for (int i = 0; i < mCount; ++i) {
    motors [i].attach (ports [i]);
    motorPos [i] = 0;
    motors [i].write (motorPos [i]);
  }
  //Setup serial connection
  Serial.begin(115200);
  Serial.setTimeout(8);
  
  delay(3000);
  
  for (int i = 0; i < mCount; ++i) {
    motorPos [i] = 50;
    motors [i].write (motorPos[i]);
  }

  
  pololuSetup();
  //attachRPMSensors();
  
  delay(2000);

}

//Read commands and set the motors
void loop() {
  
  receiveData();

  sensors();
}

void sensors() {
  
  distanceSensor();
  getVoltage();
  //getRPM();
  pololuLoop();
  
  for (int i = 0; i < 4; ++i) {
  Serial.write(motors [i].read() >> 8);
  Serial.write(motors [i].read() & mask);
  receiveData();
  }
  
  sendTelemetry();
  
  
  if (cm >= 150)
  {
   delay(15); 
  }
  else if (cm >= 50)
  {
   delay(23); 
  }
  else if (cm >= 1)
  {
   delay(30); 
  }
  else
  {
   delay(23); 
  }
}

void receiveData() {
  if (Serial.available()) {      
    for (int i = 0; i < mCount; ++i) {
            motorPos [i] = Serial.parseInt();
            
            if (motorPos [i] == 0)
            {
                motorPos [i] = previousMotorPos[i]; 
            }
            
            motors [i].write (motorPos[i]);
            
            if (motorPos [i] != 0)
            {
                previousMotorPos [i] = motorPos[i]; 
            }
    }
  
  lastSignalTime = millis();
  
  }

  
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (motorPos [i]);
  }
  
  if (millis() - lastSignalTime > 3000) {
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (40); 
    }
  }
}


