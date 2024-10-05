#include <Arduino.h>
#include "Terminal.h"
#include <ESP32Video.h>
#include "PS2Keyboard.h"
#include "Serial0.h"
//#include "CodePage437_8x8.h"
#include "Vt100.h"

extern PS2Keyboard keyboard;
const uint8_t totalSpaceTab = 4;
//VGA Device
uint8_t lixo=0;
extern VGA1BitI vga;
extern Font myfont;
Vt100 vt100term;
extern void myprintch(char str);
extern void restartTimer0();
extern bool toggle0;

//Terminal states
#define  VT100_OFF   0
#define  VT100_ON    1

uint8_t vt100state = VT100_OFF;
uint8_t cmd_complete= 0;
uint8_t comando[4];

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

void Terminal::execCmdVt100()
{
  if (comando[0] == '['){
    if(comando[1] == '2'){
      if( comando[2] == 'J' ){
        Serial.print("CLEAR SCREEN ");
      }
    }else if(comando[1] == 'H'){
      Serial.print("Algum commando HHHHHHH...");
    }else if( (comando[1] == '4') && (comando[2] == '2') ){
      Serial.print("Algum commando 42m...");  
    }else
          Serial.print("Nao conheco esse comando...\n");
  }
}

void Terminal::print(uint8_t ch)
{
  vt100term.vt100_state(ch);
}
  /*
  Serial.print(ch,16);
  Serial.print("|");
  if ( vt100state == VT100_ON ){
    comando[cmd_complete] = ch;
    cmd_complete++;
    if ( cmd_complete == 3 ){
      cmd_complete = 0;
      vt100state = VT100_OFF;  
      execCmdVt100();
    }
    //return;
  }
  if ( ch == 0x1B){
    vt100state = VT100_ON;
    cmd_complete = 0;
    //return;
  }
  
  if( ch == 0x09 ){
    for(int i=0; i<totalSpaceTab; i++){
      vga.printCursor('C');
      vga.print(' ');  
    }
  }else if( ch == 0x7F ){
      vga.printCursor('C');
      vga.setCursorBS();
  }else if( ch == 0x0A || ch == 0x0D ){
    //Todo: Improve the manipulation of last line because 
    //when cursor is on the lastline and the enter are 
    //pressed the cursor goes beyound the lastline.
    vga.printCursor('C');
    vga.print("\n");
  }else{
    vga.printCursor('C');
    vga.print(ch);
  }
}
*/
void Terminal::scroll(){

}

void Terminal::showCursorCursorStatus(){
  if( cursorStatus == true ){
    vga.println("ON");
  }else{
    vga.println("OFF");
  }
}

extern bool getSemaforo();
void Terminal::showCursor(){
 if ( getSemaforo() ){
      if( toggle0 ){
        vga.printCursor('_');       
      }else{
        vga.printCursor('C');
      }
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
    //if ( ch == 10 || ch == 13){
    //  Serial2.print('\n');
    //}else{
      Serial.print(ch);
      Serial2.print(ch);
    //}
  }  
}
