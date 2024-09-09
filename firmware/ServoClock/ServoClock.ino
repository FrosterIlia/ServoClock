#include <Wire.h>
#include "Digit.h"

#define ADDRESS 0x40

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

// value means off_time value
// value 345 is around 0 degrees rotation
// value 2130 is around 180 degrees rotation
Digit digit1(ADDRESS);

void setup() {

  Wire.begin();
  Serial.begin(9600);




  // Wire.beginTransmission(ADDRESS);
  // Wire.write(MODE1);
  // Wire.write(0b00100001);

  // Wire.endTransmission();

  // Wire.beginTransmission(ADDRESS);
  // Wire.write(MODE2);
  // Wire.write(0b00000100);

  // Wire.endTransmission();
}
int servo_value;
void loop() {
  if (Serial.available() > 1) {
    servo_value = Serial.parseInt();

    digit1.write_servo(1, servo_value);
    delay(1000);
    digit1.write_servo(2, servo_value);
    delay(1000);
    digit1.write_servo(3, servo_value);
    delay(1000);
    digit1.write_servo(4, servo_value);
    delay(1000);
    digit1.write_servo(5, servo_value);
    delay(1000);

    digit1.write_servo(6, servo_value);
    delay(1000);
    digit1.write_servo(7, servo_value);
  }



  //   Wire.beginTransmission(ADDRESS);

  // Wire.write(LED0_ON_L);
  // Wire.write(0x00);
  // Wire.write(0x00);
  // Wire.write(byte(value));
  // Wire.write(byte(value >> 8));
  // Wire.endTransmission();
  // Serial.println(value);
}

