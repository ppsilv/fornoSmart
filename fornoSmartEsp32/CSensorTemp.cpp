#include "fornoSmart.h"
#include "MAX6675.h"


const int dataPin   = 4;
const int clockPin  = 5;
const int selectPin = 15;

static uint8_t	key = 0;
uint8_t	linha = 0;
uint8_t	coluna = 0;

MAX6675 thermoCouple(selectPin, dataPin, clockPin);

uint32_t start, stop;
uint16_t temp2set=0;
uint16_t temperatura=0;
static int multiplicador[3] = {100,10,1};

void setupSensor()
{
  Serial.println(__FILE__);
  Serial.print("MAX6675_LIB_VERSION: ");
  Serial.println(MAX6675_LIB_VERSION);
  Serial.println();
  delay(250);

  SPI.begin();

  thermoCouple.begin();
  thermoCouple.setSPIspeed(4000000);
}

float getCelsius()
{
    delay(400);
    thermoCouple.read();
    return thermoCouple.getCelsius();
}

void sensorLoop()
{
  delay(100);
  start = micros();
  int status = thermoCouple.read();
  stop = micros();
  float temp = thermoCouple.getCelsius();

  Serial.print(millis());
  Serial.print("\tstatus: ");
  Serial.print(status);
  Serial.print("\ttemp: ");
  Serial.print(temp);
  Serial.print("\traw: ");
  Serial.print(thermoCouple.getRawData(), HEX);
  Serial.print("\tus: ");
  Serial.println(stop - start);

  delay(1000);
}