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
 volt_v = (analogRead(0) * volt_vPow) / 1024.0;
 volt_v2 = volt_v / (volt_r2 / (volt_r1 + volt_r2)); 
}

void sendTelemetry()
{
  String s = "" + String (accel_x / 26.2 , 2);
  s += " " + String(accel_y / 26.2 , 2);
  s += " " + String(accel_z / 26.2 , 2);
  Serial.print (s); Serial.print(" ");
  Serial.print(ToDeg(roll)); Serial.print(" ");
  Serial.print(ToDeg(pitch)); Serial.print(" ");
  Serial.print(ToDeg(yaw)); Serial.print(" ");
  /*Serial.print(pressure); Serial.print(" ");
  Serial.print(altitude); Serial.print(" ");
  Serial.print(temperature); Serial.print(" ");*/
  
  for (int i=0; i<4; ++i)
  {
    Serial.print(rpm[i]); Serial.print(" ");
  }
  
  Serial.print(volt_v2, 2); Serial.print(" ");
  Serial.print(cm); Serial.println("");
}

void attachRPMSensors()
{
  PCintPort::attachInterrupt(2, rpm1, RISING); 
  PCintPort::attachInterrupt(3, rpm2, RISING); 
  PCintPort::attachInterrupt(4, rpm3, RISING); 
  PCintPort::attachInterrupt(5, rpm4, RISING); 
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
