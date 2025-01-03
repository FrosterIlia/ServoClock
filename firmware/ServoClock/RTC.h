#pragma once
#include <Arduino.h>
#include "Wire.h"

#define DS3231_ADDRESS 0b01101000


class RTC {
public:

  void request_time() {
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDRESS, 3);

    if (Wire.available() >= 3) {
      seconds_raw = Wire.read();
      minutes_raw = Wire.read();
      hours_raw = Wire.read();
    }

    seconds = (seconds_raw >> 4) * 10 + (seconds_raw & 0b00001111);
    minutes = (minutes_raw >> 4) * 10 + (minutes_raw & 0b00001111);
    hours = (hours_raw >> 4) * 10 + (hours_raw & 0b00001111);

  }

  void set_sys_time(){
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.write((__TIME__[6] - 48) << 4 | (__TIME__[7] - 48)); //seconds
    Wire.write((__TIME__[3] - 48) << 4 | (__TIME__[4] - 48)); // minutes
    Wire.write((__TIME__[0] - 48) << 4 | (__TIME__[1] - 48)); // hours
    
    Wire.endTransmission();
  }

  void set_time(uint16_t time){

    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.write(0x00); // seconds
    Wire.write(((time % 100) / 10) << 4 | (time % 10)); // minutes
    Wire.write((time / 1000) << 4 | ((time / 100) % 10)); // hours
    
    Wire.endTransmission();
  }

  uint8_t get_seconds(){
    return seconds;
  }
  uint8_t get_minutes(){
    return minutes;
  }
  uint8_t get_hours(){
    return hours;
  }


private:
  uint8_t seconds, minutes, hours;
  uint8_t seconds_raw, minutes_raw, hours_raw;
};
