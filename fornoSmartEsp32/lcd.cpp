#include "fornoSmart.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
static bool backlight = false;

void setupLcd()
{
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  backlight = true;

  lcd.setCursor(0, 0);
  // print message
  lcd.print(MSG_001);  
}
void turn_on_back_light()
{
  backlight = true;
  lcd.backlight();
}
void turn_off_back_light()
{
  backlight = false;
  lcd.noBacklight();
}

void turn_back_light()
{
  if ( backlight ){
    backlight = false;
    lcd.noBacklight();
  }else if ( ! backlight ){
    backlight = true;
    lcd.backlight();
  }

}