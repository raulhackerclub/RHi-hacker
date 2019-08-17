/* RAUL HACKER CLUB!
 * RHI Hacker
 * 
 * motor1front - cor verde
 * motor1back - cor azul
 * 
 * motor2front - cor verde
 * motor2back - cor azul
 * 
 * 
 */

int motor1front = 3, motor1back = 4, motor2front = 5, motor2back = 6;
int commands[3] = {1,2,3};
bool didExecuteCommands = false;

void Stop()
{
 digitalWrite(motor1front,LOW);
 digitalWrite(motor1back,LOW);
 digitalWrite(motor2front,LOW);
 digitalWrite(motor2back,LOW);
}
void Forward()
{
 digitalWrite(motor1front,HIGH);
 digitalWrite(motor1back,LOW);
 digitalWrite(motor2front,HIGH);
 digitalWrite(motor2back,LOW);
 delay(500);
 Stop();
}
void turnRight()
{
 digitalWrite(motor1front,HIGH);
 digitalWrite(motor1back,LOW);
 digitalWrite(motor2front,LOW);
 digitalWrite(motor2back,HIGH);
 delay(2000);
 Stop();
}
void turnLeft()
{
 digitalWrite(motor1front,LOW);
 digitalWrite(motor1back,HIGH);
 digitalWrite(motor2front,HIGH);
 digitalWrite(motor2back,LOW);
 delay(2000);
 Stop();
}

void setup() 
{ 
 pinMode(motor1front,OUTPUT);
 pinMode(motor1back,OUTPUT); 
 pinMode(motor2front,OUTPUT);
 pinMode(motor2back,OUTPUT);

 Stop();
 delay(1500);
}

void loop() 
{
  if( !didExecuteCommands )
  {
    for(int i=1; i<4; i++)
    {
      switch(i)
      {
        case 1:
          Forward();
          break;
  
        case 2: 
          turnRight();
          break;
  
        case 3: 
          turnLeft();
          break;
      }
      delay(1000);
    }
    didExecuteCommands = true;
  }
}
