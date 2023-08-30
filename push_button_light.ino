int pushbutton=12;
int red=7;
int yellow=6;
int green=5;
void setup()
{
  Serial.begin(9600);
  pinMode(pushbutton, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(yellow,OUTPUT);
}

void loop()
{
  int buttonstate=digitalRead(pushbutton);
  digitalWrite(red,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(green,HIGH);
  if(buttonstate==HIGH)
  {
    digitalWrite(red,LOW);
    digitalWrite(yellow,HIGH);
    digitalWrite(green,LOW);
    Serial.println(buttonstate);
    delay(3000);
    digitalWrite(red,HIGH);
    digitalWrite(yellow,LOW);
    digitalWrite(green,LOW);
    delay(6000);
  }
  else
  {
   	digitalWrite(red,LOW);
  	digitalWrite(yellow,LOW);
  	digitalWrite(green,HIGH);
  }
  //delay(1);
}
