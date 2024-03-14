#include <type_traits>
#include "Serial0.h"

CircularBuffer<char, 255> buffer;
Serial0 * Serial0::instance = 0;


Serial0::Serial0(){

}
Serial0::~Serial0(){
}
Serial0 * Serial0::getInstance(){
    if ( instance == NULL ){
      instance = new Serial0();
    }
    return instance;
}
void onReceiveFunction(void) {
  char b;
  char available = Serial1.available();
  //received_bytes += available;

  while (available --) {
    b=(char)Serial1.read();
    buffer.push(b);
    Serial.print(b);
  }
}
uint8_t Serial0::GetChar(){
  return buffer.pop();
}
void Serial0::begin() {
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  Serial1.onReceive(onReceiveFunction, false); // sets a RX callback function for Serial 1
}
 
void Serial0::run() {
  if( ! buffer.isEmpty() ){
    char b = buffer.pop();
    Serial.printf("Serial [%02x]\n",b);
  }
}

