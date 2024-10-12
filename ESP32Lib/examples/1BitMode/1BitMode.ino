//This example shows a simple "Hello world!" on a VGA screen.
//You need to connect a VGA screen cable to the pins specified below.
//cc by-sa 4.0 license
//bitluni

#include <ESP32Video.h>
#include <Ressources/Font6x8.h>

//pin configuration
const int redPin = 22;
const int greenPin = 19;
const int bluePin = 5;
const int hsyncPin = 23;
const int vsyncPin = 15;

//VGA Device
VGA1BitI videodisplay;

const uint8_t frontColors[] = {0x2,0x0,0x1,0x4,0x1,0x7,0x3};
const uint8_t backColors[] = {0x0,0x7,0x0,0x6,0x7,0x0,0x4};

void setup()
{
  Serial.begin(115200);
	//initializing vga at the specified pins
	videodisplay.init(VGAMode::MODE320x240, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//selecting the font
	videodisplay.setFont(Font6x8);
  videodisplay.setCursor(0,0);
  ////displaying the text
	////default coloring
	//videodisplay.println("Hello World!");
	//delay(10000);
	////custom coloring
  //for(int i = 0; i < 10; i++)
  //{
  //  videodisplay.setFrontGlobalColor(0,0,255);
  //  videodisplay.setBackGlobalColor(0,0,0);
  //  delay(500);
  //	videodisplay.setFrontGlobalColor(255,255,0);
  //	videodisplay.setBackGlobalColor(0,0,255);
  //  delay(500);
  //}
  //delay(2000);
	videodisplay.clear();
  videodisplay.println("Hello World!");
}

void loop()
{
  while(true){
    while(!Serial.available());
    uint8_t ch = Serial.read();
    //videodisplay.print((const char)ch);
    videodisplay.print("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    if( ch ==13){
      videodisplay.println("");
    }
  }
	//for(int i = 0; i < 10; i++)
	//{
	//	videodisplay.setCursor(random(1,300),random(1,220));
	//	videodisplay.println("Hello World!");
	//	delay(250);
	//}
	//delay(1500);
	//videodisplay.clear();
	//static int currentpalette = 0;
	//videodisplay.frontGlobalColor = frontColors[currentpalette];
	//videodisplay.backGlobalColor = backColors[currentpalette];
	//currentpalette = (currentpalette + 1)%sizeof(frontColors);

}
1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopq
1
Linha x0 y0    ......
Linha x1 y0    ......
Linha x2 y0    ......
Linha x3 y0    ......
Linha x4 y0    ......
Linha x5 y0    ......
               ......
2              ......
Linha x0 y1
Linha x1 y1
Linha x2 y1
Linha x3 y1
Linha x4 y1
Linha x5 y1
3
Linha x0 y2
Linha x1 y2
Linha x2 y2
Linha x3 y2
Linha x4 y2
Linha x5 y2
4
Linha x0 y3
Linha x1 y3
Linha x2 y3
Linha x3 y3
Linha x4 y3
Linha x5 y3
5
Linha x0 y4
Linha x1 y4
Linha x2 y4
Linha x3 y4
Linha x4 y4
Linha x5 y4
6
Linha x0 y5
Linha x1 y5
Linha x2 y5
Linha x3 y5
Linha x4 y5
Linha x5 y5
7
Linha x0 y6
Linha x1 y6
Linha x2 y6
Linha x3 y6
Linha x4 y6
Linha x5 y6
8
Linha x0 y7
Linha x1 y7
Linha x2 y7
Linha x3 y7
Linha x4 y7
Linha x5 y7






