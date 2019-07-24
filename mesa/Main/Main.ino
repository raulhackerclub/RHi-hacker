// Leitura dos resistores:
// 220 - 62
// 1k - 131
// 10k 587
// 100k - 949

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


const int activeSlotCount = 4;
const int MAX_SENSOR_VAL = 1023;
const int sensorPins[activeSlotCount] = {A0, A1, A2, A3};
const int ledPins[activeSlotCount] = {8, 9, 10, 11};


int sensorsValue[activeSlotCount] = {MAX_SENSOR_VAL, MAX_SENSOR_VAL, MAX_SENSOR_VAL, MAX_SENSOR_VAL};
bool ledsState[activeSlotCount] = {false, false, false, false};

void setup() {
  // configura os pinos analógicos como entrada
  for (int i = 0; i < activeSlotCount; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
  }

  // configura os pinos dos leds saída
  for (int i = 0; i < activeSlotCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // identifica os estados que cada led deve ter baseado na presença de um (qualquer) resistor
  for (int i = 0; i < activeSlotCount; i++) {
    int sensorValue = analogRead(sensorPins[i]);

    // MAX_SENSOR_VAL - 10 para acomodar flutuações
    if (sensorValue < MAX_SENSOR_VAL - 10)
      ledsState[i] = true;
    else
      ledsState[i] = false;
  }

  // acende ou apaga os leds baseado no array de estados dos leds
  for (int i = 0; i < activeSlotCount; i++) {
    if (ledsState[i])
      digitalWrite(ledPins[i], HIGH);
    else
      digitalWrite(ledPins[i], LOW);
  }

  // não precisa ser muito rápido
  delay(150);
}

//
// Métodos auxiliares
//

/**
 * Identifica os comandos que estão na mesa e preenche array com a lista de comandos identificados
 */
void identifyCommands(int *cmds) {
  for (int i=0; i < activeSlotCount; i++) {
    int pin = sensorPins[i];
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
