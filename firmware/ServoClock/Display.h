#pragma once
#include <Arduino.h>
#include "Digit.h"

#define ADDRESS1 0x40
#define ADDRESS2 ADDRESS1 + 1
#define ADDRESS3 ADDRESS2 + 1
#define ADDRESS4 ADDRESS3 + 1


#define DIGITS_NUMBER 4


Digit digits[DIGITS_NUMBER] = { ADDRESS1, ADDRESS2, ADDRESS3, ADDRESS4 };

uint16_t myPow(uint16_t number, uint16_t power){
  uint16_t result = number;
  for (uint8_t i = 0; i < power - 1; i++){
    
    result *= number;
  }

  return result;
}

class Display {
public:

  Display() {
    init();
  };

  void tick() {
    for (uint8_t i = 0; i <= DIGITS_NUMBER; i++) {
      digits[i].tick();
    }
  }

  void write_number(int number) {
    uint8_t number_length = count_digits(number);

    uint16_t divisor = myPow(10, number_length - 1);
    Serial.println(divisor);


    for (uint8_t i = 0; i < number_length; i++) {

      digits[i].write(number / divisor);

      number %= divisor;
      divisor /= 10;
    }
    for (uint8_t i = number_length; i < DIGITS_NUMBER; i++) {
      Serial.println(i);
      digits[i].clear_digit();
    }
  }

  void write_digit(uint8_t digit_num, uint8_t digit) {
    digits[digit_num].write(digit);
  }

  void clear() {
    for (uint8_t i = 0; i <= DIGITS_NUMBER; i++) {
      digits[i].clear_digit();
    }
  }

private:
  uint8_t count_digits(uint32_t number) {
    if (number == 0) {
      return 1;  // Special case for 0
    }

    uint8_t digits = 0;
    while (number > 0) {
      number /= 10;  // Remove the last digit
      digits++;      // Increment digit count
    }

    return digits;
  }
};

