#include <Arduino.h>
#include <UI.h>

#define LCD_ADDR 0x3F
#define KPD_ADDR 0x21

UI ihm(LCD_ADDR, KPD_ADDR);

void setup()
{
  ihm.begin();
}

void loop()
{
  ihm.display();
}
