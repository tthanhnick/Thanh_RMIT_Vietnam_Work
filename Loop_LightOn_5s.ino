void setup()
{
  pinMode(12, OUTPUT);
}

void loop()
{
  for(int i=1;i<=5;i++){
  digitalWrite(12, HIGH);
  delay(200); // Wait for 1000 millisecond(s)
  digitalWrite(12, LOW);
  delay(200); // Wait for 1000 millisecond(s)
  }
  delay(1000);
}
