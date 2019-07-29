#include <SPI.h>
#include "IO.h"

//
// Valores de resistência padrão para os comandos, usando resistores de
// 220, 1k, 10k e 100k respectivamente
//
const int CMD_UP = 62;
const int CMD_RIGHT = 131;
const int CMD_DOWN = 587;
const int CMD_LEFT = 949;

/**
 * Quanto de espaço reserva deve ser considerado ao medir o valor analógico de resistência
 * @type {Number}
 */
const int DEFAULT_WIGGLE = 30;

//
// Variações possíveis para os valores de resistência
//
const int CMD_UP_V[2] = {CMD_UP - DEFAULT_WIGGLE, CMD_UP + DEFAULT_WIGGLE};
const int CMD_RIGHT_V[2] = {CMD_RIGHT - DEFAULT_WIGGLE, CMD_RIGHT + DEFAULT_WIGGLE};
const int CMD_DOWN_V[2] = {CMD_DOWN - DEFAULT_WIGGLE, CMD_DOWN + DEFAULT_WIGGLE};
const int CMD_LEFT_V[2] = {CMD_LEFT - DEFAULT_WIGGLE, CMD_LEFT + DEFAULT_WIGGLE};

/**
 * Possíveis estados de execução
 */
enum ExecutionState {
  IDLE,
  BUILDING_COMMANDS,
  SENDING_COMMANDS,
  AWAITING_CONFIRMATION,
  FOLLOWING_EXECUTION,
};

const int activeSlotCount = 8;
const int MAX_SENSOR_VAL = 1023;

// pinos
const int slotsSensorPins[activeSlotCount] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int slotsLedPins[activeSlotCount] = {4, 5, 6, 7, 8, 9, 10, 11};
const int goBtnPin = 3;
const int goBtnLedPin = 2;

/**
 * Estado de execução atual
 * @type {ExecutionStates}
 */
ExecutionState currentState = IDLE;

bool ledsState[activeSlotCount] = {};
int goBtnState = LOW;

// ===  variáveis BUILDING_COMMANDS

/**
 * string de commandos atual
 * @type {String}
 */
String currentCmdString = "";


// ===  variáveis SENDING_COMMANDS

bool isSendingCommand = false;
bool cmdTransmissionResult = false;


// ===  variáveis AWAITING_CONFIRMATION

bool isAwaitingConfirmation = false;
bool confirmationResult = false;

IO io;

void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  io = io;

  // configura os pinos analógicos como entrada
  for (int i = 0; i < activeSlotCount; i++) {
    pinMode(slotsSensorPins[i], INPUT_PULLUP);
  }

  // configura os pinos dos leds saída
  for (int i = 0; i < activeSlotCount; i++) {
    pinMode(slotsLedPins[i], OUTPUT);
  }

  pinMode(goBtnPin, INPUT_PULLUP);
  pinMode(goBtnLedPin, OUTPUT);
}

void loop() {
  switch (currentState) {
    case BUILDING_COMMANDS:
      doBuildCommands();
      break;
    case SENDING_COMMANDS:
      doSendCommands();
      break;
    case AWAITING_CONFIRMATION:
      doAwaitConfirmation();
      break;
    case FOLLOWING_EXECUTION:
      doFollowExecution();
      break;
    default:
      doIdle();
      break;
  }

  delay(50);
}

/**
 * Acende e apaga os leds dos slot que tiverem peças encaixadas
 * transiciona para o estado BUILDING_COMMANDS
 * @return {void}
 */
void doIdle() {
  Serial.println("Idling...");


  goBtnState = digitalRead(goBtnPin);

  // === condição de transição para o próximo estado
  if (goBtnState == LOW) {
    // zera o estado dos leds
    for (int i = 0; i < activeSlotCount; i++) {
      ledsState[i] = false;
      digitalWrite(slotsLedPins[i], LOW);
    }
    // zera o estado do botão de início
    goBtnState = LOW;

    currentState = BUILDING_COMMANDS;

  } else {
    // identifica os estados que cada led deve ter baseado na presença de um (qualquer) resistor
    for (int i = 0; i < activeSlotCount; i++) {
      int sensorValue = analogRead(slotsSensorPins[i]);

      // MAX_SENSOR_VAL - 10 para acomodar flutuações
      if (sensorValue < MAX_SENSOR_VAL - 10)
        ledsState[i] = true;
      else
        ledsState[i] = false;
    }

    // acende ou apaga os leds baseado no array de estados dos leds
    for (int i = 0; i < activeSlotCount; i++) {
      if (ledsState[i])
        digitalWrite(slotsLedPins[i], HIGH);
      else
        digitalWrite(slotsLedPins[i], LOW);
    }

    // não precisa ser muito rápido
    delay(150);
  }
}

void doBuildCommands() {
  Serial.println("Building command...");

  currentCmdString = "";

  int cmdList[activeSlotCount] = {};
  identifyCommands(cmdList);

  currentCmdString = buildCommandString(cmdList);

  // === transição para o próximo estado
  currentState = SENDING_COMMANDS;
}

/* Envia string com commandos para serem executados pelo robô
 * @return {void}
 */
void doSendCommands() {
  Serial.println("sending command...");

  if (!isSendingCommand) {
    cmdTransmissionResult = io.send(currentCmdString);
    isSendingCommand = true;
  }

  // === condição de transição para o próximo estado
  if (cmdTransmissionResult) {
    // zerando variáveis
    isSendingCommand = false;
    cmdTransmissionResult = false;
    currentCmdString = "";
    currentState = AWAITING_CONFIRMATION;
  }
}

void doAwaitConfirmation() {
  Serial.println("await confirmation...");

  String confirmationText;

  if (!isAwaitingConfirmation) {
    io.receive();
    isAwaitingConfirmation = true;
  }

  confirmationResult = confirmationText.equals("OK");

  // === condição de transição para o próximo estado
  if (confirmationResult) {
    // zerando variáveis
    isAwaitingConfirmation = false;
    confirmationResult = false;
    currentState = FOLLOWING_EXECUTION;
  }
}

void doFollowExecution() {

}

//
// Métodos auxiliares
//

/**
 * Identifica os comandos que estão na mesa e preenche array com a lista de comandos identificados
 * @param  {int[]} int *cmds Lista initializada onde os comandos serão escritos
 * @return {int[]} a lista de comandos identificados
 */
void identifyCommands(int *cmds) {
  for (int i=0; i < activeSlotCount; i++) {
    int pin = slotsSensorPins[i];
    int value = analogRead(pin);

    if (value >= CMD_UP_V[0] && value <= CMD_UP_V[1]) {
      cmds[i] = CMD_UP;
    } else if (value >= CMD_RIGHT_V[0] && value <= CMD_DOWN_V[1]) {
      cmds[i] = CMD_RIGHT;
    } else if (value >= CMD_DOWN_V[0] && value <= CMD_DOWN_V[1]) {
      cmds[i] = CMD_DOWN;
    } else if (value >= CMD_LEFT_V[0] && value <= CMD_LEFT_V[1]) {
      cmds[i] = CMD_LEFT;
    }
  }
}

/**
 * Constrói uma string que contem a lista de comandos a ser enviada para o robô
 * @param  {[type]} int cmds[] a lista de comandos identificados
 * @return {String} a String a ser enviada
 */
String buildCommandString(int cmds[]) {
  String cmdString = "";

  for (int i=0; i < activeSlotCount; i++) {
    // a string conterá apenas os comandos identificados
    if (cmds[i] == 0)
      break;

    switch (cmds[i]) {
      case CMD_UP:
        cmdString.concat(":UP");
        break;
      case CMD_RIGHT:
        cmdString.concat(":RIGHT");
        break;
      case CMD_DOWN:
        cmdString.concat(":DOWN");
        break;
      case CMD_LEFT:
        cmdString.concat(":LEFT");
        break;
    }
  }

  if (cmdString[0] == ':')
    cmdString.remove(0);

  return cmdString;
}
