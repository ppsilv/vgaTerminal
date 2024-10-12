#include <type_traits>
#include "Serial0.h"
#include "Terminal.h"

#define BAUD01  14
#define BAUD02  12

//CircularBuffer<char, 255> buffer;
TSerial0 * TSerial0::instance = 0;

//Terminal
extern Terminal terminal;

TSerial0::TSerial0(){

}
TSerial0::~TSerial0(){
}
TSerial0 * TSerial0::getInstance(){
   	pinMode(BAUD01, INPUT_PULLUP);
	  pinMode(BAUD02, INPUT_PULLUP);  

    if ( instance == NULL ){
      instance = new TSerial0();
    }
    return instance;
}
//Caracter vem da serial e vai em direção ao terminal
void onReceiveFunction(void) {
  unsigned char b;
  unsigned char available = Serial2.available();
  while (available) {
    b=(unsigned char)Serial2.read();
    terminal.print(b);
    --available;
  }
}

int TSerial0::getBaud() {
  uint8_t baudPin1,baudPin2;

  baudPin1 = digitalRead(BAUD01);
  baudPin2 = digitalRead(BAUD02);

  if ( baudPin1 == 0 && baudPin2 == 0){ //9600
        BAUD = 9600;
  }else if ( baudPin1 == 0 && baudPin2 == 1){ //19200
        BAUD = 19200;
  }else if ( baudPin1 == 1 && baudPin2 == 0){ //38400
        BAUD = 38400;
  }else if ( baudPin1 == 1 && baudPin2 == 1){ //57600
        BAUD = 57600;
  }
  return BAUD;
}

void TSerial0::begin() {

  Serial2.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  Serial2.onReceive(onReceiveFunction, false); // sets a RX callback function for Serial 1
}
 

