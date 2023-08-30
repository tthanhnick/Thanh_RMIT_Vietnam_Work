void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  Serial.begin(9600);
}

void loop()
{
  int potValue;
  potValue=analogRead(A0);
  Serial.println(potValue);
  analogWrite(5,potValue/4);
  delay(10);
}
