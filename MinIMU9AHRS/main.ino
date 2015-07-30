#include <Wire.h>
#include <Servo.h>
#include <math.h>



const unsigned int mCount = 4;
Servo motors[mCount];

const unsigned int ports [mCount] = {6, 7, 8, 9}; //Port of each servo
byte motorPos [mCount];
int badReceiveCounter = 0;

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
      Serial.write((motors [i].read()) >> 8);
      Serial.write((motors [i].read()) & mask);
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
    int motorSum = 0;
    for (int i = 0; i < mCount; ++i) {
            motorPos [i] = Serial.parseInt();  
            motorSum += motorPos [i];
            if ((motorPos [i] != 40 && motorPos [i] != 0 && motorPos[i] < 62) || 
                (motorPos[i] > 114) )//||
                //(abs(motorPos[i]-previousMotorPos[i]) > 60))
            {
                motorPos [i] = (badReceiveCounter >= 10 ? 40 : previousMotorPos [i]);
                ++badReceiveCounter; 
                continue;
            }
            badReceiveCounter = 0;         
            if (motorPos [i] != 0)
            {
                if (i == 2)
                {
                    motorPos [i] = motorPos [i] + 1; 
                }  
            }
    }
    int chkSum = Serial.parseInt();
    if (chkSum != motorSum)
      ++badReceiveCounter;
    for (int i = 0; i < mCount; ++i) {
       if (chkSum != motorSum)
         motors [i].write (previousMotorPos[i]);
       else {
         previousMotorPos[i] = motorPos[i];
         motors [i].write (motorPos[i]); 
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
