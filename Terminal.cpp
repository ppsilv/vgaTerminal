#include <Arduino.h>
#include "Terminal.h"
#include "ESP32Lib.h"

//VGA Device
extern VGA3Bit vga;

extern void myprintch(char str);

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
void Terminal::setCursorPosition(uint8_t row, uint8_t col){
  vga.setCursor(col, row);
}
void Terminal::clearScren(){

}

void Terminal::printStatus(){
  vga.setCursor(10, 10);
}

