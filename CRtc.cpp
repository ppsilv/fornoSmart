#include "fornoSmart.h"

CRtc * CRtc::instancia=0x00;
i2c_rtc_m41t00s rtc;
uint8_t crtcTecla=0;

CRtc::CRtc()
{
	instancia=0x00;
	sprintf((char *) cdata, (const char*)("  /  /") );
	sprintf((char *) chora, (const char*)("  :  :") );
	dow=0;
	linha = 0;
	coluna= 0;
	key   = 0;
}

void CRtc::acertoRtc()
{
    crtcTecla = 0;
    lcd.cursor();
    acertaData();
    acertaHora();
    acertaDow();
    gravaRtc();
    lcd.noCursor();
}
CRtc * CRtc::getInstancia()
{
  if ( instancia == 0x00 ){
    instancia = new CRtc();
  }
  return instancia;
}

void CRtc::acertaData()
{
  lcd.clear();
  lcd.print("Digite a data");
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna, linha);
  lcd.print(cdata);
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna, linha);
  key = getKey();
  while( key != KEY_ENTER){
    if( key == KEY_RIGHT){
      coluna++;
      lcd.setCursor(coluna,linha);
    }else if( key == KEY_LEFT){
      coluna--;
      lcd.setCursor(coluna,linha);
    }else if( (key >= 0) && (key <= 9) ){
      if( coluna < 10){
        lcd.print(key);
        cdata[coluna] = key+0x30;
        coluna++;
        if( (coluna == 2) || (coluna == 5) ){
          coluna++;
        }
        lcd.setCursor(coluna,linha);
      }
    }
    key = getKey();
  }
  Serial.print("Data..: ");
  Serial.println(cdata);
  return;
}

void CRtc::acertaHora()
{
  lcd.clear();
  lcd.print("Digite a hora");
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna, linha);
  lcd.print(chora);
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna, linha);
  key = getKey();
  while( key != KEY_ENTER){
    if( key == KEY_RIGHT){
      coluna++;
      lcd.setCursor(coluna,linha);
    }else if( key == KEY_LEFT){
      coluna--;
      lcd.setCursor(coluna,linha);
    }else if( (key >= 0) && (key <= 9) ){
      if( coluna < 8){
        lcd.print(key);
        chora[coluna] = key+0x30;
        coluna++;
        if( (coluna == 2) || (coluna == 5) ){
          coluna++;
        }
        lcd.setCursor(coluna,linha);      
      }
    }
    key = getKey();

  }
  Serial.print("Hora..: ");
  Serial.println(chora);
  return;
}
void CRtc::acertaDow()
{
  lcd.clear();
  lcd.print("Dom a Sab:1 a 7");
  linha = 1;
  coluna = 0;
  lcd.setCursor(coluna, linha);
  key = getKey();
  while( key != KEY_ENTER){
    if( (key >= 0) && (key <= 9) ){
      if ( (key == 0)||(key == 8)||(key == 9) ){
        lcd.setCursor(coluna+2, linha);
        lcd.print(key);
        lcd.print(" nao pode...");
        delay(2000);
        lcd.setCursor(coluna+1, linha);
        lcd.print("              ");
        lcd.setCursor(coluna, linha);
      }else{
        lcd.print(key);
        dow = key;
      }
    }
    lcd.setCursor(coluna, linha);
    key = getKey();
  }
  lcd.clear();
}
void CRtc::gravaRtc()
{
  uint8_t x, y, u8Status;
  i2c_rtc_m41t00s::time_t newtime;
  //domingo=1,segunda=2,terça=3,quarta=4,quinta=5,sexta=6,sabado=7
  //0123456789    01234567
  //28/07/2023    12:12:12
  Serial.print("Data  ");
  Serial.print(cdata);
  Serial.print("  hora  ");
  Serial.println(chora);
  x = cdata[8]; y = cdata[9];
  newtime.year  = 10 * (x - '0') + (y - '0');
  x = cdata[3]; y = cdata[4];
  newtime.month = 10 * (x - '0') + (y - '0');
  x = cdata[0]; y = cdata[1];
  newtime.day   = 10 * (x - '0') + (y - '0');


  x = chora[0]; y = chora[1];
  newtime.hour  = 10 * (x - '0') + (y - '0');
  x = chora[3]; y = chora[4];
  newtime.min   = 10 * (x - '0') + (y - '0');
  x = chora[6]; y = chora[7];
  newtime.sec   = 10 * (x - '0') + (y - '0');

  x = chora[6]; y = chora[7];

  newtime.dow = dow;

  u8Status = rtc.set(&newtime);
  if (u8Status)
  {
    Serial.print("Unable to set time due to I2C error 0x");
    Serial.print(u8Status, HEX);
    Serial.println(".");
  }else{
    Serial.println("Relogio acertado...");
  }
}
void CRtc::getTime()
{
  char data[12]={0};
  //lcd.setCursor(0,1);
  //lcd.print("                ");
  if( !rtc.get() ){
    if( !calrm->getAlarmeStatus() ){
      sprintf(data,"%02d:%02d:%02d     ",rtc.time.hour,rtc.time.min,rtc.time.sec);
      lcd.setCursor(0,1);
      lcd.print(data);
    }else{
      sprintf(data,"Hora:%02d:%02d:%02d",rtc.time.hour,rtc.time.min,rtc.time.sec);
      lcd.setCursor(0,0);
      lcd.print(data);
      sprintf(data,"Alrm:%02s",calrm->getHoradoAlarme());
      lcd.setCursor(0,1);
      lcd.print(data);
    }
  }
}

void CRtc::getDate()
{
  char data[12]={0};
  //lcd.setCursor(0,1);
  //lcd.print("                ");
  if( !rtc.get() ){
    if( !calrm->getAlarmeStatus() ){
      sprintf(data,"%02d/%02d/20%02d   ",rtc.time.day,rtc.time.month,rtc.time.year);
      lcd.setCursor(0,0);
      lcd.print(data);
    }  
  }
}

void CRtc::rodaRtcAcerto()
{
  if( crtcTecla == 2 ){
    acertoRtc();
  }
}
