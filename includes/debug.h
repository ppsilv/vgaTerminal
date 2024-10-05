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

#ifndef DIY_VT100_DEBUG_H
#define DIY_VT100_DEBUG_H

#include "common.h"

__BEGIN_DECLS

#include "config.h"

#if (DEBUG_BUILD == 1)

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>

void debug_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void debug_vprintf(const char *fmt, va_list va);
void debug_puts(const char *arg);

#define NEW_LINE "\n"

#define LOG(str) debug_puts(str)
#define LOGF(fmt, ...) debug_printf(fmt, ##__VA_ARGS__);

#define LOG_LN(str)		\
	debug_puts(str);	\
	debug_puts(NEW_LINE)

#define LOGF_LN(fmt, ...)				\
	debug_printf(fmt, ##__VA_ARGS__);	\
	debug_puts(NEW_LINE)

#else

#define LOG(str)
#define LOGF(fmt, ...)
#define LOG_LN(str)
#define LOGF_LN(fmt, ...)

#endif

__END_DECLS

#endif
