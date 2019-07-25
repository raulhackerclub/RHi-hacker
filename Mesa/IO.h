#include "Arduino.h"

class IO {
  public:
    IO();
    bool send(String cmdString);
    String receive();
  private:
//    RF24 radio;
    byte address;
    int csnPin;
    int cePin;
    // int sckPin;
    // int mosiPin;
    // int misoPin;
};
