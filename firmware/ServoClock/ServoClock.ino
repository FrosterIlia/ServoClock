#include <Wire.h>
#include "Digit.h"
#include "Timer.h"
#include "Display.h"

#define ADDRESS1 0x40
#define ADDRESS2 0x41

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09


// value means off_time value
// value 345 is around 0 degrees rotation
// value 2130 is around 180 degrees rotation

Digit digit1(ADDRESS1);
Digit digit2(ADDRESS2);


void setup() {

  Wire.begin();
  Serial.begin(9600);
  digit1.clear_digit();
  digit2.clear_digit();

}

Timer timer(2000);
int value = 0;

void loop() {
  digit1.tick();
  digit2.tick();

  if (timer.isReady()) {

    value++;
    if (value == 10) value = 0;

    digit1.write(value);
    digit2.write(value);

  }
}
