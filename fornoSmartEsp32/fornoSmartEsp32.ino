
#include "fornoSmart.h"

#if CONFIG_FREERTOS_UNICORE
#define TASK_RUNNING_CORE 0
#else
#define TASK_RUNNING_CORE 1
#endif

void TaskLed(void *pvParameters);

//State machine
static uint8_t estado=1;
static uint16_t tempo_para_assar=0;
static uint16_t temperatura_para_assar=0;

void do_tela_inicial();
void do_tela_menu1();
void do_tela_edicao_timer();
void do_tela_edicao_temperatura();
void do_tela_aquecendo_resistencias();
void do_tela_assando_comida();


void setup() {
  setupLcd();

  ota_setup();
  Wire.begin();
  Serial.begin(115200);

  setupSensor();
  setupClock();
  setupResistencias();

  xTaskCreate(
    TaskLed, "Led Blink"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    NULL  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );
}

void loop() {
  ArduinoOTA.handle();

  switch( estado ){
    case 1:
        do_tela_inicial();
        break;
    case 2:
        do_tela_menu1();
        break;
    case 3:
        do_tela_edicao_timer();
        break;
    case 4:
        do_tela_edicao_temperatura();
        break;
    case 5:
        do_tela_aquecendo_resistencias();
        break;
    case 6:
        do_tela_assando_comida();
        break;        
  }

}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskLed(void *pvParameters) {

  pinMode(LED, OUTPUT);
  for (;;) {                        
    digitalWrite(LED, HIGH); 
    delay(500);
    digitalWrite(LED, LOW);  
    delay(500);
  }
}


void do_tela_inicial(){
  uint8_t key=0;
  lcd.clear();
  key = getKey();
  while( key == NO_KEY){
    loopClock();
    lcd.setCursor(9, 1);
    lcd.print("Press *");
    key = getKey();
    if ( key == KEY_ASTERISCO){
      Serial.println("Digitado asterisco...");
      estado = 2;
    }
  }
}
void do_tela_menu1(){
  uint8_t key=0;
  lcd.clear();
  lcd.print("F1=Timer F2=Temp");
  lcd.setCursor(0, 1);
  lcd.print("Esc = Fim Digite");
  key = getKey();
  while( key == NO_KEY){
    key = getKey();
    if ( key == KEY_ESC){
      Serial.println("Digitado esc...");
      estado = 1;
    }else if( key == KEY_F1){
      Serial.println("Digitado F1...");
      estado = 3;
    }else if( key == KEY_F2){
      Serial.println("Digitado F2...");
      estado = 4;
    }
  }
}
void do_tela_edicao_timer(){
  Serial.println("Edicao timer");  
  estado = 1;

  if( (tempo_para_assar=getNumber("Qual o Tempo")) > 0 ){
    estado = 4;
  }
}
void do_tela_edicao_temperatura(){
  Serial.println("Edicao temperatura");  
  estado = 1;
  if( (temperatura_para_assar=getNumber("Qual Temperatura")) > 0 ){
    estado = 5;
  }
}
void do_tela_aquecendo_resistencias(){
  liga_resistencias();
  uint16_t temp_lida = getCelsius();

  if ( temp_lida >= (temperatura_para_assar - (temperatura_para_assar/10) ) ){
    Serial.println("Temperatura chegou no patamar de cosimento ");
    estado = 6;
  }
}
void do_tela_assando_comida(){
  CTimer timer_para_assar= CTimer(tempo_para_assar);
  estado = 1;
  bool pisca_info=false;

  Serial.println("Assando a comida...");
  while(timer_para_assar.verifyTimerTimeout() != false){
    loopClock();
    if( pisca_info ){
      lcd.setCursor(9, 1);
      lcd.print("       ");
    }else{
      lcd.setCursor(9, 1);
      lcd.print(tempo_para_assar);
    }
    delay(1000);
  }
}
