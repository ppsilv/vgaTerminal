/****************************************************************************************************************************
  TimerInterruptTest.ino
  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.2+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32TimerInterrupt
  Licensed under MIT license

*****************************************************************************************************************************/
/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/
#include "ESP32Lib.h"
//VGA Device
extern VGA3Bit vga;

#if !defined( ESP32 )
	#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32_New_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
#define _TIMERINTERRUPT_LOGLEVEL_     0

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32TimerInterrupt.h"

// Init ESP32 timer 0 and 1
ESP32Timer ITimer0(0);
//ESP32Timer ITimer1(1);
bool toggle0 = false;
bool toggled = true;

// With core v2.0.0+, you can't use Serial.print/println in ISR or crash.
// and you can't use float calculation inside ISR
// Only OK in core v1.0.6-
bool IRAM_ATTR TimerHandler0(void * timerNo)
{
	toggle0 = !toggle0;
	return true;
}

void restartTimer0(){
  ITimer0.restartTimer();
}

//bool IRAM_ATTR TimerHandler1(void * timerNo)
//{
//	static bool toggle1 = false;
//
//	//timer interrupt toggles outputPin
//	//digitalWrite(PIN_D3, toggle1);
//	toggle1 = !toggle1;
//
//	return true;
//}

#define TIMER0_INTERVAL_MS        150
#define TIMER0_DURATION_MS        5000

//#define TIMER1_INTERVAL_MS        3000
//#define TIMER1_DURATION_MS        15000

void setupCursor()
{
	//Serial.print(F("\nStarting TimerInterruptTest on "));
	//Serial.println(ARDUINO_BOARD);
	//Serial.println(ESP32_TIMER_INTERRUPT_VERSION);

	// Interval in microsecs
	if (!ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0)){
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));
  }
  //else{  
	//	Serial.print(F("Starting  ITimer0 OK, millis() = "));
	//	Serial.println(millis());
	//}
		
}
