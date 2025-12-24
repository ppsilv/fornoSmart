#include "fornoSmart.h"

#define NO_KEY    -1

CAlarme * CAlarme::instancia=0x00;
uint8_t  calarmeon=0;
int multiplicador[3] = {100,10,1};

CAlarme::CAlarme()
{
	sprintf(calarme,"               ");
	key = 0;
	linha = 0;
	coluna = 0;
  calarmeon = 0;
  qtd_minutos = 0;
  CTimer alarmeTimeout = CTimer();
}

CAlarme * CAlarme::getInstancia()
{
    if( instancia == 0x00 ){
      CAlarme::instancia = new CAlarme();
    }
    return instancia;
}

void CAlarme::setAlarme(uint16_t alarme)
{
  Serial.println("Function set alarme...");
}

uint8_t CAlarme::criaAlarme()
{
  uint8_t col0,col1,col3,fl=0;

  calarmeon = 0;
  lcd.clear();
  lcd.print("Quanto Tempo");
  lcd.cursor_on();
  lcd.blink();
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna,linha);
  Serial.println(calarme);
  lcd.print(calarme);
  lcd.setCursor(coluna,linha);
  key = getKey();
  qtd_minutos = 0;
  alarmeTimeout.setTimerStatus(false);
  while( key != KEY_ENTER){
    key = getKey();
    switch( key ){
      case KEY_ESC:
          key = NO_KEY;
          clock_on = 1;
          bflagAlarme = false;
          lcd.clear();
          return 0;
          break;
      case KEY_RIGHT:
          coluna++;
          lcd.setCursor(coluna,linha);
          break;
      case KEY_LEFT:
          coluna--;
          lcd.setCursor(coluna,linha);
          lcd.print(' ');
          lcd.setCursor(coluna,linha);
          break;
      default:    
          if( (key >= '0') && (key <= '9') ){
            qtd_minutos += multiplicador[coluna] * (key - '0');
            Serial.print("Total minutos");
            Serial.println(qtd_minutos); 
            lcd.print(key - '0');
            coluna++;
          }
    }
    if(coluna >= 3){
      coluna = 3;
      lcd.setCursor(coluna,linha);
    }
  }
  Serial.print("Valor do alarme: ");
  Serial.println(qtd_minutos);
  alarmeTimeout.setTimer(qtd_minutos);
  clock_on = 1;
  bflagAlarme = true;
  liga_temp_control();
  lcd.clear();
  lcd.cursor_off();
  lcd.blink_off();
  return 1;
}
void CAlarme::piscaIcone(uint8_t status)
{
  if( bflagAlarme && alarmeTimeout.getTimerStatus() ){
    lcd.setCursor(11,0);
    if(status)
      lcd.print("TIMER");
    else  
      lcd.print("     ");
  }else if(bflagAlarme){
    lcd.setCursor(11,0);
    lcd.print("STOP");
  }
}
bool CAlarme::verificaAlarme()
{
  if( bflagAlarme ){
    if( alarmeTimeout.verifyTimer() ){
      lcd.clear();
      bflagAlarme = false;
      desliga_temp_control();
      //songIn(ALTO_FALANTE);   
      return true;
    }
  }
  alarmeTimeout.setTimerStatus(true);
  return false;
}

void CAlarme::setAlarm(bool status)
{
    bflagAlarme = status;
}

void CAlarme::rodaAlarme()
{
  if( calarmeon == 1 ){
    criaAlarme();
  }
}

bool CAlarme::getAlarmeStatus()
{
  return bflagAlarme;
}

char * CAlarme::getHoradoAlarme()
{
    return horaDoAlarme  ;
}