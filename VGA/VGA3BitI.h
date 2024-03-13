/*
	Author: bitluni 2019
	License: 
	Creative Commons Attribution ShareAlike 4.0
	https://creativecommons.org/licenses/by-sa/4.0/
	
	For further details check out: 
		https://youtube.com/bitlunislab
		https://github.com/bitluni
		http://bitluni.net
*/
#ifndef __VGA3Biti_H__
#define __VGA3Biti_H__

#include "VGA.h"
#include "../Graphics/GraphicsR1G1B1A1X2S2Swapped.h"

class VGA3BitI : public VGA , public GraphicsR1G1B1A1X2S2Swapped
{
  public:
	VGA3BitI();	//8 bit based modes only work with I2S1
	bool init(const Mode &mode, const int RPin, const int GPin, const int BPin, const int hsyncPin, const int vsyncPin, const int clockPin = -1);
	bool init(const Mode &mode, const PinConfig &pinConfig);
	virtual void initSyncBits();
	virtual long syncBits(bool hSync, bool vSync);
	virtual int bytesPerSample() const;
	virtual float pixelAspect() const;
	virtual void propagateResolution(const int xres, const int yres);
	virtual void show(bool vSync = false);
  protected:
	bool useInterrupt();
	static void interrupt(void *arg);

	static void interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits, void *arg);
};




#endif
