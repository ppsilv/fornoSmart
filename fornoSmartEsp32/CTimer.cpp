#include "CTimer.h"

CTimer::CTimer()
{
}  
CTimer::CTimer(uint16_t time)
{
  this->time = time;
  this->timeout = millis() + this->time;
}  
//Cria o timer com o tempo especificado
void CTimer::setTimer()
{
  
  this->timeout = millis() + this->time;
}
void CTimer::setTimer(uint32_t time)
{
  this->time = time*1000;
  this->timeout = millis() + this->time;
}
//Verifica se ocorreu timeout
bool CTimer::verifyTimer()
{
  if( status ){
    if( millis() > this->timeout){
      return true;
    }
    return false;
  }
  return false;
}
bool CTimer::verifyTimerTimeout()
{
  if( millis() > this->timeout){
    return true;
  }
  return false;
}
void CTimer::timerRenew(uint16_t time)
{
  this->time = time;
  this->timeout = millis() + this->time;
}
bool CTimer::getTimerStatus()
{
  return status;
}

void CTimer::setTimerStatus(bool status)
{
  this->status = status;
}
