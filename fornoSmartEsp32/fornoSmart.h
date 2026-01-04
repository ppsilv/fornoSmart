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
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

#include <time.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

#include <Keypad.h>
#include "CTimer.h"

#include <stdio.h>
#include <time.h>
#include <string.h> // For memset, good practice

extern void ota_setup();

#define LED_FREQUENCY     500
//#define PIN_BUZZER        
#define ALTO_FALANTE      11
#define ERASE_EEPROM      12
#define BUZZER_FREQUENCY  175

#define ON                1
#define OFF               0
#define RESISTENCIAS     17

#define KEY_F1 65
#define KEY_F2 66
#define KEY_ARROBA  35
#define KEY_ASTERISCO  42
#define KEY_1  49
#define KEY_2  50
#define KEY_3  51
#define KEY_UP 85

#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_DW 68

#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_ESC 83

#define KEY_LEFT   76
#define KEY_0     48
#define KEY_RIGHT   82
#define KEY_ENTER 69
       
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
extern int8_t * getVersion();
extern LiquidCrystal_I2C lcd;
extern bool resistencias;
extern void songIn(int buzzerPin);

//prototypes
extern int8_t getKey();
extern void setupClock();
extern void setupLcd();
extern void setupSensor();
extern void telaInicial();
extern bool lineEdit(char *msg);
extern void setupResistencias();
extern void liga_resistencias();
extern void desliga_resistencias();
extern float getCelsius();
extern void liga_temp_control();
extern void desliga_temp_control();
extern int getNumber(char * msg);
extern char * getText(char * msg);
extern uint32_t getTimeStamp();

//Mensagens
//                    "1234567890123456"
#define MSG_001       "FornoSmart    V5"    // ESP control time and temperature without PID
#define LED 18

#endif
