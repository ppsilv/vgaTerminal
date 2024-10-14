#include <Arduino.h>
#include <ESP32Lib.h>

#include "Terminal.h"
#include "PS2Keyboard.h"
#include "Serial0.h"
#include "vt100.h"
//#include "CodePage437_8x8.h"
//#include "Vt100.h"

extern PS2Keyboard keyboard;
const uint8_t totalSpaceTab = 4;
//VGA Device
uint8_t lixo = 0;
//VGA1BitI vga;
extern VGA3Bit vga;
//VT100
vt100 vt100term;

extern Font myfont;

extern void myprintch(char str);
extern void restartTimer0();
extern bool toggle0;

Terminal::Terminal(){
		screenTotalChar = 0; 	// contador de caracter na tela
		totalRow = 0;		// total de linhas da tela
		totalCol = 0;		// total de colunas da tela
		
		//variaveis do estado da tela	
		row = 0;		//linha atual
		col = 0;		//coluna atual
		totalChar = 0;	//total de caractere atualmente na tela
		bgcolor = 0;	//cor de fundo da tela
		fgcolor = 0;	//cod do texto

    cursorStatus = true;
    future= millis()+alarme;
}

Terminal::~Terminal(){

}

void Terminal::println(){

}
extern bool getSemaforo();
extern portMUX_TYPE timerMux;

void Terminal::print(const char ch)
{
/******************************************************/    
//      DEBUG DEBUG DEBUG DEBUG
  if ((ch == 0xd ) || (ch == 0xA)  ){
    Serial.print(ch);
  }else if ( ch > 31 && ch < 128 ){
    Serial.print(ch);
  }else
    Serial.print('.');
/******************************************************/    
  if( vt100term.execCmdVt100(ch) ){
    return;
  }

  if( ch == 0x0A ){
    
  }else if ( ch == 0x0D ){
    vga.clearCursor();
    vga.print('\n');
  }else if( ch == 0x09 ){
    for(int i=0; i<totalSpaceTab; i++){
      vga.clearCursor();
      vga.print(' ');   
    }
  }else if( ch == 0x7F ){
    vga.setCursorBS();
  }else{
      setCursorStatus(false);
      vga.clearCursor();
      vga.print((const char )ch);
      setCursorStatus(true);
  }
}

void Terminal::showCursorCursorStatus(){
  if( cursorStatus == true ){
    vga.println("ON");
  }else{
    vga.println("OFF");
  }
}

void Terminal::setCursorPosition(uint8_t row, uint8_t col){
  vga.setCursor(col, row);
}

//Last line with terminal parameters
void Terminal::printStatus(){
  vga.setCursor(10, 10);
}

void Terminal::run(){
  char ch = keyboard.GetIso8859Code();
  //char ch = keyboard.GetCharcode();
  
  if (ch != 0){
      Serial2.print(ch);
  }  
}
