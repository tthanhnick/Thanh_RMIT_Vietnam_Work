void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(random(1,10));
  delay(1000);
}
