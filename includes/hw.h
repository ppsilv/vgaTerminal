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

#ifndef DIY_VT100_HW_H
#define DIY_VT100_HW_H

#include "common.h"
//#include <unicore-mx/stm32/gpio.h>

__BEGIN_DECLS

typedef uint8_t vt100_row;
typedef uint8_t vt100_col;

#define SCREEN_ROW 48
#define SCREEN_COL 80
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define SCREEN_CHAR_HEIGHT 10
#define SCREEN_CHAR_WIDTH 10

#define SCREEN_BRIGHTNESS_MAX 100
#define SCREEN_BRIGHTNESS_MIN 5
#define SCREEN_BRIGHTNESS_DELTA 5

#define UART_SPEED_DEFAULT 9600
#define SCREEN_BRIGHTNESS_DEFAULT 60

/* TODO: parity enable and parity type set */

#define SHOW_SPLASH 1 /* dev */

#if !defined(SHOW_SPLASH)
# define SHOW_SPLASH 1
#endif

#define AVO_INSTALLED 1

__END_DECLS

#endif
