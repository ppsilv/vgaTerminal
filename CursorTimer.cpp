/*
 Repeat timer example

 This example shows how to use hardware timer in ESP32. 
 The timer calls onTimer function every second. 
 The timer can be stopped with button attached to PIN 0
 (IO0).

 This example code is in the public domain.
 */
#include "Arduino.h"
#include "esp32lib.h"
#include "Terminal.h"

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//volatile uint32_t isrCounter = 0;
//volatile uint32_t lastIsrAt = 0;

volatile bool toggle0;
volatile bool toggled;

extern VGA3Bit vga;
extern Terminal terminal;

//void ARDUINO_ISR_ATTR onTimer(){
//  // Increment the counter and set the time of ISR
//  if (vga.getOnScroll()){
//    return;
//  }
//  portENTER_CRITICAL_ISR(&timerMux);
//  //isrCounter = isrCounter + 1;
//  //lastIsrAt = millis();
//  if (toggle0 == true) {
//    toggle0 = false;
//    vga.clearCursor();
//  }
//  else{
//    toggle0 = true;
//    vga.printCursor();
//  }
//  portEXIT_CRITICAL_ISR(&timerMux);
//  // Give a semaphore that we can check in the loop
//  xSemaphoreGiveFromISR(timerSemaphore, NULL);
//  // It is safe to use digitalRead/Write here if you want to toggle an output
//}

//void setupTimer() {
//
//  // Create semaphore to inform us when the timer has fired
//  timerSemaphore = xSemaphoreCreateBinary();
//
////  // Set timer frequency to 1Mhz
////  timer = timerBegin(4000000);
////
////  // Attach onTimer function to our timer.
////  timerAttachInterrupt(timer, &onTimer);
////
////  // Set alarm to call onTimer function every second (value in microseconds).
////  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
////  timerAlarm(timer, 500000, true, 0);
////
////}
//
//bool getSemaforo()
//{
//  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
//    return true;
//  } 
//
//  return false;
//}

volatile int timeCursor=0;
const int timeoutCursor=250;
extern 		bool cursorStatus;

void setupCursor(){
  timeCursor = millis();
  
}

void treatCursor(){
    if( terminal.getCursorStatus() == false ){
      timeCursor = millis();
      return;
    }
    if( millis() > (timeCursor + timeoutCursor)){
      timeCursor = millis();
      if (toggle0 == true) {
        toggle0 = false;
        vga.clearCursor();
      }
      else{
        toggle0 = true;
        vga.printCursor();
      }
    }
}
