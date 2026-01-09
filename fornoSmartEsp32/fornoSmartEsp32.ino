/*
Alteração feita no arquivo LiquidCrystal_I2C.cpp
            void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row){
linha 144     int row_offsets[] = { 0x00, 0x40, 0x10, 0x50 };
              if ( row > _numlines ) {
                row = _numlines-1;    // we count rows starting w/0
              }
              command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
            }

*/
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
    //digitalWrite(LED, HIGH); 
    analogWrite( LED, 25);
    delay(500);
    analogWrite( LED, 0);
    delay(500);
  }
}

void TaskTempControl(void *pvParameters) {
  Serial.print("TaskTempControl:temperatura_para_assar ");
  Serial.println(temperatura_para_assar);

  //PID Section
  //I/O
  int PWM_pin = RESISTENCIAS;  //Pin for PWM signal to the MOSFET driver (the BJT npn with pullup)

  //Variables
  float set_temperature = temperatura_para_assar;            //Default temperature setpoint. Leave it 0 and control it with rotary encoder
  float temperature_read = 0.0;
  float PID_error = 1;
  float previous_error = 0;
  float elapsedTime, Time, timePrev;
  float PID_value = 0;
  float last_set_temperature = 0;

  //PID constants
  //////////////////////////////////////////////////////////
  int kp = 90;   int ki = 30;   int kd = 80;
  //////////////////////////////////////////////////////////
  int PID_p = -1;    int PID_i = -30;    int PID_d = 0;
 // float last_kp = 0;
 // float last_ki = 0;
 // float last_kd = 0;
  get_temp_flag=false;  

  Time = millis();
  for (;;) {              
    if( get_temp_flag ){
      set_temperature = temperatura_para_assar;   
      // First we read the real value of temperature
      temperature_read = getCelsius();
      //Next we calculate the error between the setpoint and the real value
      PID_error = set_temperature - temperature_read + 2;
      //Calculate the P value
      PID_p = 0.01*kp * PID_error;
      //Calculate the I value in a range on +-2
      PID_i = 0.01*PID_i + (ki * PID_error);
      
      //For derivative we need real time to calculate speed change rate
      timePrev = Time;                            // the previous time is stored before the actual time read
      Time = millis();                            // actual time read
      elapsedTime = (Time - timePrev) / 1000; 
      //Now we can calculate the D calue
      PID_d = 0.01*kd*((PID_error - previous_error)/elapsedTime);
      //Final total PID value is the sum of P + I + D
      PID_value = PID_p + PID_i + PID_d;

      //We define PWM range between 0 and 255
      if(PID_value < 0){    PID_value = 255;    }
      if(PID_value > 255){    PID_value = 0;  }
      //Now we can write the PWM signal to the mosfet on digital pin D3
      //Since we activate the MOSFET with a 0 to the base of the BJT, we write 255-PID value (inverted)
      analogWrite( PWM_pin, 255-PID_value );
      previous_error = PID_error;     //Remember to store the previous error for next loop.

      delay(125);
/*
      lcd.setCursor(0,0);
      lcd.print(PID_p);
      lcd.print("|");
      lcd.print(PID_i);
      lcd.print("|");
      lcd.print(PID_d);

      lcd.setCursor(0,1);
      lcd.print("S:");
      lcd.setCursor(2,1);
      //lcd.print(set_temperature,1);
      lcd.print(PID_value,1);
      lcd.setCursor(9,1);
      lcd.print("R:");
      lcd.setCursor(11,1);
      lcd.print(temperature_read,1);    
*/
    }else{
      analogWrite( PWM_pin, 0 );
    }
    delay(125);
  }
}

void TaskTempControNoPid(void *pvParameters) {
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
  float temp_lida = getCelsius();

  key = getKey();
  while( key == NO_KEY ){
    ArduinoOTA.handle();
    telaInicial();
    lcd.setCursor(0, 3);
    lcd.print("Pressione *");
    key = getKey();
    if ( key == NO_KEY ){
      turn_off_back_light();
    }else{
      turn_on_back_light();
    }    
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
    if( key == KEY_3){
      temp_lida = getCelsius();
      lcd.setCursor(10, 0);
      lcd.print(temp_lida);
      delay(3000);
      lcd.setCursor(10, 0);
      lcd.print("     ");
    }
    if( key == KEY_4){
      turn_back_light();
    }
    if( key == KEY_9){
      lcd.clear();
      lcd.print("TE AMO LAISA");
      delay(5000);
    }
  }
}
void do_tela_menu1(){
  uint8_t key=0;
  lcd.clear();
  lcd.print("F1=Timer F2=Temp");
  lcd.setCursor(0, 3);
  lcd.print("Ent=Inic.Esc=Fim");
  key = getKey();
  while( key == NO_KEY){
    ArduinoOTA.handle();
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
static uint16_t tmp_tempo_para_assar=0;
static uint16_t tmp_temperatura_para_assar=0;
void do_tela_edicao_timer(){
  Serial.println("Edicao timer");  
  estado = TELAINICIAL;
  lcd.clear();
  if( (tmp_tempo_para_assar=getNumber("Quantos minutos?",0)) > 0 ){
    if(tmp_tempo_para_assar > 0){
      tempo_para_assar=tmp_tempo_para_assar;
      tempo_para_assar_convertido = tempo_para_assar*60;
      termino_tempo_para_assar = tempo_para_assar_convertido + (long)getTimeStamp();
    }
  }
  Serial.println("Edicao temperatura");  
  if( (tmp_temperatura_para_assar=getNumber("Qual Temperatura",2)) > 0 ){
    if( tmp_temperatura_para_assar > 0){
      temperatura_para_assar = tmp_temperatura_para_assar - 2;
    }
  }

}
void do_tela_edicao_temperatura(){
  Serial.println("Edicao temperatura");  
  estado = TELAINICIAL;
  lcd.clear();
  if( (tmp_temperatura_para_assar=getNumber("Qual Temperatura",0)) > 0 ){
    if( tmp_temperatura_para_assar > 0){
      temperatura_para_assar = tmp_temperatura_para_assar - 2;
    }
  }
  Serial.println("Edicao timer");  
  if( (tmp_tempo_para_assar=getNumber("Quantos minutos?",2)) > 0 ){
    if(tmp_tempo_para_assar > 0){
      tempo_para_assar=tmp_tempo_para_assar;
      tempo_para_assar_convertido = tempo_para_assar*60;
      termino_tempo_para_assar = tempo_para_assar_convertido + (long)getTimeStamp();
    }
  }

}
void do_tela_aquecendo_resistencias(){
  CTimer timer_para_aquecer= CTimer(500);
  uint8_t pisca_info=1;
  float temp_lida = getCelsius();
  uint8_t key=0;

  estado = ASSANDO;
  get_temp_flag = true;
  return;

  lcd.clear();
  get_temp_flag = true;
  Serial.println("Aquecendo as resistencias");  
  while(1){
    temp_lida = getCelsius();
    if ( temp_lida >= (float)(temperatura_para_assar - (temperatura_para_assar/10) ) ){
      Serial.println("Temperatura chegou no patamar de cozimento ");
      estado = ASSANDO;
      get_temp_flag = true;
      desliga_resistencias();
      break;
    }
    key = getKey();
    if(key == KEY_ESC){
      estado = TELAINICIAL;
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
void do_tela_aquecendo_resistenciasNoPid(){
  CTimer timer_para_aquecer= CTimer(500);
  uint8_t pisca_info=1;
  float temp_lida = getCelsius();
  uint8_t key=0;

  lcd.clear();
  liga_resistencias();
  Serial.println("Aquecendo as resistencias");  
  while(1){
    temp_lida = getCelsius();
    if ( temp_lida >= (float)(temperatura_para_assar - (temperatura_para_assar/10) ) ){
      Serial.println("Temperatura chegou no patamar de cozimento ");
      estado = ASSANDO;
      get_temp_flag = true;
      desliga_resistencias();
      break;
    }
    key = getKey();
    if(key == KEY_ESC){
      estado = TELAINICIAL;
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
    delay(500);
    key = getKey();
    if(key == KEY_ESC){
      break;
    }
    telaInicial();
    lcd.setCursor(0,0);
    lcd.print("ASSANDO  ");
    lcd.setCursor(9, 0);
    lcd.print(temp_lida);
    lcd.write(0xDF);
    lcd.write('C');
    tempo_restante = termino_tempo_para_assar - (long)getTimeStamp();
    localtime_r(&tempo_restante, &ltm);
    sprintf(buffer,"%d:%02d:%02d\0",ltm.tm_hour-21,ltm.tm_min,ltm.tm_sec);
    lcd.setCursor(9, 1);
    lcd.print(buffer);
  }
  tempo_para_assar = 0;
  temperatura_para_assar = 0;
  get_temp_flag = false;
  desliga_resistencias();
  toneEnd1();
  turn_off_back_light();
}
void do_tela_log()
{
  uint8_t key=0;
  lcd.clear();
  lcd.print("Temper.: ");
  lcd.print(temperatura_para_assar);
  lcd.write(0xDF);
  lcd.write('C');
  lcd.setCursor(0, 1);
  lcd.print("Tempo: ");  
  lcd.print(tempo_para_assar);
  lcd.print(" min");
  key = getKey();
  while( key == NO_KEY){
    key = getKey();
    if ( key == KEY_ESC){
      Serial.println("Digitado asterisco...");
      estado = TELAMENU1;
    }
  }  
}
