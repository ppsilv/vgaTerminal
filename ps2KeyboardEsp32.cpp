#include <Arduino.h>
#include "PS2Keyboard.h"
#include "soc/rtc_wdt.h"

//http://www-ug.eecg.utoronto.ca/desl/nios_devices_SoC/datasheets/PS2%20Protocol.htm
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
//https://www.win.tue.nl/~aeb/linux/kbd/scancodes-12.html


PS2Keyboard keyboard;
char dat;

void setupPs2Keyboard()
{
	Serial.begin(115200);
  
  disableCore0WDT();
  delay(500); // experienced crashes without this delay!
  disableCore1WDT(); 
	delay(500);


rtc_wdt_protect_off();
rtc_wdt_disable();

	Serial.write("\n\n\n\nKeyboard test Init\n");

  keyboard.begin();

	if (keyboard.WaitForKeyboard() == false)
	{
		Serial.write("Keyboard not found... \nPress F1 to continue\n \nStoped......");
		while (1);
	}

	keyboard.setNumLock();
	keyboard.handleLeds();

	Serial.write("\nInitialization Completed\n");
  Serial.printf("Buffer size: [%02d]\n",keyboard.GetBufferSize());
}


void loopPs2Keyboard()
{
	dat = keyboard.GetCharcode();

	if (dat != 0){
		Serial.print(dat);
  }

  keyboard.handleLeds();
}
