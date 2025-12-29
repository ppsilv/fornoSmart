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

char keys[] = {
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
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

uint8_t key=0;
bool keyEsc=0;
uint32_t escTimeout=0;
const uint16_t escTime = 1000 ;
uint8_t piscaKeyEsc = 0;

int8_t getKey(){
  return kpd.getKey();
}

int8_t getKeyBlock(){
  int8_t key;
  while( (key=kpd.getKey()) == NO_KEY ){}
  Serial.print("KEY ");Serial.println(key);
 return key;
}
/*
void trataKeypad(){
  key = getKey();
  switch(key){
    case KEY_ESC:
        if( keyEsc == false ){
          keyEsc = true;
          escTimeout = millis() + escTime;

        }else{
          keyEsc = false;
        }
        break;
    case KEY_F1:
        calarmeon = 1;

        break;    
    case KEY_F2:        

        set_temp_control();
        break;            
    case KEY_ARROBA:        
        set_temp_control();
    case KEY_ASTERISCO:        
        lcd.setCursor(11,1);
        lcd.print(getCelsius());
        break;    
  }
}
*/
char cline[17]={'\0'};
char pos=0;

void lcdBackSpace(char pos)
{
  char pos1 = pos--;  
  lcd.setCursor(pos1, 1);
  lcd.print(' ');
}
void lcdlineClear(char line)
{
  lcd.setCursor(0, line);
  lcd.print("                ");
  lcd.setCursor(0, line);
}
void lcdResetLine()
{
  pos=0;
  memset(cline, ' ', 16);
  lcdlineClear(1);  
}
bool lineEdit(char *msg)
{
  lcdResetLine();
  lcd.cursor_on();
  lcd.blink(); 
  lcd.clear();
  lcd.print(msg);
  lcd.setCursor(0, 1);
  kpd.getKey(); 
  while(1){
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) 
                {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                case PRESSED:
                    break;
                case HOLD:
                    break;
                case RELEASED:
                    if( kpd.key[i].kchar == 'E'){
                      Serial.println(cline);
                      pos=0;
                      lcd.clear();
                      lcd.cursor_off();
                      lcd.blink_off();
                      return true;
                    }else if(kpd.key[i].kchar == 'S'){
                      lcd.clear();
                      lcd.cursor_off();
                      lcd.blink_off();
                      return false;  
                    }else if( kpd.key[i].kchar == '#'){
                      lcdResetLine();
                    }else if( kpd.key[i].kchar == 'L' ){
                      pos--;  
                      lcdBackSpace(pos);
                      lcd.setCursor(pos, 1);
                    }else if( kpd.key[i].kchar == 'R' ){
                      pos++;
                      lcd.setCursor(pos, 1);
                      cline[pos]='\0';
                    }else{
                      cline[pos++] = kpd.key[i].kchar;
                      lcd.print(kpd.key[i].kchar);
                    }
                    break;
                case IDLE:
                    break;
                }
            }
        }
    }
  }
  return false;
}

int getNumber(char * msg)
{
  if ( ! lineEdit(msg) )
    return 0;
  return atoi(cline);
}
char * getText(char * msg)
{
  lineEdit(msg);
  return cline;
}



