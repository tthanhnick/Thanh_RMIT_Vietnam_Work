void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop()
{
  int sensorval=digitalRead(2);
  Serial.println(sensorval);
  if(sensorval==HIGH){
    
  digitalWrite(13, LOW);
  }else{
  digitalWrite(13, HIGH);
  }
}
