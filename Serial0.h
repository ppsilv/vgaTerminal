#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <Arduino.h>
#include <CircularBuffer.hpp>  //https://github.com/ppsilv/CircularBuffer
#include "ESP32Lib.h"

class Serial0{
public:
      static Serial0 * getInstance();  
        //void onReceiveFunction(void);
        //uint8_t GetChar();
        void begin();
        void run();
        void clearScreen();
        int getBaud();
   
private:
  static Serial0 * instance;
   Serial0();
   ~Serial0();
   const int DATA_SIZE = 26;         // 26 bytes is a lower than RX FIFO size (127 bytes) 
   const int BAUD      = 4800;       // Any baudrate from 300 to 115200
   const int TEST_UART = 0;          // Se igual a 1 Serial1 will be used for the loopback testing with different RX FIFO FULL values
   const int RXPIN     = 16;         // GPIO 4 => RX for Serial1
   const int TXPIN     = 17;         // GPIO 5 => TX for Serial1

};

#endif
