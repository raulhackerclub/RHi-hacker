  /*
  * Arduino Wireless Communication Tutorial
  *       Example 1 - Receiver Code
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
  const char CMD_ENCAIXADO[] = "CMD_ENCAIXADO";
  
  void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
  }
  
  void loop() {
    if (radio.available()) {
      char text[32] = "";
      radio.read(&text, sizeof(text));
      
      if (strcmp(text, CMD_ENCAIXADO) == 0) {
        Serial.println("high");
        digitalWrite(13, HIGH);  
      } else {
        Serial.println("low");
        digitalWrite(13, LOW);  
      }
      
      
      
    }
  }
