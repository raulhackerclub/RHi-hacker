#ifndef IO_h

#define IO_h

#include "Arduino.h"

class IO {
  public:
    IO();
    bool send(String cmdString);
    String receive();
  private:
    // byte _address;
    // int _csnPin;
    // int _cePin;
    // int _sckPin;
    // int _mosiPin;
    // int _misoPin;
};

#endif