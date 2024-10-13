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

//uint16_t myRED = vga.RGB(0,0,255);
//uint16_t myGREEN = vga.RGB(0,255,0);
//uint16_t myBLUE = vga.RGB(255,0,0);
//uint16_t myWHITE = vga.RGB(255, 255,255);
//uint16_t myYELLOW = vga.RGB(0,255,255);
//uint16_t myCYAN = vga.RGB(255,255,0);
//uint16_t myMAGENTA = vga.RGB(255,0,255);
//uint16_t myBLACK = vga.RGB(0,0,0);
//uint16_t myCOLORS[7] = {myBLUE, myWHITE, myCYAN, myYELLOW, myMAGENTA, myRED, myGREEN};



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
/*
void Terminal::execCmdVt100()
{
  if (comando[0] == '['){
    if(comando[1] == '2'){
      if( comando[2] == 'J' ){  //Clear screen
        Serial.print("Limpando a tela...\n");
        vga.clear();
        cmd_complete = 0;
        myvt100state = VT100_OFF;  
      }
    }else if(comando[1] == 'H'){  //Home possition
        Serial.print("Posicionando o cursor a tela...\n");
        vga.setCursorText( 0, 0);
        cmd_complete = 0;
        myvt100state = VT100_OFF;  
    }else if(comando[1] == '0'){  //Home possition
        if(comando[2] == 'm'){
          Serial.print("Limpando configuracao...\n");
          vga.setTextColor(myCOLORS[VERDE]);             
          cmd_complete = 0;
          myvt100state = VT100_OFF;  
        }      
    }else if( (comando[1] == '4') && (comando[2] == '1') ){
      vga.setTextColor(myCOLORS[VERMELHO]);             
    }else if( (comando[1] == '4') && (comando[2] == '2') ){
      vga.setTextColor(myCOLORS[VERDE]);             
    }else if( (comando[1] == '4') && (comando[2] == '3') ){
      vga.setTextColor(myCOLORS[AMARELO]);             
    }else if( (comando[1] == '4') && (comando[2] == '4') ){
      vga.setTextColor(myCOLORS[AZUL]);             
    }else if( (comando[1] == '4') && (comando[2] == '5') ){
      vga.setTextColor(myCOLORS[MAGENTA]);             
    }else if( (comando[1] == '4') && (comando[2] == '6') ){
      vga.setTextColor(myCOLORS[CIANO]);             
    }else if( (comando[1] == '4') && (comando[2] == '7') ){
      vga.setTextColor(myCOLORS[BRANCO]);             
    }else
      Serial.print(""); 
      //vga.print("|Nao conheco esse comando...\n");
  }
}
*/
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
    //Todo: Improve the manipulation of last line because 
    //when cursor is on the lastline and the enter are 
    //pressed the cursor goes beyound the lastline.
    vga.print("\n");
  }else if( ch == 0x09 ){
    for(int i=0; i<totalSpaceTab; i++){
      vga.print(' ');   
    }
  }else if( ch == 0x7F ){
    vga.setCursorBS();
  }else{
    vga.clearCursor();
    vga.print((const char )ch);
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
