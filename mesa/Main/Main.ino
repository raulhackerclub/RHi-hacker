// Leitura dos resistores:
// 220 - 62
// 1k - 131
// 10k 587
// 100k - 949

const int activeSlotCount = 4;
const int MAX_SENSOR_VAL = 1023;

const int sensorPins[activeSlotCount] = {A0, A1, A2, A3};
const int sensorsValue[activeSlotCount] = {MAX_SENSOR_VAL, MAX_SENSOR_VAL, MAX_SENSOR_VAL, MAX_SENSOR_VAL};
const int ledPins[activeSlotCount] = {8, 9, 10, 11};
const bool ledsState[activeSlotCount] = {false, false, false, false};

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
