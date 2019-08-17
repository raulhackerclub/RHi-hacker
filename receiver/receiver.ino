/*
 * RAUL HACKER CLUB 
 * Projeto: RHi Hacker
 * 
 * Detalhe importante sobre o código:
 * É necessário fazer o download da biblioteca NRF24: https://github.com/nRF24/RF24
 * 
 * Detalhes sobre conexões do hardware:
 * Modelo de placa Arduino utilizada: MEGA 2560
 * VCC: 1.9V ~ 3.6V
 * CE: 7
 * CSN: 8
 * MOSI: 51 / 11[UNO]
 * SCK: 52 / 13[UNO]
 * MISO: 50 / 12[UNO]
 * IRQ: <não será necessário usar este pino neste projeto>
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 patrulha(7, 8); // CE, CSN

const byte canal[6] = "77771";

void setup() 
{
  Serial.begin(9600);
  patrulha.begin();
  patrulha.openReadingPipe(0, canal);
  patrulha.setPALevel(RF24_PA_MIN);
  patrulha.startListening();
}

void loop()
 {
  if( patrulha.available() ) 
  {
    char msg[32] = "";
    patrulha.read(&msg, sizeof(msg));
    Serial.println(msg);
  }
}
