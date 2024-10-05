#include "Vt100.h"

PLACE_IN_DTCM_BSS
struct _vt100 vt100;

Vt100 termVt100;
/**
 * The heart of the VT100.
 * This route the received character to the appropriate function.
 *
 * Basically, this is a state machine.
 * When a control character is received, it try to find a the next state of the
 *  based on the match.
 */

/* see http://www.real-world-systems.com/docs/ANSIcode.html */
/*
static void vt100_state_feed_C1(uint8_t data);
static void vt100_state_feed_hash(uint8_t data);
static void vt100_state_feed_opensquarebracket(uint8_t data);
static void vt100_state_feed_opensmallbracket(uint8_t data);
static void vt100_state_feed_closesmallbracket(uint8_t data);
static void vt100_state_feed_question(uint8_t data);
*/
//PLACE_IN_DTCM_BSS
//void (*vt100_state_feed)(uint8_t) = NULL;
Vt100::Vt100(){

}
Vt100::~Vt100(){
  
}



void Vt100::param_add(uint8_t data)
{
	/* prevent overflow */
	if (vt100.param.count >= PARAM_QUEUE_SIZE) {
		LOG_LN("Could not parse the parameter, overflow");
		return;
	}

	/* parse the parameters and add it to list */
	if (data == ';') {
		vt100.param.data[vt100.param.count++] = 0;
		return;
	}

	if (data < '0' || data > '9') { /* is digit, else ignore */
		LOGF_LN("Could not parse the parameter (0x%"PRIx8"), "
			"not a digit", data);
		return;
	}

	/* first time? */
	if (!vt100.param.count) {
		vt100.param.count = 1;
		vt100.param.data[0] = 0;
	}

	vt100.param.data[vt100.param.count - 1] *= 10;
	vt100.param.data[vt100.param.count - 1] += data - '0';
}

void vt52_state_feed_cda(uint8_t data)
{
	vt100.param.data[vt100.param.count++] = data - ' ' + 1;
	if (vt100.param.count == 2) {
		termVt100.vt100_CUP();
		termVt100.vt100_state_feed(NULL);
	}
}

void ansi_aux_keypad_and_cursor_code(uint8_t data) /* ESC O */
{
	switch (data) {
	case 'A': /* ESC O A */
		termVt100.vt100_CUU();
	break;
	case 'B': /* ESC O B */
		termVt100.vt100_CUD();
	break;
	case 'C': /* ESC O C */
		termVt100.vt100_CUF();
	break;
	case 'D': /* ESC O D */
		termVt100.vt100_CUB();
	break;
	default:
		termVt100.vt100_putch(data);
	break;
	}

	termVt100.set_vt100_state_feed(NULL);
}

void vt100_state_feed_C1(uint8_t data) /* ESC */
{
	if (vt100.DECANM) { /* ANSI mode */
		switch (data) {
		case '[': /* ESC [ */
			termVt100.set_vt100_state_feed(&vt100_state_feed_opensquarebracket);
		return ;
		case '#': /* ESC # */
			termVt100.set_vt100_state_feed(&vt100_state_feed_hash);
		return ;
		case '(': /* ESC ( */
			termVt100.set_vt100_state_feed(&vt100_state_feed_opensmallbracket);
		return ;
		case ')': /* ESC ) */
			termVt100.set_vt100_state_feed(&vt100_state_feed_closesmallbracket);
		return ;
		case '7': /* ESC 7 */
			termVt100.vt100_DECSC();
		break;
		case '8': /* ESC 8 */
			termVt100.vt100_DECRC();
		break;
		case 'c': /* ESC c */
			termVt100.vt100_RIS();
		break;
		case 'D': /* ESC D */
			termVt100.vt100_IND();
		break;
		case 'E': /* ESC E */
			termVt100.vt100_NEL();
		break;
		case 'H': /* ESC H */
			termVt100.vt100_HTS();
		break;
		case 'M': /* ESC M */
			termVt100.vt100_RI();
		break;
		case 'O': /* ESC O */
			termVt100.set_vt100_state_feed(&ansi_aux_keypad_and_cursor_code);
		return ;
		case 'Z': /* ESC Z */
			termVt100.vt100_DECID();
		break;

		/* same as VT52 mode */
		case '=': /* ESC = */
			termVt100.vt52_DECKPAM();
		break;
		case '>': /* ESC > */
			termVt100.vt52_DECKPNM();
		break;
		case '1': /* ESC 1 */
			/* TODO: Graphics processor on */
		break;
		case '2': /* ESC 2 */
			/* TODO: Graphics processor off */
		break;
		}
	} else { /* VT52 mode */
		switch (data) {
		case '=': /* ESC = */
			termVt100.vt52_DECKPAM();
		break;
		case '>': /* ESC > */
			termVt100.vt52_DECKPNM();
		break;
		case '<': /* ESC < */
			termVt100.vt52_enter_ansi_mode();
		break;
		case 'A': /* ESC A */
			termVt100.vt100_CUU();
		break;
		case 'B': /* ESC B */
			termVt100.vt100_CUD();
		break;
		case 'C': /* ESC C */
			termVt100.vt100_CUF();
		break;
		case 'D': /* ESC D */
			termVt100.vt100_CUB();
		break;
		case 'F': /* ESC F */
			termVt100.vt52_enter_graphics_mode();
		break;
		case 'G': /* ESC G */
			termVt100.vt52_exit_graphics_mode();
		break;
		case 'H': /* ESC H */
			/* No arguments */
			termVt100.vt100_CUP();
		break;
		case 'I': /* ESC I */
			termVt100.vt100_RI();
		break;
		case 'J': /* ESC J */
			/* No arguments */
			termVt100.vt100_ED();
		break;
		case 'K': /* ESC K */
			/* No arguments */
			termVt100.vt100_EL();
		break;
		case 'Y': /* ESC Y <line> <column> */
			termVt100.set_vt100_state_feed(&vt52_state_feed_cda);
		return ;
		case 'Z': /* ESC Z */
			termVt100.vt52_ident();
		break;
		case '1': /* ESC 1 */
			/* TODO: Graphics processor on */
		break;
		case '2': /* ESC 2 */
			/* TODO: Graphics processor off */
		break;
		}
	}

	termVt100.vt100_state_feed(NULL);
}

void vt100_state_feed_opensmallbracket(uint8_t data) /* ESC ( */
{
	switch (data) {
	case 'A': /* ESC ( A */
		termVt100.vt100_char_set(true, VT100_CHAR_UK);
	break;
	case 'B': /* ESC ( B */
		termVt100.vt100_char_set(true, VT100_CHAR_ASCII);
	break;
	case '0': /* ESC ( 0 */
		termVt100.vt100_char_set(true, VT100_CHAR_SPECIAL_GRAPHICS);
	break;
	case '1': /* ESC ( 1 */
		termVt100.vt100_char_set(true, VT100_CHAR_ALT_ROM_STANDARD);
	break;
	case '2': /* ESC ( 2 */
		termVt100.vt100_char_set(true, VT100_CHAR_ALT_ROM_SPECIAL_GRAPHICS);
	break;
	}
}

void vt100_state_feed_closesmallbracket(uint8_t data) /* ESC ) */
{
	switch (data) {
	case 'A': /* ESC ) A */
		/* TODO: United Kingdom (UK) */
		termVt100.vt100_char_set(false, VT100_CHAR_UK);
	break;
	case 'B': /* ESC ) B */
		termVt100.vt100_char_set(false, VT100_CHAR_ASCII);
	break;
	case '0': /* ESC ) 0 */
		termVt100.vt100_char_set(false, VT100_CHAR_SPECIAL_GRAPHICS);
	break;
	case '1': /* ESC ) 1 */
		termVt100.vt100_char_set(false, VT100_CHAR_ALT_ROM_STANDARD);
	break;
	case '2': /* ESC ) 2 */
		termVt100.vt100_char_set(false, VT100_CHAR_ALT_ROM_SPECIAL_GRAPHICS);
	break;
	}
}

void vt100_state_feed_opensquarebracket(uint8_t data) /* ESC [ */
{
	switch (data) {
	case '?': /* ESC [ ? */
		termVt100.set_vt100_state_feed(vt100_state_feed_question);
	return;
	case 'D': /* ESC [ Pn D */
		termVt100.vt100_CUB();
	break;
	case 'B': /* ESC [ Pn B */
		termVt100.vt100_CUD();
	break;
	case 'C': /* ESC [ Pn C */
		termVt100.vt100_CUF();
	break;
	case 'A': /* ESC [ Pn A */
		termVt100.vt100_CUU();
	break;
	case 'H': /* ESC [ Pn ; Pn H */
	case 'f': /* ESC [ Pn ; Pn f */
		termVt100.vt100_CUP();
	break;
	case 'c': /* ESC [ Pn c */
		termVt100.vt100_DECID();
	break;
	case 'q': /* ESC [ Ps q */
		termVt100.vt100_DECLL();
	break;
	case 'r': /* ESC [ Pn; Pn r */
		termVt100.vt100_DECSTBM();
	break;
	case 'y': /* ESC [ 2 ; Ps y */
		termVt100.vt100_DECTST();
	break;
	case 'J': /* ESC [ Ps J */
		termVt100.vt100_ED();
	break;
	case 'K': /* ESC [ Ps K */
		termVt100.vt100_EL();
	break;
	case 'm': /* ESC [ Ps ; . . . ; Ps m */
		termVt100.vt100_SGR();
	break;
	case 'n': /* ESC [ Ps n */
		termVt100.vt100_DSR();
	break;
	case 'x': /* ESC [ <sol> x */
		termVt100.vt100_DECREPTPARAM();
	break;
	case 'g': /* ESC [ Ps g */
		termVt100.vt100_TBC();
	break;
	case 'h': /* ESC [ Ps ; Ps ; . . . ; Ps h */
		termVt100.ansi_SM();
	break;
	case 'l': /* ESC [ Ps ; Ps ; . . . ; Ps l */
		termVt100.ansi_RM();
	break;
	default:
		termVt100.param_add(data);
	return;
	}

	termVt100.vt100_state_feed(NULL);
}

void vt100_state_feed_hash(uint8_t data) /* ESC # */
{
	switch (data) {
	case '3': /* ESC # 3 */
		termVt100.vt100_DECDHL(true);
	break;
	case '4': /* ESC # 4 */
		termVt100.vt100_DECDHL(false);
	break;
	case '5': /* ESC # 5 */
		termVt100.vt100_DECSWL();
	break;
	case '6': /* ESC # 6 */
		termVt100.vt100_DECDWL();
	break;
	case '8': /* ESC # 8 */
		termVt100.vt100_DECALN();
	break;
	}

	termVt100.vt100_state_feed(NULL);
}

void vt100_state_feed_question(uint8_t data) /* ESC [ ? */
{
	switch (data) {
	case 'l': /* ESC [ ? Ps;Ps;...Ps l */
		termVt100.vt100_RM();
	break;
	case 'h': /* ESC [ ? Ps;Ps;...Ps h */
		termVt100.vt100_SM();
	break;
	default:
		termVt100.param_add(data);
	return;
	}

	termVt100.vt100_state_feed(NULL);
}

/**
 * Input to VT100 state
 * When a new character is received, the machine goes into new state
 */
void Vt100::vt100_state(uint8_t data)
{
	if (data < 0x20) { /* C0 control characters */
		switch (data) {
		case ASCII_ESC:
			set_vt100_state_feed(vt100_state_feed_C1);
			vt100.param.count = 0;
		break;
		case ASCII_ETX:
			uart_send(ASCII_ACK);
		break;
		case ASCII_ENQ:
			uart_send_answerback_string();
		break;
		case ASCII_BEL:
			bell_code_recv();
		break;
		case ASCII_HT:
			vt100_HT();
		break;
		case ASCII_BS:
			vt100_BS();
		break;
		case ASCII_VT: /* Interpreted as LF. */
		case ASCII_FF: /* Interpreted as LF. */
		case ASCII_LF:
			vt100_LF();
		break;
		case ASCII_CR:
			vt100_CR();
		break;
		case ASCII_XON:
			vt100_XON();
		break;
		case ASCII_XOFF:
			vt100_XOFF();
		break;
		case ASCII_SUB: /* Interpreted as CAN. */
		case ASCII_CAN:
			/* If sent during a control sequence,
			 * the sequence is immediately terminated and not executed.
			 * It also causes the error character to be displayed. */
			set_vt100_state_feed(NULL);
			vt100_putch(176); /* will this do? */
		break;
		case ASCII_SI:
			vt100_shift_out(false);
		break;
		case ASCII_SO:
			vt100_shift_out(true);
		break;
		}
	} else if (data == ASCII_DEL) {
		/* Ignore */
	} else if (vt100_state_feed != NULL) {
		vt100_state_feed(data);
	} else {
		vt100_putch(data);
	}
}

void Vt100::vt100_putch(uint8_t data){
  vga.print(data);
}
void Vt100::vt100_BS(void){
  Serial.println("Vt100::vt100_CUP -Not implemented");
}
void Vt100::vt100_CUP(){
  Serial.println("Vt100::vt100_CUP -Not implemented");
}
void Vt100::vt100_CUU(){
  Serial.println("Vt100::vt100_CUU -Not implemented");
}
void Vt100::vt100_CUD(){
  Serial.println("Vt100::vt100_CUD -Not implemented");
}
void Vt100::vt100_CUF(){
  Serial.println("Vt100::vt100_CUF -Not implemented");
}
void Vt100::vt100_CUB(){
  Serial.println("Vt100::vt100_CUB -Not implemented");
}
void Vt100::vt100_RI(void){
  Serial.println("Vt100::vt100_RI -Not implemented");
}
void Vt100::vt100_RIS(void){
  Serial.println("Vt100::vt100_RIS -Not implemented");
}

void Vt100::vt100_NEL(void){
  Serial.println("Vt100::vt100_NEL -Not implemented");
}

void Vt100::vt100_HTS(void){
  Serial.println("Vt100::vt100_HTS -Not implemented");
}

void Vt100::vt100_DECID(void){
  Serial.println("Vt100::vt100_DECID -Not implemented");
}

void Vt100::vt52_DECKPNM(void){
  Serial.println("vt52_DECKPAM -Not implemented");
}
void Vt100::vt52_DECKPAM(){
  Serial.println("vt52_DECKPAM -Not implemented");
}
void Vt100::vt52_enter_ansi_mode(){
  Serial.println("vt52_enter_ansi_mode -Not implemented");
}
void Vt100::vt52_enter_graphics_mode(){
  Serial.println("vt52_enter_graphics_mode -Not implemented");
}
void Vt100::vt52_exit_graphics_mode(){
  Serial.println("vt52_exit_graphics_mode -Not implemented");
}
void Vt100::vt100_ED(){
  Serial.println("Vt100::vt100_ED -Not implemented");
}
void Vt100::vt52_ident(){
  Serial.println("vt52_ident -Not implemented");
}
void Vt100::vt100_char_set(bool b, uint8_t char_uk){
  Serial.println("Vt100::vt100_char_set -Not implemented");
}
void Vt100::vt100_EL(void){
  Serial.println("Vt100::vt100_EL -Not implemented");
}

void Vt100::vt100_DECLL(){
  Serial.println("Vt100::vt100_DECLL -Not implemented");
}
void Vt100::vt100_DECSTBM(){
  Serial.println("Vt100::vt100_DECSTBM -Not implemented");
}
void Vt100::vt100_DECTST(){
  Serial.println("Vt100::vt100_DECTST -Not implemented");
}
void Vt100::vt100_SGR(){
  Serial.println("Vt100::vt100_SGR -Not implemented");
}


void Vt100::vt100_RM(){
  Serial.println("Vt100::vt100_RM -Not implemented");
}
void Vt100::vt100_SM(){
  Serial.println("Vt100::vt100_SM -Not implemented");
}
void uart_send(){
  Serial.println("uart_send -Not implemented");
}
void uart_send_answerback_string(){
  Serial.println("uart_send_answerback_string -Not implemented");
}
void Vt100::vt100_HT(){
  Serial.println("Vt100::vt100_HT -Not implemented");
}
void Vt100::vt100_LF(){
  Serial.println("Vt100::vt100_LF -Not implemented");
}
void Vt100::vt100_CR(){
  Serial.println("Vt100::vt100_CR -Not implemented");
}
void Vt100::vt100_XON(){
  Serial.println("Vt100::vt100_XON -Not implemented");
}
void Vt100::vt100_XOFF(){
  Serial.println("Vt100::vt100_XOFF -Not implemented");
}
void Vt100::vt100_shift_out(bool b){
  Serial.println("Vt100::vt100_shift_out -Not implemented");
}
void Vt100::vt100_DECDHL(bool b){
  Serial.println("Vt100::vt100_DECDHL -Not implemented");
}
void Vt100::vt100_DECSWL(){
  Serial.println("Vt100::vt100_DECSWL -Not implemented");
}
void Vt100::vt100_DECDWL(){
  Serial.println("Vt100::vt100_DECDWL -Not implemented");
}
void Vt100::vt100_DECALN(){
  Serial.println("Vt100::vt100_DECALN -Not implemented");
}
void Vt100::vt100_DECSC(void){
  Serial.println("Vt100::vt100_DECSC -Not implemented");
}
void Vt100::vt100_DECRC(void){
  Serial.println("Vt100::vt100_DECRC -Not implemented");
}
void Vt100::vt100_DSR(){
  Serial.println("Vt100::vt100_DSR -Not implemented");
}
void Vt100::vt100_DECREPTPARAM(){
  Serial.println("Vt100::vt100_DECREPTPARAM -Not implemented");
}
void Vt100::vt100_TBC(){
  Serial.println("Vt100::vt100_TBC -Not implemented");
}
void Vt100::ansi_SM(){
  Serial.println("Vt100::ansi_SM -Not implemented");
}
void Vt100::ansi_RM(){
  Serial.println("Vt100::ansi_RM -Not implemented");
} 
void Vt100::vt100_IND(void){
  Serial.println("Vt100::vt100_IND -Not implemented");
} 
void Vt100::uart_send(uint8_t p){
  Serial.println("Vt100::uart_send -Not implemented");
}
void Vt100::uart_send_answerback_string(){
  Serial.println("Vt100::uart_send_answerback_string -Not implemented");
}
void Vt100::bell_code_recv(){
  Serial.println("Vt100::bell_code_recv -Not implemented");
}
