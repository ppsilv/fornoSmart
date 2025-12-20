#include "fornoSmart.h"

void liga_temp_control()
{
  Serial.println("Ligando as resistencias do forno...");
  digitalWrite(RESISTENCIAS, ON); 
}

void desliga_temp_control()
{
  Serial.println("DESLigando as resistencias do forno...");
  digitalWrite(RESISTENCIAS, OFF); 
}