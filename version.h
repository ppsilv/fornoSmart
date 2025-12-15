#ifndef __CVERSION_H__
#define __CVERSION_H__
#include <EEPROM.h>
#include <stdint.h>
#include "fornoSmart.h"
#define SERIAL_NUMBER     "20251215"
#define MCU "UNO"
#define VERSION_SIZE  0x10

extern i2c_rtc_m41t00s rtc;

int8_t version[VERSION_SIZE+1];

const uint8_t mes[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

uint16_t getWeekNumber()
{
        int i=0;
    rtc.get();
    uint16_t weekNumber = 0;
    Serial.print("rtc.time.month ["); Serial.print(rtc.time.month); Serial.println("]");
    for(i=1; i < rtc.time.month; i++){
        weekNumber +=  mes[i];
    }
    weekNumber += rtc.time.day;
    
    Serial.print("weekNumber ["); Serial.print(weekNumber); Serial.println("]");
    return weekNumber;
}

int8_t * getVersion()
{
  //apaga eeprom se a chave foi pressionada...
  Serial.print("getVersion: Tecla eraseEprom (");
  Serial.print(eraseEprom);
  Serial.println(")");

  if( eraseEprom == 0 ){
    Serial.println("Tecla eraseEprom Apagando a eeprom...");
    for (int i = 0 ; i < VERSION_SIZE; i++) {
       EEPROM.write(i, 0xFF);
    }
  }

  uint8_t eepromByte0 = EEPROM.read(0);
  getWeekNumber();
  if( eepromByte0 == 0xFF){
    sprintf((char *) version, "%s-%04d%s", SERIAL_NUMBER, getWeekNumber(),MCU );
	  Serial.println("Programming EEProm..");
	  Serial.println((char *)version);

    for (int i=0, j=0 ; i < VERSION_SIZE ; i++,j++) {
      EEPROM.write(i, version[j]);
    }
  }else{
    for (int i=0, j=0 ; i < VERSION_SIZE ; i++,j++) {
      version[j]=EEPROM.read(i);
    } 
	  Serial.println((char *)version);
  }
  return version;
}

#endif
