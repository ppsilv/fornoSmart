#include "fornoSmart.h"

#define NO_KEY    -1

CAlarme * CAlarme::instancia=0x00;
uint8_t  calarmeon=0;
//int multiplicador[3] = {100,10,1};
uint8_t iconeStatus = 0;
uint8_t  verificaTemp = 10;
uint16_t timerdelay=500;

static uint16_t blink_delay=500;

CTimer TPiscaIcone = CTimer(timerdelay);


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

    if( calarmeon == 0 ){
      return 0;
    }

  calarmeon = 0;
  qtd_minutos = 0;
  qtd_minutos = getNumber("Quanto Tempo");
  
  if( qtd_minutos == 0){
    clock_on = 1;
    return 0;
  }
  alarmeTimeout.setTimerStatus(false);
  Serial.print("Valor do alarme: ");
  Serial.println(qtd_minutos);
  alarmeTimeout.setTimer(qtd_minutos);
  clock_on = 1;
  bflagAlarme = true;
  liga_temp_control();

  return 1;
}
void CAlarme::piscaIcone()
{
  if ( ! TPiscaIcone.verifyTimerTimeout() ){
    return;
  }
  iconeStatus += 1;
  iconeStatus &= 1;
  TPiscaIcone.timerRenew(blink_delay);
  verificaTemp += 1;

  if( bflagAlarme && alarmeTimeout.getTimerStatus() ){
    lcd.setCursor(11,0);
    if(iconeStatus)
      lcd.print(temperatura);
    else  
      lcd.print("     ");
    lcd.setCursor(11,1);
    lcd.print(getCelsius());
  }else if( bflagAlarme ){
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
