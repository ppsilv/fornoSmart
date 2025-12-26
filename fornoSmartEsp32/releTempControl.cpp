#include "fornoSmart.h"

bool resistencias=false;

void setupResistencias()
{
  pinMode(RESISTENCIAS, OUTPUT);
}

void liga_resistencias()
{
  if( resistencias )
    return;
  Serial.println("Ligando as resistencias do forno...");
  digitalWrite(RESISTENCIAS, ON); 
  resistencias = true;
}

void desliga_resistencias()
{
  if( ! resistencias )
    return;
  Serial.println("DESLigando as resistencias do forno...");
  digitalWrite(RESISTENCIAS, OFF); 
  resistencias = false;
}
