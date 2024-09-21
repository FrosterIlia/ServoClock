#pragma once
#include <Arduino.h>
#include "Digit.h"

#define ADDRESS 0x40

class Display {
public:

Display() : digit1(ADDRESS), 
            digit2(ADDRESS + 1), 
            digit3(ADDRESS + 2), 
            digit4(ADDRESS + 3) {
  digits[0] = &digit1;
  digits[1] = &digit2;
  digits[2] = &digit3;
  digits[3] = &digit4;
}
  void display_number(uint16_t number) {
    uint8_t counter = 4;
    while (number > 0) {
      digit1.write(number % 10);
      number /= 10;
      counter--;
    }
  }

private:
  Digit digit1;
  Digit digit2;
  Digit digit3;
  Digit digit4;
  Digit *digits[4];
};