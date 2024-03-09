#include <cstdint>

#define SHIFT 1
#define SCROLL_LOCK   1
#define NUM_LOCK      2
#define CAPS_LOCK     4

#define C_SCROLL_LOCK   ~SCROLL_LOCK
#define C_NUM_LOCK      ~NUM_LOCK
#define C_CAPS_LOCK     ~CAPS_LOCK

class PS2Keyboard
{
public:
	PS2Keyboard();
  ~PS2Keyboard();
  void begin();
	byte GetScancode();
  byte GetCharcode();
	void SendByte(byte b);
	bool WaitForKeyboard();
  void setNumLock();
  void handleLeds();
	bool SendByteWithConfirm(byte b, byte ACK, byte Tries, unsigned short Wait);
	bool SetKeyboardLights( );
  void interruptHandler();
  int  GetBufferSize();
private:
  static PS2Keyboard* keyboard0Ptr;
  static PS2Keyboard* keyboard1Ptr;
  static PS2Keyboard* keyboard2Ptr;
  static PS2Keyboard* keyboard3Ptr;
  static PS2Keyboard* keyboard4Ptr;
  static PS2Keyboard* keyboard5Ptr;
  static PS2Keyboard* keyboard6Ptr;
  static PS2Keyboard* keyboard7Ptr;
        static void kbdInterrupt0();
        static void kbdInterrupt1();
        static void kbdInterrupt2();
        static void kbdInterrupt3();
        static void kbdInterrupt4();
        static void kbdInterrupt5();
        static void kbdInterrupt6();
        static void kbdInterrupt7();  
	byte GetParity(byte b);
};
