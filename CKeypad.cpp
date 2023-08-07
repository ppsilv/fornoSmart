/*
Modo normal
Função  key     Descrição
F1        A     Cria alarmes
ESC       ESC   Entra no modo acertar relogio.

Modo acertar relogio
F2        B     Chama rotina para acertar o relogio data, hora e dow(day of week)

*/

#include "fornoSmart.h"
#include <Keypad.h>
#include "CKeypad.h"

#define ESC_SIMBOL    "@"

const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
char hexaKeys[5][4] = {
                                {'A','B','#','*'},
                                { 1, 2, 3,'^'},
                                { 4, 5, 6,'v'},
                                { 7, 8, 9,'e'},
                                {'<', 0, '>','E'}
                            };
byte rowPins[5] =  {10, 7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[4] =  {8, 9, 2, 3 };    //connect to the column pinouts of the keypad


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
  if( key == 'e' ){
    if( keyEsc == false ){
      keyEsc = true;
      escTimeout = millis() + escTime;
    }else{
      keyEsc = false;
    }
  }else if( key == 'A' ){
    calarmeTecla = 1;
  }else if( ( key == 'B' ) && keyEsc ){
    if( keyEsc ){
      crtcTecla = 2;
      keyEsc = false;
    }else{
      crtcTecla = 1;
    }
  }
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
}



