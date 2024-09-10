#include <Wire.h>
#include "Digit.h"
#include "Timer.h"

#define ADDRESS 0x40

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

// value means off_time value
// value 345 is around 0 degrees rotation
// value 2130 is around 180 degrees rotation
Digit digit1(ADDRESS);
int servo_value;
void setup() {

  Wire.begin();
  Serial.begin(9600);
  digit1.clear_digit();

}

Timer timer(1200);
int value = 0;

void loop() {
  if(timer.isReady()){
    
    value++;
    if (value == 10) value = 0;

    digit1.write(value);
  } 
  


  if (Serial.available() > 1) {
    servo_value = Serial.parseInt();
    digit1.write(servo_value);

  }
}
