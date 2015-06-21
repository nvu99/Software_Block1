#include <Wire.h>
#include <Servo.h>
#include <math.h>



const unsigned int mCount = 4;
Servo motors[mCount];

const unsigned int ports [mCount] = {6, 7, 8, 9}; //Port of each servo
byte motorPos [mCount];

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

  
  //pololuSetup();
  //attachRPMSensors();
  
  delay(2000);

  analogWrite(1, LOW);
}

//Read commands and set the motors
void loop() {
  
  receiveData();
  receiveData();

  sensors();
}

void sensors() {
  
  delay(15);
  //distanceSensor();
  //getVoltage();
  //getRPM();
  //pololuLoop();
  receiveData();
  
  for (int i = 0; i < 4; ++i) {
    
    if (i != 2)
    {
      Serial.write(motors [i].read() >> 8);
      Serial.write(motors [i].read() & mask);
    }
    else
    {
      Serial.write((motors [i].read() - 1) >> 8);
      Serial.write((motors [i].read() - 1) & mask);
    }
    
  receiveData();
  }
  
  sendTelemetry();
  
  
  /*if (cm >= 150)
  {
   delay(30); 
  }
  else if (cm >= 50)
  {
   delay(40); 
  }
  else if (cm >= 1)
  {
   delay(50); 
  }
  else
  {
   delay(30); 
  }*/
  receiveData();
  receiveData();
}

void receiveData() {
  if (Serial.available()) {      
    for (int i = 0; i < mCount; ++i) {
            motorPos [i] = Serial.parseInt();
            
            if (motorPos [i] < 40 && previousMotorPos [i] - motorPos [i] > 22)
            {
                motorPos [i] = previousMotorPos [i]; 
            }
            
            else if (motorPos [i] > 130) 
            {
                motorPos [i] = previousMotorPos [i];
            }
            
            else if (motorPos [i] == 63 && previousMotorPos [i] - motorPos [i] > 6)
            {
                motorPos [i] = previousMotorPos [i];
            }
            
            if (i == 2)
            {
                motorPos [i] = motorPos [i] + 1; 
            }
            
            motors [i].write (motorPos[i]);
            
            if (motorPos [i] != 0)
            {
                previousMotorPos [i] = motorPos[i]; 
            }
            
    }
  
  lastSignalTime = millis();
  }
  
  /*for (int i = 0; i < mCount; ++i) {
     motors [i].write (motorPos [i]);
  }*/
  
  if (millis() - lastSignalTime > 1500) {
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (40); 
    }
  }
}


