#ifndef __FORNO_SMART_H__
#define __FORNO_SMART_H__
/*
  Mcu pins used in this project.

  0,1,13,A0,A1,A2,A3,A4,A5 -> Livres
  2,3,4,5,6,7,8,9,10 -------> Keypad
  11 -----------------------> Auto falante
  12 -----------------------> button para apagar eeprom
  
*/

#include <Arduino.h>
#include <Wire.h> 
#include <stdint.h>
#include <LiquidCrystal_I2C.h>


#include "i2c_rtc_m41t00s.h"
#include "CRtc.h"
#include "CKeypad.h"
#include "CAlarmes.h"


#define LED_FREQUENCY     500
      
#define ALTO_FALANTE      11
#define ERASE_EEPROM      12
#define BUZZER_FREQUENCY  175

#define ON                1
#define OFF               0
#define RESISTENCIAS     12


#define KEY_F1          'A'
#define KEY_F2          'B'
#define KEY_HASHTAG     '#'
#define KEY_ASTERISCO   '*'
#define KEY_DOWN        'V'
#define KEY_LEFT        '<'
#define KEY_UP          '^'
#define KEY_RIGHT       '>'
#define KEY_ENTER       'E'
#define KEY_ESC         'e'
       
#define KEY_NULL        0x00

#define STATE_MAC_NULL0         0x100
#define STATE_MAC_NULL          0x00
#define STATE_MAC_AH_ON         0x01
#define STATE_MAC_AH_OFF        0x02
#define STATE_ACERTA_DATA       0x03
#define STATE_ACERTA_HORA       0x04
#define STATE_ACERTA_DOW        0x05 
#define STATE_GRAVA_DATA_HORA   0x06
#define STATE_CRIA_ALARME1      0x07

extern uint8_t eraseEprom;
extern i2c_rtc_m41t00s rtc;
extern int8_t * getVersion();
extern LiquidCrystal_I2C lcd ;
extern uint8_t calarmeTecla;
extern uint8_t crtcTecla;
extern CAlarme  *  calrm;

extern void songIn(int buzzerPin);
extern int8_t getKey();
extern int8_t getKeyBlock();
extern void trataKeypad();

/*
extern uint8_t key;
extern uint8_t state_machine ;
extern int8_t linha;
extern int8_t coluna;
extern char cdata[11];
extern char chora[11];
extern char dow; 
extern char cflag;
extern char calarme[6];
extern char horaDoAlarme[9];
extern unsigned long  lalarme;
extern bool bflagAlarme;
extern void printBflag();
*/
/*
extern void getKey();
extern void montaTelaAjuste();
extern uint8_t posicionaCursor();
extern void acertaData();
extern void acertaHora();
extern void acertaDow();
extern void gravaRtc();
extern void setAlarme(char * alarme);
extern uint8_t criaAlarme();
extern bool verficaAlarme();
*/

#endif
