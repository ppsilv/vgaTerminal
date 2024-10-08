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

#ifndef __VT100__H__
#define __VT100__H__

#include <Arduino.h>
#include <ESP32Video.h>
#include "includes/hw.h"
#include "includes/debug.h"
#include "common.h"
#include "Vt100structs.h"

//int (TMyClass::*pt2ConstMember)(float, char, char) const = NULL;
static void ansi_aux_keypad_and_cursor_code(uint8_t data);
static void vt100_state_feed_C1(uint8_t data);
static void vt100_state_feed_question(uint8_t data) ;
static void vt100_state_feed_hash(uint8_t data) ;
static void vt100_state_feed_opensquarebracket(uint8_t data);
static void vt100_state_feed_opensmallbracket(uint8_t data);
static void vt100_state_feed_closesmallbracket(uint8_t data);
static void ansi_aux_keypad_and_cursor_code(uint8_t data);
static void vt52_state_feed_cda(uint8_t data);

extern VGA1BitI vga;

//typedef void (Vt100::*vt100_state_feed_p)(uint8_t) ;

class Vt100 {
public:
        Vt100();
        ~Vt100();
        
        void vt100_state(uint8_t data);
        //typedef void (*vt100_state_feed_p)(uint8_t) ;
        //vt100_state_feed_p vt100_state_feed;    

        void set_vt100_state_feed(void(*func_ptr)(uint8_t))
        {
            vt100_state_feed = func_ptr;
        }


      void(*vt100_state_feed)(uint8_t);

      
      /**
      * Place paramter in the array
      * This is used by VT100 state machine to store paramter.
      * It assume that @a data is raw data.
      * @param data Raw data
      */
      void param_add(uint8_t data);
      /*
      * cda: Cursor Direct Address (VT52)
      * This is a hack to the parser.
      * Does:
      * 1. Line value
      * 2. Column value
      * 3. Pass to CUP
      * 4. Goto C0 state
      */
      //void vt52_state_feed_cda(uint8_t data);
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
      //void ansi_aux_keypad_and_cursor_code(uint8_t data);
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
      //void vt100_state_feed_C1(uint8_t data);
      //void vt100_state_feed_question(uint8_t data) ;
      //void vt100_state_feed_hash(uint8_t data) ;
      //void vt100_state_feed_opensquarebracket(uint8_t data);
      //void vt100_state_feed_opensmallbracket(uint8_t data);
      //void vt100_state_feed_closesmallbracket(uint8_t data);

      void vt100_putch(const char  data);
      void vt100_BS(void);
      void vt100_CUP(void);
      void vt100_CUD(void);
      void vt100_CUU(void);
      void vt100_CUF(void);
      void vt100_CUB(void);
      void vt100_DECRC(void);
      void vt100_DECSC(void);
      void vt100_RI(void);
      void vt100_RIS(void);
      void vt100_IND(void);
      void vt100_NEL(void);
      void vt100_HTS(void);
      void vt100_DECID(void);
      void vt52_DECKPNM(void);
      void vt52_DECKPAM(void);
      void vt52_enter_ansi_mode(void);
      void vt52_enter_graphics_mode(void);
      void vt52_exit_graphics_mode(void);
      void vt100_ED(void);
      void vt52_ident(void);
      void vt100_char_set(bool b, uint8_t char_uk);
      void vt100_EL(void);
      void vt100_DECLL();
      void vt100_DECSTBM();
      void vt100_DECTST();
      void vt100_SGR();
      void vt100_DSR();
      void vt100_DECREPTPARAM();
      void vt100_TBC();
      void ansi_SM();
      void ansi_RM();  
      void vt100_DECDHL(bool b);
      void vt100_DECSWL();
      void vt100_DECDWL();  
      void vt100_DECALN();  
      void vt100_RM();
      void vt100_SM();
      void uart_send(uint8_t p);
      void uart_send_answerback_string();
      void bell_code_recv();
      void vt100_HT();
      void vt100_LF();
      void vt100_CR();
      void vt100_XON();
      void vt100_XOFF();
      void vt100_shift_out(bool b);

};

extern Vt100 termVt100;

#endif
