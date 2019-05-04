  /*
  * Arduino Wireless Communication Tutorial
  *     Example 1 - Transmitter Code
  *                
  * by Dejan Nedelkovski, www.HowToMechatronics.com
  * 
  * Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
  */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
  }
  void loop() {
    sensorValue = analogRead(sensorPin);
    
    if (sensorValue > 80 && sensorValue < 100) {
      char text[] = "CMD_ENCAIXADO";
      radio.write(&text, sizeof(text));    
    } else {
      char text[] = "CMD_IDLE";
      radio.write(&text, sizeof(text));  
    }   
    
    delay(1000);
  }
