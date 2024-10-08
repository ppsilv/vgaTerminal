//This example shows several 2d drawing features available for VGA
//You need to connect a VGA screen cable and an external DAC (simple R2R does the job) to the pins specified below.
//cc by-sa 4.0 license
//bitluni

//include libraries
#include <ESP32Video.h>
#include <Ressources/Font6x8.h>

//include a sprite
#include "rock.h"

//pin configuration
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 32;
const int vsyncPin = 33;

//VGA Device
VGA14Bit videodisplay;

//initial setup
void setup()
{
	//we need double buffering for smooth animations
	videodisplay.setFrameBufferCount(2);
	//initializing i2s vga (with only one framebuffer)
	videodisplay.init(VGAMode::MODE320x200, redPins, greenPins, bluePins, hsyncPin, vsyncPin);
	//setting the font
	videodisplay.setFont(Font6x8);
}

//just draw each frame
void loop()
{
	//some value for color ping pong
	static int c = 0;
	static int d = 1;
	c += d;
	if (c == 0 || c == 255)
		d = -d;

	//radius ping pong
	static int r = 0;
	static int dr = 1;
	r += dr;
	if (r == 0 || r == 31)
		dr = -dr;

	//clear the back buffer with black and start drawing
	videodisplay.clear(0);

	int x, y;
	x = 22;
	y = 5;
	//set the text cursor
	videodisplay.setCursor(x + 30, y);
	//print the text, println also exists
	videodisplay.print("dot(x,y,c)");
	//set a single pixel. dotAdd add the colors. dotMix uses the alpha to mix the colors
	videodisplay.dot(x + 60, y + 20, videodisplay.RGB(c, 0, 255 - c));

	x = 170;
	y = 5;
	videodisplay.setCursor(x, y);
	videodisplay.print("line(x0,y0,x1,y1,c)");
	//draw a line
	videodisplay.line(x + c / 8 + 50, y + 10, x + 32 + 40 - c / 8, y + 30, videodisplay.RGB(0, c, 255 - c));

	x = 15;
	y = 40;
	videodisplay.setCursor(x + 10, y);
	videodisplay.print("rect(x, y, w, h, c)");
	//draw a rectangle with the given width and height
	videodisplay.rect(x + 50, y + 15, 3 + c / 8, 19 - c / 16, videodisplay.RGB(0, c, 255 - c));

	x = 165;
	y = 40;
	videodisplay.setCursor(x, y);
	//draw a filled rectangle
	videodisplay.print("fillRect(x, y, w, h, c)");
	videodisplay.fillRect(x + 50, y + 15, 35 - c / 8, 3 + c / 16, videodisplay.RGB(255 - c, c, 0));

	x = 25;
	y = 80;
	videodisplay.setCursor(x + 10, y);
	//draw a circle with the given radius
	videodisplay.print("circle(x,y,r,c)");
	videodisplay.circle(x + 55, y + 20, 1 + r / 4, videodisplay.RGB(255 - c, 0, c));

	x = 172;
	y = 80;
	videodisplay.setCursor(x, y);
	//draw a filled circle
	videodisplay.print("fillCircle(x,y,r,c)");
	videodisplay.fillCircle(x + 60, y + 20, 8 - r / 4, videodisplay.RGB(c / 2, c / 2, 255 - c));

	x = 10;
	y = 120;
	videodisplay.setCursor(x + 10, y);
	//draw an ellipse
	videodisplay.print("ellipse(x,y,rx,ry,c)");
	videodisplay.ellipse(x + 70, y + 20, 1 + r / 2, 8 - r / 4, videodisplay.RGB(255 - c, c, 0));

	x = 160;
	y = 120;
	videodisplay.setCursor(x, y);
	//draw a filled ellipse
	videodisplay.print("fillEllipse(x,y,rx,ry,c)");
	videodisplay.fillEllipse(x + 70, y + 20, 16 - r / 2, 1 + r / 4, videodisplay.RGB(255 - c, c / 2, c / 2));

	x = 15;
	y = 160;
	videodisplay.setCursor(x + 35, y);
	videodisplay.print("print(text)");
	//generate a string
	char text[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for (int i = 0; i < 10; i++)
		text[i] = 33 + (i + (c >> 2));
	videodisplay.setCursor(x + 37, y + 17);
	//set the text and background color with opaque alpha (use RGBA to get a semi transparent back)
	videodisplay.setTextColor(videodisplay.RGB(c, 255 - c, 255), videodisplay.RGB(0, c / 2, 127 - c / 2));
	videodisplay.print(text);
	//reset the text color. no second parameter makes the background transparent
	videodisplay.setTextColor(videodisplay.RGB(255, 255, 255));

	x = 165;
	y = 160;
	videodisplay.setCursor(x + 15, y);
	videodisplay.print("image(image,x,y)");
	//draw the imported sprite. use millis() to calculate the sprite number 
	//Sprites uses "image(.." internally
	rock.drawMix(videodisplay, (millis() / 50) & 15, x + 65, y + 25);

	//show the rendering
	videodisplay.show();
}
