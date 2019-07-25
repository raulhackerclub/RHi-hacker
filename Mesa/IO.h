#include "Arduino.h"

class IO {
  public:
    IO();
    bool send(String cmdString);
    String receive();
}