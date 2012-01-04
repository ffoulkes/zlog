/*
 * This file is part of the Xlog Library.
 *
 * Copyright (C) 2011 by Hardy Simpson <HardySimpson1984@gmail.com>
 *
 * The Xlog Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Xlog Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the Xlog Library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __zc_error_h
#define __zc_error_h

#include <stdarg.h>

#define zc_debug(fmt, args...) \
	zc_debug_inner(__FILE__, __LINE__, fmt, ## args)

#define zc_error(fmt, args...) \
	zc_error_inner(__FILE__, __LINE__, fmt, ## args)

#define zc_assert(expr,rv) \
	if(!(expr)) { \
		zc_error("expr is null"); \
		return rv; \
	}

extern int zc_debug_inner(const char *file, const long line, const char *fmt, ...);
extern int zc_error_inner(const char *file, const long line, const char *fmt, ...);

#endif