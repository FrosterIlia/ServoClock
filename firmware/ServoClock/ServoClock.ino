#include <Wire.h>
#include "Digit.h"
#include "Timer.h"
#include "Display.h"
#include "RTC.h"
#include "EasyButton.h"

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09


#define YELLOW_BUTTON_PIN 4
#define RED_BUTTON_PIN 2


// value means off_time value
// value 345 is around 0 degrees rotation
// value 2130 is around 180 degrees rotation


Display display;
RTC rtc;

EasyButton red_button(RED_BUTTON_PIN);
EasyButton yellow_button(YELLOW_BUTTON_PIN);

void setup() {

  Wire.begin();
  Serial.begin(9600);
  display.clear();

  // rtc.set_sys_time();

}

uint8_t digit_values[4];

Timer timer(1000);
Timer settings_timer(1000);
uint8_t settings_counter;
bool loading_flag = true;
void loop() {
  display.tick();
  red_button.tick();
  yellow_button.tick();


  if (red_button.get_state()){ // settings
    static uint16_t current_time;
    static bool current_digit_state = 1;
    if (loading_flag){
      settings_counter = 1; 
      rtc.request_time();
      current_time = rtc.get_hours() * 100 + rtc.get_minutes();

      uint8_t number_length = count_digits(current_time);
      uint16_t divisor = myPow(10, number_length - 1);

      for (uint8_t i = 0; i < number_length; i++) {

        digit_values[i] = current_time / divisor;

        current_time %= divisor;
        divisor /= 10;
      }
      for (uint8_t i = number_length; i < DIGITS_NUMBER; i++) {
        digit_values[i] = 0;
      }

      loading_flag = false;
    }
    
    if (settings_timer.isReady()){ // blinking
      if (current_digit_state) display.clear_digit(settings_counter - 1);
      else {
        for (uint8_t i = 0; i < 4; i++){
          if (i == settings_counter - 1) display.write_digit(settings_counter - 1, digit_values[settings_counter - 1]);
          else display.write_digit(i, digit_values[i]);
        }
      }
      current_digit_state = !current_digit_state;
    }

    if (yellow_button.isHolded()) { // switch between digits
      if (++settings_counter > 4) settings_counter = 1;
    }

    if (yellow_button.isClick()){ //increment digit
      if (settings_counter - 1 == 0){
        if (++digit_values[settings_counter - 1] > 2) digit_values[settings_counter - 1] = 0;
      }
      else{
        if (++digit_values[settings_counter - 1] > 9) digit_values[settings_counter - 1] = 0;
      }
    }
  }
  else if (red_button.isReleased()){
    loading_flag = true;
    Serial.println(digit_values[0] * 1000 + digit_values[2] * 10 + digit_values[1] * 100 + digit_values[3]);
    rtc.set_time(digit_values[0] * 1000 + digit_values[2] * 10 + digit_values[1] * 100 + digit_values[3]);
  }
  else{
    
    if (timer.isReady()) { //work mode
    rtc.request_time();
    display.write_number(rtc.get_hours() * 100 + rtc.get_minutes());

    }
  } 
}

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

