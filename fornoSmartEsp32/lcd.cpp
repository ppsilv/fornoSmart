#include "fornoSmart.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


void setupLcd()
{
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  // print message
  lcd.print(MSG_001);  
}
