#pragma once
#include <Arduino.h>
#include <Wire.h>

#define MODE1 0x00
#define MODE2 0x01


#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

#define SERVO_MIN 345
#define SERVO_MAX 2130

#define SERVO_UP 1250
#define SERVO_DOWN_NORM 500
#define SERVO_DOWN_REVERSE 2000  // for segment 2 and 3 since they are reversed
#define SERVO_HALF_DOWN_NORM 800
#define SERVO_HALF_DOWN_REVERSE 1700

#define TIMEOUT 200


//0b 0 0 0 0 0 0 0 0
//0b   A B C D E F G
//Font
#define D_0 0b01111110
#define D_1 0b00110000
#define D_2 0b01101101
#define D_3 0b01111001
#define D_4 0b00110011
#define D_5 0b01011011
#define D_6 0b01011111
#define D_7 0b01110000
#define D_8 0b01111111
#define D_9 0b01111011

const uint8_t font[] PROGMEM = { D_0, D_1, D_2, D_3, D_4, D_5, D_6, D_7, D_8, D_9 };

class Digit {
public:

  Digit(uint8_t address) {
    init();
    _address = address;
    _servos = 0;

    PCA_setup();
  };

  void clear_digit() {
    PCA_setup();
    clear();
  }

  void write(uint8_t value) {
    PCA_setup();
    uint8_t mask = pgm_read_byte(&font[value]);


    for (int8_t i = 6; i >= 0; i--) {
      if (mask & (1 << i)) {

        write_servo(7 - i, 1);
      } else {

        write_servo(7 - i, 0);
      }
    }
  }

  void tick() {
    if (millis() - timer > TIMEOUT && timer_started) {
      timer_started = 0;
      // Serial.println("timer done");
      move_side_servos();
    }
    
  }

private:

  void move_half_down(uint8_t servo_number) {

    Wire.beginTransmission(_address);

    Wire.write(LED0_ON_L + (servo_number - 1) * 4);
    Wire.write(0x00);
    Wire.write(0x00);

    if (servo_number == 2 || servo_number == 3) {
      Wire.write(byte(SERVO_HALF_DOWN_REVERSE));
      Wire.write(byte(SERVO_HALF_DOWN_REVERSE >> 8));
    } else {
      Wire.write(byte(SERVO_HALF_DOWN_NORM));
      Wire.write(byte(SERVO_HALF_DOWN_NORM >> 8));
    }

    Wire.endTransmission();
  }

  void write_servo(uint8_t servo_number, uint8_t value) {  // 1 - up, 0 - down


    if (servo_number == 7) {
      if (bitRead(_servos, 0) == value) return;
      if (bitRead(_servos, 4)) {  // servo 3
        move_half_down(3);

      }
      if (bitRead(_servos, 2)) {  // servo 5
        move_half_down(5);

      }
      timer = millis();
      timer_started = 1;

      
    }

    Wire.beginTransmission(_address);

    Wire.write(LED0_ON_L + (servo_number - 1) * 4);
    Wire.write(0x00);
    Wire.write(0x00);


    if (value) {
      Wire.write(byte(SERVO_UP));
      Wire.write(byte(SERVO_UP >> 8));
    } else {
      if (servo_number == 2 || servo_number == 3) {
        Wire.write(byte(SERVO_DOWN_REVERSE));
        Wire.write(byte(SERVO_DOWN_REVERSE >> 8));
      } else {
        Wire.write(byte(SERVO_DOWN_NORM));
        Wire.write(byte(SERVO_DOWN_NORM >> 8));
      }
    }

    Wire.endTransmission();

    if (value) _servos |= (1 << (7 - servo_number));
    else _servos &= ~(1 << (7 - servo_number));
  }

  void move_side_servos() {
    write_servo(3, bitRead(_servos, 4));
    write_servo(5, bitRead(_servos, 2));
  }

  void clear() {
    _servos = 255;
    for (int8_t i = 6; i >= 0; i--) {
      write_servo(7 - i, 0);
    }
    _servos = 0;
  }

  void PCA_setup() {

    Wire.beginTransmission(_address);
    Wire.write(MODE1);
    Wire.write(0b00100001);  // enable auto-increment and enable oscillator

    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MODE2);
    Wire.write(0b00000100);  // leave as default

    Wire.endTransmission();
  }

  uint8_t _address;
  uint8_t value;
  uint8_t _servos;
  uint32_t timer;
  bool timer_started;
};