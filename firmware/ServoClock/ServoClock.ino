#include <Wire.h>
#include "Digit.h"
#include "Timer.h"
#include "Display.h"



#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09


// value means off_time value
// value 345 is around 0 degrees rotation
// value 2130 is around 180 degrees rotation


Display display;

void setup() {

  Wire.begin();
  Serial.begin(9600);
  display.clear();

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  // Serial.println("hello1");

  // Serial.println("hello2");
 
}

Timer timer(2500);
int value = 0;
void loop() {
  display.tick();


  // Serial.print(digitalRead(2));
  // Serial.print(" ");
  // Serial.println(digitalRead(4));
  if (timer.isReady() && digitalRead(2)) {

    value++;
    if (value == 10) value = 0;
    display.write_number(1533);


  }
}
