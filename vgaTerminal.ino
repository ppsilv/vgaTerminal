/*
  Autor: Paulo da Silva
  Data.: 02/10/2024
  Versão: 0.1.00 
          A versão 0.1.00 foi aproveitado os códigos Serial0,PS2Keyboard e Terminal e o
          código vgaTerminal foi totalmente refeito, pois a versão 0.0.04 não estava compilando
          
*/
#include "esp32lib.h"
#include <Ressources/Font6x8.h>
#include <Ressources/CodePage437_8x8.h>
#include "Serial0.h"
#include "PS2Keyboard.h"
#include "Terminal.h"

//pin configuration
const int redPin = 22;
const int greenPin = 19;
const int bluePin = 5;
const int hsyncPin = 23;
const int vsyncPin = 15;

//VGA Device
//VGA1BitI vga;
VGA3Bit vga;
//Serial
TSerial0 *serial0 = TSerial0::getInstance();
//Keyboard
PS2Keyboard keyboard;
//Terminal
Terminal terminal;

extern void setupTimer();

const uint8_t frontColors[] = {0x2,0x0,0x1,0x4,0x1,0x7,0x3,0x3};
const uint8_t backColors[] = {0x0,0x7,0x0,0x6,0x7,0x0,0x4,0x4};

char * versao = "2.0.00";
char * screenMode = "640x400";
int baudrate = serial0->getBaud() ;
char * uartConfig = "8N1";
char * keyboardType = "US-Keyboard";

extern void loopTimer();
extern uint16_t myCOLORS[7];

void header()
{
  vga.setTextColor(myCOLORS[AMARELO]);             
  vga.print("         1          2         3         4         5         6         7         ");
	vga.print("01234567890123456789012345678901234567890123456789012345678901234567890123456789");
  vga.setTextColor(myCOLORS[VERMELHO]);             
	vga.print("               ******* Open  Software - Copyright (C) 2024 *******\n");
  vga.setTextColor(myCOLORS[VERDE]);             
	vga.print("Terminal serial.: Versao ");vga.println((const char *)versao);
  vga.print("Screen mode.....: ");vga.println((const char *)screenMode);
  vga.print("Serial..........: ");vga.print(baudrate); vga.print("-"); vga.println((const char *)uartConfig);//serial0->getBaud();vga.println("-8N1" );
  vga.print("Keyboard........: ");vga.println((const char *)keyboardType);
  vga.print("Cursor..........: ");terminal.showCursorCursorStatus();
	vga.print("Free memory.....: ");vga.println((int)heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
  vga.print("********************************************************************************\n");  
}
extern void setupCursor();
extern void treatCursor();

void setup()
{
  Serial.begin(115200);
	//initializing vga at the specified pins
  if ( memcmp(screenMode,"640x400",7 ) == 0 ){
	  vga.init(vga.MODE640x350, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  }else if ( memcmp(screenMode,"320x200",7 ) == 0 ){
    vga.init(vga.MODE320x200, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  }
  //vga.setResolution(640,  200);
	//selecting the font
  vga.setFont(CodePage437_8x8);
  vga.clear();
  delay(500);
  vga.setCursor(0,0);
  vga.setTextColor(myCOLORS[VERDE]);             
	//vga.setFrontGlobalColor(0,255,0);
	//vga.setBackGlobalColor(0,0,0);  

  //Serial0 
  serial0->begin(); 
  //Keyboard
  keyboard.begin(); 
	if (keyboard.WaitForKeyboard() == false)
	{
		Serial.write("Keyboard not found... \nPress F1 to continue\n \nStoped......");
		while (1);
	}
	keyboard.setNumLock();
  //Initialization Completed
  Serial.printf("Buffer size: [%02d]\n",keyboard.GetBufferSize());
  
  //setupTimer();

  Serial.write("Initialization Completed\n");
  delay(1000);
 
  vga.setCursorText(0,0);
  vga.setTextColor(myCOLORS[VERDE]);             

  //Print terminal header
  header();
  setupCursor();
}

void loop()
{
  terminal.run();  
  keyboard.verifyStatus();
  treatCursor();
}
