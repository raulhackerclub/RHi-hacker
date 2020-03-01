/* RAUL HACKER CLUB!
 * RHI Hacker
 */
 
#include <AccelStepper.h>
#include <AFMotor.h>

///////////////////////
//// SETUP MOTORS /////
///////////////////////

// two stepper motors one on each port
AF_Stepper motor1(200, 1);
AF_Stepper motor2(200, 2);

void forwardstep1() {  
  motor1.onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  motor1.onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(FORWARD, SINGLE);
}
// Motor shield has two motor ports, now we'll wrap them in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

///////////////////////
// MOTORS SETUP DONE //
///////////////////////

const int commandsSize = 3;
int commands[commandsSize] = {3, 1};
int command;
bool areSteppersDisabled = false;
bool didExecuteCommands = false;
bool isExecutingCommand = false;
int commandExecution;
int currentCommandIndex = -1;

void forward()
{
  stepper1.moveTo(1000);
  stepper2.moveTo(1000);
}

void turnRight()
{
  stepper1.moveTo(1000);
  stepper2.moveTo(-1000);
}

void turnLeft()
{
  stepper1.moveTo(-1000);
  stepper2.moveTo(1000);
}

void setup() 
{ 
  Serial.begin(9600);

  stepper1.setMaxSpeed(600.0);
  stepper1.setAcceleration(600.0);
    
  stepper2.setMaxSpeed(600.0);
  stepper2.setAcceleration(600.0);

  currentCommandIndex = 0;
  Serial.print(commandsSize);
  Serial.println(" commands");
}

void loop() 
{
  stepper1.run();
  stepper2.run();

  if( !isExecutingCommand )
  {
    // pega o primeiro commando da lista se não estiver vazia
    if (currentCommandIndex < commandsSize)
    {
      command = commands[currentCommandIndex];
      currentCommandIndex += 1;
      
      Serial.print("Current command is ");
      Serial.println(command);
    }

    // desliga as bobinas caso não hajam mais comandos
    else if ( !areSteppersDisabled )
    {
      Serial.println("No more commands");
      motor1.release();
      motor2.release();
      areSteppersDisabled = true;
      return;
    }
    
    // indica que um comndo está sendo executado
    isExecutingCommand = true;

    switch(command)
    {
      case 1:
        forward();
        break;

      case 2: 
        turnRight();
        break;

      case 3: 
        turnLeft();
        break;
    }

    commandExecution = millis();
  }

  if (!stepper1.isRunning()) 
  {
    isExecutingCommand = false;
  }
}
