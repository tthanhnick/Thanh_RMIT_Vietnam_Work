void setup()
{
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int potValue;
  potValue=analogRead(A5);
  int temp=(potValue*4.88-500)/10;
  Serial.println(temp);
  if(temp <=0){
    analogWrite(10,LOW);
    analogWrite(9,LOW);
   	analogWrite(11,potValue);
    delay(10);
    analogWrite(11,LOW);
    delay(10);
  }else if(temp<=30){
    analogWrite(11,LOW);
    analogWrite(9,LOW);
   	analogWrite(10,potValue);
    delay(10);
    analogWrite(10,LOW);
    delay(10);
  }else{
    analogWrite(11,LOW);
    analogWrite(10,LOW);
    analogWrite(9,potValue);
    delay(10);
    analogWrite(9,LOW);
    delay(10);
  }
}
