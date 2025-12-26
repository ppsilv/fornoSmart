#ifndef __CPISCA_LED__
#define __CPISCA_LED__
#include "Arduino.h"

class CPiscaLed{
public:
    CPiscaLed(){
      time = millis() + timeout ;
      pinMode(pinLed,OUTPUT);
    }

    void pisca(){

      if( millis() > time ){

        if( ledStatus == false ){
        	time = millis() + timeout ;
        	this->led_on();
        	ledStatus = true;
        }else{
        	time = millis() + timeout ;
        	this->led_off();
        	ledStatus = false;
        }
      }
    }
    
    void configTimeout(uint16_t timeout){
    	this->timeout = timeout;
    }

private:
  const uint8_t pinLed = LED_BUILTIN;
  uint16_t timeout = 300;
  uint16_t time;
  bool ledStatus=false;

  void led_on(){
    digitalWrite(pinLed,HIGH);
  }

  void led_off(){
    digitalWrite(pinLed,LOW);
  }

};

#endif
