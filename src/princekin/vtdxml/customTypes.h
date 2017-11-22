/* 
 * Copyright (C) 2002-2012 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#pragma once
#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H
#include <wchar.h>
#include <string.h>
#include "vtdException.h"
//#include <cstdint>

using namespace std;
#ifndef isNaN
#define isNaN(x) ((x) != (x))
#endif

/*#ifndef min
#define min(a,b)  (((a) > (b)) ? (b) : (a)) 
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif*/

namespace com_ximpleware {
	typedef wchar_t UCSChar;
	typedef long long Long;
	typedef char Byte;
	typedef unsigned char UByte;

	typedef enum {ROOT,
		PARENT,
		FIRST_CHILD,
		LAST_CHILD,
		NEXT_SIBLING,
		PREV_SIBLING} 
	navDir;

	typedef enum {FORMAT_ASCII,
		FORMAT_ISO_8859_1,
		FORMAT_UTF8,
		FORMAT_ISO_8859_2,
		FORMAT_ISO_8859_3,
		FORMAT_ISO_8859_4,
		FORMAT_ISO_8859_5,
		FORMAT_ISO_8859_6,
		FORMAT_ISO_8859_7,
		FORMAT_ISO_8859_8,
		FORMAT_ISO_8859_9,
		FORMAT_ISO_8859_10,
		FORMAT_ISO_8859_11,
		FORMAT_ISO_8859_12,
		FORMAT_ISO_8859_13,
		FORMAT_ISO_8859_14,
		FORMAT_ISO_8859_15,
		FORMAT_ISO_8859_16,
		FORMAT_WIN_1250,
		FORMAT_WIN_1251,
		FORMAT_WIN_1252,
		FORMAT_WIN_1253,
		FORMAT_WIN_1254,
		FORMAT_WIN_1255,
		FORMAT_WIN_1256,
		FORMAT_WIN_1257,
		FORMAT_WIN_1258,
		FORMAT_UTF_16BE = 63,
		FORMAT_UTF_16LE =64} 
	encoding_t;


	typedef enum {TOKEN_STARTING_TAG,
		TOKEN_ENDING_TAG,
		TOKEN_ATTR_NAME,
		TOKEN_ATTR_NS,
		TOKEN_ATTR_VAL,
		TOKEN_CHARACTER_DATA,
		TOKEN_COMMENT,
		TOKEN_PI_NAME,
		TOKEN_PI_VAL,
		TOKEN_DEC_ATTR_NAME,
		TOKEN_DEC_ATTR_VAL,
		TOKEN_CDATA_VAL,
		TOKEN_DTD_VAL,
		TOKEN_DOCUMENT}
	tokenType;


	const float MAXFLOAT = 3.402823466e+38F;
	const float MINFLOAT = 1.175494351e-38F;

	const int MAXINT = 0x7fffffff;
	const int MININT = 0x80000001;

	const Long MAXLONG = 0x7fffffffffffffffLL;
	const Long MINLONG = 0x8000000000000001LL;
};

#endif
