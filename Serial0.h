#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <Arduino.h>
#include <CircularBuffer.hpp>  //https://github.com/ppsilv/CircularBuffer
#include <ESP32Video.h>

/*
  Testado
  9600
  19200
  38400
  57600
  115200
*/


class TSerial0{
public:
      static TSerial0 * getInstance();  
        //void onReceiveFunction(void);
        //uint8_t GetChar();
        void begin();
        int getBaud();
   
private:
  static TSerial0 * instance;
   TSerial0();
   ~TSerial0();
   const int DATA_SIZE = 26;         // 26 bytes is a lower than RX FIFO size (127 bytes) 
   int BAUD ;       // Any baudrate from 300 to 115200
   const int TEST_UART = 0;          // Se igual a 1 Serial1 will be used for the loopback testing with different RX FIFO FULL values
   const int RXPIN     = 16;         // GPIO 16 => RX for Serial1
   const int TXPIN     = 17;         // GPIO 17 => TX for Serial1

};

#endif
