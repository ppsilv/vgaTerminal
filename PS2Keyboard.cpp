#include <Arduino.h>
#include "PS2Keyboard.h"



const PS2Keymap_t *keymap=NULL;

#define TransistorIRQPin  26
#define TransistorDataPin 27
#define DataPin     32
#define IRQPin      33
//Oito estados para a varialvel uint8_t state
#define BREAK     0x01
#define MODIFIER  0x02
#define SHIFT_L   0x04
#define SHIFT_R   0x08
#define ALTGR     0x10
#define SCROLL    0x20
#define NUMLOCK   0x40
#define CAPS      0x80


extern void hnd(void);

PS2Keyboard* PS2Keyboard::keyboard0Ptr;
/*
PS2Keyboard* PS2Keyboard::keyboard1Ptr;
PS2Keyboard* PS2Keyboard::keyboard2Ptr;
PS2Keyboard* PS2Keyboard::keyboard3Ptr;                              
PS2Keyboard* PS2Keyboard::keyboard4Ptr;
PS2Keyboard* PS2Keyboard::keyboard5Ptr;
PS2Keyboard* PS2Keyboard::keyboard6Ptr;
PS2Keyboard* PS2Keyboard::keyboard7Ptr;
*/
const char chrsNS[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '`',  0,
    0,    0,    0,    0,    0,    'q',  '1',  0,    0,    0,    'z',   's',  'a',  'w',  '2',  0,
    0,    'c',  'x',  'd',  'e',  '4',  '3',  0,    0,    ' ',  'v',  'f',  't',  'r',  '5',  0,
    0,    'n',  'b',  'h',  'g',  'y',  '6',  0,    0,    0,    'm',  'j',  'u',  '7',  '8',  0,
    0,    ',',  'k',  'i',  'o',  '0',  '9',  0,    0,    '.',  '/',  'l',  ';',  'p',  '-',  0,
    0,    0,    '\'', 0,    '[',  '=',  0,    0,    0,    0,    '\n', ']',  0,    '\\', 0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   251,  '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

const char chrsSH[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '~',  0,
    0,    0,    0,    0,    0,    'Q',  '!',  0,    0,    0,    'Z',  'S',  'A',  'W',  '@',  0,
    0,    'C',  'X',  'D',  'E',  '$',  '#',  0,    0,    ' ',  'V',  'F',  'T',  'R',  '%',  0,
    0,    'N',  'B',  'H',  'G',  'Y',  '^',  0,    0,    0,    'M',  'J',  'U',  '&',  '*',  0,
    0,    '<',  'K',  'I',  'O',  ')',  '(',  0,    0,    '>',  '?',  'L',  ':',  'P',  '_',  0,
    0,    0,    '\"', 0,    '{',  '+',  0,    0,    0,    0,    '\n', '}',  0,    '|',  0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   0,    '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};

const char chrsSH1[]={
    0,    249,  0,    245,  243,  241,  242,  252,  0,    250,  248,  246,  244,  '\t', '~',  0,
    0,    0,    0,    0,    0,    'q',  '!',  0,    0,    0,    'z',  's',  'a',  'w',  '@',  0,
    0,    'c',  'x',  'd',  'e',  '$',  '#',  0,    0,    ' ',  'v',  'f',  't',  'r',  '%',  0,
    0,    'n',  'b',  'h',  'g',  'y',  '^',  0,    0,    0,    'm',  'j',  'u',  '&',  '*',  0,
    0,    '<',  'k',  'i',  'O',  ')',  '(',  0,    0,    '>',  '?',  'l',  ':',  'p',  '_',  0,
    0,    0,    '\"', 0,    '{',  '+',  0,    0,    0,    0,    '\n', '}',  0,    '|',  0,    0,
    0,    0,    0,    0,    0,    0,    '\b', 0,    0,    '1',  0,    '4',  '7',  0,    0,    0,
    '0',  '.',  '2',  '5',  '6',  '8',  '\033',0,   0,    '+',  '3',  '-',  '*',  '9',  0,    0,
    0,    0,    0,    247,  0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0};


#define BUFFER_SIZE 256
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t head, tail;


static volatile uint8_t SendByteState;
static volatile byte    SendBits[10];
static volatile uint8_t disableisr;
static volatile uint8_t fulldisableisr;
static volatile uint16_t waitcounter;

bool bs = false;
bool tab = false;


volatile uint8_t state=0;
volatile bool capsChanged=false;
volatile bool numChanged=false;

#pragma GCC push_options
#pragma GCC optimize ("O2")

int PS2Keyboard::GetBufferSize()
{
  return(sizeof(buffer));
}

byte PS2Keyboard::GetParity(byte b)
{
	byte count = 0;
	for (byte i = 0; i < 8; i++)
	{
		if (((b >> i) & 0x1) == 1)
			count++;
	}
	return ((count % 2) == 1 ? 0 : 1);
}

void PS2Keyboard::SendByte(byte b)
{
	SendBits[0] = 1;

	for (byte j = 0; j < 8; j++)
		SendBits[j + 1] = ((b >> j) & 0x1) == 0x1 ? 0x0 : 0x1;

	SendBits[9] = GetParity(b) == 0x1 ? 0x0 : 0x1;

	SendByteState  = 0;
	fulldisableisr = 1;
	digitalWrite(TransistorIRQPin, 1);
	delayMicroseconds(100);
	digitalWrite(TransistorDataPin, 1);
	delayMicroseconds(100);
	digitalWrite(TransistorIRQPin, 0);
	disableisr = 1;
	fulldisableisr = 0;

	while (disableisr != 0); //Aguarda a interrupção transmitir

	delay(10);
}

bool PS2Keyboard::SendByteWithConfirm(byte b, byte ACK, byte Tries, unsigned short Wait)
{
	unsigned long wait = millis();
	byte Test = 0;
	byte dat = 0;

	do
	{
		SendByte(b);
		while (1)
		{
			dat = GetScancode();
			if (dat == ACK)
				return(true);

			if (millis() - wait > Wait) //Sek
			{
				Test++;
				break;
			}
		}
	} while (Test < Tries);

	return(false);
}

void PS2Keyboard::verifyStatus()
{
    if( capsChanged == true   ){
        capsChanged = false;  
        SetKeyboardLights();
    }
    if( numChanged == true   ){
        numChanged = false;  
        SetKeyboardLights();
    } 
}

void PS2Keyboard::setNumLock()
{
    state |= NUMLOCK;
    SetKeyboardLights();
}

bool PS2Keyboard::SetKeyboardLights() //byte NumLock, byte CapsLock, byte ScrollLock)
{
  //11100000
	byte b = (state >> 5);
 
	if (SendByteWithConfirm(0xED, 0xFA, 1, 500) == false) //Change Lights
		return(false);

	if (SendByteWithConfirm(b, 0xFA, 1, 500) == false) //Lights data
		return(false);

	return(true);
}

bool PS2Keyboard::WaitForKeyboard()
{
	unsigned long wait = millis();
	byte Test = 0;

	if (SendByteWithConfirm(0xFF, 0xAA, 5, 2000) == false) //Reset Keyboard
		return(false);

	if (SendByteWithConfirm(0xEE, 0xEE, 1, 500) == false) //Echo Test (connectivity test)
		return(false);

	if (SendByteWithConfirm(0xF4, 0xFA, 1, 500) == false) //Enable Keyboard
		return(false);

	return(true);
}
#pragma GCC pop_options

byte PS2Keyboard::GetScancode()
{
	uint8_t c, i;

	i = tail;
	if (i == head) return 0;
	i++;

	if (i >= BUFFER_SIZE) i = 0;
	c = buffer[i];
	tail = i;
	return (c);
}
byte PS2Keyboard::GetCharcode()
{
	uint8_t c;//, i;

	//i = tail;
	if (tail == head) return 0;
	c = buffer[tail];
	tail++;
  return(chrsNS[c]);
}
//void KeyboardISR() //FALLING EDGE
//void ICACHE_RAM_ATTR PS2Keyboard::interruptHandler() 
void IRAM_ATTR PS2Keyboard::interruptHandler()
{
	static uint8_t bitcount = 0;
	static uint8_t incoming = 0;
	static uint32_t prev_ms = 0;
	uint32_t now_ms;
	uint8_t n, val;

	if (fulldisableisr != 0)
		return;
	if (disableisr != 0){
		SendByteState++;
		switch (SendByteState){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			digitalWrite(TransistorDataPin, SendBits[SendByteState]);
			break;                              
		case 10:
			digitalWrite(TransistorDataPin, 0);
			break;
		case 11:
			waitcounter = 0;
			while (digitalRead(DataPin) != 0){
				waitcounter++;                             
				if (waitcounter > 32767){
					disableisr = 0;
					return;                             
				}
			};
			waitcounter = 0;
			while (digitalRead(DataPin) != 1){
				waitcounter++;
				if (waitcounter > 32767){
					disableisr = 0;
					return;
				}
			};
			disableisr = 0;
			break;
		}
		return;
	}

	val = digitalRead(DataPin);
	now_ms = millis();
	if ((now_ms - prev_ms) > 250){
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7){
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11){
		//uint8_t i = head + 1;
		//if (i >= BUFFER_SIZE) i = 0;
    head++;
		if (head != tail) {
			buffer[head] = incoming;
			//head = i;
		}
    bitcount = 0;
		incoming = 0;
	}
}

PS2Keyboard::PS2Keyboard()
{
}

void PS2Keyboard::begin()
{
	pinMode(IRQPin, INPUT_PULLUP);
	pinMode(DataPin, INPUT_PULLUP);
	pinMode(TransistorDataPin, OUTPUT);
	pinMode(TransistorIRQPin, OUTPUT);

	digitalWrite(TransistorDataPin, 0);
	digitalWrite(TransistorIRQPin, 0);

	disableisr = 0;
	head = 0;
	tail = 0;
	fulldisableisr = 0;

  keymap = &PS2Keymap_US;
 
    if (keyboard0Ptr==nullptr) {
        Serial.println("keyboard0Ptr");
        keyboard0Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt0, FALLING);
    }/*
    else if (keyboard1Ptr==nullptr) {
        Serial.println("keyboard1Ptr");
        keyboard1Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt1, FALLING);
    }
    else if (keyboard2Ptr==nullptr) {
        Serial.println("keyboard2Ptr");
        keyboard2Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt2, FALLING);
    }
    else if (keyboard3Ptr==nullptr) {
        Serial.println("keyboard3Ptr");
        keyboard3Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt3, FALLING);
    }
    else if (keyboard4Ptr==nullptr) {
        Serial.println("keyboard4Ptr");
        keyboard4Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt4, FALLING);
    }
    else if (keyboard5Ptr==nullptr) {
        Serial.println("keyboard5Ptr");
        keyboard5Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt5, FALLING);
    }
    else if (keyboard6Ptr==nullptr) {
        Serial.println("keyboard6Ptr");
        keyboard6Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt6, FALLING);
    }
    else if (keyboard7Ptr==nullptr) {
        Serial.println("keyboard7Ptr");
        keyboard7Ptr = this;
        attachInterrupt(digitalPinToInterrupt(IRQPin), kbdInterrupt7, FALLING);
    }*/
  
}

void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt0() {
    keyboard0Ptr->interruptHandler();
}/*
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt1() {
    keyboard1Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt2() {
    keyboard2Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt3() {
    keyboard3Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt4() {
   keyboard4Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt5() {
    keyboard5Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt6() {
    keyboard6Ptr->interruptHandler();
}
void ICACHE_RAM_ATTR PS2Keyboard::kbdInterrupt7() {
    keyboard7Ptr->interruptHandler();
}*/

PS2Keyboard::~PS2Keyboard(){
  detachInterrupt(IRQPin);
}

char PS2Keyboard::GetIso8859Code(void)
{
	uint8_t s;
	char c;

	while (1) {
		s = GetScancode();
		if (!s) return 0;
		if (s == 0xF0) {
			state |= BREAK;
		} else if (s == 0xE0) {
			state |= MODIFIER;
		} else {
			if (state & BREAK) {
				if (s == 0x12) {
					state &= ~SHIFT_L;
				} else if (s == 0x58) {
					//state &= ~CAPS;
				} else if (s == 0x59) {
					state &= ~SHIFT_R;
				} else if (s == 0x77) {
					//state &= ~NUMLOCK;
				} else if (s == 0x7E) {
					//state &= ~SCROLL;
				} else if (s == 0x11 && (state & MODIFIER)) {
					state &= ~ALTGR;
				}
				// CTRL, ALT & WIN keys could be added
				// but is that really worth the overhead?
				state &= ~(BREAK | MODIFIER);
				continue;
			}
			if (s == 0x12) {
				state |= SHIFT_L;
				continue;
			} else if (s == 0x58) {
        if ( state & CAPS )
				  state &= ~CAPS;
        else  
          state |= CAPS;
        capsChanged = true;  
				continue;
			} else if (s == 0x59) {
				state |= SHIFT_R;
				continue;
			} else if (s == 0x77) {
        if ( state & NUMLOCK )
				  state &= ~NUMLOCK;
        else  
          state |= NUMLOCK;
        numChanged = true;  
				continue;
			} else if (s == 0x7E) {
        if ( state & SCROLL )
				  state &= ~SCROLL;
        else  
          state |= SCROLL;
				continue;
			} else if (s == 0x11 && (state & MODIFIER)) {
				state |= ALTGR;
			}
			c = 0;
			if (state & MODIFIER) {
				switch (s) {
				  case 0x70: c = PS2_INSERT;      break;
				  case 0x6C: c = PS2_HOME;        break;
				  case 0x7D: c = PS2_PAGEUP;      break;
				  case 0x71: c = PS2_DELETE;      break;
				  case 0x69: c = PS2_END;         break;
				  case 0x7A: c = PS2_PAGEDOWN;    break;
				  case 0x75: c = PS2_UPARROW;     break;
				  case 0x6B: c = PS2_LEFTARROW;   break;
				  case 0x72: c = PS2_DOWNARROW;   break;
				  case 0x74: c = PS2_RIGHTARROW;  break;
				  case 0x4A: c = '/';             break;
				  case 0x5A: c = PS2_ENTER;       break;
				  default: break;
				}
			} 
      else if ((state & ALTGR) && keymap->uses_altgr) {
				if (s < PS2_KEYMAP_SIZE)
					c = keymap->altgr[s];
			} else if ( (state & CAPS) && (state & (SHIFT_L | SHIFT_R) )) {
				if (s < PS2_KEYMAP_SIZE)
					c = keymap->capsshift[s];
			} else if (state & (SHIFT_L | SHIFT_R)) {
				if (s < PS2_KEYMAP_SIZE)
					c = keymap->shift[s];
			} else if (state & CAPS) {
				if (s < PS2_KEYMAP_SIZE)
					c = toUpperCase(keymap->noshift[s]);
			} else {
				if (s < PS2_KEYMAP_SIZE)
					c = keymap->noshift[s];
			}
			state &= ~(BREAK | MODIFIER);
			if (c) return c;
		}
	}
}

//backup da interrupção
/*
//void KeyboardISR() //FALLING EDGE
//void ICACHE_RAM_ATTR PS2Keyboard::interruptHandler() 
void IRAM_ATTR PS2Keyboard::interruptHandler() 
{

	if (fulldisableisr != 0)
		return;

	if (disableisr != 0){
		SendByteState++;
		switch (SendByteState){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			digitalWrite(TransistorDataPin, SendBits[SendByteState]);
			break;                              

		case 10:
			digitalWrite(TransistorDataPin, 0);
			break;
		case 11:
			waitcounter = 0;
			while (digitalRead(DataPin) != 0){
				waitcounter++;                             
				if (waitcounter > 32767){
					disableisr = 0;
					return;                             
				}
			};
			waitcounter = 0;
			while (digitalRead(DataPin) != 1){
				waitcounter++;
				if (waitcounter > 32767){
					disableisr = 0;
					return;
				}
			};
			disableisr = 0;
			break;
		}
		return;
	}
  
	static uint8_t bitcount = 0;
	static uint8_t incoming = 0;
	static uint32_t prev_ms = 0;
	uint32_t now_ms;
	uint8_t n, val;

	val = digitalRead(DataPin);
	now_ms = millis();
	if (now_ms - prev_ms > 500){
		bitcount = 0;
		incoming = 0;
	}
	prev_ms = now_ms;
	n = bitcount - 1;
	if (n <= 7){
		incoming |= (val << n);
	}
	bitcount++;
	if (bitcount == 11){
    if (incoming == 0x12 || incoming == 0x59)
      modifs |=SHIFT;
    if ( (teclaStatus == 1) || (incoming == 0xAA) || (incoming == 0xEE || (incoming == 0xFA)) ){
      switch(incoming){
        case 0x12:
        case 0x59:
          modifs &= !SHIFT;
          break;
        case 0x58:
          cpslk = !cpslk;
          break;
        case 0x77:
          numlk = !numlk;
          break;
        case 0x7E:
          scrollk = !scrollk;
          break;
        default:  
          uint8_t i = head + 1;
          if (i >= BUFFER_SIZE) i = 0;
          if (i != tail){
            buffer[i] = incoming;
            head = i;
          }
      } 
      teclaStatus = 0;
    }
    //Serial.print(" [");    Serial.print(incoming, 16);    Serial.print("] ");
    switch(incoming){
      case 0xF0:
        teclaStatus = 1;
        break;
    }
		bitcount = 0;
		incoming = 0;
	}
}


*/