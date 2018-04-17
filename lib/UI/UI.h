#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <Menu.h>

class UI {

public:
  UI(uint8_t lcd, uint8_t keypad);
  void begin();
  void display(const Menu_t &menu);
private:
  // Address
  uint8_t lcd_addr = 0x3F;
  uint8_t keypad_addr = 0x21;
  // Backlight
  uint8_t bl_duration = 60; // in seconds
  bool bl_status = false;
  unsigned long bl_time = 0;
  void bl_timer();
  // keypad
  Button_t readButton();
  // Objects
  Keypad_I2C* kpd;
  LiquidCrystal_I2C* lcd;

};

#endif
