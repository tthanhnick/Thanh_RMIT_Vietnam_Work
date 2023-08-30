int light =9;
int sw1=13;
int sw2=12;
int sw3=11;
void setup()
{
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT);
  pinMode(light,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(sw1)==0 && digitalRead(sw2)==0 && digitalRead(sw3)==0){
    Serial.println("OFF-OFF-OFF");
    digitalWrite(light,1);
    delay(818);
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==0 && digitalRead(sw2)==0 && digitalRead(sw3)==1){
    Serial.println("OFF-OFF-ON");
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==0 && digitalRead(sw2)==1 && digitalRead(sw3)==0){
    Serial.println("OFF-ON-OFF");
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==0 && digitalRead(sw2)==1 && digitalRead(sw3)==1){
    Serial.println("OFF-ON-ON");
    digitalWrite(light,1);
    delay(818);
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==1 && digitalRead(sw2)==0 && digitalRead(sw3)==0){
    Serial.println("ON-OFF-OFF");
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==1 && digitalRead(sw2)==0 && digitalRead(sw3)==1){
    Serial.println("ON-OFF-ON");
    digitalWrite(light,1);
    delay(818);
    digitalWrite(light,0);
    delay(111);
  }
  else if(digitalRead(sw1)==1 && digitalRead(sw2)==1 && digitalRead(sw3)==0){
    Serial.println("ON-ON-OFF");
    digitalWrite(light,1);
    delay(818);
    digitalWrite(light,0);
    delay(111);
  }
  else {
    Serial.println("ON-ON-ON");
  	digitalWrite(light,0);
    delay(111);
  }
  delay(100); 
}
