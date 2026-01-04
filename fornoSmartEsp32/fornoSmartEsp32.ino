
#include "fornoSmart.h"

#if CONFIG_FREERTOS_UNICORE
#define TASK_RUNNING_CORE 0
#else
#define TASK_RUNNING_CORE 1
#endif

void TaskLed(void *pvParameters);
void TaskTempControl(void *pvParameters);

//State machine
static uint8_t estado=1;
static uint32_t termino_tempo_para_assar=0;
static uint16_t tempo_para_assar=0;
static uint16_t tempo_para_assar_convertido=0;
static uint16_t temperatura_para_assar=0;
static uint16_t res_aquecida=0;

static bool get_temp_flag = false;

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
#define TELALOG     7
void toneInit(){
  tone(ALTO_FALANTE, 1500);
  delay(250);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 750);
  delay(250);
  noTone(ALTO_FALANTE);
}
void toneInit1(){
  tone(ALTO_FALANTE, 3000);
  delay(250);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1500);
  delay(250);
  noTone(ALTO_FALANTE);
}
void toneEnd(){
  tone(ALTO_FALANTE, 1000);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 250);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1000);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 250);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1000);
  delay(500);
  noTone(ALTO_FALANTE);
}
void toneEnd1(){
  tone(ALTO_FALANTE, 1500);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1000);
  delay(200);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1500);
  delay(100);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1000);
  delay(200);
  noTone(ALTO_FALANTE);

  tone(ALTO_FALANTE, 1500);
  delay(400);
  noTone(ALTO_FALANTE);
}
void tecla(){
  tone(ALTO_FALANTE, 1000);
  delay(150);
  noTone(ALTO_FALANTE);
}

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
  xTaskCreate(
    TaskTempControl, "TempControl"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    NULL  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );
  pinMode(ALTO_FALANTE, OUTPUT); 
  lcd.clear();
  toneInit();
  toneInit1();
}

void loop() {
  ArduinoOTA.handle();
 /*
 while(1){
  uint8_t key = getKey();
  if( key == KEY_1){
    toneInit();
    Serial.println("Tem som no falante..?");
  }
  if( key == KEY_2){
    toneInit1();
    Serial.println("Tem som no falante..?");
  }
  if( key == KEY_3){
    toneEnd();
    Serial.println("Tem som no falante..?");
  }
  delay(100);

 }
*/
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
    case TELALOG:
        do_tela_log();
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

void TaskTempControl(void *pvParameters) {
  float temp_lida = getCelsius();
  Serial.print("TaskTempControl:temperatura_para_assar ");
  Serial.println(temperatura_para_assar);
  bool flag_res_on = false;
  for (;;) {              
    if( get_temp_flag ){
      temp_lida = getCelsius();
      if( (temp_lida > (temperatura_para_assar + 2)) && flag_res_on ){
        desliga_resistencias();
        flag_res_on = false;
      }
      if( ( temp_lida < (temperatura_para_assar -2)) && !flag_res_on ){
        liga_resistencias();
        flag_res_on = true;
      }
    }else{
     // Serial.println("Thread NOT ruuning.");
    }
    delay(500);
  }
}

void do_tela_inicial(){
  uint8_t key=0;
  lcd.clear();
  key = getKey();
  while( key == NO_KEY ){
    telaInicial();
    lcd.setCursor(9, 1);
    lcd.print("Press *");
    key = getKey();
    if ( key == KEY_ASTERISCO){
      Serial.println("Digitado asterisco...");
      estado = TELAMENU1;
    }if ( key == KEY_ARROBA ){
      Serial.println("Digitado asterisco...");
      estado = TELALOG;
    }
    if( key == KEY_1){
      toneEnd();
    }
    if( key == KEY_2){
      toneEnd1();
    }
  }
}
void do_tela_menu1(){
  uint8_t key=0;
  lcd.clear();
  lcd.print("F1=Timer F2=Temp");
  lcd.setCursor(0, 1);
  lcd.print("Ent=Inic.Esc=Fim");
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
    }else if( key == KEY_ENTER){
      if( tempo_para_assar > 0 && temperatura_para_assar > 0 ){
        estado = AQUECENDO;
        Serial.println("Iniciando o aquecimento");
      }else{
        lcd.clear();
        //lcd.print("1234567890123456");
        lcd.print("P/ ASSAR precisa");
        lcd.setCursor(0, 1);
        lcd.print("temper. e timer");
        delay(2000);
      }
    }
  }
}
void do_tela_edicao_timer(){
  Serial.println("Edicao timer");  
  estado = TELAINICIAL;

  if( (tempo_para_assar=getNumber("Quantos minutos?")) > 0 ){
    estado = TELAINICIAL;

    tempo_para_assar_convertido = tempo_para_assar*60;

    termino_tempo_para_assar = tempo_para_assar_convertido + (long)getTimeStamp();
    Serial.printf("Timestamp                            : %ld\n", (long)getTimeStamp());
    Serial.printf("Timestamp            tempo para assar: %010d\n", tempo_para_assar);
    Serial.printf("Timestamp tempo para assar convertido: %010d\n", tempo_para_assar_convertido);
    Serial.printf("Timestamp    termino tempo para assar: %ld\n", (long)termino_tempo_para_assar);
  }
}
void do_tela_edicao_temperatura(){
  Serial.println("Edicao temperatura");  
  estado = TELAINICIAL;
  if( (temperatura_para_assar=getNumber("Qual Temperatura")) > 0 ){
    estado = TELAINICIAL;
  }
}
void do_tela_aquecendo_resistencias(){
  CTimer timer_para_aquecer= CTimer(500);
  uint8_t pisca_info=1;
  float temp_lida = getCelsius();

  lcd.clear();
  liga_resistencias();
  Serial.println("Aquecendo as resistencias");  
  while(1){
    temp_lida = getCelsius();
    if ( temp_lida >= (float)(temperatura_para_assar - (temperatura_para_assar/10) ) ){
      Serial.println("Temperatura chegou no patamar de cozimento ");
      estado = ASSANDO;
      get_temp_flag = true;
      break;
    }
    
    lcd.setCursor(4, 0);
    lcd.print(temp_lida);
    lcd.write(0xDF);
    lcd.write('C');
    if( timer_para_aquecer.verifyTimerTimeout() ){
      timer_para_aquecer.timerRenew(500);
      pisca_info++;
      pisca_info &= 0x1;
    }
    if( pisca_info ){
      lcd.setCursor(4, 1);
      lcd.print("         ");
    }else{
      lcd.setCursor(4, 1);
      lcd.print("AQUECENDO");
    }
  }
}

void do_tela_assando_comida(){
  uint8_t key=0;
  float temp_lida = getCelsius();
 // CTimer timer_para_piscar=CTimer(500);
  time_t tempo_restante = termino_tempo_para_assar - (long)getTimeStamp();;
  
  estado = TELAINICIAL;
  uint8_t pisca_info=1;
  char buffer[6]; 
  struct tm ltm;

  lcd.clear();
  Serial.println("Assando a comida...");
  termino_tempo_para_assar = tempo_para_assar_convertido + (long)getTimeStamp();
  while( termino_tempo_para_assar > (long)getTimeStamp() ){
    temp_lida = getCelsius();
    key = getKey();
    if(key == KEY_ESC){
      break;
    }
    telaInicial();
    lcd.setCursor(9, 0);
    lcd.print(temp_lida);
    lcd.write(0xDF);
    lcd.write('C');

    //if( timer_para_piscar.verifyTimerTimeout() ){
    //  timer_para_piscar.timerRenew(500);
    //  pisca_info++;
    //  pisca_info &= 0x1;
  //
      tempo_restante = termino_tempo_para_assar - (long)getTimeStamp();
    //}    
    //if( pisca_info ){
    //  lcd.setCursor(9, 1);
    //  lcd.print("       ");
    //}else{
      localtime_r(&tempo_restante, &ltm);
      sprintf(buffer,"%d:%02d:%02d\0",ltm.tm_hour-21,ltm.tm_min,ltm.tm_sec);

      lcd.setCursor(9, 1);
      //Serial.println(buffer);
//      lcd.print(tempo_restante);
      lcd.print(buffer);
   // }
  }
  tempo_para_assar = 0;
  temperatura_para_assar = 0;
  get_temp_flag = false;
  desliga_resistencias();
  toneEnd1();
}
void do_tela_log()
{
  uint8_t key=0;
  lcd.clear();
  lcd.print("Temperatura: ");
  lcd.print(temperatura_para_assar);
  lcd.setCursor(0, 1);
  lcd.print("Tempo: ");  
  lcd.print(tempo_para_assar);
  lcd.print("min");
  key = getKey();
  while( key == NO_KEY){
    key = getKey();
    if ( key == KEY_ESC){
      Serial.println("Digitado asterisco...");
      estado = TELAMENU1;
    }
  }  
}
