#define botao 2
#define encaixe A0
#define LED 3

void setup()
{
  pinMode(botao,INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if( analogRead(encaixe) == 46)
  {
    digitalWrite(LED,HIGH);
  }
  else
  {
    digitalWrite(LED,LOW);
  }
  
  if( !digitalRead(botao) )
  {
    if( analogRead(encaixe) == 46)
    {
      Serial.println("Frente!");
    }
    else
    {
      Serial.println("Vazio.");
    }
  }
  delay(125);
}