#include <Arduino.h>
#include <ESP32Lib.h>

#include "Terminal.h"
#include "PS2Keyboard.h"
#include "Serial0.h"
//#include "CodePage437_8x8.h"
//#include "Vt100.h"

extern PS2Keyboard keyboard;
const uint8_t totalSpaceTab = 4;
//VGA Device
uint8_t lixo = 0;
//VGA1BitI vga;
extern VGA3Bit vga;

extern Font myfont;
//Vt100 vt100term;
extern void myprintch(char str);
extern void restartTimer0();
extern bool toggle0;

uint16_t myRED = vga.RGB(0,0,255);
uint16_t myGREEN = vga.RGB(0,255,0);
uint16_t myBLUE = vga.RGB(255,0,0);
uint16_t myWHITE = vga.RGB(255, 255,255);
uint16_t myYELLOW = vga.RGB(255,255,0);
uint16_t myCYAN = vga.RGB(0,255,255);
uint16_t myMAGENTA = vga.RGB(255,0,255);
uint16_t myBLACK = vga.RGB(0,0,0);
uint16_t myCOLORS[7] = {myBLUE, myWHITE, myYELLOW, myCYAN, myMAGENTA, myRED, myGREEN};


//Terminal states
#define  VT100_OFF   0
#define  VT100_ON    1

uint8_t myvt100state = VT100_OFF;
uint8_t cmd_complete = 0;
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
      if( comando[2] == 'J' ){  //Clear screen
        vga.clear();
        cmd_complete = 0;
        myvt100state = VT100_OFF;  
      }
    }else if(comando[1] == 'H'){  //Home possition
        vga.setCursor( 0, 0);
        cmd_complete = 0;
        myvt100state = VT100_OFF;  
    }else if(comando[1] == '0'){  //Home possition
        if(comando[2] == 'm'){
          //vga.setFrontGlobalColor(0,255,0);
          vga.setTextColor(myCOLORS[VERDE]);             
          cmd_complete = 0;
          myvt100state = VT100_OFF;  
        }
    }else if( (comando[1] == '4') && (comando[2] == '1') ){
      vga.setTextColor(myCOLORS[VERMELHO]);             
      //vga.setFrontGlobalColor(0,0,255);
    }else if( (comando[1] == '4') && (comando[2] == '2') ){
      vga.setTextColor(myCOLORS[VERDE]);             
      //vga.setFrontGlobalColor(0,255,0);
    }else if( (comando[1] == '4') && (comando[2] == '3') ){
      vga.setTextColor(myCOLORS[AMARELO]);             
      //vga.setFrontGlobalColor(255,255,0);
    }else if( (comando[1] == '4') && (comando[2] == '4') ){
      vga.setTextColor(myCOLORS[AZUL]);             
      //vga.setFrontGlobalColor(0,0,255);
    }else if( (comando[1] == '4') && (comando[2] == '5') ){
      vga.setTextColor(myCOLORS[CIANO]);             
      //vga.setFrontGlobalColor(0,128,128);
    }else if( (comando[1] == '4') && (comando[2] == '6') ){
      vga.setTextColor(myCOLORS[MAGENTA]);             
     // vga.setFrontGlobalColor(128,128,0);
    }else if( (comando[1] == '4') && (comando[2] == '7') ){
      //vga.setFrontGlobalColor(255,255,255);    
      vga.setTextColor(myCOLORS[BRANCO]);             
    }else
      Serial.print(""); 
      //vga.print("|Nao conheco esse comando...\n");
  }
}

extern volatile bool disable;
void Terminal::print(const char ch)
{
  //vt100term.vt100_state(ch);
  static char lastChar;
  Serial.print(ch);
  //Serial.print("|");
  if ( myvt100state == VT100_ON ){
    //Serial.print(ch);
    comando[cmd_complete] = ch;
    cmd_complete++;
    if ( cmd_complete > 3 && ch == 'm' ){
      cmd_complete = 0;
      myvt100state = VT100_OFF;  
    }
    if ( cmd_complete == 3 ){
      execCmdVt100();
    }
    return;
  }
  if ( ch == 0x1B){
    myvt100state = VT100_ON;
    cmd_complete = 0;
    return;
  }
  if( ch == 0x0A ){

  }else if ( ch == 0x0D ){
    //Todo: Improve the manipulation of last line because 
    //when cursor is on the lastline and the enter are 
    //pressed the cursor goes beyound the lastline.
    /*
      Fiz essa gambiarra pois estou de saco cheio do codigo
      ruim do bitluni. Eita codigo porco.
    */
    if( lastChar != 0x0D){
      vga.print("\n");
    }
    lastChar = 0x0D;

  }else if( ch == 0x09 ){
    for(int i=0; i<totalSpaceTab; i++){
      vga.print(' ');   
    }
  }else if( ch == 0x7F ){
    vga.setCursorBS();
  }else{
    vga.print((const char )ch);
    lastChar = ch;
  }
}

void Terminal::showCursorCursorStatus(){
  if( cursorStatus == true ){
    vga.println("OFF");
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
