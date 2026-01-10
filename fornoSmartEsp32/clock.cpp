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

  String ano = String(tmstruct.tm_year-100);//String((tmstruct.tm_year) + 1900) ;
  std::string mes = format_number(imes );
  std::string dia = format_number(tmstruct.tm_mday);
  
  std::string hora =  format_number(tmstruct.tm_hour);
  std::string min  =  format_number(tmstruct.tm_min);
  std::string seg  =  format_number(tmstruct.tm_sec);

  sdata = dia +"/"+ mes +"/" + ano.c_str();
  shora = hora +":"+ min  +":"+ seg; // hour.c_str();

}
void printTime()
{
  lcd.setCursor(0,1);
  lcd.print(shora.c_str());

  lastTime = shora;
  lastDate = sdata;
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
}

void telaInicial() 
{
  getDateTime();
  printDateTime();
}

void print_time() 
{
  getDateTime();
  printTime();
}

uint32_t getTimeStamp() {
    struct tm tm = {0}; // Initialize all members to zero (good practice)

    // Set the date and time values
    // Note: tm_year is years since 1900, tm_mon is months since January (0-11)
    tm.tm_year = 2024 - 1900; // Example Year: 2024
    tm.tm_mon  = 7 - 1;       // Example Month: July (6 for July)
    tm.tm_mday = 15;          // Example Day: 15
    tm.tm_hour = 10;          // Example Hour: 10
    tm.tm_min  = 30;          // Example Minute: 30
    tm.tm_sec  = 0;           // Example Second: 0
    tm.tm_isdst = -1;         // Let mktime determine Daylight Saving Time

    getLocalTime(&tm);

    // Convert the struct tm to a time_t timestamp
    time_t timestamp = mktime(&tm);

    if (timestamp == (time_t)-1) {
      Serial.printf("Error converting time to timestamp\n");
    } else {
     // Serial.printf("Timestamp (seconds since epoch): %ld\n", (long)timestamp);
    }

    return timestamp;
}
