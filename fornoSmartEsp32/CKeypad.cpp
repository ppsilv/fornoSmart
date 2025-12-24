/*
Modo normal
Função  key     Descrição
F1        A     Cria alarmes
ESC       ESC   Entra no modo acertar relogio.

Modo acertar relogio
F2        B     Chama rotina para acertar o relogio data, hora e dow(day of week)

*/
#include "fornoSmart.h"

#define ESC_SIMBOL    "@"

bool startAlarme=false;

char hexaKeys[] = {
                    'A', 'B', '#', '*',
                    '1', '2', '3', 'U',
                    '4', '5', '6', 'D',
                    '7', '8', '9', 'S',
                    'L', '0', 'R', 'E'
                 };



const byte ROWS = 5; //five rows
const byte COLS = 4; //four columns
byte rowPins[5] =  {13, 12, 14, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[4] =  {23, 32, 33, 25};    //connect to the column pinouts of the keypad


//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
    
uint8_t key=0;
bool keyEsc=0;
uint32_t escTimeout=0;
const uint16_t escTime = 1000 ;
uint8_t piscaKeyEsc = 0;



int8_t getKey(){
  return customKeypad.getKey();
}

int8_t getKeyBlock(){
  int8_t key;
  while( (key=customKeypad.getKey()) == NO_KEY ){}
  Serial.print("KEY ");Serial.println(key);
 return key;
}

void trataKeypad(){
  key = getKey();
  if ( key != NO_KEY){
    lcd.setCursor(12,1);
    lcd.print(key);
  }
  switch(key){
    case KEY_ESC:
        if( keyEsc == false ){
          keyEsc = true;
          escTimeout = millis() + escTime;
          clock_on = 1;
        }else{
          keyEsc = false;
        }
        break;
    case KEY_F1:
        calarmeon = 1;
        clock_on = 0;
        break;    
    case KEY_F2:
        
        clock_on = 1;
        break;    
  }

  /*
  if( millis() > escTimeout  ){
    escTimeout = millis() + escTime;
    if( keyEsc ){
      if( piscaKeyEsc == 0 ){
        lcd.setCursor(15,0);
        lcd.print(ESC_SIMBOL);
        piscaKeyEsc = 1;
      }else{
        lcd.setCursor(15,0);
        lcd.print(" ");
        piscaKeyEsc = 0;
      }
    }else{
      lcd.setCursor(15,0);
      lcd.print(" ");
      piscaKeyEsc = 0;
    }  
  }
  */
}



