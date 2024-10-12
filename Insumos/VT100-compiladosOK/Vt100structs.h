/*
 * This file is part of diy-VT100.
 * Copyright (C) 2016 Kuldeep Singh Dhaka <kuldeep@madresistor.com>
 *
 * diy-VT100 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * diy-VT100 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with diy-VT100.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VT100_STRUCTS_H__
#define __VT100_STRUCTS_H__

#include "includes/hw.h"
#include "includes/debug.h"
#include "common.h"

#include "config.h"

#define ANSWERBACK_SIZE 20

/* The number of bytes required to store when
 *  1 bit information for each column is required.
 *
 * Used
 *  - TAB stop
 *  - data_prop (AVO_INSTALLED == 0)
 */
#define BYTES_FOR_1BIT_COLUMN ((SCREEN_COL + 7) / 8)

struct _vt100_cursor {
	vt100_row row;
	vt100_col col;
};

enum _vt100_char_set {
	VT100_CHAR_ASCII,
	VT100_CHAR_UK,
	VT100_CHAR_SPECIAL_GRAPHICS,
	VT100_CHAR_ALT_ROM_STANDARD,
	VT100_CHAR_ALT_ROM_SPECIAL_GRAPHICS
};

#if (AVO_INSTALLED == 1)

struct _vt100_data_prop {
	uint8_t invert:1;
	uint8_t underscore:1; /* aka underline */
	uint8_t bold:1;
	uint8_t blink:1;
	enum _vt100_char_set char_set:3;
};

struct _vt100_screen_char {
	uint8_t data;
	struct _vt100_data_prop prop;
};

#else

struct _vt100_screen_char {
	uint8_t data:7;
	uint8_t prop:1;
};

#endif

struct _vt100_row_prop {
	uint8_t double_width:1;

	uint8_t double_height:1;
	uint8_t double_height_top:1; /* top:1 | bot:0 */

	/* lcd driver use (if mantaining buffer) */
	uint8_t redraw:1;
};

#define PARAM_QUEUE_SIZE 32

struct _vt100_param {
	uint8_t count;
	uint8_t data[PARAM_QUEUE_SIZE];
};

struct _vt100 {
	/* DEC PRIV -- VR */

	/**
	 * Set = The auxiliary keypad keys will transmit control sequences
	 * Reset = The auxiliary keypad keys will send ASCII codes
	 *         corresponding to the characters engraved on the keys.
	 */
	uint8_t DECKPAM:1;

	/**
	 * DECCKM -- Cursor Keys Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. This mode is only effective
	 * when the terminal is in keypad application mode (see DECKPAM)
	 * and the ANSI/VT52 mode (DECANM) is set (see DECANM). Under these
	 * conditions, if the cursor key mode is reset, the four cursor
	 * function keys will send ANSI cursor control commands.
	 * If cursor key mode is set, the four cursor function keys will
	 * send application functions.
	 */
	uint8_t DECCKM:1;

	/**
	 * Grahics ON
	 */
	uint8_t DECGON:1;

	/**
	 * DECOM -- Origin Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes the
	 * origin to be at the upper-left character position on the screen.
	 * Line and column numbers are, therefore, independent of current
	 * margin settings. The cursor may be positioned outside the
	 * margins with a cursor position (CUP) or horizontal and vertical
	 * position (HVP) control.
	 *
	 * The set state causes the origin to be at the upper-left character
	 * position within the margins. Line and column numbers are
	 * therefore relative to the current margin settings. The cursor is
	 * not allowed to be positioned outside the margins.
	 *
	 * The cursor is moved to the new home position when this mode is set or reset.
	 * Lines and columns are numbered consecutively, with the origin being line 1, column 1.
	 */
	uint8_t DECOM:1;

	/* VT100 PRIVATE -- VR */

	/**
	 * Terminal can send unsolicited reports
	 */
	uint8_t UNSOLIC:1;

	/**
	 * Set = Place the terminal in LOCAL mode
	 * Reset = Place the terminal in ON-LINE mode
	 */
	uint8_t LOCAL:1;

	/**
	 * Set = Show SET-UP B
	 * Reset = Show SET-UP A
	 */
	uint8_t SETUP_TYPE:1;

	/**
	 * Set = Show SET-UP
	 * Reset = Show Data
	 */
	uint8_t SETUP_SHOW:1;

	/* KEYBOARD -- VR */
	uint8_t KBD_SHIFT:1;
	uint8_t KBD_CTRL:1;
	uint8_t KBD_CAPS:1;

	/* HW -- PRIVATE -- NA */
	uint8_t HW_PRIV0:1;
	uint8_t HW_PRIV1:1;
	uint8_t HW_PRIV2:1;
	uint8_t HW_PRIV3:1;

	/* XON / XOFF */
	/**
	 * Set = XOFF send to HOST
	 * Reset = XON send to HOST or XOFF not send to HOST
	 */
	uint8_t XOFF_SEND:1;

	/**
	 * Set = XOFF send by user (scroll lock)
	 * Reset = XOFF not send by user or XON send by user (scroll lock)
	 */
	uint8_t XOFF_SCROLL:1;

	/**
	 * Set = XOFF send by HOST
	 * Reset = XOFF not send by host (or XON send by host)
	 */
	uint8_t XOFFED:1;

	/* DEC PRIV -- NVR */

	/**
	 * DECANM -- ANSI/VT52 Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes only
	 * VT52 compatible escape sequences to be interpreted and executed.
	 * The set state causes only ANSI "compatible" escape and control
	 * sequences to be interpreted and executed.
	 */
	uint8_t DECANM:1;

	/**
	 * DECARM -- Auto Repeat Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and reset
	 * mode (RM) control sequences. The reset state causes no keyboard
	 * keys to auto-repeat. The set state causes certain keyboard keys
	 * to auto-repeat.
	 */
	uint8_t DECARM:1;

	/**
	 * Auto Send send XOFF and XON to host
	 */
	uint8_t AUTOX:1;

	/**
	 * Byte per character
	 * Set = 8 bits per character
	 * Reset = 7 bits per character
	 */
	uint8_t BPC:1;

	/**
	 * DECCOLM -- Column Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes a
	 * maximum of 80 columns on the screen. The set state causes a
	 * maximum of 132 columns on the screen.
	 */
	uint8_t DECCOLM:1;

	/**
	 * Set = Block cursor
	 * Reset = Underline cursor
	 */
	uint8_t CURSOR:1;

	/**
	 * DECINLM -- Interlace Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state (non-interlace)
	 * causes the video processor to display 240 scan lines per frame.
	 * The set state (interlace) causes the video processor to display
	 * 480 scan lines per frame.
	 * There is no increase in character resolution.
	 */
	uint8_t DECINLM:1;

	/**
	 * LNM -- Line Feed/New Line Mode
	 *
	 * This is a parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes the
	 * interpretation of the line feed (LF), defined in
	 * ANSI Standard X3.4-1977, to imply only vertical movement of the
	 * active position and causes the RETURN key (CR) to send the
	 * single code CR. The set state causes the LF to imply movement to
	 * the first position of the following line and causes the RETURN
	 * key to send the two codes (CR, LF). This is the New Line (NL) option.
	 *
	 * This mode does not affect the index (IND), or
	 * next line (NEL) format effectors.
	 */
	uint8_t LNM:1;

	/**
	 * Sound short bell when a key is clicked
	 *
	 * Note: SHIFT or CTRL keys do not generate any keyclick because
	 * these keys do not transmit any codes but only modify
	 * the codes transmitted by other keys.
	 */
	uint8_t KEY_CLICK:1;

	/**
	 * Sound long bell when cursor is 8 character from right margin
	 */
	uint8_t MARGIN_BELL:1;

	/**
	 * Set = Parity check ON
	 * Reset = Parity check OFF
	 */
	uint8_t PARITY:1;

	/**
	 * Set = ODD parity
	 * Reset = EVEN partity
	 */
	uint8_t PARITY_SENSE:1;

	/**
	 * DECSCNM -- Screen Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes the
	 * screen to be black with white characters. The set state causes
	 * the screen to be white with black characters.
	 */
	uint8_t DECSCNM:1;

	/**
	 * DECSCLM -- Scrolling Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes
	 * scrolls to "jump" instantaneously. The set state causes scrolls
	 * to be "smooth" at a maximum rate of six lines per second.
	 */
	uint8_t DECSCLM:1;

	/**
	 * DECAWM -- Autowrap Mode (DEC Private)
	 *
	 * This is a private parameter applicable to set mode (SM) and
	 * reset mode (RM) control sequences. The reset state causes any
	 * displayable characters received when the cursor is at the right
	 * margin to replace any previous characters there. The set state
	 * causes these characters to advance to the start of the next line,
	 * doing a scroll up if required and permitted.
	 */
	uint8_t DECAWM:1;

	/**
	 * Set = Key 3 has Euro sign
	 * Reset = Key 3 has '#' sign
	 */
	uint8_t SHIFTED:1;

	/**
	 * Set = 50Hz power
	 * Reset = 60Hz power
	 */
	uint8_t POWER:1;

	/* Misc */

	/**
	 * Keyboard locked
	 */
	uint8_t KBD_LOCKED:1;

	uint8_t brightness;
	uint8_t uart_rx;
	uint8_t uart_tx;

	uint8_t tab_stop[BYTES_FOR_1BIT_COLUMN];

	uint8_t answerback[ANSWERBACK_SIZE];

	vt100_row setup_col;

	struct _vt100_cursor cursor;

	/* Margin values send by host */
	struct {
		vt100_row top, bottom;
		vt100_col left, right;
	} margin;

	/**
	 * Character set Shift In, Shift Out
	 */
	uint8_t CHAR_SET_SO:1;
	enum _vt100_char_set G0:3, G1:3;

#if (AVO_INSTALLED)
	struct _vt100_data_prop data_prop;
#else
	enum _vt100_char_set char_set:3;
	uint8_t data_prop:1;
#endif

	struct _vt100_screen_char buffer[SCREEN_ROW][SCREEN_COL];

	vt100_row row_map;

	struct _vt100_row_prop row_prop[SCREEN_ROW];

	struct _vt100_param param;
};

extern struct _vt100 vt100;

/**
 * Convert virtual @a row to physical row that can be used on
 * vt100.buffer and vt100.row_prop
 */
#define ROW_MAP(row) (((row) + vt100.row_map) % SCREEN_ROW)
/*
void vt100_state(uint8_t data)
static void vt100_state_feed_question(uint8_t data) 
static void vt100_state_feed_hash(uint8_t data) 
static void vt100_state_feed_opensquarebracket(uint8_t data)
static void vt100_state_feed_closesmallbracket(uint8_t data)
static void vt100_state_feed_opensmallbracket(uint8_t data) 
static void vt100_state_feed_C1(uint8_t data)
static void ansi_aux_keypad_and_cursor_code(uint8_t data)
static void vt52_state_feed_cda(uint8_t data)
void (*vt100_state_feed)(uint8_t) = NULL;
*/

#endif
