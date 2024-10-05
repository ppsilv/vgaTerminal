#include <Arduino.h>
#include <ESP32Video.h>

extern VGA1BitI vga;

void vt100_putch(uint8_t data){
  vga.print(data);
}

void vt100_CUP(){
  Serial.println("vt100_CUP -Not implemented");
}
void vt100_CUU(){
  Serial.println("vt100_CUU -Not implemented");
}
void vt100_CUD(){
  Serial.println("vt100_CUD -Not implemented");
}
void vt100_CUF(){
  Serial.println("vt100_CUF -Not implemented");
}
void vt100_CUB(){
  Serial.println("vt100_CUB -Not implemented");
}
void vt100_RI(void){
  Serial.println("vt100_RI -Not implemented");
}
void vt100_RIS(void){
  Serial.println("vt100_RIS -Not implemented");
}

void vt100_NEL(void){
  Serial.println("vt100_NEL -Not implemented");
}

void vt100_HTS(void){
  Serial.println("vt100_HTS -Not implemented");
}

void vt100_DECID(void){
  Serial.println("vt100_DECID -Not implemented");
}

void vt52_DECKPNM(void){
  Serial.println("vt52_DECKPAM -Not implemented");
}
void vt52_DECKPAM(){
  Serial.println("vt52_DECKPAM -Not implemented");
}
void vt52_enter_ansi_mode(){
  Serial.println("vt52_enter_ansi_mode -Not implemented");
}
void vt52_exit_graphics_mode(){
  Serial.println("vt52_exit_graphics_mode -Not implemented");
}
void vt100_ED(){
  Serial.println("vt100_ED -Not implemented");
}
void vt52_ident(){
  Serial.println("vt52_ident -Not implemented");
}
void vt100_char_set(bool b, uint8_t char_uk){
  Serial.println("vt100_char_set -Not implemented");
}
void vt100_EL(void){
  Serial.println("vt100_EL -Not implemented");
}

void vt100_DECLL(){
  Serial.println("vt100_DECLL -Not implemented");
}
void vt100_DECSTBM(){
  Serial.println("vt100_DECSTBM -Not implemented");
}
void vt100_DECTST(){
  Serial.println("vt100_DECTST -Not implemented");
}
void vt100_SGR(){
  Serial.println("vt100_SGR -Not implemented");
}


void vt100_RM(){
  Serial.println("vt100_RM -Not implemented");
}
void vt100_SM(){
  Serial.println("vt100_SM -Not implemented");
}
void uart_send(){
  Serial.println("uart_send -Not implemented");
}
void uart_send_answerback_string(){
  Serial.println("uart_send_answerback_string -Not implemented");
}
void vt100_HT(){
  Serial.println("vt100_HT -Not implemented");
}
void vt100_LF(){
  Serial.println("vt100_LF -Not implemented");
}
void vt100_CR(){
  Serial.println("vt100_CR -Not implemented");
}
void vt100_XON(){
  Serial.println("vt100_XON -Not implemented");
}
void vt100_XOFF(){
  Serial.println("vt100_XOFF -Not implemented");
}
void vt100_shift_out(bool b){
  Serial.println("vt100_shift_out -Not implemented");
}
