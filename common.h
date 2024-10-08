/*
 * This file is part of diy-VT100.
 * Copyright (C) 2016, 2017 Kuldeep Singh Dhaka <kuldeep@madresistor.com>
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

#ifndef DIY_VT100_COMMON_H
#define DIY_VT100_COMMON_H

/* __BEGIN_DECLS should be used at the beginning of your declarations,
   so that C++ compilers don't mangle their names.  Use __END_DECLS at
   the end of C declarations. */
#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

/* ASCII code */
#define ASCII_NULL		0x00 /* null(\0)					CTRL @ */
#define ASCII_SOH		0x01 /* Start of header				CTRL A */
#define ASCII_SOX		0x02 /* Start of text				CTRL B */
#define ASCII_ETX		0x03 /* End of text					CTRL C */
#define ASCII_EOT		0x04 /* End of transmission			CTRL D */
#define ASCII_ENQ		0x05 /* Enquiry						CTRL E */
#define ASCII_ACK		0x06 /* Acknowledge					CTRL F */
#define ASCII_BEL		0x07 /* Bell(\a)					CTRL G */
#define ASCII_BS		0x08 /* Backspace(\b)				CTRL H */
#define ASCII_HT		0x09 /* Horizontal tabulation(\t)	CTRL I */
#define ASCII_LF		0x0A /* Line feed(\n)				CTRL J */
#define ASCII_VT		0x0B /* vertical tabulation			CTRL K */
#define ASCII_FF		0x0C /* Form feed(\f)				CTRL L */
#define ASCII_CR		0x0D /* Carrage return(\r)			CTRL M */
#define ASCII_SO		0x0E /* Shift out					CTRL N */
#define ASCII_SI		0x0F /* Shift in					CTRL O */
#define ASCII_DLE		0x10 /* Data link escape			CTRL P */
#define ASCII_DC1		0x11 /* Device Control 1 (XON)		CTRL Q */
#define ASCII_DC2		0x12 /* Device Control 2			CTRL R */
#define ASCII_DC3		0x13 /* Device Control 3 (XOFF)		CTRL S */
#define ASCII_DC4		0x14 /* Device Control 4			CTRL T */
#define ASCII_NAK		0x15 /* Negative acknowledge		CTRL U */
#define ASCII_SYN		0x16 /* Negative acknowledge		CTRL V */
#define ASCII_ETB		0x17 /* End of Transmission Block	CTRL W */
#define ASCII_CAN		0x18 /* Cancel						CTRL X */
#define ASCII_EM		0x19 /* End of Transmission Block	CTRL Y */
#define ASCII_SUB		0x1A /* Substitute					CTRL Z */
#define ASCII_ESC		0x1B /* Escape						CTRL [ */
#define ASCII_FS		0x1C /* File Separator				CTRL \ */
#define ASCII_GS		0x1D /* File Separator				CTRL ] */
#define ASCII_RS		0x1E /* Record Separator			CTRL ^ */
#define ASCII_US		0x1F /* Unit Separator				CTRL _*/

/* extra(sometime useful) */
#define ASCII_XON		ASCII_DC1
#define ASCII_XOFF		ASCII_DC3
#define ASCII_ESCAPE	ASCII_ESC
#define ASCII_BELL		ASCII_BEL
#define ASCII_TAB		ASCII_HT

#define ASCII_SPACE		0x20
#define ASCII_DEL		127 /* Delete CTRL ? */

#define EXTASCII_BOX_TOPLEFT 0xDA
#define EXTASCII_BOX_TOPRIGHT 0xBF
#define EXTASCII_BOX_BOTLEFT 0xC0
#define EXTASCII_BOX_BOTRIGHT 0xD9
#define EXTASCII_BOX_HORZ 0xC4
#define EXTASCII_BOX_VERT 0xB3

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

/* We have alot of memory free, why not reuse it!
 * RAM provide fast access. */
#define PLACE_IN_DTCM_DATA __attribute__((section(".data_dtcm")))
#define PLACE_IN_DTCM_BSS __attribute__((section(".bss_dtcm")))


__END_DECLS

#endif
