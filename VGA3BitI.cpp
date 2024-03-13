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
#include "VGA/VGA3BitI.h"
 
	VGA3BitI::VGA3BitI()	//8 bit based modes only work with I2S1
		: VGA(1), GraphicsR1G1B1A1X2S2Swapped()
	{
		interruptStaticChild = &VGA3BitI::interrupt;
	}

	bool VGA3BitI::init(const Mode &mode, const int RPin, const int GPin, const int BPin, const int hsyncPin, const int vsyncPin, const int clockPin )
	{
		int pinMap[8] = {
			RPin,
			GPin,
			BPin,
			-1, -1, -1,
			hsyncPin, vsyncPin
		};
		return VGA::init(mode, pinMap, 8, clockPin);
	}

	bool VGA3BitI::init(const Mode &mode, const PinConfig &pinConfig)
	{
		int pins[8];
		pinConfig.fill3Bit(pins);
		return VGA::init(mode, pins, 8, pinConfig.clock);
	}

	 void VGA3BitI::initSyncBits()
	{
		hsyncBitI = mode.hSyncPolarity ? 0x40 : 0;
		vsyncBitI = mode.vSyncPolarity ? 0x80 : 0;
		hsyncBit = hsyncBitI ^ 0x40;
		vsyncBit = vsyncBitI ^ 0x80;
	}

	 long VGA3BitI::syncBits(bool hSync, bool vSync)
	{
		return ((hSync ? hsyncBit : hsyncBitI) | (vSync ? vsyncBit : vsyncBitI)) * 0x1010101;
	}

	 int VGA3BitI::bytesPerSample() const
	{
		return 1;
	}

	 float VGA3BitI::pixelAspect() const
	{
		return 1;
	}

	 void VGA3BitI::propagateResolution(const int xres, const int yres)
	{
		setResolution(xres, yres);
	}

	 void VGA3BitI::show(bool vSync )
	{
		if (!frameBufferCount)
			return;
		if (vSync)
		{
			vSyncPassed = false;
			while (!vSyncPassed)
				delay(0);
		}
		Graphics::show(vSync);
	}


	bool VGA3BitI::useInterrupt()
	{ 
		return true; 
	}


void IRAM_ATTR VGA3BitI::interrupt(void *arg)
{
	VGA3BitI * staticthis = (VGA3BitI *)arg;
	
	unsigned long *signal = (unsigned long *)staticthis->dmaBufferDescriptors[staticthis->dmaBufferDescriptorActive].buffer();
	unsigned long *pixels = &((unsigned long *)staticthis->dmaBufferDescriptors[staticthis->dmaBufferDescriptorActive].buffer())[(staticthis->mode.hSync + staticthis->mode.hBack) / 4];
	unsigned long base, baseh;
	if (staticthis->currentLine >= staticthis->mode.vFront && staticthis->currentLine < staticthis->mode.vFront + staticthis->mode.vSync)
	{
		baseh = (staticthis->hsyncBit | staticthis->vsyncBit) * 0x1010101;
		base = (staticthis->hsyncBitI | staticthis->vsyncBit) * 0x1010101;
	}
	else
	{
		baseh = (staticthis->hsyncBit | staticthis->vsyncBitI) * 0x1010101;
		base = (staticthis->hsyncBitI | staticthis->vsyncBitI) * 0x1010101;
	}
	for (int i = 0; i < staticthis->mode.hSync / 4; i++)
		signal[i] = baseh;
	for (int i = staticthis->mode.hSync / 4; i < (staticthis->mode.hSync + staticthis->mode.hBack) / 4; i++)
		signal[i] = base;

	int y = (staticthis->currentLine - staticthis->mode.vFront - staticthis->mode.vSync - staticthis->mode.vBack) / staticthis->mode.vDiv;
	if (y >= 0 && y < staticthis->mode.vRes)
		staticthis->interruptPixelLine(y, pixels, base, arg);
	else
		for (int i = 0; i < staticthis->mode.hRes / 4; i++)
		{
			pixels[i] = base;
		}
	for (int i = 0; i < staticthis->mode.hFront / 4; i++)
		signal[i + (staticthis->mode.hSync + staticthis->mode.hBack + staticthis->mode.hRes) / 4] = base;
	staticthis->currentLine = (staticthis->currentLine + 1) % staticthis->totalLines;
	staticthis->dmaBufferDescriptorActive = (staticthis->dmaBufferDescriptorActive + 1) % staticthis->dmaBufferDescriptorCount;
	if (staticthis->currentLine == 0)
		staticthis->vSyncPassed = true;
}

void IRAM_ATTR VGA3BitI::interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits, void *arg)
{
	VGA3BitI * staticthis = (VGA3BitI *)arg;
	unsigned char *line = staticthis->frontBuffer[y];
	int j = 0;
	for (int i = 0; i < staticthis->mode.hRes / 4; i++)
	{
		int p0 = (line[j] >> 0) & 7;
		int p1 = (line[j++] >> 4) & 7;
		int p2 = (line[j] >> 0) & 7;
		int p3 = (line[j++] >> 4) & 7;
		pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	}
}

