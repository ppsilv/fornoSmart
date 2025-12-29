
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

#define TELAINICIAL 1
#define TELAMENU1   2
#define EDICAOTIMER 3
#define EDICAOTEMP  4
#define AQUECENDO   5
#define ASSANDO     6

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
    case TELAINICIAL:
        do_tela_inicial();
        break;
    case TELAMENU1:
        do_tela_menu1();
        break;
    case EDICAOTIMER:
        do_tela_edicao_timer();
        break;
    case EDICAOTEMP:
        do_tela_edicao_temperatura();
        break;
    case AQUECENDO:
        do_tela_aquecendo_resistencias();
        break;
    case ASSANDO:
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
    telaInicial();
    lcd.setCursor(9, 1);
    lcd.print("Press *");
    key = getKey();
    if ( key == KEY_ASTERISCO){
      Serial.println("Digitado asterisco...");
      estado = TELAMENU1;
    }
  }
}
void do_tela_menu1(){
  uint8_t key=0;
  lcd.clear();
  lcd.print("F1=Timer F2=Temp");
  lcd.setCursor(0, 1);
  lcd.print("Esc=Fim  Escolha");
  key = getKey();
  while( key == NO_KEY){
    key = getKey();
    if ( key == KEY_ESC){
      Serial.println("Digitado esc...");
      estado = TELAINICIAL;
    }else if( key == KEY_F1){
      Serial.println("Digitado F1...");
      estado = EDICAOTIMER;
    }else if( key == KEY_F2){
      Serial.println("Digitado F2...");
      estado = EDICAOTEMP;
    }
  }
}
void do_tela_edicao_timer(){
  Serial.println("Edicao timer");  
  estado = TELAINICIAL;

  if( (tempo_para_assar=getNumber("Qual o Tempo")) > 0 ){
    estado = EDICAOTEMP;
  }
}
void do_tela_edicao_temperatura(){
  Serial.println("Edicao temperatura");  
  estado = TELAINICIAL;
  if( (temperatura_para_assar=getNumber("Qual Temperatura")) > 0 ){
    estado = AQUECENDO;
  }
}
void do_tela_aquecendo_resistencias(){
  liga_resistencias();
  uint16_t temp_lida = getCelsius();

  if ( temp_lida >= (temperatura_para_assar - (temperatura_para_assar/10) ) ){
    Serial.println("Temperatura chegou no patamar de cozimento ");
    estado = ASSANDO;
  }
}
void do_tela_assando_comida(){
  CTimer timer_para_assar= CTimer(tempo_para_assar);
  estado = TELAINICIAL;
  bool pisca_info=false;

  Serial.println("Assando a comida...");
  while(timer_para_assar.verifyTimerTimeout() != false){
    telaInicial();
    if( pisca_info ){
      lcd.setCursor(9, 1);
      lcd.print("       ");
    }else{
      lcd.setCursor(9, 1);
      lcd.print(timer_para_assar.getTimeToGo());
    }
    delay(1000);
  }
  tempo_para_assar = 0;
  temperatura_para_assar = 0;
}
