void setup()
{
  pinMode(6, INPUT);
  pinMode(9,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(6)==1){
    Serial.println("ON");
     digitalWrite(9,1);
    delay(500);
  }
  else{
    Serial.println("OFF");
  	digitalWrite(9, 0);
  	delay(500); 
  }
  delay(100); 
}
