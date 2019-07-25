#include "Arduino.h"
#include "IO.h"
//#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>

const byte address[6] = "00001";
const int csnPin = 20;
const int cePin = 21;

RF24 radio(cePin, csnPin);

// const int sckPin; = 52;
// const int mosiPin; = 51;
// const int misoPin; = 50;

IO::IO() {
  Serial.println("Inicializando radio");

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("Radio inicializado");
}

bool IO::send(String cmdString) {
  Serial.println("Enviando texto");

  char text[sizeof(cmdString)];
  cmdString.toCharArray(text, sizeof(cmdString));

  radio.stopListening();
  radio.write(&text, sizeof(text));

  Serial.print("Texto ENVIADO ");
  Serial.println(text);
}


String IO::receive() {
  Serial.println("Recebendo texto");

  radio.startListening();

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.print("Texto RECEBIDO ");
    Serial.println(text);
  } else {
    Serial.println("Radio não disponível");
  }
}
