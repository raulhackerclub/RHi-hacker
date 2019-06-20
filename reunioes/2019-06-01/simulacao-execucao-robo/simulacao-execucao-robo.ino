#define LED 8
#define botao 2

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  pinMode(botao,INPUT_PULLUP);

  digitalWrite(LED,LOW);
}

void loop()
{
  if( analogRead(A0)>=90 && analogRead(A0) <=100)
  {
    digitalWrite(LED,HIGH);
    Serial.println("encaixado");

    if(!digitalRead(botao))
    {
      for (int count=0;count<5;count++)
      {
        digitalWrite(LED,HIGH);
        delay(500);
        digitalWrite(LED,LOW);
        delay(500);
      }
    }
  }
  else
  {
    digitalWrite(LED,LOW);
    Serial.println("Not encaixado");
  }
  delay(125);
}
