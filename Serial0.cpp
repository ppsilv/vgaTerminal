#include <type_traits>
#include "Serial0.h"
#include "Terminal.h"


//CircularBuffer<char, 255> buffer;
TSerial0 * TSerial0::instance = 0;

//VGA Device
//extern VGA1BitI vga;
//Terminal
extern Terminal terminal;

TSerial0::TSerial0(){

}
TSerial0::~TSerial0(){
}
TSerial0 * TSerial0::getInstance(){
    if ( instance == NULL ){
      instance = new TSerial0();
    }
    return instance;
}
//Caracter vem da serial e vai em direção ao terminal
void onReceiveFunction(void) {
  char b;
  char available = Serial2.available();
  while (available --) {
    b=(char)Serial2.read();
    terminal.print(b);
  }
}

int TSerial0::getBaud() {
  return BAUD;
}

void TSerial0::begin() {
  Serial2.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  Serial2.onReceive(onReceiveFunction, false); // sets a RX callback function for Serial 1
}
 

