/*
 Repeat timer example

 This example shows how to use hardware timer in ESP32. 
 The timer calls onTimer function every second. 
 The timer can be stopped with button attached to PIN 0
 (IO0).

 This example code is in the public domain.
 */
#include "Arduino.h"
#include <ESP32Video.h>

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//volatile uint32_t isrCounter = 0;
//volatile uint32_t lastIsrAt = 0;

volatile bool toggle0;
volatile bool toggled;
volatile bool disable;
//VGA Device
//extern VGA1BitI vga;

void ARDUINO_ISR_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  if (disable){
    return;
  }
  portENTER_CRITICAL_ISR(&timerMux);
  //isrCounter = isrCounter + 1;
  //lastIsrAt = millis();
  if (toggle0 == true) 
    toggle0 = false;
  else  
    toggle0 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
}

void setupTimer() {

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Set timer frequency to 1Mhz
  timer = timerBegin(4000000);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(timer, 500000, true, 0);
  disable = false;
}

bool getSemaforo()
{
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    return true;
  } 

  return false;
}
/*
void loopTimer()
{
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    uint32_t isrCount = 0, isrTime = 0;
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
    // Print it
    //Serial.print("onTimer no. ");
    //Serial.print(isrCount);
    //Serial.print(" at ");
    //Serial.print(isrTime);
    //Serial.println(" ms");

  }

}
*/
