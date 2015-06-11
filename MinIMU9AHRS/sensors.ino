void distanceSensor()
{
   const int pingPin = 12;
   int inPin = 11;
  
   pinMode(pingPin,OUTPUT);
   
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(5);
   digitalWrite(pingPin, LOW);
   
   pinMode(inPin,INPUT);
   duration = pulseIn(inPin, HIGH, 16000);
   
   cm = microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

void getVoltage()
{
 volt_v = (analogRead(1) * volt_vPow) / 1024.0;
 volt_v2 = volt_v / (volt_r2 / (volt_r1 + volt_r2)); 
}

void sendTelemetry()
{
  /*String s = "" + String (accel_x / 26.2 , 2);
  s += " " + String(accel_y / 26.2 , 2);
  s += " " + String(accel_z / 26.2 , 2);
  Serial.write (s); Serial.write(" ");*/
  Serial.write(accel_x * 1000 / 262 >> 8);
  Serial.write(accel_x * 1000 / 262 & mask);
  Serial.write(accel_y * 1000 / 262 >> 8);
  Serial.write(accel_y * 1000 / 262 & mask);
  Serial.write(accel_z * 1000 / 262 >> 8);
  Serial.write(accel_z * 1000 / 262 & mask);
  Serial.write((int)(ToDeg(roll)*100) >> 8);
  Serial.write((int)(ToDeg(roll)*100) & mask);
  Serial.write((int)(ToDeg(pitch)*100) >> 8);
  Serial.write((int)(ToDeg(pitch)*100) & mask);
  Serial.write((int)(ToDeg(yaw)*100) >> 8);
  Serial.write((int)(ToDeg(yaw)*100) & mask);
  
  for (int i=0; i<4; ++i)
  {
    Serial.write(rpm[i] >> 8); 
    Serial.write(rpm[i] & mask); 
  }
  
  Serial.write((int)(volt_v2*100) >> 8);
  Serial.write((int)(volt_v2*100) & mask);
  Serial.write((int)cm >> 8); 
  Serial.write((int)cm & mask);
  
  /*Serial.write((int)(pressure * 100) >> 8);
  Serial.write((int)(pressure * 100) & mask);
  Serial.write((int)(altitude * 100) >> 8);
  Serial.write((int)(altitude * 100) & mask); 
  Serial.write((int)(temperature * 100) >> 8);
  Serial.write((int)(temperature * 100) & mask);*/
  
  Serial.write("\n");
  //debugPrint();
}

void attachRPMSensors()
{
  for (int i=0; i<4; ++i)
  {
    pinMode(interruptPins[i], INPUT);
    digitalWrite(interruptPins[i], HIGH); 
  }
  
  attachInterrupt(0, rpm1, CHANGE); 
  attachInterrupt(1, rpm2, CHANGE); 
  attachInterrupt(2, rpm3, CHANGE); 
  attachInterrupt(3, rpm4, CHANGE); 
}

void getRPM()
{
  for (int i=0; i<4; ++i)
  {
   if (half_revolutions[i] >= 20)
   {
     rpm[i] = 30*1000/(millis() - timeold[i])*half_revolutions[i];
     timeold[i] = millis();
     half_revolutions[i] = 0;   
   } 
  }
}

void rpm1()
{
  half_revolutions[0]++; 
}

void rpm2()
{
  half_revolutions[1]++; 
}

void rpm3()
{
  half_revolutions[2]++; 
}

void rpm4()
{
  half_revolutions[3]++; 
}
