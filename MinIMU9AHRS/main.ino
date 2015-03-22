#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_LSM303_U.h>
#include <Servo.h>
#include <math.h>

const int mCount = 4;
Servo motors[mCount];

int ports [mCount] = {2, 3, 4, 5}; //Port of each servo
int motorPos [mCount];

float volt_vPow = 4.7;
float volt_r1 = 100000;
float volt_r2 = 10000;
 
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

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
  
  Serial.println("Initializing sensors:");
  if(!mag.begin())
  {
    Serial.println("LSM303 disconnected");
    //while(1);
  }
  
  
  if(!accel.begin())
  {
   
    Serial.println("LSM303 disconnected");
    //while(1);
  }
  
  displaySensorDetails();
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
 
  sensors_event_t eventm; 
  mag.getEvent(&eventm);
  
  float Pi = 3.14159;
  
  // Calculate the angle of the vector y,x
  float heading = (atan2(eventm.magnetic.y,eventm.magnetic.x) * 180) / Pi;
  
  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  
  sensors_event_t eventa; 
  accel.getEvent(&eventa);
  
  pololuLoop();
  
  Serial.print(heading);Serial.print(" ");
  Serial.print(eventa.acceleration.x); Serial.print(" ");
  Serial.print(eventa.acceleration.y); Serial.print(" ");
  Serial.print(eventa.acceleration.z-9.81); Serial.print(" ");
  Serial.print(ToDeg(roll)); Serial.print(" ");
  Serial.print(ToDeg(pitch)); Serial.print(" ");
  Serial.print(ToDeg(yaw)); Serial.print(" ");
  Serial.print(pressure); Serial.print(" ");
  Serial.print(altitude); Serial.print(" ");
  Serial.print(temperature); Serial.print(" ");
  Serial.print(volt_def, DEC); Serial.println(" ");
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
