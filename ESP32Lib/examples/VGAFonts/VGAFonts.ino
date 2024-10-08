//This example shows how to use different fonts on a VGA screen.
//You need to connect a VGA screen cable to the pins specified below.
//cc by-sa 4.0 license
//bitluni

#include <ESP32Video.h>
#include <Ressources/CodePage437_8x8.h>
#include <Ressources/CodePage437_8x14.h>
#include <Ressources/CodePage437_8x16.h>
#include <Ressources/CodePage437_8x19.h>
#include <Ressources/CodePage437_9x16.h>
#include <Ressources/Font6x8.h>

//pin configuration
const int redPin = 14;
const int greenPin = 19;
const int bluePin = 27;
const int hsyncPin = 32;
const int vsyncPin = 33;

//VGA Device
VGA3Bit videodisplay;

void setup()
{
	//initializing vga at the specified pins
	videodisplay.init(VGAMode::MODE640x400, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
	//selecting the font
	videodisplay.setFont(Font6x8);
	//set color
	videodisplay.setTextColor(videodisplay.RGB(255, 0, 0), videodisplay.RGB(0, 0, 255));
	//displaying the character set
	videodisplay.println("Font6x8");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
	videodisplay.setFont(CodePage437_8x8);
	videodisplay.setTextColor(videodisplay.RGB(0, 255, 0), videodisplay.RGB(255, 0, 0));
	videodisplay.println("CodePage437_8x8");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
	videodisplay.setFont(CodePage437_8x14);
	videodisplay.setTextColor(videodisplay.RGB(0, 0, 255), videodisplay.RGB(0, 255, 0));
	videodisplay.println("CodePage437_8x14");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
	videodisplay.setFont(CodePage437_8x16);
	videodisplay.setTextColor(videodisplay.RGB(255, 255, 0), videodisplay.RGB(0, 255, 255));
	videodisplay.println("CodePage437_8x16");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
	videodisplay.setFont(CodePage437_8x19);
	videodisplay.setTextColor(videodisplay.RGB(255, 0, 255), videodisplay.RGB(255, 255, 0));
	videodisplay.println("CodePage437_8x19");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
	videodisplay.setFont(CodePage437_9x16);
	videodisplay.setTextColor(videodisplay.RGB(0, 255, 255), videodisplay.RGB(255, 0, 255));
	videodisplay.println("CodePage437_9x16");
	for (int i = 0; i < 256; i++)
		videodisplay.print((char)i);
	videodisplay.println();
}

void loop()
{
}
