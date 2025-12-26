#include "fornoSmart.h"

long timezone = -3;
byte daysavetime = 0;
struct tm tmstruct ;

std::string sdata = "";
std::string shora = "";
std::string lastDate = "";
std::string lastTime = "";

extern void fontePrint(std::string str);
extern void setTextColor(int cor, int fundo);
extern void setCursor(int col, int row);
extern void setTextSize(int size);
extern void clear(int fundo);
bool clock_on=false;

std::string format_number(int number) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << number;
    return ss.str();
}
unsigned int getHour()
{
  return tmstruct.tm_hour;
}
unsigned int getMinuto()
{
  return tmstruct.tm_min;
}
void getDateTime()
{
  getLocalTime(&tmstruct);

  int imes = tmstruct.tm_mon + 1;

  String ano = String((tmstruct.tm_year) + 1900) ;
  std::string mes = format_number(imes );
  std::string dia = format_number(tmstruct.tm_mday);
  
  std::string hora =  format_number(tmstruct.tm_hour);
  std::string min  =  format_number(tmstruct.tm_min);
  std::string seg  =  format_number(tmstruct.tm_sec);

  sdata = dia +"/"+ mes +"/" + ano.c_str();
  shora = hora +":"+ min  +":"+ seg; // hour.c_str();

}

void printDateTime()
 {


  lcd.setCursor(0,0);
  lcd.print(sdata.c_str());  
  lcd.setCursor(0,1);
  lcd.print(shora.c_str());

  lastTime = shora;
  lastDate = sdata;

}

void setupClock() {

  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  tmstruct.tm_year = 0;
  getDateTime();
  lastTime = shora;
  lastDate= sdata;
  printDateTime();
  clock_on = true;
}

void loopClock() 
{
    if( ! clock_on ){                     
      return; 
    }
  
  getDateTime();
  printDateTime();
}