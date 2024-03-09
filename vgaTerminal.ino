/*
  Libs used:
            VGA.............: https://github.com/ppsilv/ESP32Lib (Forked from THE GREAT bitlunes)
            CircularBuffer..: https://github.com/ppsilv/CircularBuffer 

*/
#include <ESP32Lib.h>
//#include <Ressources/Font8x8.h>
#include <Ressources/CodePage437_8x8.h>
#include <Ressources/Font8x8.h>
#include <CircularBuffer.hpp>
#include"PS2Keyboard.h"

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
extern PS2Keyboard keyboard;
void bitluni(int x, int y, int s)
{
	vga.fillCircle(x + 2 * s, y + 2 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillCircle(x + 22 * s, y + 2 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillCircle(x + 2 * s, y + 22 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillCircle(x + 22 * s, y + 22 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillRect(x, y + 2 * s, 24 * s, 20 * s, vga.RGB(128, 0, 0));
	vga.fillRect(x + 2 * s, y, 20 * s, 24 * s, vga.RGB(128, 0, 0));
	vga.fillCircle(x + 7 * s, y + 4 * s, 2 * s, vga.RGB(255, 255, 255));
	vga.fillCircle(x + 15 * s, y + 6 * s, 2 * s, vga.RGB(255, 255, 255));
	vga.fillCircle(x + 11 * s, y + 16 * s, 6 * s, vga.RGB(255, 255, 255));
	vga.fillCircle(x + 13 * s, y + 16 * s, 6 * s, vga.RGB(255, 255, 255));
	vga.fillCircle(x + 11 * s, y + 16 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillCircle(x + 13 * s, y + 16 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillRect(x + 11 * s, y + 14 * s, 2 * s, 4 * s, vga.RGB(128, 0, 0));
	vga.fillRect(x + 9 * s, y + 14 * s, 2 * s, 2 * s, vga.RGB(128, 0, 0));
	vga.fillRect(x + 5 * s, y + 4 * s, 4 * s, 12 * s, vga.RGB(255, 255, 255));
	vga.fillRect(x + 9 * s, y + 10 * s, 4 * s, s, vga.RGB(255, 255, 255));
}
/******************************************/
// Configurações
bool  cursorMode  = true;     //true curson on false cursor off
byte  screenMode  = 5;        //5 = 640x350

void printVga(char i){
  vga.print(i);
}

void setup2()
{
	//initializing the graphics mode
//	vga.init(vga.MODE800x600, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	vga.init(vga.MODE640x350, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//setting the font
	//vga.setFont(Font6x8);
  vga.setFont(CodePage437_8x8);

	//clearing with white background
	vga.clear(vga.RGB(0xffffff));
	//text position
	vga.setCursor(10, 10);
	//black text color no background color
	vga.setTextColor(vga.RGB(0, 255, 0));
	//show the remaining memory
	vga.print("free memory: ");
	vga.print((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
	//draw the logo
	bitluni(150, 60, 20);
  while(1) delay(10000);
}
uint8_t linha;
uint8_t coluna;

void setup()
{
  setupUart();
  pinMode(Red_0, INPUT);
  pinMode(Green_0, INPUT);
  pinMode(Blue_0, INPUT);

	//initializing vga at the specified pins
	//vga.init(vga.MODE320x240, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  //vga.init(vga.MODE500x480, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	vga.init(vga.MODE640x350, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//selecting the font
	//vga.setFont(CodePage437_8x8);
  vga.setFont(myfont);
	//black text color no background color
	vga.setTextColor(vga.RGB(0, 255, 0));
	//displaying the text
	vga.println("***** Open Software - Copyright (C) 2024 *****\n");
	vga.println("      Terminal serial\n");
  vga.println("      Screen mode: 640x350");
  vga.println("      Baudrate...: 9600");
  vga.println("      Keyboard...: US-Keyboard");
	vga.print  ("      Free memory: ");	vga.println((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  vga.println("");

	bitluni(540, 1, 4);

  vga.println("Initializing keyboard...");
  vga.println("");
  setupPs2Keyboard();
  vga.println(myfont.getFontWidth());
  
}

void loop()
{
  for(int j=0; j<8; j++){
    for(int i=0; i <9;i++){
      vga.print(' ');
    }
    vga.print(j);
  }
  vga.print("\n");
  for(int j=0; j<8; j++){
    for(int i=0; i <10;i++){
      vga.print(i);
    }
  }

  while(1){
    //loopUart();
    char ch = keyboard.GetCharcode();

    if (ch != 0){
      //Serial.printf("[%02X]\n",ch);
      //Serial.printf("*%c*",ch);
      if( ch == '\n' ){
        vga.print("\n");
      }else{
        if(ch == 'A'){
          vga.setCursor(10, 10);
        }
        vga.print(ch);
      }
      vga.scroll(1,5);
    }
    keyboard.handleLeds();
  }
}