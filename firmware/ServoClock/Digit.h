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


//0b 0 0 0 0 0 0 0 0
//0b   A B C D E F G
//Font
#define D_0 0b01111110
#define D_1 0b00110000
#define D_2 0b01101101
#define D_3 0b01111001
#define D_4 0b00110011
#define D_5 0b01011011
#define D_6 0b01111101
#define D_7 0b01110000
#define D_8 0b01111111
#define D_9 0b01111011

const uint8_t font[] PROGMEM = { D_0, D_1, D_2, D_3, D_4, D_5, D_6, D_7, D_8, D_9 };

class Digit {
public:
  Digit(uint8_t address) {
    init();
    _address = address;

    PCA_setup();
  };


  void write_servo(uint8_t servo_number, uint8_t value) {  // 1 - up, 0 - down

    if (servo_number == 7){
      write_servo(3, 0);
      write_servo(5, 0);
      delay(400);
      
    }

    Wire.beginTransmission(_address);

    Wire.write(LED0_ON_L + (servo_number - 1) * 4);
    Wire.write(0x00);
    Wire.write(0x00);

    
    if (value) {
      Wire.write(byte(SERVO_UP));
      Wire.write(byte(SERVO_UP >> 8));
    }
    else {
      if (servo_number == 2 || servo_number == 3) {
        Wire.write(byte(SERVO_DOWN_REVERSE));
        Wire.write(byte(SERVO_DOWN_REVERSE >> 8));
      }
      else {
        Wire.write(byte(SERVO_DOWN_NORM));
        Wire.write(byte(SERVO_DOWN_NORM >> 8));
      }
    }

    Wire.endTransmission();

    

    if (servo_number == 7){
      delay(400);
      write_servo(3, 1);
      write_servo(5, 1);
      
    }
  }

private:

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
  bool servos[7];
};