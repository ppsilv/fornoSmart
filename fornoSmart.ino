#include "fornoSmart.h"
#include "version.h"
#include "CPiscaLed.h"
#include "CAlarmes.h"
#include "CRtc.h"

LiquidCrystal_I2C lcd(0x27,16,2);
CPiscaLed led =  CPiscaLed();
CAlarme  *  calrm = CAlarme::getInstancia();
CRtc  *  crtc = CRtc::getInstancia();


uint8_t eraseEprom=1;

void setup() {
  unsigned char eraseTimeout = 5 ;

  // initialize digital pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ALTO_FALANTE, OUTPUT);
  pinMode(ERASE_EEPROM, INPUT_PULLUP);

	Serial.begin(115200);
	lcd.init();
	lcd.backlight();

	lcd.setCursor(0,0);
	lcd.print("Forno Eletrico  ");
	lcd.setCursor(0,1);
	lcd.print("Aguarde inicio  ");

  Serial.println("Pressione a tecla para apagar eeprom...");
  while( ((eraseEprom = digitalRead( (ERASE_EEPROM))) == HIGH) && eraseTimeout >= 1 ){
    eraseTimeout--;
    delay(1000);
  }
  Serial.print("eraseEprom (");
  Serial.print(eraseEprom);
  Serial.println(")");

	lcd.setCursor(0,1);
	lcd.print((char *) getVersion() );
/*
  lcd.setCursor(0,2);
  lcd.print("Forno Eletrico 20231");
  lcd.setCursor(0,3);
  lcd.print((char *) getVersion() );
*/

 // lcd.clear();
  led.configTimeout(400);

//  songIn(ALTO_FALANTE);
  lcd.clear();
}

// the loop function runs over and over again forever
void loop() {
	led.pisca();
  
  
  crtc->getDate();
  crtc->getTime();
  
  calrm->rodaAlarme();
  calrm->verificaAlarme();
  
  trataKeypad();
  
	delay(50);
  //  CAlarme::getInstancia()->verificaAlarme();
}


//void ftrataKeyboard(void *pvParameters __attribute__((unused)) )
//{
//
//  while(true){
//    vTaskDelay(10/portTICK_PERIOD_MS);
//          
//    if ( le_keyboard() > 0 ){
//      
//      // wait for a second
//      vTaskDelay(2000/portTICK_PERIOD_MS);
//      lcd.clear();
//    }
//  }
//}
