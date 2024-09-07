#include <Wire.h>

#define ADDRESS 0x40

#define MODE1 0x00
#define MODE2 0x01

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09



void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("begin setup");

  Wire.beginTransmission(ADDRESS);
  Wire.write(MODE1);
  Wire.write(0b00100001);

  Wire.endTransmission();

  Wire.beginTransmission(ADDRESS);
  Wire.write(MODE2);
  Wire.write(0b00000100);

  Wire.endTransmission();
  Serial.println("setup done");



}
int value;
void loop() {
  if (Serial.available() > 1){
    char key = Serial.read();
    value = Serial.parseInt();

    Wire.beginTransmission(ADDRESS);

  Wire.write(LED0_ON_L);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(byte(value));
  Wire.write(byte(value >> 8));
  Wire.endTransmission();
  Serial.println(value);
  }

}
