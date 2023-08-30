int red=13;
int yellow=12;
int green=11;
void setup()
{
 pinMode(red, OUTPUT);
 pinMode(yellow, OUTPUT);
 pinMode(green, OUTPUT);
 digitalWrite(red, LOW);
 digitalWrite(yellow, LOW);
 digitalWrite(green, LOW);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(red, LOW);
 digitalWrite(yellow, HIGH);
 digitalWrite(green, LOW);
  Serial.println("Light Mode: Yield ");
  delay(1000);
  
  digitalWrite(red, LOW);
 digitalWrite(yellow, LOW);
 digitalWrite(green, HIGH);
  Serial.println("Light Mode : Go ");
  delay(2000);
  
  digitalWrite(red, HIGH);
 digitalWrite(yellow, LOW);
 digitalWrite(green, LOW);
  Serial.println("Light Mode : Stop ");
  delay(2000);
}
