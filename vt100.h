#pragma once
#include <Arduino.h>
#include <ESP32Lib.h>
//VGA1BitI vga;
extern VGA3Bit vga;

#define VT100_INIT      0  
#define VT100_ON        1
#define VT100_PAR       2  
#define VT100_CMD_0     3
#define VT100_CMD_1     4
#define VT100_CMD_2     5
#define VT100_CMD_3     6
#define VT100_CMD_4     7
#define VT100_CMD_4EXEC 8

uint16_t myRED = vga.RGB(0,0,255);
uint16_t myGREEN = vga.RGB(0,255,0);
uint16_t myBLUE = vga.RGB(255,0,0);
uint16_t myWHITE = vga.RGB(255, 255,255);
uint16_t myYELLOW = vga.RGB(0,255,255);
uint16_t myCYAN = vga.RGB(255,255,0);
uint16_t myMAGENTA = vga.RGB(255,0,255);
uint16_t myBLACK = vga.RGB(0,0,0);
extern uint16_t myCOLORS[7] = {myBLUE, myWHITE, myCYAN, myYELLOW, myMAGENTA, myRED, myGREEN};
/*
Site com definição dos comandos VT100
http://www.braun-home.net/michael/info/misc/VT100_commands.htm
*/
class vt100
{
  public:
    vt100(){
      vt100_state = VT100_INIT;
      cor = 6;
    }
    ~vt100(){}
    bool execCmdVt100(const char ch){
      bool ret = false;
      switch  ( vt100_state ){
        case VT100_INIT:
          if ( ch == 0x1B){
            vt100_state = VT100_ON;
            ret = true;
          }
          break;
        case VT100_ON:
          if ( ch == '['){
            vt100_state = VT100_PAR;
            ret = true;
          }else{
            vt100_state = VT100_INIT;
            ret = true;
          }
          break;
        case VT100_PAR:
          if ( ch == '0'){
            vt100_state = VT100_CMD_0;
            ret = true;
          }else if ( ch == '2'){
            vt100_state = VT100_CMD_2;
            ret = true;
          }else if ( ch == '4'){
            vt100_state = VT100_CMD_4;
            ret = true;
          }else if ( ch == 'H'){
            Serial.print("Posicionando o cursor a tela...\n");
            vga.setCursorText( 0, 0);
            vt100_state = VT100_INIT;
            ret = true;
          }
          break;
        case VT100_CMD_0:
            if( ch == 'm' ){  //Clear screen
              Serial.print("Limpando atributos da tela...\n");
              vt100_state = VT100_INIT;
              ret = true;
            }
          break;
        case VT100_CMD_2:
            if( ch == 'J' ){  //Clear screen
              Serial.print("Limpando a tela...\n");
              vga.clear();
              vt100_state = VT100_INIT;
              ret = true;
            }
          break;
        case VT100_CMD_4:
            if( ch == '1' ){
              cor = VERMELHO;             
              ret = true;
            }else if( ch == '2' ){
              cor = VERDE;           
              ret = true;
            }else if( ch == '3' ){
              cor = AMARELO;             
              ret = true;
            }else if( ch == '4' ){
              cor = AZUL;         
              ret = true;
            }else if( ch == '5' ){
              cor = MAGENTA;             
              ret = true;
            }else if( ch == '6' ){
              cor = CIANO;        
              ret = true;
            }else if( ch == '7' ){
              cor = BRANCO;            
              ret = true;
            }
            vt100_state = VT100_CMD_4EXEC;
          break;
        case VT100_CMD_4EXEC:
            if( ch == 'm' ){
              Serial.print("cor");
              vga.setTextColor(myCOLORS[cor]); 
              ret = true;
            }              
            vt100_state = VT100_INIT;
          break;          
      }
      return ret;
    }

  private:
    uint8_t vt100_state;
    uint8_t cor;
};