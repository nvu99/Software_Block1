#include <Wire.h>
#include <Servo.h>
#include <math.h>

const int mCount = 4;
Servo motors[mCount];

int ports [mCount] = {2, 3, 4, 5}; //Port of each servo
int motorPos [mCount];

float volt_vPow = 4.7;
float volt_r1 = 100000;
float volt_r2 = 10000;

void setup() {
 
  for (int i = 0; i < mCount; ++i) {
    motors [i].attach (ports [i]);
    motorPos [i] = 0;
    motors [i].write (motorPos [i]);
  }
  //Setup serial connection
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  delay(3000);
  
  for (int i = 0; i < mCount; ++i) {
    motorPos [i] = 50;
    motors [i].write (motorPos[i]);
  }
  
  pololuSetup();
}

//Read commands and set the motors
void loop() {
  if (Serial.available()) {      
    for (int i = 0; i < mCount; ++i) {
            motorPos [i] = Serial.parseInt();
    }
  }       
  for (int i = 0; i < mCount; ++i) {
     motors [i].write (motorPos [i]); 
  }
  
  sensors();
        
}

void sensors() {
 float volt_v = (analogRead(0) * volt_vPow) / 1024.0;
 float volt_v2 = volt_v / (volt_r2 / (volt_r1 + volt_r2));
 float volt_def = floor(volt_v2 * 100) / 100;
 
  for (int i = 0; i < 4; ++i) {
  Serial.print(motors [i].read(), DEC);
  Serial.print(" ");
  }
 
  float Pi = 3.14159;
  
  
  pololuLoop();
  String s = " " + (int)accel_x;
  s += " ";
  s += " " + (int)accel_y;
  s += " "; 
  s += " " + (int)accel_z;
  s += " ";
  Serial.print (s);
  Serial.print(ToDeg(roll)); Serial.print(" ");
  Serial.print(ToDeg(pitch)); Serial.print(" ");
  Serial.print(ToDeg(yaw)); Serial.print(" ");
  Serial.print(pressure); Serial.print(" ");
  Serial.print(altitude); Serial.print(" ");
  Serial.print(temperature); Serial.print(" ");
  Serial.print(volt_def, DEC); Serial.println(" ");
}