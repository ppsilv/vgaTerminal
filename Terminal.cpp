#include <Arduino.h>
#include "Terminal.h"
#include "ESP32Lib.h"
#include "PS2Keyboard.h"
#include "Serial0.h"

PS2Keyboard keyboard;

//VGA Device
extern VGA3Bit vga;

extern void myprintch(char str);
extern void restartTimer0();
extern bool toggle0;
extern bool toggled;


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

void Terminal::print(char ch){
  col++;
  if ( col >= 79 ){
    col = 0;
    row++;
  }
  
  if( ch == '\n' ){
    vga.println("");
  }else{
    vga.print(ch);
  }
}

void Terminal::scroll(){

}

void Terminal::showCursor(){
    if((toggle0 == toggled) && cursorStatus ){
      if( toggle0 ){
        vga.printCursor('_');
      }else{
        vga.printCursor('C');
      }
      toggled = !toggle0;
      restartTimer0 ();
      if ( millis() > future ){
        future= millis()+alarme;
      }
    }
}	

void Terminal::setCursorPosition(uint8_t row, uint8_t col){
  vga.setCursor(col, row);
}

void Terminal::clearScren(){

}

//Last line with terminal parameters
void Terminal::printStatus(){
  vga.setCursor(10, 10);
}

void Terminal::run(){
  //Serial0 * serial = Serial0::getInstance();
  char ch = keyboard.GetCharcode();
  if (ch != 0){
    print(ch);
    Serial1.write(ch);
  }  
}
