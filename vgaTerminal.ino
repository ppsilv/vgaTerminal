#include "Arduino.h"
#include "ESP32Lib.h"

//#include <Ressources/Font8x8.h>
#include "Ressources/CodePage437_8x8.h"
#include "Ressources/Font8x8.h"
#include <CircularBuffer.hpp>
#include "PS2Keyboard.h"
#include "Terminal.h"

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

Font myfont = CodePage437_8x8;
extern void loopUart();
extern CircularBuffer<char, 255> buffer;
extern void setupUart();
extern void setupPs2Keyboard();
extern void loopPs2Keyboard();
extern int myprintf( char* str, ...);

extern PS2Keyboard keyboard;

extern void setupCursor();
extern bool toggle0;
extern bool toggled;
bool cursorStatus = true;

const unsigned int alarme=5250;
unsigned long future= millis()+alarme;

void setup() {
  Serial.begin(115200);
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
	vga.println("***** Open Software - Copyright (C) 2024 *****\n");
	vga.println("      Terminal serial\n");
  vga.println("      Screen mode: 640x350");
  vga.println("      Baudrate...: 9600");
  vga.println("      Keyboard...: US-Keyboard");
  vga.println("      Cursor.....: ON");
	vga.print  ("      Free memory: ");	vga.println((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  vga.println("**********************************************\n");  

  setupPs2Keyboard();
  setupCursor();

  delay(1000);

  //vga.clear();

}
extern void restartTimer0();

void loop(){
  Terminal terminal;
  while(1){
    //loopUart();
    char ch = keyboard.GetCharcode();
    if((toggle0 == toggled) && cursorStatus ){
      if( toggle0 ){
        vga.printCursor('I');
      }else{
        vga.printCursor('C');
      }
      toggled = !toggle0;
      restartTimer0 ();
      if ( millis() > future ){
        future= millis()+alarme;
      }
    }
    if (ch != 0){
      terminal.print(ch);
    }
    keyboard.handleLeds();
  }
}

