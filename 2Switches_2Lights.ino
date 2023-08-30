int light =9;
int sw1=12;
int sw2=6;
void setup()
{
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(light, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(sw1)==1 && digitalRead(sw2)==1){
    Serial.println("ON-ON");
    digitalWrite(light,1);
    delay(500);
  }
  else if(digitalRead(sw1)==0 && digitalRead(sw2)==1)
  {
    Serial.println("OFF-ON");
    digitalWrite(light,1);
    delay(200);
    digitalWrite(light,0);
    delay(200);
  }
  else if(digitalRead(sw1)==1 && digitalRead(sw2)==0)
  {
    Serial.println("ON-OFF");
    digitalWrite(light,1);
    delay(500);
    digitalWrite(light,0);
    delay(500);
  }
  else {
    Serial.println("OFF-OFF");
  	digitalWrite(light, 0);
  	//delay(500);
  }
  delay(100); 
}
