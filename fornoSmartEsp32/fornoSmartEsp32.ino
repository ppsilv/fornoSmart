/* Basic Multi Threading Arduino Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
// Please read file README.md in the folder containing this example.
#include "fornoSmart.h"

CAlarme * alr = CAlarme::getInstancia();

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

#if CONFIG_FREERTOS_UNICORE
#define TASK_RUNNING_CORE 0
#else
#define TASK_RUNNING_CORE 1
#endif

#define ANALOG_INPUT_PIN A0

// Define two tasks for Blink & AnalogRead.
void Clock(void *pvParameters);
void TaskLed(void *pvParameters);

void setupClock();
void loopClock();

TaskHandle_t analog_read_task_handle;  // You can (don't have to) use this to be able to manipulate a task from somewhere else.

//Flags
uint8_t clock_on = 1;

// The setup function runs once when you press reset or power on the board.
void setup() {
  clock_on = 1;
  alr->setAlarm(false);
  calarmeon = 0;
  ota_setup();
  
  Wire.begin();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  lcd.setCursor(0, 0);
  // print message
  lcd.print(MSG_001);

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  setupClock();

  // Set up two tasks to run independently.
  uint32_t blink_delay = 500;  // Delay between changing state on LED pin

  xTaskCreate(
    Clock, "Clock"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&blink_delay  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );

  xTaskCreate(
    TaskLed, "Led Blink"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&blink_delay  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    2  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );
  // Essa forma de criar a task posiciona no TASK_RUNNING_CORE,core especificado
  /*
  xTaskCreatePinnedToCore(
    TaskAnalogRead, "Analog Read", 2048  // Stack size
    ,
    NULL  // When no parameter is used, simply pass NULL
    ,
    1  // Priority
    ,
    &analog_read_task_handle  // With task handle we will be able to manipulate with this task.
    ,
    TASK_RUNNING_CORE  // Core on which the task will run
  );
*/
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
    ArduinoOTA.handle();
    trataKeypad();
    if( calarmeon ){
      alr->criaAlarme();
      calarmeon = 0;
    }
    alr->verificaAlarme();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Clock(void *pvParameters) {  // This is a task.
  for (;;) {   
    if( clock_on){                     
      loopClock(); 
    }
    delay(1000);
  }
}
void TaskLed(void *pvParameters) {  // This is a task.
  uint32_t blink_delay1 = *((uint32_t *)pvParameters);

  pinMode(LED, OUTPUT);

  for (;;) {                        
    digitalWrite(LED, HIGH); 
    delay(blink_delay1);
    alr->piscaIcone(1);
    digitalWrite(LED, LOW);  
    delay(blink_delay1);
    alr->piscaIcone(0);
  }
}

