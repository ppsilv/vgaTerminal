
#include <Arduino.h>
#include <CircularBuffer.hpp>  //https://github.com/ppsilv/CircularBuffer 
//#include <ESP32Lib.h>

#define DATA_SIZE 26    // 26 bytes is a lower than RX FIFO size (127 bytes) 
#define BAUD 9600       // Any baudrate from 300 to 115200
#define TEST_UART 1     // Serial1 will be used for the loopback testing with different RX FIFO FULL values
#define RXPIN 16         // GPIO 4 => RX for Serial1
#define TXPIN 17         // GPIO 5 => TX for Serial1

CircularBuffer<char, 255> buffer;


volatile size_t sent_bytes = 0, received_bytes = 0;
extern void printVga(char i);

void onReceiveFunction(void) {
  char b;
  size_t available = Serial1.available();
  received_bytes += available;
  //Serial.printf("onReceive Callback:: There are %d bytes available: ", available);
  while (available --) {
    b=(char)Serial1.read();
    //Serial.print(b);
    buffer.push(b);
    printVga(b);
  }
  //Serial.println();
}

void setupUart() {
  // Debug
  //Serial.begin(115200);

  // UART1 will have its RX<->TX cross connected
  Serial1.begin(BAUD, SERIAL_8N1, RXPIN, TXPIN);
  Serial1.onReceive(onReceiveFunction, false); // sets a RX callback function for Serial 1

}
 
void loopUart() {
  char b = buffer.pop();
  Serial1.write(b);

}

