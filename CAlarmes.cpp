#include "fornoSmart.h"

#define NO_KEY    -1

CAlarme * CAlarme::instancia=0x00;
uint8_t  calarmeTecla=0;

CAlarme::CAlarme()
{
	sprintf(calarme,"  :  ");
	key = 0;
	linha = 0;
	coluna = 0;
  calarmeTecla = 0;
}

CAlarme * CAlarme::getInstancia()
{
    if( instancia == 0x00 ){
      CAlarme::instancia = new CAlarme();
    }
    return instancia;
}

void CAlarme::setAlarme(char * alarme)
{
  unsigned long lhora,lminu;
  unsigned long hora,minuto;
  //alarme formatado hh:mm

  Serial.println("Function set alarme...");

  hora   = (*(alarme+0) - '0') * 10;
  hora  += (*(alarme+1) - '0');
  minuto = (*(alarme+3) - '0') * 10;
  minuto+= (*(alarme+4) - '0');

  lalarme = ((unsigned long)hora * (unsigned long)3600) + ((unsigned long)minuto * (unsigned long)60)  ;
  lalarme *= (unsigned long)1000; 
  alarmeTimeout = (unsigned long)millis() + (unsigned long)lalarme; 
  bflagAlarmeSignTime = millis()+ bflagAlarmeSignTimeout;

  unsigned int hora1   = rtc.time.hour+hora;
  unsigned int minuto1 = rtc.time.min+minuto;

  if(hora1 >= 24){
    hora1 -= 24;
  }
  if( minuto1 > 60 ){
    minuto1 = (minuto1 - 60);
    hora1++;
  }
  rtc.get();
  
  sprintf(horaDoAlarme, "%02u:%02u:%02u",hora1,minuto1, rtc.time.sec);

  bflagAlarme = true;

  sprintf(charVal, "%08ld", lalarme);
  Serial.print("lalarmes: ");
  Serial.print(charVal);

  sprintf(charVal, "%08ld", millis());
  Serial.print("   millis(): ");
  Serial.print(charVal);

  sprintf(charVal, "%08ld", alarmeTimeout);
  Serial.print("   lalarme + millis(): ");
  Serial.println(charVal);

  Serial.print("Hora do alarme: ");Serial.println(horaDoAlarme);
  lcd.clear();
}

uint8_t CAlarme::criaAlarme()
{
  uint8_t col0,col1,col3,fl=0;

  calarmeTecla = 0;

  lcd.clear();
  
  lcd.print("Digite o Tempo");
  linha = 1;
  coluna = 0;  
  lcd.setCursor(coluna,linha);
  Serial.println(calarme);
  lcd.print(calarme);
  lcd.setCursor(coluna,linha);
  key = getKey();
  
  while( key != KEY_ENTER){
    if( key == KEY_RIGHT){
      coluna++;
      lcd.setCursor(coluna,linha);
    }else if( key == KEY_LEFT){
      coluna--;
      lcd.setCursor(coluna,linha);
    }else if( (key >= 0) && (key <= 9) ){
      switch(coluna){
        case 0: 
          col0 = key;
          if( col0 > 2){
            fl=1;
          }   
          break;
        case 1: 
          col1 = key; 
          if(col0 ==2){
            if(col1 >3){
              fl =1;
            }
          }
          break;
        case 2:
           coluna++; 
        case 3: 
          col3 = key; 
          if(col3 > 5){
              fl =1;
          }
          break;
      }
      Serial.print("Coluna ");Serial.print(coluna);
      Serial.print(" key ");Serial.println(key);
      if( coluna <= 4 ){
        lcd.setCursor(coluna,linha);
        lcd.print(key);
        calarme[coluna] = key+0x30;
        if(!fl)
          coluna++;
        else
          fl=0;
      }  
      lcd.setCursor(coluna,linha);
    }
    key = getKey();
    if( key == 'e' ){
      return(0);
    }
  }
  setAlarme(calarme);
  return 1;
}

bool CAlarme::verificaAlarme()
{
  if( bflagAlarme ){
    if( millis() > alarmeTimeout){
      bflagAlarme = false;
  sprintf(charVal, "%08ld", millis());
  Serial.print("Alarme terminou  millis(): ");
  Serial.println(charVal);      
      return true;
    }
    piscaIcone();
  }
  return false;
}

void CAlarme::piscaIcone()
{
  static bool bflagAlarmeSignStatus = false; 
  if( bflagAlarme ){
    if (millis() > bflagAlarmeSignTime ){
  //sprintf(charVal, "%08ld",(unsigned long) millis());
  //Serial.print(" printBflag  millis(): ");
  //Serial.print(charVal);      

      bflagAlarmeSignTime = millis()+ bflagAlarmeSignTimeout;
  //sprintf(charVal, "%08ld", (unsigned long)bflagAlarmeSignTime);
  //Serial.print(" printBflag  bflagAlarmeSignTime: ");
  //Serial.println(charVal);      


      if( ! bflagAlarmeSignStatus ){
        lcd.setCursor(iconeColuna,iconeLinha);
        lcd.print(iconeAlarme);
        bflagAlarmeSignStatus = true;
      }
      else{
        lcd.setCursor(iconeColuna,iconeLinha);
        lcd.print(" ");
        bflagAlarmeSignStatus = false;
      }  
    }
  }else{
    lcd.setCursor(iconeColuna,iconeLinha);
    lcd.print(" ");
  }
}

void CAlarme::rodaAlarme()
{
  if( calarmeTecla == 1 ){
    criaAlarme();
  }
}