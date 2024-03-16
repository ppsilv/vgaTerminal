#include <Arduino.h>
#include <CircularBuffer.hpp>

#include "ESP32Lib.h"
#include "CodePage437_8x8.h"
#include "Ressources/Font8x8.h"
#include "PS2Keyboard.h"
#include "Terminal.h"
#include "Serial0.h"
#include "soc/rtc_wdt.h"

//pin configuration
const int redPin = 22;
const int greenPin = 19;
const int bluePin = 5;
const int hsyncPin = 23;
const int vsyncPin = 15;

//Pin as input for while TODO: Better this situation
const int Red_0   = 21;
const int Green_0 = 18;
const int Blue_0  = 4;

//VGA Device
VGA3Bit vga;
extern PS2Keyboard keyboard;
Terminal terminal;
Serial0 *serial0 = Serial0::getInstance();

Font myfont = CodePage437_8x8;
extern void setupCursor();

void setup() {
  Serial.begin(115200);

  disableCore0WDT();
  delay(500); // experienced crashes without this delay!
  disableCore1WDT(); 
	delay(500);

  rtc_wdt_protect_off();
  rtc_wdt_disable();

  vga.init(vga.MODE640x350, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//setting the font
	//vga.setFont(Font6x8);
  vga.setFont(CodePage437_8x8);
	//clearing with white background
	vga.clear(vga.RGB(0, 0, 0));
	//text position
	vga.setCursor(0, 0);
	//black text color no background color
	vga.setTextColor(vga.RGB(0, 255, 0));
	//show the the parameters
  vga.println("         1          2         3         4         5         6         7         ");
	vga.println("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
	vga.println("               ******* Open  Software - Copyright (C) 2024 *******\n");
	vga.println("Terminal serial.: V 0.0.03\n");
  vga.println("Screen mode.....: 640x350");
  vga.println("Serial..........: 9600 8N1");
  vga.println("Keyboard........: US-Keyboard");
  vga.println("Cursor..........: ON");
	vga.print  ("Free memory.....: ");	vga.println((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  vga.println("******************************************************************************\n"); 
  //Inicialization
  //********************* Serial
  serial0->begin(); 
  //********************* Keyboard
  keyboard.begin(); 
	if (keyboard.WaitForKeyboard() == false)
	{
		Serial.write("Keyboard not found... \nPress F1 to continue\n \nStoped......");
		while (1);
	}
	keyboard.setNumLock();
  Serial.printf("Buffer size: [%02d]\n",keyboard.GetBufferSize());
  setupCursor();
  Serial.write("Initialization Completed\n");
  delay(1000);
  //vga.clear();
}


void loop(){
  terminal.run();
  terminal.showCursor();
  keyboard.verifyStatus();
}

