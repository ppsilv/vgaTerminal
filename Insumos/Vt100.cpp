#include "Vt100.h"

PLACE_IN_DTCM_BSS
struct _vt100 vt100;

/**
 * The heart of the VT100.
 * This route the received character to the appropriate function.
 *
 * Basically, this is a state machine.
 * When a control character is received, it try to find a the next state of the
 *  based on the match.
 */

/* see http://www.real-world-systems.com/docs/ANSIcode.html */

static void vt100_state_feed_C1(uint8_t data);
static void vt100_state_feed_hash(uint8_t data);
static void vt100_state_feed_opensquarebracket(uint8_t data);
static void vt100_state_feed_opensmallbracket(uint8_t data);
static void vt100_state_feed_closesmallbracket(uint8_t data);
static void vt100_state_feed_question(uint8_t data);

PLACE_IN_DTCM_BSS
void (*vt100_state_feed)(uint8_t) = NULL;

/**
 * Place paramter in the array
 * This is used by VT100 state machine to store paramter.
 * It assume that @a data is raw data.
 * @param data Raw data
 */
void param_add(uint8_t data)
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

/*
 * cda: Cursor Direct Address (VT52)
 * This is a hack to the parser.
 * Does:
 * 1. Line value
 * 2. Column value
 * 3. Pass to CUP
 * 4. Goto C0 state
 */
void vt52_state_feed_cda(uint8_t data)
{
	vt100.param.data[vt100.param.count++] = data - ' ' + 1;
	if (vt100.param.count == 2) {
		vt100_CUP();
		vt100_state_feed = NULL;
	}
}

/* Cursor Control Keys - The keyboard also contains four keys labeled
 * with arrows in each of four directions. These keys transmit escape
 * sequences. If the host echoes these escape sequences back to the
 * terminal, the cursor will move one character up, down, right, or left.
 * Table 3-6 shows the escape sequences generated by each key.
 *
 *                Table 3-6 Cursor Control Key Codes
 *                ==================================
 *
 *                                  ANSI Mode and        ANSI Mode and
 * Cursor Key (Arrow)  VT52 Mode*   Cursor Key Mode      Cursor Key Mode
 *                                     Reset                Set
 *   Up                ESC A        ESC [ A              ESC O A
 *   Down              ESC B        ESC [ B              ESC O B
 *   Right             ESC C        ESC [ C              ESC O C
 *   Left              ESC D        ESC [ D              ESC O D
 *
 *
 *
 *
 * In ANSI mode, if the codes are echoed back to the VT100,
 * or if the terminal is in local mode, the last character
 * of the sequence will be displayed on the screen;
 * e.g., PF1 will display a "P".
 *
 * ESC O Pc
 *
 *          Table 3-8 ANSI Mode Auxiliary Keypad Codes
 *          ==========================================
 *
 * Key    Keypad Numeric Mode    Keypad Application Mode
 *  0           0                ESC O p
 *  1           1                ESC O q
 *  2           2                ESC O r
 *  3           3                ESC O s
 *  4           4                ESC O t
 *  5           5                ESC O u
 *  6           6                ESC O v
 *  7           7                ESC O w
 *  8           8                ESC O x
 *  9           9                ESC O y
 *  - (dash)    - (dash)         ESC O m
 *  , (comma)   , (comma)        ESC O l
 *  . (period)  . (period)       ESC O n
 * ENTER        Same as          ESC O M
 *               RETURN key
 * PF1          ESC O P          ESC O P
 * PF2          ESC O Q          ESC O Q
 * PF3          ESC O R          ESC O R
 * PF4          ESC O S          ESC O S
 *
 *
 *
 *        Table 3-7 VT52 Mode Auxiliary Keypad Codes
 *        ==========================================
 *
 * Key    Keypad Numeric Mode    Keypad Application Mode
 *  0           0                ESC ? p
 *  1           1                ESC ? q
 *  2           2                ESC ? r
 *  3           3                ESC ? s
 *  4           4                ESC ? t
 *  5           5                ESC ? u
 *  6           6                ESC ? v
 *  7           7                ESC ? w
 *  8           8                ESC ? x
 *  9           9                ESC ? y
 *  - (dash)    - (dash)         ESC ? m
 *  , (comma)   , (comma)        ESC ? l
 *  . (period)  . (period)       ESC ? n
 *  ENTER       Same as          ESC ? M
 *                RETURN key
 *  PF1         ESC P            ESC P
 *  PF2         ESC Q            ESC Q
 *  PF3         ESC R            ESC R
 *  PF4         ESC S            ESC S
 */
void ansi_aux_keypad_and_cursor_code(uint8_t data) /* ESC O */
{
	switch (data) {
	case 'A': /* ESC O A */
		vt100_CUU();
	break;
	case 'B': /* ESC O B */
		vt100_CUD();
	break;
	case 'C': /* ESC O C */
		vt100_CUF();
	break;
	case 'D': /* ESC O D */
		vt100_CUB();
	break;
	default:
		vt100_putch(data);
	break;
	}

	vt100_state_feed = NULL;
}

void vt100_state_feed_C1(uint8_t data) /* ESC */
{
	if (vt100.DECANM) { /* ANSI mode */
		switch (data) {
		case '[': /* ESC [ */
			vt100_state_feed = vt100_state_feed_opensquarebracket;
		return;
		case '#': /* ESC # */
			vt100_state_feed = vt100_state_feed_hash;
		return;
		case '(': /* ESC ( */
			vt100_state_feed = vt100_state_feed_opensmallbracket;
		return;
		case ')': /* ESC ) */
			vt100_state_feed = vt100_state_feed_closesmallbracket;
		return;
		case '7': /* ESC 7 */
			vt100_DECSC();
		break;
		case '8': /* ESC 8 */
			vt100_DECRC();
		break;
		case 'c': /* ESC c */
			vt100_RIS();
		break;
		case 'D': /* ESC D */
			vt100_IND();
		break;
		case 'E': /* ESC E */
			vt100_NEL();
		break;
		case 'H': /* ESC H */
			vt100_HTS();
		break;
		case 'M': /* ESC M */
			vt100_RI();
		break;
		case 'O': /* ESC O */
			vt100_state_feed = ansi_aux_keypad_and_cursor_code;
		return;
		case 'Z': /* ESC Z */
			vt100_DECID();
		break;

		/* same as VT52 mode */
		case '=': /* ESC = */
			vt52_DECKPAM();
		break;
		case '>': /* ESC > */
			vt52_DECKPNM();
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
			vt52_DECKPAM();
		break;
		case '>': /* ESC > */
			vt52_DECKPNM();
		break;
		case '<': /* ESC < */
			vt52_enter_ansi_mode();
		break;
		case 'A': /* ESC A */
			vt100_CUU();
		break;
		case 'B': /* ESC B */
			vt100_CUD();
		break;
		case 'C': /* ESC C */
			vt100_CUF();
		break;
		case 'D': /* ESC D */
			vt100_CUB();
		break;
		case 'F': /* ESC F */
			vt52_enter_graphics_mode();
		break;
		case 'G': /* ESC G */
			vt52_exit_graphics_mode();
		break;
		case 'H': /* ESC H */
			/* No arguments */
			vt100_CUP();
		break;
		case 'I': /* ESC I */
			vt100_RI();
		break;
		case 'J': /* ESC J */
			/* No arguments */
			vt100_ED();
		break;
		case 'K': /* ESC K */
			/* No arguments */
			vt100_EL();
		break;
		case 'Y': /* ESC Y <line> <column> */
			vt100_state_feed = vt52_state_feed_cda;
		return;
		case 'Z': /* ESC Z */
			vt52_ident();
		break;
		case '1': /* ESC 1 */
			/* TODO: Graphics processor on */
		break;
		case '2': /* ESC 2 */
			/* TODO: Graphics processor off */
		break;
		}
	}

	vt100_state_feed = NULL;
}

/**
 * G0 Sets Sequence  G1 Sets Sequence  Meaning
 * ESC ( A           ESC ) A           United Kingdom Set
 * ESC ( B           ESC ) B           ASCII Set
 * ESC ( 0           ESC ) 0           Special Graphics
 * ESC ( 1           ESC ) 1           Alternate Character ROM
 *                                       Standard Character Set
 * ESC ( 2           ESC ) 2           Alternate Character ROM
 *                                       Special Graphics
 */

void vt100_state_feed_opensmallbracket(uint8_t data) /* ESC ( */
{
	switch (data) {
	case 'A': /* ESC ( A */
		vt100_char_set(true, VT100_CHAR_UK);
	break;
	case 'B': /* ESC ( B */
		vt100_char_set(true, VT100_CHAR_ASCII);
	break;
	case '0': /* ESC ( 0 */
		vt100_char_set(true, VT100_CHAR_SPECIAL_GRAPHICS);
	break;
	case '1': /* ESC ( 1 */
		vt100_char_set(true, VT100_CHAR_ALT_ROM_STANDARD);
	break;
	case '2': /* ESC ( 2 */
		vt100_char_set(true, VT100_CHAR_ALT_ROM_SPECIAL_GRAPHICS);
	break;
	}
}

void vt100_state_feed_closesmallbracket(uint8_t data) /* ESC ) */
{
	switch (data) {
	case 'A': /* ESC ) A */
		/* TODO: United Kingdom (UK) */
		vt100_char_set(false, VT100_CHAR_UK);
	break;
	case 'B': /* ESC ) B */
		vt100_char_set(false, VT100_CHAR_ASCII);
	break;
	case '0': /* ESC ) 0 */
		vt100_char_set(false, VT100_CHAR_SPECIAL_GRAPHICS);
	break;
	case '1': /* ESC ) 1 */
		vt100_char_set(false, VT100_CHAR_ALT_ROM_STANDARD);
	break;
	case '2': /* ESC ) 2 */
		vt100_char_set(false, VT100_CHAR_ALT_ROM_SPECIAL_GRAPHICS);
	break;
	}
}

void vt100_state_feed_opensquarebracket(uint8_t data) /* ESC [ */
{
	switch (data) {
	case '?': /* ESC [ ? */
		vt100_state_feed = vt100_state_feed_question;
	return;
	case 'D': /* ESC [ Pn D */
		vt100_CUB();
	break;
	case 'B': /* ESC [ Pn B */
		vt100_CUD();
	break;
	case 'C': /* ESC [ Pn C */
		vt100_CUF();
	break;
	case 'A': /* ESC [ Pn A */
		vt100_CUU();
	break;
	case 'H': /* ESC [ Pn ; Pn H */
	case 'f': /* ESC [ Pn ; Pn f */
		vt100_CUP();
	break;
	case 'c': /* ESC [ Pn c */
		vt100_DECID();
	break;
	case 'q': /* ESC [ Ps q */
		vt100_DECLL();
	break;
	case 'r': /* ESC [ Pn; Pn r */
		vt100_DECSTBM();
	break;
	case 'y': /* ESC [ 2 ; Ps y */
		vt100_DECTST();
	break;
	case 'J': /* ESC [ Ps J */
		vt100_ED();
	break;
	case 'K': /* ESC [ Ps K */
		vt100_EL();
	break;
	case 'm': /* ESC [ Ps ; . . . ; Ps m */
		vt100_SGR();
	break;
	case 'n': /* ESC [ Ps n */
		vt100_DSR();
	break;
	case 'x': /* ESC [ <sol> x */
		vt100_DECREPTPARAM();
	break;
	case 'g': /* ESC [ Ps g */
		vt100_TBC();
	break;
	case 'h': /* ESC [ Ps ; Ps ; . . . ; Ps h */
		ansi_SM();
	break;
	case 'l': /* ESC [ Ps ; Ps ; . . . ; Ps l */
		ansi_RM();
	break;
	default:
		param_add(data);
	return;
	}

	vt100_state_feed = NULL;
}

void vt100_state_feed_hash(uint8_t data) /* ESC # */
{
	switch (data) {
	case '3': /* ESC # 3 */
		vt100_DECDHL(true);
	break;
	case '4': /* ESC # 4 */
		vt100_DECDHL(false);
	break;
	case '5': /* ESC # 5 */
		vt100_DECSWL();
	break;
	case '6': /* ESC # 6 */
		vt100_DECDWL();
	break;
	case '8': /* ESC # 8 */
		vt100_DECALN();
	break;
	}

	vt100_state_feed = NULL;
}

void vt100_state_feed_question(uint8_t data) /* ESC [ ? */
{
	switch (data) {
	case 'l': /* ESC [ ? Ps;Ps;...Ps l */
		vt100_RM();
	break;
	case 'h': /* ESC [ ? Ps;Ps;...Ps h */
		vt100_SM();
	break;
	default:
		param_add(data);
	return;
	}

	vt100_state_feed = NULL;
}

/**
 * Input to VT100 state
 * When a new character is received, the machine goes into new state
 */
void vt100_state(uint8_t data)
{
	if (data < 0x20) { /* C0 control characters */
		switch (data) {
		case ASCII_ESC:
			vt100_state_feed = vt100_state_feed_C1;
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
			vt100_state_feed = NULL;
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

