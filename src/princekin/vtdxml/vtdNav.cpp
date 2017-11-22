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
#include "vtdNav.h"
#include "vtdGen.h"
#include "bookMark.h"
#include "elementFragmentNs.h"

//#include <exception>
using namespace com_ximpleware;
using namespace std;

//using namespace com_ximpleware_char;
//using namespace com_ximpleware;

//inline Long VTDNav::getChar(int offset){
//	Long temp = 0;
//	switch (encoding) {
//			case FORMAT_ASCII : 
//			case FORMAT_ISO_8859_1 :			
//				temp = XMLDoc[offset];
//				if (temp == '\r') {
//					if (XMLDoc[offset + 1] == '\n') {
//						return '\n'|(2LL<<32);
//					} else {
//						return '\n'|(1LL<<32);
//					}
//				}   
//				return temp|(1LL<<32);
//
//			case FORMAT_UTF8 :
//				temp = XMLDoc[offset];
//				if (temp<=127){
//					if (temp == '\r') {
//						if (XMLDoc[offset + 1] == '\n') {
//							return '\n'|(2LL<<32);
//						} else {
//							return '\n'|(1LL<<32);
//						}
//					}
//					return (temp|(1LL<<32));
//				}
//				return handle_utf8(temp,offset);
//
//			case FORMAT_UTF_16BE :
//				return handle_utf16be( offset);
//
//			case FORMAT_UTF_16LE :
//				return handle_utf16le(offset);
//				// implement UTF-16LE to UCS4 conversion
//				//System.out.println("UTF 16 LE unimplemented for now");
//			default :
//				return getChar4OtherEncoding(offset);
//	}
//}

//int VTDNav::getCharUnit( int offset){
//	return (encoding <=FORMAT_UTF8)
//		? XMLDoc[offset]:
//	(encoding <= FORMAT_WIN_1258)
//		? decode(offset)
//		: ((encoding == FORMAT_UTF_16BE)
//		? (XMLDoc[offset << 1]
//	<< 8 | XMLDoc[(offset << 1) + 1])
//		: (XMLDoc[(offset << 1) + 1]
//	<< 8 | XMLDoc[offset << 1]));
//}

//Get the starting offset of the token at the given index.
int VTDNav::getTokenOffset(int index) {
	return (int) ((vtdBuffer->longAt(index)) & offsetMask);
}
Long VTDNav::handle_utf8(Long temp, int offset) {
	int c, d, a, i;
	Long val;
	//temp = XMLDoc[currentOffset];
	switch (UTF8Char_byteCount((int) temp)) {

	case 2:
		c = 0x1f;
		d = 6;
		a = 1;
		break;
	case 3:
		c = 0x0f;
		d = 12;
		a = 2;
		break;
	case 4:
		c = 0x07;
		d = 18;
		a = 3;
		break;
	case 5:
		c = 0x03;
		d = 24;
		a = 4;
		break;
	case 6:
		c = 0x01;
		d = 30;
		a = 5;
		break;
	default: {
		throw NavException("navigation exception during getChar \n"
			"UTF 8 encoding error: should never happen");
	}
	}

	val = (temp & c) << d;
	i = a - 1;
	while (i >= 0) {
		temp = XMLDoc[offset + a - i];
		if ((temp & 0xc0) != 0x80) {
			throw NavException("navigation exception during getChar \n"
				"UTF 8 encoding error: should never happen");
		}
		val = val | ((temp & 0x3f) << ((i << 2) + (i << 1)));
		i--;
	}

	return val | ((Long)(a + 1) << 32);
}
Long VTDNav::handle_utf16le(int offset) {
	int val, temp = XMLDoc[(offset << 1) + 1] << 8 | XMLDoc[offset << 1];
	if (temp < 0xdc00 || temp > 0xdfff) {
		if (temp == '\r') {
			if (XMLDoc[(offset << 1) + 2] == '\n' && XMLDoc[(offset << 1) + 3]
					== 0) {
				return '\n' | (2LL << 32);
			} else {
				return '\n' | (1LL << 32);
			}
		}
		return temp | (1LL << 32);
	} else {
		val = temp;
		temp = (XMLDoc[(offset << 1) + 3] << 8) | XMLDoc[(offset << 1) + 2];
		if (temp < 0xd800 || temp > 0xdc00) {
			throw NavException("navigation exception during getChar \n"
				"UTF 16 LE encoding error: should never happen");
		}
		val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;

		return val | (2LL << 32);
	}
}
Long VTDNav::handle_utf16be(int offset) {
	int val, temp = XMLDoc[offset << 1] << 8 | XMLDoc[(offset << 1) + 1];
	if ((temp < 0xd800) || (temp >= 0xdc00)) { // not a high surrogate
		if (temp == '\r') {
			if (XMLDoc[(offset << 1) + 3] == '\n' && XMLDoc[(offset << 1) + 2]
					== 0) {
				return '\n' | (2LL << 32);
			} else {
				return '\n' | (1LL << 32);
			}
		}
		//currentOffset++;
		return temp | (1LL << 32);
	} else {
		val = temp;
		temp = XMLDoc[(offset << 1) + 2] << 8 | XMLDoc[(offset << 1) + 3];
		if (temp < 0xdc00 || temp > 0xdfff) {
			throw NavException("navigation exception during getChar \n"
				"UTF 16 BE encoding error: should never happen");
		}
		val = ((val - 0xd800) << 10) + (temp - 0xdc00) + 0x10000;
		return val | (2LL << 32);
	}
}

bool VTDNav::matchRawTokenString2(Long l, const UCSChar *s) {
	int len;
	int offset;
	/*if (s == NULL){
	 throwException2(invalid_argument,
	 " invalid argument for matchRawTokenString2, s can't be NULL");
	 }*/
	//throw new IllegalArgumentException("string can't be null");
	len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
	// a little hardcode is always bad
	offset = (int) l;
	return compareRawTokenString2(offset, len, s) == 0;
}
bool VTDNav::matchTokenString1(int offset, int len, const UCSChar *s) {
	int endOffset;
	int l;
	Long l1;
	if (s == NULL) {
		throw InvalidArgumentException(
				" invalid argument for matchRawTokenString1, s can't be NULL");
	}
	//throw new IllegalArgumentException("string can't be null");

	//currentOffset = offset;
	endOffset = offset + len;

	if (encoding < FORMAT_UTF8) {
		int i = 0;
		l = (int) wcslen(s);
		for (i = 0; i < l && offset < endOffset; i++) {
			if ((XMLDoc[offset] & 0xff) != '&') {
				if (s[i] != (XMLDoc[offset] & 0xff))
					return false;
				offset++;
			} else {
				l1 = getCharResolved(offset);
				if (s[i] != (int) l1) {
					return false;
				}
				offset += (int) (l1 >> 32);
			}
		}
		if (i == l && offset == endOffset)
			return true;
		else
			return false;
	} else {
		int i = 0;
		l = (int) wcslen(s);
		for (i = 0; i < l && offset < endOffset; i++) {
			l1 = getCharResolved(offset);
			offset += (int) (l1 >> 32);
			if (s[i] != (int) l1) {
				return false;
			}
		}
		if (i == l && offset == endOffset)
			return true;
		else
			return false;
	} //return true;
}
bool VTDNav::matchTokenString2(Long l, const UCSChar *s) {
	int len, offset;
	if (s == NULL) {
		throw InvalidArgumentException(
				" invalid argument for matchTokenString2, s can't be NULL");
	}
	//	 throw new IllegalArgumentException("string can't be null");

	len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
	// a little hardcode is always bad
	offset = (int) l;
	return matchRawTokenString1(offset, len, s);
}
int VTDNav::NSval(int i) {
	return (int) (vtdBuffer->longAt(i) & MASK_TOKEN_NS_MARK);
}
int VTDNav::parseInt2(int index, int radix) {
	Long pos, result;
	int endOffset, c;
	bool neg;

	if (radix <= 1 || radix > 36) {
		throw InvalidArgumentException("radix out of range ( <2 or > 36 )");
	}
	Long l;
	int t = getTokenType(index);
	bool b = (t == TOKEN_CHARACTER_DATA) || (t == TOKEN_ATTR_VAL);
	int offset = getTokenOffset(index);
	endOffset = offset + getTokenLength(index);

	{
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}

	// trim leading whitespaces
	while ((c == ' ' || c == '\n' || c == '\t' || c == '\r') && (offset
			<= endOffset)) {
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}

	if (offset > endOffset) {// all whitespace
		throw NumberFormatException(" empty string for parseInt2");
	}

	neg = (c == '-');
	if (neg || c == '+') {
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}//skip sign

	result = 0;
	pos = 1;
	while (offset <= endOffset) {
		int digit = Character_digit((char) c, radix);
		if (digit < 0)
			break;

		//Note: for binary we can simply shift to left to improve performance
		result = result * radix + digit;
		//pos *= radix;

		{
			l = b ? getCharResolved(offset) : getChar(offset);
			c = (int) l;
			offset += (int) (l >> 32);
		}
	}

	if (result > MAXINT) {// all whitespace
		throw NumberFormatException(" integer value over(under) flow");
	}
	// throw new NumberFormatException("Overflow: " + toString(index));

	// take care of the trailing
	while (offset <= endOffset && isWS(c)) {
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}
	if (offset == (endOffset + 1))
		return (int) ((neg) ? (-result) : result);
	else {// all whitespace
		throw NumberFormatException(" invalid char during parseInt2");
		return -1;
	}
	// throw new NumberFormatException(toString(index));
}
Long VTDNav::parseLong2(int index, int radix) {
	int endOffset, c;
	Long result, pos;
	bool neg;
	Long l;

	if (radix <= 1 || radix > 36) {
		throw InvalidArgumentException("radix out of range ( <2 or > 36 )");
	}
	int t = getTokenType(index);
	bool b = (t == TOKEN_CHARACTER_DATA) || (t == TOKEN_ATTR_VAL);

	int offset = getTokenOffset(index);
	endOffset = offset + getTokenLength(index);

	{
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}
	// trim leading whitespaces
	while ((c == ' ' || c == '\n' || c == '\t' || c == '\r') && (offset
			<= endOffset)) {
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}

	if (offset > endOffset) {// all whitespace
		throw NumberFormatException(" empty string for parseLong2");
	}

	neg = (c == '-');
	if (neg || c == '+') {
		l = b ? getCharResolved(offset) : getChar(offset);
		c = (int) l;
		offset += (int) (l >> 32);
	}//skip sign

	result = 0;
	pos = 1;
	while (offset <= endOffset) {
		int digit = Character_digit((char) c, radix);
		if (digit < 0)
			break;

		//Note: for binary we can simply shift to left to improve performance
		result = result * radix + digit;
		//pos *= radix;

		{
			l = b ? getCharResolved(offset) : getChar(offset);
			c = (int) l;
			offset += (int) (l >> 32);
		}
	}

	if (result > MAXLONG) {// all whitespace
		throw NumberFormatException(" long value over(under) flow");
	}
	// throw new NumberFormatException("Overflow: " + toString(index));

	// take care of the trailing
	while (offset <= endOffset && isWS(c)) {
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			c = (int) l;
			offset += (int) (l >> 32);
		}
	}
	if (offset == (endOffset + 1))
		return ((neg) ? (-result) : result);
	else {// all whitespace
		throw NumberFormatException(" invalid char during parseLong2");
		return -1;
	}
}
void VTDNav::resolveLC() {
	if (context[0] <= 0)
		return;
	resolveLC_l1();
	if (context[0] == 1)
		return;
	resolveLC_l2();
	if (context[0] == 2)
		return;
	resolveLC_l3();
}
bool VTDNav::resolveNS(const UCSChar *URL) {
	int i, offset, preLen;

	if (context[0] == -1)
		return false;
	i = getTokenLength((context[0] != 0) ? context[context[0]] : rootIndex);
	offset
			= getTokenOffset(
					(context[0] != 0) ? context[context[0]] : rootIndex);
	preLen = (i >> 16) & 0xffff;

	i = lookupNS2(offset, preLen);
	switch (i) {
	case 0:
		if (URL == NULL) {
			if (getTokenLength(i) == 0)
				return true;
		} else {
			return false;
		}
	default:
		if (URL == NULL) {
			return false;
		} else {
			return matchTokenString2(i, URL);
		}
	}
}
bool VTDNav::resolveNS2(const UCSChar *URL, int offset, int len) {
	int i;
	i = lookupNS2(offset, len);
	switch (i) {
	case 0:
		if (URL == NULL) {
			return true;
		} else {
			return false;
		}
	default:
		if (URL == NULL) {
			return false;
		} else {
			return matchTokenString(i, URL);
		}
	}
} //UCSChar *ln){}
int VTDNav::lookupNS2(int offset, int len) {
	Long l;
	int i, k;
	tokenType type;
	bool hasNS = false;
	int size = vtdBuffer->size;
	// look for a match in the current hiearchy and return true
	for (i = context[0]; i >= 0; i--) {
		int s = (i != 0) ? context[i] : rootIndex;
		switch (NSval(s)) { // checked the ns marking
		case 0xc0000000:
			s = s + 1;
			if (s >= size)
				break;
			type = getTokenType(s);

			while (s < size && (type == TOKEN_ATTR_NAME || type
					== TOKEN_ATTR_NS)) {
				if (type == TOKEN_ATTR_NS) {
					// Get the token length
					int temp = getTokenLength(s);
					int preLen = ((temp >> 16) & 0xffff);
					int fullLen = temp & 0xffff;
					int os = getTokenOffset(s);
					// xmlns found
					if (temp == 5 && len == 0) {
						return s + 1;
					} else if ((fullLen - preLen - 1) == len) {
						// prefix length identical to local part of ns declaration
						bool a = true;
						int j;
						for (j = 0; j < len; j++) {
							if (getCharUnit(os + preLen + 1 + j)
									!= getCharUnit(offset + j)) {
								a = false;
								break;
							}
						}
						if (a == true) {
							return s + 1;
						}
					}
				}
				//return (URL != null) ? true : false;
				s += 2;
				type = getTokenType(s);
			}
			break;
		case 0x80000000:
			break;
		default: // check the ns existence, mark bit 31:30 to 11 or 10
			k = s + 1;
			if (k >= size)
				break;
			type = getTokenType(k);

			while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
				if (type == TOKEN_ATTR_NS) {
					// Get the token length

					int temp = getTokenLength(k);
					int preLen = ((temp >> 16) & 0xffff);
					int fullLen = temp & 0xffff;
					int os = getTokenOffset(k);
					hasNS = true;
					// xmlns found
					if (temp == 5 && len == 0) {
						l = vtdBuffer->longAt(s);
						hasNS = false;

						vtdBuffer->modifyEntry(s, l | 0x00000000c0000000LL);
						return k + 1;
					} else if ((fullLen - preLen - 1) == len) {
						// prefix length identical to local part of ns declaration
						bool a = true;
						int j;
						for (j = 0; j < len; j++) {
							if (getCharUnit(os + preLen + 1 + j)
									!= getCharUnit(offset + j)) {
								a = false;
								break;
							}
						}
						if (a == true) {
							l = vtdBuffer->longAt(s);
							//hasNS = false;

							vtdBuffer->modifyEntry(s, l | 0x00000000c0000000LL);

							return k + 1;
						}
					}
				}
				//return (URL != null) ? true : false;
				k += 2;
				if (k >= size)
					break;
				type = getTokenType(k);
			}
			l = vtdBuffer->longAt(s);

			if (hasNS) {
				hasNS = false;
				vtdBuffer->modifyEntry(s, l | 0x00000000c0000000LL);
			} else {
				vtdBuffer->modifyEntry(s, l | 0x0000000080000000LL);
			}

			break;
		}
	}
	return 0;
}

Long VTDNav::getChar4OtherEncoding(int offset) {

	if (encoding <= FORMAT_WIN_1258) {
		int temp = decode(offset);
		if (temp == '\r') {
			if (XMLDoc[offset + 1] == '\n') {
				return '\n' | (2LL << 32);
			} else {
				return '\n' | (1LL << 32);
			}
		}
		return temp | (1LL << 32);
	} else {
		throw NavException(
				"navigation exception during getChar4OtherEncoding \n "
					"Unknown encoding error: should never happen");
		//return 0;
	}
}
int VTDNav::decode(int offset) {
	char ch = XMLDoc[offset];
	switch (encoding) {
	case FORMAT_ISO_8859_2:
		return iso_8859_2_decode((int) ch);
	case FORMAT_ISO_8859_3:
		return iso_8859_3_decode((int) ch);
	case FORMAT_ISO_8859_4:
		return iso_8859_4_decode((int) ch);
	case FORMAT_ISO_8859_5:
		return iso_8859_5_decode((int) ch);
	case FORMAT_ISO_8859_6:
		return iso_8859_6_decode((int) ch);
	case FORMAT_ISO_8859_7:
		return iso_8859_7_decode((int) ch);
	case FORMAT_ISO_8859_8:
		return iso_8859_8_decode((int) ch);
	case FORMAT_ISO_8859_9:
		return iso_8859_9_decode((int) ch);
	case FORMAT_ISO_8859_10:
		return iso_8859_10_decode((int) ch);
	case FORMAT_ISO_8859_11:
		return iso_8859_11_decode((int) ch);
	case FORMAT_ISO_8859_13:
		return iso_8859_13_decode((int) ch);
	case FORMAT_ISO_8859_14:
		return iso_8859_14_decode((int) ch);
	case FORMAT_ISO_8859_15:
		return iso_8859_15_decode((int) ch);
	case FORMAT_WIN_1250:
		return windows_1250_decode((int) ch);
	case FORMAT_WIN_1251:
		return windows_1251_decode((int) ch);
	case FORMAT_WIN_1252:
		return windows_1252_decode((int) ch);
	case FORMAT_WIN_1253:
		return windows_1253_decode((int) ch);
	case FORMAT_WIN_1254:
		return windows_1254_decode((int) ch);
	case FORMAT_WIN_1255:
		return windows_1255_decode((int) ch);
	case FORMAT_WIN_1256:
		return windows_1256_decode((int) ch);
	case FORMAT_WIN_1257:
		return windows_1257_decode((int) ch);
	default:
		return windows_1258_decode((int) ch);
	}
}
int VTDNav::compareRawTokenString2(int offset, int len, const UCSChar *s) {
	int endOffset;
	int l, i;
	Long l1;
	//throw new IllegalArgumentException("string can't be null");

	//currentOffset = offset;
	endOffset = offset + len;

	l = (int) wcslen(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		l1 = getChar(offset);
		offset += (int) (l1 >> 32);
		if (s[i] < (int) l1)
			return 1;
		if (s[i] > (int) l1)
			return -1;
	}

	if (i == l && offset < endOffset)
		return 1;
	if (i < l && offset == endOffset)
		return -1;
	return 0;
}
int VTDNav::compareTokenString2(int offset, int len, const UCSChar *s) {
	int endOffset;
	int l, i;
	Long l1;

	//throw new IllegalArgumentException("string can't be null");

	//currentOffset = offset;
	endOffset = offset + len;

	l = (int) wcslen(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		l1 = getCharResolved(offset);
		offset += (int) (l1 >> 32);
		if (s[i] < (int) l1)
			return 1;
		if (s[i] > (int) l1)
			return -1;
	}

	if (i == l && offset < endOffset)
		return 1;
	if (i < l && offset == endOffset)
		return -1;
	return 0;
}
UCSChar* VTDNav::toStringUpperCase(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}

	while (offset < endOffset) {
		l = getCharResolved(offset);
		offset += (int) (l >> 32);
		if ((int) l > 96 && (int) l < 123)
			s[k++] = (UCSChar)(l - 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l;
	}
	s[k] = 0;
	return s;
}
UCSChar* VTDNav::toStringLowerCase(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getCharResolved(offset);
		offset += (int) (l >> 32);
		if ((int) l > 64 && (int) l < 91)
			s[k++] = (UCSChar)(l + 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}
//UCSChar* VTDNav::toRawStringUpperCase( int index){}
//UCSChar* VTDNav::toRawStringLowerCase( int index){}
UCSChar* VTDNav::toRawStringUpperCase(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getChar(offset);
		offset += (int) (l >> 32);
		if ((int) l > 96 && (int) l < 123)
			s[k++] = (UCSChar)(l - 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l;
	}
	s[k] = 0;
	return s;
}
UCSChar* VTDNav::toRawStringLowerCase(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getChar(offset);
		offset += (int) (l >> 32);
		if ((int) l > 64 && (int) l < 91)
			s[k++] = (UCSChar)(l + 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}
void VTDNav::resolveLC_l1() {
	int k;
	if (l1index < 0 || l1index >= l1Buffer->size || context[1]
			!= l1Buffer->upper32At(l1index)) {
		if (l1index >= l1Buffer->size || l1index < 0) {
			l1index = 0;
		}
		if (l1index + 1 < l1Buffer->size && context[1] != l1Buffer->upper32At(
				l1index + 1)) {
			int init_guess;
			k = context[1];
			init_guess = (int) (l1Buffer->size * ((float) /*context[1]*/k
					/ vtdBuffer->size));
			if (l1Buffer->upper32At(init_guess) > k /*context[1]*/) {
				while (l1Buffer->upper32At(init_guess) != k /*context[1]*/) {
					init_guess--;
				}
			} else if (l1Buffer->upper32At(init_guess) < k /*context[1]*/) {
				while (l1Buffer->upper32At(init_guess) != k /*context[1]*/) {
					init_guess++;
				}
			}
			l1index = init_guess;
		} else {
			if (context[1] >= l1Buffer->upper32At(l1index)) {
				while (context[1] != l1Buffer->upper32At(l1index) && l1index
						< l1Buffer->size) {
					l1index++;
				}
			} else {
				while (context[1] != l1Buffer->upper32At(l1index) && l1index
						>= 0) {
					l1index--;
				}
			}

		}
		// for iterations, l1index+1 is the logical next value for l1index
	}
}
void VTDNav::resolveLC_l2() {
	int temp = l1Buffer->lower32At(l1index), i, k;
	if (l2lower != temp) {
		l2lower = temp;
		// l2lower shouldn't be -1 !!!!  l2lower and l2upper always get resolved simultaneously
		l2index = l2lower;
		l2upper = l2Buffer->size - 1;
		k = l1Buffer->size;
		for (i = l1index + 1; i < k; i++) {
			temp = l1Buffer->lower32At(i);
			if (temp != (int) 0xffffffff) {
				l2upper = temp - 1;
				break;
			}
		}
	} // intelligent guess again ??

	if (l2index < 0 || l2index >= l2Buffer->size || context[2]
			!= l2Buffer->upper32At(l2index)) {
		if (l2index >= l2Buffer->size || l2index < 0)
			l2index = l2lower;
		if (l2index + 1 < l2Buffer->size && context[2] == l2Buffer->upper32At(
				l2index + 1))
			l2index = l2index + 1;
		else if (l2upper - l2lower >= 16) {
			int init_guess = l2lower + (int) ((l2upper - l2lower)
					* ((float) context[2] - l2Buffer->upper32At(l2lower))
					/ (l2Buffer->upper32At(l2upper) - l2Buffer->upper32At(
							l2lower)));
			if (l2Buffer->upper32At(init_guess) > context[2]) {
				while (context[2] != l2Buffer->upper32At(init_guess))
					init_guess--;
			} else if (l2Buffer->upper32At(init_guess) < context[2]) {
				while (context[2] != l2Buffer->upper32At(init_guess))
					init_guess++;
			}
			l2index = init_guess;
		} else if (context[2] < l2Buffer->upper32At(l2index)) {
			while (context[2] != l2Buffer->upper32At(l2index)) {
				l2index--;
			}
		} else {
			while (context[2] != l2Buffer->upper32At(l2index)) {
				l2index++;
			}
		}
	}
}
void VTDNav::resolveLC_l3() {
	int i, k, temp = l2Buffer->lower32At(l2index);
	k = l2Buffer->size;
	if (l3lower != temp) {
		//l3lower and l3upper are always together
		l3lower = temp;
		// l3lower shouldn't be -1
		l3index = l3lower;
		l3upper = l3Buffer->size - 1;
		for (i = l2index + 1; i < k; i++) {
			temp = l2Buffer->lower32At(i);
			if (temp != (int) 0xffffffff) {
				l3upper = temp - 1;
				break;
			}
		}
	}

	if (l3index < 0 || l3index >= l3Buffer->size || context[3]
			!= l3Buffer->intAt(l3index)) {
		if (l3index >= l3Buffer->size || l3index < 0)
			l3index = l3lower;
		if (l3index + 1 < l3Buffer->size && context[3] == l3Buffer->intAt(
				l3index + 1))
			l3index = l3index + 1;
		else if (l3upper - l3lower >= 16) {
			int init_guess = l3lower + (int) ((l3upper - l3lower)
					* ((float) (context[3] - l3Buffer->intAt(l3lower))
							/ (l3Buffer->intAt(l3upper) - l3Buffer->intAt(
									l3lower))));
			if (l3Buffer->intAt(init_guess) > context[3]) {
				while (context[3] != l3Buffer->intAt(init_guess))
					init_guess--;
			} else if (l3Buffer->intAt(init_guess) < context[3]) {
				while (context[3] != l3Buffer->intAt(init_guess))
					init_guess++;
			}
			l3index = init_guess;
		} else if (context[3] < l3Buffer->intAt(l3index)) {
			while (context[3] != l3Buffer->intAt(l3index)) {
				l3index--;
			}
		} else {
			while (context[3] != l3Buffer->intAt(l3index)) {
				l3index++;
			}
		}
	}
}
void VTDNav::recoverNode_l1(int index) {
	int i;
	if (context[1] == index) {

	} else if (l1index != -1 && context[1] > index && l1index + 1
			< l1Buffer->size && l1Buffer->upper32At(l1index + 1) < index) {

	} else {
		i = (index / vtdSize) * l1Buffer->size;
		if (i >= l1Buffer->size)
			i = l1Buffer->size - 1;

		if (l1Buffer->upper32At(i) < index) {
			while (i < l1Buffer->size - 1 && l1Buffer->upper32At(i) < index) {
				i++;
			}
			if (l1Buffer->upper32At(i) > index)
				i--;
		} else {
			while (l1Buffer->upper32At(i) > index) {
				i--;
			}
		}
		context[1] = l1Buffer->upper32At(i);
		l1index = i;
	}
}
void VTDNav::recoverNode_l2(int index) {
	int i = l1Buffer->lower32At(l1index);

	if (l2lower != i) {
		l2lower = i;
		// l2lower shouldn't be -1 !!!! l2lower and l2upper always get
		// resolved simultaneously
		//l2index = l2lower;
		l2upper = l2Buffer->size - 1;
		for (int k = l1index + 1; k < l1Buffer->size; k++) {
			i = l1Buffer->lower32At(k);
			if (i != -1) {
				l2upper = i - 1;
				break;
			}
		}
	}
	// guess what i would be in l2 cache
	int t1 = l2Buffer->upper32At(l2lower);
	int t2 = l2Buffer->upper32At(l2upper);
	//System.out.print("   t2  ==>"+t2+"   t1  ==>"+t1);
	i = min(
			l2lower + (int) (((float) (index - t1) / (t2 - t1 + 1)) * (l2upper
					- l2lower)), l2upper);
	//System.out.print("  i1  "+i);
	while (i < l2Buffer->size - 1 && l2Buffer->upper32At(i) < index) {
		i++;
	}
	//System.out.println(" ==== i2    "+i+"    index  ==>  "+index);

	while (l2Buffer->upper32At(i) > index && i > 0)
		i--;
	context[2] = l2Buffer->upper32At(i);
	l2index = i;
}

void VTDNav::recoverNode_l3(int index) {
	int i = l2Buffer->lower32At(l2index);

	if (l3lower != i) {
		//l3lower and l3upper are always together
		l3lower = i;
		// l3lower shouldn't be -1
		//l3index = l3lower;
		l3upper = l3Buffer->size - 1;
		for (int k = l2index + 1; k < l2Buffer->size; k++) {
			i = l2Buffer->lower32At(k);
			if (i != -1) {
				l3upper = i - 1;
				break;
			}
		}
	}
	int t1 = l3Buffer->intAt(l3lower);
	int t2 = l3Buffer->intAt(l3upper);
	i = min(
			l3lower + (int) (((float) (index - t1) / (t2 - t1 + 1)) * (l3upper
					- l3lower)), l3upper);
	while (i < l3Buffer->size - 1 && l3Buffer->intAt(i) < index) {
		i++;
	}
	while (l3Buffer->intAt(i) > index && i > 0)
		i--;
	//System.out.println(" i ===> "+i);
	context[3] = l3Buffer->intAt(i);
	l3index = i;
}

VTDNav::VTDNav(int r, encoding_t enc, bool ns1, int depth, UByte *x, int xLen,
		FastLongBuffer *vtd, FastLongBuffer *l1, FastLongBuffer *l2,
		FastIntBuffer *l3, int so, int len,
							   bool br1):
			shallowDepth(true),
			rootIndex(r),
			nestingLevel(depth + 1),
			//context (new int[nestingLevel]),
			atTerminal(false), l2upper(-1), l2lower(-1), l3upper(-1),
			l3lower(-1), l2index(-1), l3index(-1), l1index(-1), vtdBuffer(vtd),
			l1Buffer(l1), l2Buffer(l2), l3Buffer(l3), XMLDoc(x),
			offsetMask((ns1) ? MASK_TOKEN_OFFSET1 : MASK_TOKEN_OFFSET2),
			LN(0),// record txt and attrbute for XPath eval purposes
			encoding(enc),
			ns(ns1),
			docOffset(so), // starting offset of the XML doc wrt XMLDoc
			docLen(len), // size of XML document
			vtdSize(vtd->size),// # of entries in vtdBuffer equvalent
			// to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
			bufLen(xLen), // size of XMLDoc in bytes
			br(br1), // buffer reuse flag
			fib(NULL),
			name(NULL), nameIndex(-1), localName(NULL), localNameIndex(-1),
			count(0), currentNode(NULL), URIName(NULL) {
	//VTDNav* vn = NULL;
	int i;
	//exception e;

	if (l1 == NULL || l2 == NULL || l3 == NULL || vtd == NULL || x == NULL
			|| so < 0 || len < 0 || xLen < 0 || // size of x
			r < 0 || depth < 0 || (enc < FORMAT_ASCII || enc > FORMAT_UTF_16LE)) {
		throw InvalidArgumentException(
				"Invalid argument when creating VTDGen failed ");
	}

	/*vn = (VTDNav *) malloc(sizeof(VTDNav));
	 if (vn==NULL){
	 throwException2(out_of_mem,
	 "VTDNav allocation failed ");
	 return NULL;
	 }*/

	if (ns1 == true)
		offsetMask = MASK_TOKEN_OFFSET1;
	else
		offsetMask = MASK_TOKEN_OFFSET2;

	//atTerminal = false;

	//context = (int *)malloc(nestingLevel*sizeof(int));
	context = new int[nestingLevel];
	/*if (context == NULL){
	 throwException2(out_of_mem,
	 "VTDNav allocation failed ");
	 return NULL;
	 }*/
	context[0] = 0;
	for (i = 1; i < nestingLevel; i++) {
		context[i] = -1;
	}
	//currentOffset = 0;
	try {
		contextBuf = new ContextBuffer(10, nestingLevel + 9);
		contextBuf2 = new ContextBuffer(10, nestingLevel + 9);
		fib = new FastIntBuffer(5);
	} catch (std::bad_alloc&) {
		delete contextBuf;
		delete contextBuf2;
		delete fib;
		//free(stackTemp);
		delete[] context;
		//free();
		throw OutOfMemException("VTDNav allocation failed ");
		//return NULL;
	}

	//stackTemp = (int *)malloc((nestingLevel+9)*sizeof(int));
	stackTemp = new int[nestingLevel + 9];
	if (contextBuf == NULL || stackTemp == NULL) {
		delete contextBuf;
		delete contextBuf2;
		delete[] stackTemp;
		delete[] context;
		//free();
		throw OutOfMemException("VTDNav allocation failed ");
		//return NULL;
	}
	maxLCDepthPlusOne = 4;

	
	/*l1index = l2index = l3index = -1;
	 l2lower = l2upper = -1;
	 l3lower = l3upper = -1;
	 docOffset = so;
	 docLen = len;
	 vtdSize = vtd->size;
	 bufLen = xLen;
	 br = br1;
	 LN = 0;*/
	//return vn;
}

VTDNav::VTDNav(int r, encoding_t enc, bool ns1, int depth, UByte *x, int xLen,
		FastLongBuffer *vtd, FastLongBuffer *l1, FastLongBuffer *l2,
		//FastIntBuffer *l3,
		int so, int len,
							   bool br1):
			shallowDepth(false),
			rootIndex(r),
			nestingLevel(depth + 1),
			//context (new int[nestingLevel]),
			atTerminal(false), l2upper(-1), l2lower(-1), l3upper(-1),
			l3lower(-1), l2index(-1), l3index(-1), l1index(-1), vtdBuffer(vtd),
			l1Buffer(l1), l2Buffer(l2), l3Buffer(NULL), XMLDoc(x),
			offsetMask((ns1) ? MASK_TOKEN_OFFSET1 : MASK_TOKEN_OFFSET2),
			LN(0),// record txt and attrbute for XPath eval purposes
			encoding(enc),
			ns(ns1),
			docOffset(so), // starting offset of the XML doc wrt XMLDoc
			docLen(len), // size of XML document
			vtdSize(vtd->size),// # of entries in vtdBuffer equvalent
			// to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
			bufLen(xLen), // size of XMLDoc in bytes
			br(br1), // buffer reuse flag
			name(NULL), nameIndex(-1), localName(NULL), localNameIndex(-1),
			count(0), currentNode(NULL), URIName(NULL) {
	//VTDNav* vn = NULL;
	int i;
	//exception e;

	if (l1 == NULL || l2 == NULL || vtd == NULL || x == NULL || so < 0 || len
			< 0 || xLen < 0 || // size of x
			r < 0 || depth < 0 || (enc < FORMAT_ASCII || enc > FORMAT_UTF_16LE)) {
		throw InvalidArgumentException(
				"Invalid argument when creating VTDGen failed ");
	}

	/*vn = (VTDNav *) malloc(sizeof(VTDNav));
	 if (vn==NULL){
	 throwException2(out_of_mem,
	 "VTDNav allocation failed ");
	 return NULL;
	 }*/

	if (ns1 == true)
		offsetMask = MASK_TOKEN_OFFSET1;
	else
		offsetMask = MASK_TOKEN_OFFSET2;

	//atTerminal = false;

	//context = (int *)malloc(nestingLevel*sizeof(int));
	context = new int[nestingLevel];
	/*if (context == NULL){
	 throwException2(out_of_mem,
	 "VTDNav allocation failed ");
	 return NULL;
	 }*/
	context[0] = 0;
	for (i = 1; i < nestingLevel; i++) {
		context[i] = -1;
	}

	maxLCDepthPlusOne = 4;

}

VTDNav::~VTDNav() {

	delete (contextBuf);
	delete (contextBuf2);
	if (br == false && master) {
		delete (vtdBuffer);
		delete (l1Buffer);
		delete (l2Buffer);
		delete (l3Buffer);
	}
	delete[] context;
	delete[] stackTemp;
	if (name != NULL)
		delete name;
	if (localName != NULL)
		delete localName;
	if (URIName != NULL)
		delete URIName;
	if (currentNode != NULL)
		delete currentNode;
	//free(XMLDoc);

}
//Return the attribute count of the element at the cursor position.
int VTDNav::getAttrCount() {
	int count = 0;
	int index;
	if (context[0] == -1)
		return 0;
	index = getCurrentIndex() + 1;
	while (index < vtdSize) {
		int type = getTokenType(index);
		if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_VAL || type
				== TOKEN_ATTR_NS) {
			if (type == TOKEN_ATTR_NAME || (!ns && (type == TOKEN_ATTR_NS))) {
				count++;
			}
		} else
			break;
		index++;
	}
	return count;
}
//Get the token index of the attribute value given an attribute name.
int VTDNav::getAttrVal(const UCSChar *an) {
	//int size = vtdBuffer->size;
	int index;
	tokenType type;
	if (context[0] == -1)
		return -1;
	index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;

	if (index < vtdSize)
		type = getTokenType(index);
	else
		return -1;

	if (ns == false) {
		while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
			if (matchRawTokenString(index, an)) { // ns node visible only ns is disabled
				return index + 1;
			}
			index += 2;
			if (index >= vtdSize)
				return -1;
			type = getTokenType(index);
		}
	} else {
		while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
			if (type == TOKEN_ATTR_NAME && matchRawTokenString(index, an)) { // ns node visible only ns is disabled
				return index + 1;
			}
			index += 2;
			if (index >= vtdSize)
				return -1;
			type = getTokenType(index);
		}
	}
	return -1;
}
//Get the token index of the attribute value of given URL and local name.
//If ns is not enabled, the lookup will return -1, indicating a no-found.
//Also namespace nodes are invisible using this method.
int VTDNav::getAttrValNS(const UCSChar* URL, const UCSChar *ln) {

	int size, index;
	tokenType type;
	if (context[0] == -1)
		return -1;
	if (URL != NULL && wcslen(URL) == 0)
		URL = NULL;
	if (ns == false)
		return -1;
	if (URL == NULL)
		return getAttrVal(ln);
	size = vtdBuffer->size;
	index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
	// point to the token next to the element tag

	if (index < vtdSize)
		type = getTokenType(index);
	else
		return -1;

	while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
		int i = getTokenLength(index);
		int offset = getTokenOffset(index);
		int preLen = (i >> 16) & 0xffff;
		int fullLen = i & 0xffff;
		if (preLen != 0
		// attribute name without a prefix is not bound to any namespaces
				&& matchRawTokenString1(offset + preLen + 1,
						fullLen - preLen - 1, ln) && resolveNS2(URL, offset,
				preLen)) {
			return index + 1;
		} else if (preLen == 3 && matchRawTokenString1(offset, 3, L"xml")
				&& wcscmp(URL, L"http://www.w3.org/XML/1998/namespace") == 0) {
			// prefix matches "xml"
			return index + 1;
		}
		index += 2;
		if (index >= vtdSize)
			return -1;
		type = getTokenType(index);
	}
	return -1;

}

// Get the starting offset and length of an element
// encoded in a long, upper 32 bit is length{} lower 32 bit is offset
Long VTDNav::getElementFragment() {
	// a little scanning is needed
	// has next sibling case
	// if not
	int depth = getCurrentDepth();
	int so;
	int length = 0;
	int temp;
	int size, so2, d, i;

	if (depth == -1)
		return ((Long) docLen) << 32 | docOffset;

	if (depth == -1) {
		int i = vtdBuffer->lower32At(0);
		if (i == 0)
			return ((Long) docLen) << 32 | docOffset;
		else
			return ((Long)(docLen - 32)) | 32;
	}
	// for an element with next sibling
	so = getTokenOffset(getCurrentIndex2()) - 1;
	if (toElement(NEXT_SIBLING)) {

		int temp = getCurrentIndex(), temp2 = temp;
		int so2;
		// rewind
		while (getTokenDepth(temp) == depth && 
					(getTokenType(temp)==TOKEN_COMMENT || 
							getTokenType(temp)==TOKEN_PI_VAL ||
							getTokenType(temp)==TOKEN_PI_NAME)) {
				
				temp--;
			}
		if (temp != temp2)
			temp++;
		//temp++;
		so2 = getTokenOffset(temp) - 1;
		// look for the first '>'
		while (getCharUnit(so2) != '>') {
			so2--;
		}
		length = so2 - so + 1;
		toElement( PREV_SIBLING);
		if (encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}

	// for root element
	if (depth == 0) {
		int temp = vtdBuffer->size - 1;
		bool b = false;
		int so2 = 0;
		while (getTokenDepth(temp) == -1) {
			temp--; // backward scan
			b = true;
		}
		if (b == false)
			so2 = (encoding <= FORMAT_WIN_1258) ? (docOffset + docLen - 1)
					: ((docOffset + docLen) >> 1) - 1;
		else
			so2 = getTokenOffset(temp + 1);
		while (getCharUnit(so2) != '>') {
			so2--;
		}
		length = so2 - so + 1;
		if (encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}
	// for a non-root element with no next sibling
	temp = getCurrentIndex() + 1;
	size = vtdBuffer->size;
	// temp is not the last entry in VTD buffer
	if (temp < size - 1) {
		while (temp < size && getTokenDepth(temp) >= depth) {
			temp++;
		}
		if (temp != size) {
			int d = depth - getTokenDepth(temp) + ((getTokenType(temp)
					== TOKEN_STARTING_TAG) ? 1 : 0);
			int so2 = getTokenOffset(temp) - 1;
			int i = 0;
			// scan backward
			while (i < d) {
				if (getCharUnit(so2) == '>')
					i++;
				so2--;
			}
			length = so2 - so + 2;
			if (encoding <= FORMAT_WIN_1258)
				return ((Long) length) << 32 | so;
			else
				return ((Long) length) << 33 | (so << 1);
		}
	}
	// temp is the last entry
	// scan forward search for /> or </cc>
	so2 = (encoding <= FORMAT_WIN_1258) ? (docOffset + docLen - 1)
			: ((docOffset + docLen) >> 1) - 1;
	d = depth + 1;
	i = 0;
	while (i < d) {
		if (getCharUnit(so2) == '>') {
			i++;
		}
		so2--;
	}

	length = so2 - so + 2;

	if (encoding <= FORMAT_WIN_1258)
		return ((Long) length) << 32 | so;
	else
		return ((Long) length) << 33 | (so << 1);
}
Long VTDNav::getContentFragment() {
	int so, length, so2, temp, size, d, i;
	int depth = getCurrentDepth();
	//		 document length and offset returned if depth == -1
	if (depth == -1) {
		int i = vtdBuffer->lower32At(0);
		if (i == 0)
			return ((Long) docLen) << 32 | docOffset;
		else
			return ((Long)(docLen - 32)) | 32;
	}

	Long l = getOffsetAfterHead();
	if (l < 0)
		return -1LL;
	so = (int) l;

	length = 0;

	// for an element with next sibling
	if (toElement(NEXT_SIBLING)) {

		int temp = getCurrentIndex(), temp2 = temp;
		// rewind
		while (getTokenDepth(temp) == depth && 
					(getTokenType(temp)==TOKEN_COMMENT || 
							getTokenType(temp)==TOKEN_PI_VAL ||
							getTokenType(temp)==TOKEN_PI_NAME)) {
				
				temp--;
			}
		if (temp2 != temp)
			temp++;
		//temp++;
		so2 = getTokenOffset(temp) - 1;
		// look for the first '>'
		while (getCharUnit(so2) != '>') {
			so2--;
		}
		while (getCharUnit(so2) != '/') {
			so2--;
		}
		while (getCharUnit(so2) != '<') {
			so2--;
		}
		length = so2 - so;
		toElement( PREV_SIBLING);
		if (encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}

	// for root element
	if (depth == 0) {
		int temp = vtdBuffer->size - 1;
		bool b = false;
		int so2 = 0;
		while (getTokenDepth(temp) == -1) {
			temp--; // backward scan
			b = true;
		}
		if (b == false)
			so2 = (encoding <= FORMAT_WIN_1258) ? (docOffset + docLen - 1)
					: ((docOffset + docLen) >> 1) - 1;
		else
			so2 = getTokenOffset(temp + 1);
		while (getCharUnit(so2) != '>') {
			so2--;
		}
		while (getCharUnit(so2) != '/') {
			so2--;
		}
		while (getCharUnit(so2) != '<') {
			so2--;
		}
		length = so2 - so;
		if (encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}
	// for a non-root element with no next sibling
	temp = getCurrentIndex() + 1;
	size = vtdBuffer->size;
	// temp is not the last entry in VTD buffer
	if (temp < size) {
		while (temp < size && getTokenDepth(temp) >= depth) {
			temp++;
		}
		if (temp != size) {
			int d = depth - getTokenDepth(temp) + ((getTokenType(temp)
					== TOKEN_STARTING_TAG) ? 1 : 0);
			int so2 = getTokenOffset(temp) - 1;
			int i = 0;
			// scan backward
			while (i < d) {
				if (getCharUnit(so2) == '>')
					i++;
				so2--;
			}
			while (getCharUnit(so2) != '/') {
				so2--;
			}
			while (getCharUnit(so2) != '<') {
				so2--;
			}
			length = so2 - so;
			if (encoding <= FORMAT_WIN_1258)
				return ((Long) length) << 32 | so;
			else
				return ((Long) length) << 33 | (so << 1);
		}
		/*
		 * int so2 = getTokenOffset(temp - 1) - 1; int d = depth -
		 * getTokenDepth(temp - 1); int i = 0; while (i < d) { if
		 * (getCharUnit(so2) == '>') { i++; } so2--; } length = so2 - so +
		 * 2; if (encoding < 3) return ((long) length) < < 32 | so; else
		 * return ((long) length) < < 33 | (so < < 1);
		 */
	}
	// temp is the last entry
	// scan forward search for /> or </cc>

	so2 = (encoding <= FORMAT_WIN_1258) ? (docOffset + docLen - 1)
			: ((docOffset + docLen) >> 1) - 1;

	d = depth + 1;

	i = 0;
	while (i < d) {
		if (getCharUnit(so2) == '>') {
			i++;
		}
		so2--;
	}
	while (getCharUnit(so2) != '/') {
		so2--;
	}
	while (getCharUnit(so2) != '<') {
		so2--;
	}

	length = so2 - so;

	if (encoding <= FORMAT_WIN_1258)
		return ((Long) length) << 32 | so;
	else
		return ((Long) length) << 33 | (so << 1);
}

//ElementFragmentNs* VTDNav::getElementFragmentNs(){
//
//	if (ns == false){
//		throw NavException("getElementFragmentNS can only be called when ns is tured on");
//		return NULL;
//	}else {
//		//fill the fib with integer
//		// first get the list of name space nodes
//		FastIntBuffer *fib = NULL;
//		int i = 0;
//		int count=0;
//		int type;
//
//		int* ia = context;
//		int d =ia[0]; // -1 for document node, 0 for root element;
//		int c = getCurrentIndex2();
//		int len = (c == 0 || c == rootIndex )? 0:
//			(getTokenLength(c) & 0xffff); // get the length of qualified node
//		int newSz; Long l;
//		fib = new FastIntBuffer(3); // init size 8
//
//
//		// put the neighboring ATTR_NS nodes into the array
//		// and record the total # of them
//
//		if (d > 0){ // depth > 0 every node except document and root element
//			int k=getCurrentIndex2()+1;
//			if (k<vtdSize){
//				while(k<vtdSize) {
//					type = getTokenType(k);
//					if(type==TOKEN_ATTR_NAME || type==TOKEN_ATTR_NS){
//						if (type == TOKEN_ATTR_NS){
//							fib->append(k);
//							//System.out.println(" ns name ==>" + toString(k));
//						}
//					}
//					k+=2;
//					//type = getTokenType(vn,k);
//				}
//
//			}
//			count = fib->size;
//			d--;
//			while (d >= 0) {
//				// then search for ns node in the vinicity of the ancestor nodes
//				if (d > 0) {
//					// starting point
//					k = ia[d]+1;
//				} else {
//					// starting point
//					k = rootIndex+1;
//				}
//				if (k<vtdSize){
//
//					while(k<vtdSize){
//						type = getTokenType(k);
//						if (type==TOKEN_ATTR_NAME || type==TOKEN_ATTR_NS){
//							bool unique = true;
//							if (type == TOKEN_ATTR_NS){
//								int z = 0;
//								for (z=0;z<fib->size;z++){
//									//System.out.println("fib size ==> "+fib.size());
//									//if (fib->size==4);
//									if (matchTokens(fib->intAt(z),this,k)==true){
//										unique = false;
//										break;
//									}
//
//								}
//								if (unique)
//									fib->append(k);
//							}
//						}
//							k+=2;
//							//type = getTokenType(vn,k);
//					}
//				}
//				d--;
//			}
//			// System.out.println("count ===> "+count);
//			// then restore the name space node by shifting the array
//			newSz= fib->size-count;
//			for (i= 0; i<newSz; i++ ){
//				fib->modifyEntry(i,fib->intAt(i+count));
//			}
//			fib->resize(newSz);
//		}
//
//		l= getElementFragment();
//		return new ElementFragmentNs(this, l,fib,len);
//	}
//
//}
/**
 * Get the encoding of the XML document.
 * <pre>   0  ASCII       </pre>
 * <pre>   1  ISO-8859-1  </pre>
 * <pre>   2  UTF-8       </pre>
 * <pre>   3  UTF-16BE    </pre>
 * <pre>   4  UTF-16LE    </pre>
 */
/*inline encoding_t getEncoding(){
 return encoding;
 }*/
// Get the maximum nesting depth of the XML document (>0).
// max depth is nestingLevel -1


// This function returns of the token index of the type character data or CDATA.
// Notice that it is intended to support data orient XML (not mixed-content XML).
int VTDNav::getText() {
	if (context[0] == -1)
		return -1;
	else {
		int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
		int depth = getCurrentDepth();
		tokenType type;
		if (index < vtdSize || !atTerminal)
			type = getTokenType(index);
		else
			return -1;

		while (true) {
			if (type == TOKEN_CHARACTER_DATA || type == TOKEN_CDATA_VAL) {
				if (depth == getTokenDepth(index))
					return index;
				else
					return -1;
			} else if (type == TOKEN_ATTR_NS || type == TOKEN_ATTR_NAME) {
				index += 2; // assuming a single token for attr val
			} else if (type == TOKEN_PI_NAME || type == TOKEN_PI_VAL || type
					== TOKEN_COMMENT) {
				if (depth == getTokenDepth(index)) {
					index += 1;
				} else
					return -1;
			} else
				return -1;
			if (index >= vtdSize)
				break;
			type = getTokenType(index);
		}
		return -1;
	}

}

//Get total number of VTD tokens for the current XML document.
/*int VTDNav::getTokenCount(){
 return vtdSize;
 }*/

//Get the depth value of a token (>=0)
/*int VTDNav::getTokenDepth(int index){

 int i = (int) ((vtdBuffer->longAt(index) & MASK_TOKEN_DEPTH) >> 52);

 if (i != 255)
 return i;
 return -1;
 }*/

//Get the token length at the given index value
//please refer to VTD spec for more details
int VTDNav::getTokenLength(int index) {
	Long i = 0;
	int depth;
	int len = 0;
	int type = getTokenType(index), temp = 0;
	//int val;
	switch (type) {
	case TOKEN_ATTR_NAME:
	case TOKEN_ATTR_NS:
	case TOKEN_STARTING_TAG:
		i = vtdBuffer->longAt(index);

		return (ns == false) ? (int) ((i & MASK_TOKEN_QN_LEN) >> 32)
				: ((int) ((i & MASK_TOKEN_QN_LEN) >> 32) | ((int) ((i
						& MASK_TOKEN_PRE_LEN) >> 32) << 5));

		break;
	case TOKEN_CHARACTER_DATA:
	case TOKEN_CDATA_VAL:
	case TOKEN_COMMENT: // make sure this is total length
		depth = getTokenDepth(index);
		do {

			len = len + (int) ((vtdBuffer->longAt(index) & MASK_TOKEN_FULL_LEN)
					>> 32);
			temp = getTokenOffset(index) + (int) ((vtdBuffer->longAt(index)
					& MASK_TOKEN_FULL_LEN) >> 32);
			index++;
		} while (index < vtdSize && depth == getTokenDepth(index) && type
				== getTokenType(index) && temp == getTokenOffset(index));
		//if (int k=0)
		return len;
	default:

		return (int) ((vtdBuffer->longAt(index) & MASK_TOKEN_FULL_LEN) >> 32);

		break;
	}
}

//Get the token type of the token at the given index value.
/*inline	tokenType VTDNav::getTokenType(int index){
 return (tokenType) ((vtdBuffer->longAt(index) & MASK_TOKEN_TYPE) >> 60) & 0xf;
 }*/

//Test whether current element has an attribute with the matching name.
bool VTDNav::hasAttr(const UCSChar *an) {
	tokenType type;

	int size = vtdBuffer->size;
	int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;

	if (context[0] == -1)
		return false;

	if (index >= size)
		return false;

	type = getTokenType(index);
	if (ns == false) {
		if (wcscmp(an, L"*") == 0) {
			if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)
				return true;
			else
				return false;
		} else {
			while (index < size && (type == TOKEN_ATTR_NAME || type
					== TOKEN_ATTR_NS)) { // ns tokens becomes visible
				if (matchRawTokenString(index, an))
					return true;
				index += 2;
				type = getTokenType(index);
			}
			return false;
		}
	} else {
		if (wcscmp(an, L"*") == 0) {
			while (index < size && (getTokenType(index) == TOKEN_ATTR_NAME
					|| getTokenType(index) == TOKEN_ATTR_NS)) {
				if (type == TOKEN_ATTR_NAME)
					// TOKEN_ATTR_NS is invisible when ns == true
					return true;
				index += 2;
				type = getTokenType(index);
			}
			return false;

		} else {
			while (index < size && (type == TOKEN_ATTR_NAME || type
					== TOKEN_ATTR_NS)) {
				if (type == TOKEN_ATTR_NAME && matchRawTokenString(index, an))
					return true;
				index += 2;
				type = getTokenType(index);
			}
			return false;
		}
	}
}

//Test whether the current element has an attribute with 
//matching namespace URL and localname.
/*bool VTDNav::hasAttrNS(const UCSChar *URL,const UCSChar *localName){
 if (context[0]==-1)
 return false;
 return (getAttrValNS(URL, localName) != -1);
 }*/

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
bool VTDNav::iterate(int dp, const UCSChar *en, bool special) {
	int index = getCurrentIndex() + 1;
	tokenType tt;
	//int size = vtdBuffer.size();
	while (index < vtdSize) {
		tt = getTokenType(index);
		if (tt == TOKEN_ATTR_NAME || tt == TOKEN_ATTR_NS) {
			index += 2;
			continue;
		}

		if (isElementOrDocument(index)) {
			int depth = getTokenDepth(index);
			if (depth > dp) {
				context[0] = depth;
				if (depth > 0)
					context[depth] = index;

				if (special || matchElement(en)) {
					if (dp < maxLCDepthPlusOne)
						resolveLC();
					return true;
				}
			} else {
				return false;
			}
		}
		index++;
	}
	return false;
}

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
bool VTDNav::iterateNS(int dp, const UCSChar *URL, const UCSChar *ln) {
	int index;
	tokenType tt;
	if (ns == false)
		return false;

	index = getCurrentIndex() + 1;
	while (index < vtdSize) {
		tt = getTokenType(index);
		if (tt == TOKEN_ATTR_NAME || tt == TOKEN_ATTR_NS || tt == TOKEN_PI_NAME) {
			index = index + 2;
			continue;
		}
		//if (isElementOrDocument(index)) {
		if (tt == TOKEN_STARTING_TAG || tt == TOKEN_DOCUMENT) {
			int depth = getTokenDepth(index);
			if (depth > dp) {
				context[0] = depth;
				if (depth > 0)
					context[depth] = index;
				if (matchElementNS(URL, ln)) {
					if (dp < maxLCDepthPlusOne)
						resolveLC();
					return true;
				}
			} else {
				return false;
			}
		}
		index++;

	}
	return false;
}

// This function is called by selectElement_F in autoPilot
bool VTDNav::iterate_following(const UCSChar *en, bool special) {
	int index = getCurrentIndex() + 1;
	int tokenType;
	//int size = vtdBuffer.size;
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		if (tokenType == TOKEN_ATTR_NAME || tokenType == TOKEN_ATTR_NS
				|| tokenType == TOKEN_PI_NAME) {
			index = index + 2;
			continue;
		}
		// if (isElementOrDocument(index)) {
		if (tokenType == TOKEN_STARTING_TAG || tokenType == TOKEN_DOCUMENT) {
			int depth = getTokenDepth(index);
			context[0] = depth;
			if (depth > 0)
				context[depth] = index;
			if (special || matchElement(en)) {
				if (depth < maxLCDepthPlusOne)
					resolveLC();
				return true;
			}
		}
		index++;
	}
	return false;
}

// This function is called by selectElementNS_F in autoPilot
bool VTDNav::iterate_followingNS(const UCSChar *URL, const UCSChar *ln) {
	int index = getCurrentIndex() + 1;
	int tokenType;
	//int size = vtdBuffer.size;
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		if (tokenType == TOKEN_ATTR_NAME || tokenType == TOKEN_ATTR_NS
				|| tokenType == TOKEN_PI_NAME) {
			index = index + 2;
			continue;
		}
		// if (isElementOrDocument(index)) {
		if (tokenType == TOKEN_STARTING_TAG || tokenType == TOKEN_DOCUMENT) {
			int depth = getTokenDepth(index);
			context[0] = depth;
			if (depth > 0)
				context[depth] = index;
			if (matchElementNS(URL, ln)) {
				if (depth < maxLCDepthPlusOne)
					resolveLC();
				return true;
			}
		}
		index++;
	}
	return false;
}

//Test if the current element matches the given name.
/*bool VTDNav::matchElement( const UCSChar *en){
 /*if (en == NULL){
 throwException2(invalid_argument,
 "matchElement's element name can't be null");
 }

 // throw new IllegalArgumentException(" Element name can't be null ");
 if (wcscmp(en,L"*") == 0 && context[0] !=-1)
 return true;
 if (context[0]==-1)
 return false;
 return matchRawTokenString(
 (context[0] == 0) ? rootIndex : context[context[0]],
 en);
 }*/

//Test whether the current element matches the given namespace URL and localname.
//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
//ln is the localname that, when set to *, matches any localname
bool VTDNav::matchElementNS(const UCSChar *URL, const UCSChar *ln) {
	if (context[0] == -1)
		return false;
	else {
		int i = getTokenLength(
				(context[0] != 0) ? context[context[0]] : rootIndex);
		int offset = getTokenOffset(
				(context[0] != 0) ? context[context[0]] : rootIndex);
		int preLen = (i >> 16) & 0xffff;
		int fullLen = i & 0xffff;

		if (wcscmp(ln, L"*") == 0 || ((preLen != 0) ? matchRawTokenString1(
				offset + preLen + 1, fullLen - preLen - 1, ln)
				: matchRawTokenString1(offset, fullLen, ln))) { // no prefix, search for xmlns
			if (((URL != NULL) ? wcscmp(URL, L"*") == 0 : false)
					|| (resolveNS2(URL, offset, preLen) == true))
				return true;

			if (preLen == 3 && matchRawTokenString1(offset, preLen, L"xml")
					&& wcscmp(URL, L"http://www.w3.org/XML/1998/namespace")
							== 0)
				return true;
		}
		return false;
	}
}

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
bool VTDNav::matchRawTokenString(int index, const UCSChar *s) {
	tokenType type;
	int len;
	int offset;
	/*if (s == NULL){
	 //	 throwException2(invalid_argument,
	 //		 " invalid argument for matchRawTokenString, s can't be NULL");
	 }*/
	type = getTokenType(index);
	len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return compareRawTokenString2(offset, len, s) == 0;
}
//This method matches two VTD tokens of 2 VTDNavs
/*bool VTDNav::matchTokens( int i1, VTDNav *vn2, int i2){
 return compareTokens(i1,vn2,i2)==0;

 }*/

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
bool VTDNav::matchTokenString(int index, const UCSChar *s) {
	tokenType type;
	int offset;
	int len;
	if (s == NULL) {
		throw NumberFormatException(
				" invalid argument for matchTokenString, s can't be NULL");
	}
	type = getTokenType(index);
	len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return matchTokenString1(offset, len, s);
}

//Convert a vtd token into a double.
double VTDNav::parseDouble(int index) {
	int ch;
	int end;
	Long l;
	tokenType t = getTokenType(index);
	bool b = (t == TOKEN_CHARACTER_DATA) || (t == TOKEN_ATTR_VAL);
	Long left, right, scale, exp;
	double v;
	bool neg;
	int offset = getTokenOffset(index);
	end = offset + getTokenLength(index);
	//past the last one by one

	//ch = b?getCharResolved():getChar();
	{
		l = b ? getCharResolved(offset) : getChar(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
	}
	while (offset <= end) { /* trim leading whitespaces*/
		if (!isWS(ch))
			break;
		l = b ? getCharResolved(offset) : getChar(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
	}

	if (offset > end) {/* all whitespace */
		double d1 = 0.0;
		return d1 / d1;
	}

	neg = (ch == '-');

	if (ch == '-' || ch == '+') {
		l = b ? getCharResolved(offset) : getChar(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
	}

	/*left part of decimal*/
	left = 0;
	while (offset <= end) {
		/*must be <= since we get the next one at last.*/

		int dig = Character_digit((char) ch, 10); /*only consider decimal*/
		if (dig < 0)
			break;

		left = left * 10 + dig;

		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
	}

	/*right part of decimal*/
	right = 0;
	scale = 1;
	if (ch == '.') {
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}

		while (offset <= end) {
			/*must be <= since we get the next one at last.*/

			int dig = Character_digit((char) ch, 10);
			/*only consider decimal */
			if (dig < 0)
				break;

			right = right * 10 + dig;
			scale *= 10;

			{
				l = b ? getCharResolved(offset) : getChar(offset);
				ch = (int) l;
				offset += (int) (l >> 32);
			}
		}
	}

	/*exponent*/
	exp = 0;
	if (ch == 'E' || ch == 'e') {
		bool expneg;
		int cur;
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
		expneg = (ch == '-'); /*sign for exp*/
		if (ch == '+' || ch == '-') {
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		} /*skip the +/- sign*/

		cur = offset;
		/*remember the indx, used to find a invalid number like 1.23E*/

		while (offset <= end) {
			/*must be <= since we get the next one at last.*/

			int dig = Character_digit((char) ch, 10);
			/*only consider decimal*/
			if (dig < 0)
				break;
			exp = exp * 10 + dig;
			{
				l = b ? getCharResolved(offset) : getChar(offset);
				ch = (int) l;
				offset += (int) (l >> 32);
			}
		}
		if (cur == offset) {// all whitespace
			double d1 = 0.0;
			return d1 / d1;
			//throw new NavException("Empty string");
		}
		if (expneg)
			exp = (-exp);
	}

	//anything left must be space
	while (offset <= end) {
		if (!isWS(ch)) {// all whitespace
			double d1 = 0.0;
			return d1 / d1;
		}
		// throw new NavException(toString(index));

		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
	}

	v = (double) left;
	if (right != 0)
		v += ((double) right) / (double) scale;

	if (exp != 0)
		v = v * pow(10, (double) exp);

	return ((neg) ? (-v) : v);
}

//Convert a vtd token into a float.
float VTDNav::parseFloat(int index) {

	Long exp, left, right, scale;
	float f;
	int end, ch;
	int t = getTokenType(index);
	bool b = (t == TOKEN_CHARACTER_DATA) || (t == TOKEN_ATTR_VAL);
	bool neg;
	double v;
	Long l;
	int offset = getTokenOffset(index);
	end = offset + getTokenLength(index);
	//past the last one by one

	{
		l = b ? getCharResolved(offset) : getChar(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
	}

	while (offset <= end) { // trim leading whitespaces
		if (!isWS(ch))
			break;
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
	}

	if (offset > end) {// all whitespace
		float d1 = 0.0;
		return d1 / d1;
	}
	//throw new NavException("Empty string");

	neg = (ch == '-');

	if (ch == '-' || ch == '+') {
		l = b ? getCharResolved(offset) : getChar(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
	} //get another one if it is sign.

	//left part of decimal
	left = 0;
	while (offset <= end) {
		//must be <= since we get the next one at last.

		int dig = Character_digit((char) ch, 10); //only consider decimal
		if (dig < 0)
			break;

		left = left * 10 + dig;

		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
	}

	//right part of decimal
	right = 0;
	scale = 1;
	if (ch == '.') {
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}

		while (offset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character_digit((char) ch, 10);
			//only consider decimal
			if (dig < 0)
				break;

			right = right * 10 + dig;
			scale *= 10;

			{
				l = b ? getCharResolved(offset) : getChar(offset);
				ch = (int) l;
				offset += (int) (l >> 32);
			}
		}
	}

	//exponent
	exp = 0;
	if (ch == 'E' || ch == 'e') {
		bool expneg;
		int cur;
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
		expneg = (ch == '-'); //sign for exp
		if (ch == '+' || ch == '-') {
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}//skip the +/- sign

		cur = offset;
		//remember the indx, used to find a invalid number like 1.23E

		while (offset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character_digit((char) ch, 10);
			//only consider decimal
			if (dig < 0)
				break;

			exp = exp * 10 + dig;

			{
				l = b ? getCharResolved(offset) : getChar(offset);
				ch = (int) l;
				offset += (int) (l >> 32);
			}
		}

		if (cur == offset) {// all whitespace
			float f = 0;
			return f / f;
		}
		//	 throw new NavException(toString(index));
		//found a invalid number like 1.23E

		if (expneg)
			exp = (-exp);
	}

	/*anything left must be space*/
	while (offset <= end) {
		if (!isWS(ch)) {/* all whitespace */
			float f = 0.0;
			return f / f;
		}
		{
			l = b ? getCharResolved(offset) : getChar(offset);
			ch = (int) l;
			offset += (int) (l >> 32);
		}
	}

	v = (double) left;
	if (right != 0)
		v += ((double) right) / (double) scale;

	if (exp != 0)
		v = v * pow(10, (double) exp);

	f = (float) v;

	/*try to handle overflow/underflow */
	if (v >= MAXFLOAT)
		f = MAXFLOAT;
	else if (v <= MINFLOAT)
		f = MINFLOAT;

	if (neg)
		f = -f;
	return f;
}

//Convert a vtd token into an int
int VTDNav::parseInt(int index) {
	return parseInt2(index, 10);
}

//Convert a vtd token into a long
Long VTDNav::parseLong(int index) {
	return parseLong2(index, 10);
}

//Load the context info from ContextBuffer.
//Info saved including LC and current state of the context 
bool VTDNav::pop() {
	bool b = contextBuf->load(stackTemp);
	int i;
	if (b == false)
		return false;
	for (i = 0; i < nestingLevel; i++) {
		context[i] = stackTemp[i];
	}
	l1index = stackTemp[nestingLevel];
	l2index = stackTemp[nestingLevel + 1];
	l3index = stackTemp[nestingLevel + 2];
	l2lower = stackTemp[nestingLevel + 3];
	l2upper = stackTemp[nestingLevel + 4];
	l3lower = stackTemp[nestingLevel + 5];
	l3upper = stackTemp[nestingLevel + 6];
	atTerminal = (stackTemp[nestingLevel + 7] == 1);
	LN = stackTemp[nestingLevel + 8];
	return true;
}
bool VTDNav::pop2() {
	bool b = contextBuf2->load(stackTemp);
	int i;
	if (b == false)
		return false;
	for (i = 0; i < nestingLevel; i++) {
		context[i] = stackTemp[i];
	}
	l1index = stackTemp[nestingLevel];
	l2index = stackTemp[nestingLevel + 1];
	l3index = stackTemp[nestingLevel + 2];
	l2lower = stackTemp[nestingLevel + 3];
	l2upper = stackTemp[nestingLevel + 4];
	l3lower = stackTemp[nestingLevel + 5];
	l3upper = stackTemp[nestingLevel + 6];
	atTerminal = (stackTemp[nestingLevel + 7] == 1);
	LN = stackTemp[nestingLevel + 8];
	return true;
}
//Store the context info into the ContextBuffer.
//Info saved including LC and current state of the context 
bool VTDNav::push() {
	int i;
	for (i = 0; i < nestingLevel; i++) {
		stackTemp[i] = context[i];
	}
	stackTemp[nestingLevel] = l1index;
	stackTemp[nestingLevel + 1] = l2index;
	stackTemp[nestingLevel + 2] = l3index;
	stackTemp[nestingLevel + 3] = l2lower;
	stackTemp[nestingLevel + 4] = l2upper;
	stackTemp[nestingLevel + 5] = l3lower;
	stackTemp[nestingLevel + 6] = l3upper;
	if (atTerminal)
		stackTemp[nestingLevel + 7] = 1;
	else
		stackTemp[nestingLevel + 7] = 0;
	stackTemp[nestingLevel + 8] = LN;
	contextBuf->store(stackTemp);
	return true;
}
bool VTDNav::push2() {
	int i;
	for (i = 0; i < nestingLevel; i++) {
		stackTemp[i] = context[i];
	}
	stackTemp[nestingLevel] = l1index;
	stackTemp[nestingLevel + 1] = l2index;
	stackTemp[nestingLevel + 2] = l3index;
	stackTemp[nestingLevel + 3] = l2lower;
	stackTemp[nestingLevel + 4] = l2upper;
	stackTemp[nestingLevel + 5] = l3lower;
	stackTemp[nestingLevel + 6] = l3upper;
	if (atTerminal)
		stackTemp[nestingLevel + 7] = 1;
	else
		stackTemp[nestingLevel + 7] = 0;
	stackTemp[nestingLevel + 8] = LN;
	//printf(" !!!!!!!!!!size ===> %d \n",contextBuf2->size);
	contextBuf2->store(stackTemp);
	return true;
}
void VTDNav::sampleState(FastIntBuffer *fib) {
	for (int i = 0; i <= context[0]; i++)
		fib->append(context[i]);
	if (atTerminal)
		fib->append(LN);
	if (context[0] >= 1)
		fib->append(l1index);

	if (context[0] >= 2) {
		fib->append(l2index);
		fib->append(l2upper);
		fib->append(l2lower);
	}

	if (context[0] >= 3) {
		fib->append(l3index);
		fib->append(l3upper);
		fib->append(l3lower);
	}
}

// A generic navigation method.
// Move the current to the element according to the direction constants
// If no such element, no position change and return false (0).
/* Legal direction constants are 	<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 */
bool VTDNav::toElement(navDir direction) {
	int size, i;
	switch (direction) {
	case ROOT:
		if (context[0] != 0) {
			/*for (i = 1; i <= context[0]; i++) {
			 context[i] = 0xffffffff;
			 }*/
			context[0] = 0;
		}
		atTerminal = false;
		//l1index = l2index = l3index = -1;
		return true;
	case PARENT:
		if (atTerminal == true) {
			atTerminal = false;
			return true;
		}
		if (context[0] > 0) {
			//context[context[0]] = context[context[0] + 1] = 0xffffffff;
			//context[context[0]] = -1;
			context[0]--;
			return true;
		} else if (context[0] == 0) {
			context[0] = -1;
			return true;
		} else {
			return false;
		}
	case FIRST_CHILD:
	case LAST_CHILD:
		if (atTerminal)
			return false;
		switch (context[0]) {
		case -1:
			context[0] = 0;
			return true;
		case 0:
			if (l1Buffer->size > 0) {
				context[0] = 1;
				l1index = (direction == FIRST_CHILD) ? 0 : (l1Buffer->size - 1);
				context[1] = l1Buffer->upper32At(l1index);
				//(int) (vtdToken >> 32);
				return true;
			} else
				return false;
		case 1:
			l2lower = l1Buffer->lower32At(l1index);
			if (l2lower == -1) {
				return false;
			}
			context[0] = 2;
			l2upper = l2Buffer->size - 1;
			size = l1Buffer->size;
			for (i = l1index + 1; i < size; i++) {
				int temp = l1Buffer->lower32At(i);
				if (temp != (int) 0xffffffff) {
					l2upper = temp - 1;
					break;
				}
			}
			//System.out.println(" l2 upper: " + l2upper + " l2 lower : " + l2lower);
			l2index = (direction == FIRST_CHILD) ? l2lower : l2upper;
			context[2] = l2Buffer->upper32At(l2index);
			return true;

		case 2:
			l3lower = l2Buffer->lower32At(l2index);
			if (l3lower == -1) {
				return false;
			}
			context[0] = 3;
			size = l2Buffer->size;
			l3upper = l3Buffer->size - 1;

			for (i = l2index + 1; i < size; i++) {
				int temp = l2Buffer->lower32At(i);
				if (temp != (int) 0xffffffff) {
					l3upper = temp - 1;
					break;
				}
			}
			//System.out.println(" l3 upper : " + l3upper + " l3 lower : " + l3lower);
			l3index = (direction == FIRST_CHILD) ? l3lower : l3upper;
			context[3] = l3Buffer->intAt(l3index);

			return true;

		default:
			if (direction == FIRST_CHILD) {
				int index;
				size = vtdBuffer->size;
				index = context[context[0]] + 1;
				while (index < size) {
					Long temp = vtdBuffer->longAt(index);
					int token_type;

					token_type = (int) ((VTDNav::MASK_TOKEN_TYPE & temp) >> 60)
							& 0xf;

					if (token_type == TOKEN_STARTING_TAG) {

						int depth = (int) ((VTDNav::MASK_TOKEN_DEPTH & temp)
								>> 52);

						if (depth <= context[0]) {
							return false;
						} else if (depth == (context[0] + 1)) {
							context[0] += 1;
							context[context[0]] = index;
							return true;
						}
					}

					index++;
				} // what condition
				return false;
			} else {
				int index = context[context[0]] + 1;
				int last_index = -1;
				size = vtdBuffer->size;
				while (index < size) {
					Long temp = vtdBuffer->longAt(index);
					int token_type;
					int depth;

					depth = (int) ((VTDNav::MASK_TOKEN_DEPTH & temp) >> 52);

					token_type = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0xf;

					if (token_type == TOKEN_STARTING_TAG) {
						if (depth <= context[0]) {
							break;
						} else if (depth == (context[0] + 1)) {
							last_index = index;
						}
					}
					index++;
				}

				if (last_index == -1) {
					return false;
				} else {
					context[0] += 1;
					context[context[0]] = last_index;
					return true;
				}
			}
		}

	case NEXT_SIBLING:
	case PREV_SIBLING:
		if (atTerminal)
			return nodeToElement(direction);
		switch (context[0]) {
		case -1:
		case 0:
			return false;
		case 1:
			if (direction == NEXT_SIBLING) {
				if (l1index + 1 >= l1Buffer->size) {
					return false;
				}

				l1index++; // global incremental
			} else {
				if (l1index - 1 < 0) {
					return false;
				}
				l1index--; // global incremental
			}
			context[1] = l1Buffer->upper32At(l1index);
			return true;
		case 2:
			if (direction == NEXT_SIBLING) {
				if (l2index + 1 > l2upper) {
					return false;
				}
				l2index++;
			} else {
				if (l2index - 1 < l2lower) {
					return false;
				}
				l2index--;
			}
			context[2] = l2Buffer->upper32At(l2index);
			return true;
		case 3:
			if (direction == NEXT_SIBLING) {
				if (l3index + 1 > l3upper) {
					return false;
				}
				l3index++;
			} else {
				if (l3index - 1 < l3lower) {
					return false;
				}
				l3index--;
			}
			context[3] = l3Buffer->intAt(l3index);
			return true;
		default:
			//int index = context[context[0]] + 1;

			if (direction == NEXT_SIBLING) {
				int index = context[context[0]] + 1;
				int size = vtdBuffer->size;
				while (index < size) {
					Long temp = vtdBuffer->longAt(index);
					int token_type;

					token_type = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0xf;

					if (token_type == TOKEN_STARTING_TAG) {
						int depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);

						if (depth < context[0]) {
							return false;
						} else if (depth == (context[0])) {
							context[context[0]] = index;
							return true;
						}
					}
					index++;
				}
				return false;
			} else {
				int index = context[context[0]] - 1;
				while (index > context[context[0] - 1]) {
					// scan backforward
					Long temp = vtdBuffer->longAt(index);

					tokenType token_type = (tokenType)(
							((VTDNav::MASK_TOKEN_TYPE & temp) >> 60) & 0xf);

					if (token_type == TOKEN_STARTING_TAG) {

						int depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);

						/*if (depth < context[0]) {
						 return false;
						 } else */
						if (depth == (context[0])) {
							context[context[0]] = index;
							return true;
						}
					}
					index--;
				} // what condition
				return false;
			}
		}

	default:
		return false;
	}

}

/**
 * A generic navigation method.
 * Move the current to the element according to the direction
 * constants and the element name
 * If no such element, no position change and return false (0).
 * "*" matches any element
 * Legal direction constants are 	<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 */
bool VTDNav::toElement(navDir direction, UCSChar *en) {
	//int size;
	int temp = -1;
	int d = -1;
	int val = 0;
	bool b = false;
	if (en == NULL) {
		throw InvalidArgumentException("inavlid argument for toElement2");
	}
	if (wcscmp(en, L"*") == 0)
		return toElement(direction);
	switch (direction) {
	case ROOT:
		return toElement(ROOT);

	case PARENT:
		return toElement(PARENT);

	case FIRST_CHILD:
		if (atTerminal)
			return false;
		if (toElement(FIRST_CHILD) == false)
			return false;
		// check current element name
		if (matchElement(en) == false) {
			if (toElement(NEXT_SIBLING, en) == true)
				return true;
			else {
				//toParentElement();
				//context[context[0]] = 0xffffffff;
				context[0]--;
				return false;
			}
		} else
			return true;

	case LAST_CHILD:
		if (atTerminal)
			return false;
		if (toElement(LAST_CHILD) == false)
			return false;
		if (matchElement(en) == false) {
			if (toElement(PREV_SIBLING, en) == true)
				return true;
			else {
				//context[context[0]] = 0xffffffff;
				context[0]--;
				//toParentElement();
				return false;
			}
		} else
			return true;

	case NEXT_SIBLING:
		if (atTerminal) {
			if (nodeToElement(NEXT_SIBLING)) {
				b = true;
				if (matchElement(en)) {
					return true;
				}
			} else
				return false;
		}

		if (!b) {
			d = context[0];

			switch (d) {
			case -1:
			case 0:
				return false;
			case 1:
				val = l1index;
				break;
			case 2:
				val = l2index;
				break;
			case 3:
				val = l3index;
				break;
			default:
				break;
			}
			temp = context[d]; // store the current position
		}
		//if (d == 0)
		//	return false;
		while (toElement( NEXT_SIBLING)) {
			if (matchElement(en)) {
				return true;
			}
		}

		if (b) {
			context[0]--;//LN value should not change
			atTerminal = true;
			return false;
		} else {
			switch (d) {
			case 1:
				l1index = val;
				break;
			case 2:
				l2index = val;
				break;
			case 3:
				l3index = val;
				break;
			default:
				break;
			}
			context[d] = temp;
			return false;
		}

	case PREV_SIBLING:
		if (atTerminal) {
			if (nodeToElement(PREV_SIBLING)) {
				b = true;
				if (matchElement(en)) {
					return true;
				}
			} else
				return false;
		}
		if (!b) {
			d = context[0];

			switch (d) {
			case -1:
			case 0:
				return false;
			case 1:
				val = l1index;
				break;
			case 2:
				val = l2index;
				break;
			case 3:
				val = l3index;
				break;
			default:
				break;
			}
			temp = context[d]; // store the current position
		}
		//if (d == 0)
		//	return false;
		while (toElement( PREV_SIBLING)) {
			if (matchElement(en)) {
				return true;
			}
		}
		if (b) {
			context[0]--;//LN value should not change
			atTerminal = true;
			return false;
		} else {
			switch (d) {
			case 1:
				l1index = val;
				break;
			case 2:
				l2index = val;
				break;
			case 3:
				l3index = val;
				break;
			default:
				break;
			}
			context[d] = temp;
			return false;
		}

	default:
		return false;
		//throw new NavException("illegal navigation options");
	}
}
/*
 * A generic navigation function with namespace support.
 * Move the current to the element according to the direction constants and the prefix and local names
 * If no such element, no position change and return false(0).
 * URL * matches any namespace, including undefined namespaces
 * a null URL means hte namespace prefix is undefined for the element
 * ln *  matches any localname
 * Legal direction constants are<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 * If not ns enabled, return false immediately with no position change.
 */
bool VTDNav::toElementNS(navDir direction, UCSChar *URL, UCSChar *ln) {
	//int size;
	int temp = -1;
	int d = -1;
	int val = 0;
	bool b = false;

	if (ns == false)
		return false;
	switch (direction) {
	case ROOT:
		return toElement(ROOT);

	case PARENT:
		return toElement(PARENT);

	case FIRST_CHILD:
		if (atTerminal)
			return false;
		if (toElement(FIRST_CHILD) == false)
			return false;
		// check current element name
		if (matchElementNS(URL, ln) == false) {
			if (toElementNS(NEXT_SIBLING, URL, ln) == true)
				return true;
			else {
				//toParentElement();
				//context[context[0]] = 0xffffffff;
				context[0]--;
				return false;
			}
		} else
			return true;

	case LAST_CHILD:
		if (atTerminal)
			return false;
		if (toElement(LAST_CHILD) == false)
			return false;
		if (matchElementNS(URL, ln) == false) {
			if (toElementNS(PREV_SIBLING, URL, ln) == true)
				return true;
			else {
				//context[context[0]] = 0xffffffff;
				context[0]--;
				//toParentElement();
				return false;
			}
		} else
			return true;

	case NEXT_SIBLING:
		if (atTerminal) {
			if (nodeToElement(NEXT_SIBLING)) {
				b = true;
				if (matchElementNS(URL, ln)) {
					return true;
				}
			} else
				return false;
		}
		if (!b) {
			d = context[0];
			temp = context[d]; // store the current position
			switch (d) {
			case -1:
			case 0:
				return false;
			case 1:
				val = l1index;
				break;
			case 2:
				val = l2index;
				break;
			case 3:
				val = l3index;
				break;
			default:
				break;
			}
		}
		//if (d == 0)
		//	return false;
		while (toElement( NEXT_SIBLING)) {
			if (matchElementNS(URL, ln)) {
				return true;
			}
		}
		if (b) {
			context[0]--;//LN value should not change
			atTerminal = true;
			return false;
		} else {
			switch (d) {
			case 1:
				l1index = val;
				break;
			case 2:
				l2index = val;
				break;
			case 3:
				l3index = val;
				break;
			default:
				break;
			}
			context[d] = temp;
			return false;
		}

	case PREV_SIBLING:
		if (atTerminal) {
			if (nodeToElement(PREV_SIBLING)) {
				b = true;
				if (matchElementNS(URL, ln)) {
					return true;
				}
			} else
				return false;
		}
		if (!b) {
			d = context[0];
			temp = context[d]; // store the current position
			switch (d) {
			case -1:
			case 0:
				return false;
			case 1:
				val = l1index;
				break;
			case 2:
				val = l2index;
				break;
			case 3:
				val = l3index;
				break;
			default:
				break;
			}
		}
		//if (d == 0)
		//	return false;
		while (toElement( PREV_SIBLING)) {
			if (matchElementNS(URL, ln)) {
				return true;
			}
		}
		if (b) {
			context[0]--;//LN value should not change
			atTerminal = true;
			return false;
		} else {
			switch (d) {
			case 1:
				l1index = val;
				break;
			case 2:
				l2index = val;
				break;
			case 3:
				l3index = val;
				break;
			default:
				break;
			}
			context[d] = temp;
			return false;
		}

	default:
		return false;
		//throw new NavException("illegal navigation options");
	}

}

//This method normalizes a token into a string in a way that resembles DOM.
//The leading and trailing white space characters will be stripped.
//The entity and character references will be resolved
//Multiple whitespaces char will be collapsed into one.
UCSChar *VTDNav::toNormalizedString(int index) {
	tokenType type = getTokenType(index);

	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return toRawString(index);
	else {
		int len, endOffset;
		int ch, k, offset;
		Long l;
		bool d;
		UCSChar *s = NULL;

		len = getTokenLength(index);

		s = new UCSChar[len + 1];//(UCSChar *)malloc(sizeof(UCSChar)*(len+1));
		if (s == NULL) {
			throw OutOfMemException(" string allocation failed in toString ");
		}

		offset = getTokenOffset(index);
		endOffset = len + offset - 1; // point to the last character


		// trim off the leading whitespaces

		while (true) {
			int temp = offset;
			l = getChar(offset);
			ch = (int) l;
			offset += (UCSChar)(l >> 32);

			if (!isWS(ch)) {
				offset = temp;
				break;
			}
		}

		d = false;
		k = 0;
		while (offset <= endOffset) {
			l = getCharResolved(offset);
			ch = (int) l;
			offset += (int) (l >> 32);

			if (isWS(ch) && getCharUnit(offset - 1) != ';') {
				d = true;
			} else {
				if (d == false)
					//sb.append((char) ch); // java only supports 16 bit unicode
					s[k++] = ch;
				else {
					//sb.append(' ');
					s[k++] = (UCSChar) ' ';
					//sb.append((char) ch);
					s[k++] = (UCSChar) ch;
					d = false;
				}
			}
		}
		s[k] = 0;
		return s;
	}
}

//Convert a token at the given index to a String,
//(built-in entity and char references not resolved)
//(entities and char references not expanded).
//os and len are in bytes
UCSChar *VTDNav::toRawString(int index) {
	int offset;
	tokenType type = getTokenType(index);
	int len;
	//UCSChar *s = NULL;

	if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS)
		len = getTokenLength(index) & 0xffff;
	else
		len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toRawString(offset, len);
}

UCSChar *VTDNav::toRawString(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}
	/*if (encoding > FORMAT_WIN_1258){
	 offset = offset>>1;
	 endOffset = endOffset>>1;
	 }*/
	while (offset < endOffset) {
		l = getChar(offset);
		offset += (int) (l >> 32);
		s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}

//Convert a token at the given index to a String, (entities and char 
//references resolved).
// An attribute name or an element name will get the UCS2 string of qualified name 
//os and len are in bytes
UCSChar *VTDNav::toString(int index) {
	int offset;
	tokenType type = getTokenType(index);
	int len;

	//UCSChar *s = NULL;
	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return toRawString(index);

	len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toString(offset, len);
}

UCSChar *VTDNav::toString(int os, int len) {
	int offset = os, endOffset = os + len, k = 0;
	Long l;
	UCSChar *s = NULL;
	//UCSChar *s = NULL;
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc&) {
		throw OutOfMemException(" string allocation failed in toString ");
	}
	/*if (encoding > FORMAT_WIN_1258){
	 offset = offset>>1;
	 endOffset = endOffset>>1;
	 }*/
	while (offset < endOffset) {
		l = getCharResolved(offset);
		offset += (int) (l >> 32);
		s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}

/**
 * Set the value of atTerminal
 * This function only gets called in XPath eval
 * when a step calls for @* or child::text()
 */

/*void VTDNav::setAtTerminal(bool b){
 atTerminal = b;
 }*/

/**
 * Get the value of atTerminal
 * This function only gets called in XPath eval
 */
/*bool VTDNav::getAtTerminal(){
 return atTerminal;
 }*/

int VTDNav::lookupNS() {
	int i, offset, preLen;

	if (context[0] == -1)
		return false;
	i = getTokenLength((context[0] != 0) ? context[context[0]] : rootIndex);
	offset
			= getTokenOffset(
					(context[0] != 0) ? context[context[0]] : rootIndex);
	preLen = (i >> 16) & 0xffff;

	return lookupNS2(offset, preLen);
}

/* Write VTD+XML into a FILE pointer */
//bool writeIndex_VTDNav(VTDNav * FILE *f){}

/* overwrite */
bool VTDNav::overWrite(int index, UByte* ba, int offset, int len) {
	int t;
	if (ba == NULL || index >= vtdSize || offset < 0) {
		throw InvalidArgumentException("Illegal argument for overwrite");
		return false;
	}
	if (encoding >= FORMAT_UTF_16BE
			&& (((len & 1) == 1) || ((offset & 1) == 1))) {
		// for UTF 16, len and offset must be integer multiple
		// of 2
		return false;
	}
	t = getTokenType(index);
	if (t == TOKEN_CHARACTER_DATA || t == TOKEN_ATTR_VAL || t
			== TOKEN_CDATA_VAL) {
		int os, length, temp, k;
		length = getTokenLength(index);
		if (length < len)
			return false;
		os = getTokenOffset(index);
		temp = length - len;
		// get XML doc
		//Array.Copy(ba, offset, XMLDoc.getBytes(), os, len);
		memcpy(XMLDoc + os, ba + offset, len);
		for (k = 0; k < temp;) {
			if (encoding < FORMAT_UTF_16BE) {
				// write white spaces
				XMLDoc[os + len + k] = ' ';
				k++;
			} else {
				if (encoding == FORMAT_UTF_16BE) {
					XMLDoc[os + len + k] = 0;
					XMLDoc[os + len + k + 1] = ' ';
				} else {
					XMLDoc[os + len + k] = ' ';
					XMLDoc[os + len + k + 1] = 0;
				}
				k += 2;
			}
		}
		return true;
	}
	return false;
}

int VTDNav::compareTokenString(int index, UCSChar *s) {
	tokenType type;
	int offset;
	int len;

	type = getTokenType(index);
	len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	//return compareTokenString2(offset, len, s);
	if (type == TOKEN_CHARACTER_DATA || type == TOKEN_ATTR_VAL)
		return compareTokenString2(getTokenOffset(index), len, s);
	else
		return compareRawTokenString2(getTokenOffset(index), len, s);
}

int VTDNav::compareRawTokenString(int index, UCSChar *s) {
	tokenType type;
	int len;
	int offset;
	/*if (s == NULL){
	 //	 throwException2(invalid_argument,
	 //		 " invalid argument for matchRawTokenString, s can't be NULL");
	 }*/
	type = getTokenType(index);
	len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return compareRawTokenString2(offset, len, s);
}

int VTDNav::compareTokens(int i1, VTDNav *vn2, int i2) {
	int t1, t2;
	int ch1, ch2;
	int len1, len2;
	int endOffset1, endOffset2;
	Long l;
	int offset1, offset2;
	/*if (vn2 ==null){
	 throw new NavException(" One of VTD objects is null!!");
	 }*/

	if (i1 == i2 && this == vn2)
		return 0;

	t1 = getTokenType(i1);
	t2 = vn2->getTokenType(i2);

	offset1 = getTokenOffset(i1);
	offset2 = vn2->getTokenOffset(i2);

	len1 = (t1 == TOKEN_STARTING_TAG || t1 == TOKEN_ATTR_NAME || t1
			== TOKEN_ATTR_NS) ? getTokenLength(i1) & 0xffff
			: getTokenLength(i1);

	len2 = (t2 == TOKEN_STARTING_TAG || t2 == TOKEN_ATTR_NAME || t2
			== TOKEN_ATTR_NS) ? vn2->getTokenLength(i2) & 0xffff
			: vn2->getTokenLength(i2);

	endOffset1 = len1 + offset1;
	endOffset2 = len2 + offset2;

	for (; offset1 < endOffset1 && offset2 < endOffset2;) {
		if (t1 == TOKEN_CHARACTER_DATA || t1 == TOKEN_ATTR_VAL) {
			l = getCharResolved(offset1);
		} else {
			l = getChar(offset1);
		}
		ch1 = (int) l;
		offset1 += (int) (l >> 32);

		if (t2 == TOKEN_CHARACTER_DATA || t2 == TOKEN_ATTR_VAL) {
			l = vn2->getCharResolved(offset2);
		} else {
			l = vn2->getChar(offset2);
		}
		ch2 = (int) l;
		offset2 += (int) (l >> 32);
		if (ch1 > ch2)
			return 1;
		if (ch1 < ch2)
			return -1;
	}

	if (offset1 == endOffset1 && offset2 < endOffset2)
		return -1;
	else if (offset1 < endOffset1 && offset2 == endOffset2)
		return 1;
	else
		return 0;
}

/* Write VTD+XML into a FILE pointer */
bool VTDNav::writeIndex(FILE *f) {
	return IndexHandler::_writeIndex_L3(1, encoding, ns, true,
			nestingLevel - 1, 3, rootIndex, XMLDoc, docOffset, docLen,
			vtdBuffer, l1Buffer, l2Buffer, l3Buffer, f);
}

/* Write VTD+XML into a file of given name */
bool VTDNav::writeIndex(char *fileName) {
	FILE *f = NULL;
	bool b = false;
	f = fopen(fileName, "wb");

	if (f == NULL) {
		throw InvalidArgumentException("fileName not valid");
		//return false;
	}
	b = writeIndex(f);
	fclose(f);
	return b;
}
bool VTDNav::writeSeparateIndex(char *fileName) {
	FILE *f = NULL;
	bool b = false;
	f = fopen(fileName, "wb");

	if (f == NULL) {
		throw InvalidArgumentException("fileName not valid");
		//return false;
	}
	b = writeSeparateIndex(f);
	fclose(f);
	return b;
}
/* Write the VTDs and LCs into an file*/
bool VTDNav::writeSeparateIndex(FILE *f) {

	return IndexHandler::_writeSeparateIndex_L3(1, encoding, ns, true,
			nestingLevel - 1, 3, rootIndex,
			//XMLDoc,
			docOffset, docLen, vtdBuffer, l1Buffer, l2Buffer, l3Buffer, f);
}

/* pre-calculate the VTD+XML index size without generating the actual index */
Long VTDNav::getIndexSize() {
	int size;
	if ((docLen & 7) == 0)
		size = docLen;
	else
		size = ((docLen >> 3) + 1) << 3;

	size += (vtdBuffer->size << 3) + (l1Buffer->size << 3) + (l2Buffer->size
			<< 3);

	if ((l3Buffer->size & 1) == 0) { //even
		size += l3Buffer->size << 2;
	} else {
		size += (l3Buffer->size + 1) << 2; //odd
	}
	return size + 64;
}

/* dump XML text into a given file name */
void VTDNav::dumpXML(char *fileName) {

	FILE *f = fopen(fileName, "wb");
	if (f != NULL) {
		dumpXML(f);
		fclose(f);
	} else {
		throw IOException("can't open file");
		//throwException2(io_exception,"can't open file");
	}
}

/* dump XML text into a given file descriptor */
void VTDNav::dumpXML(FILE *f) {
	size_t i = fwrite(XMLDoc + docOffset, 1, docLen, f);
	if (i < (size_t) docLen) {
		throw IOException("can't complete the write");
		//throwException2(io_exception, "can't complete the write");
	}
}

/*Get the string length as if the token is converted into a normalized UCS string */
int VTDNav::getNormalizedStringLength(int index) {
	tokenType type = getTokenType(index);
	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return getRawStringLength(index);
	else {
		int len, endOffset, len1 = 0;
		int ch, k, offset;
		Long l;
		bool d;

		len = getTokenLength(index);
		offset = getTokenOffset(index);
		endOffset = len + offset - 1; // point to the last character

		// trim off the leading whitespaces
		while (true) {
			int temp = offset;
			l = getChar(offset);
			ch = (int) l;
			offset += (UCSChar)(l >> 32);

			if (!isWS(ch)) {
				offset = temp;
				break;
			}
		}

		d = false;
		k = 0;
		while (offset <= endOffset) {
			l = getCharResolved(offset);
			ch = (int) l;
			offset += (int) (l >> 32);

			if (isWS(ch) && getCharUnit(offset - 1) != ';') {
				d = true;
			} else {
				if (d == false)
					//sb.append((char) ch); // java only supports 16 bit unicode
					len1++;
				else {
					//sb.append(' ');
					len1 = len1 + 2;
					d = false;
				}
			}
		}
		return len1;
	}
}
/*Get the string length as if the token is converted into a UCS string (entity resolved) */
int VTDNav::getStringLength(int index) {
	tokenType type = getTokenType(index);
	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return getRawStringLength(index);
	else {
		int len = 0, len1 = 0, offset, endOffset;
		Long l;
		len = getTokenLength(index);
		offset = getTokenOffset(index);
		endOffset = offset + len;

		while (offset < endOffset) {
			l = getCharResolved(offset);
			offset += (int) (l >> 32);
			len1++;
		}
		return len1;
	}
}
/*Get the string length as if the token is converted into a UCS string (entity not resolved) */
int VTDNav::getRawStringLength(int index) {
	tokenType type = getTokenType(index);
	int len = 0, len1 = 0, offset, endOffset;
	Long l;
	if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS)
		len = getTokenLength(index) & 0xffff;
	else
		len = getTokenLength(index);
	if (encoding != FORMAT_UTF8 && encoding != FORMAT_UTF_16BE && encoding
			!= FORMAT_UTF_16LE) {
		return len;
	}
	offset = getTokenOffset(index);
	endOffset = offset + len;

	while (offset < endOffset) {
		l = getChar(offset);
		offset += (int) (l >> 32);
		len1++;
	}
	return len1;
}
/* Get the offset value right after head (e.g. <a b='b' c='c'> ) */
Long VTDNav::getOffsetAfterHead() {
	int i = getCurrentIndex(), j, offset;
	//encoding_t enc;
	if (getTokenType(i) != TOKEN_STARTING_TAG) {
		return -1;
	}
	j = i + 1;
	while (j < vtdSize && (getTokenType(j) == TOKEN_ATTR_NAME
			|| getTokenType(j) == TOKEN_ATTR_NS)) {
		j += 2;
	}

	if (i + 1 == j) {
		offset = getTokenOffset(i) + ((ns == false) ? getTokenLength(i)
				: (getTokenLength(i) & 0xff));
	} else {
		offset = getTokenOffset(j - 1) + ((ns == false) ? getTokenLength(j - 1)
				: (getTokenLength(j - 1) & 0xff)) + 1;
	}

	while (getCharUnit(offset) != '>') {
		offset++;
	}

	if (getCharUnit(offset - 1) == '/')
		return 0xffffffff00000000LL | (offset);
	else
		return offset + 1;
}

/* Test the start of token content at index i matches the content 
 of s, notice that this is to save the string allocation cost of
 using String's built-in startsWidth */
bool VTDNav::startsWith(int index, UCSChar *s) {
	tokenType type = getTokenType(index);
	int len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	int offset = getTokenOffset(index);
	Long l1;
	size_t l, i;
	int i1;
	int endOffset = offset + len;
	bool b = (type == TOKEN_CHARACTER_DATA || type == TOKEN_ATTR_VAL);
	//       System.out.print("currentOffset :" + currentOffset);
	l = wcslen(s);
	if (l > (size_t) len)
		return false;
	//System.out.println(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		if (b)
			l1 = getCharResolved(offset);
		else
			l1 = getChar(offset);
		i1 = s[i];
		if (i1 != (int) l1)
			return false;
		offset += (int) (l1 >> 32);
	}
	return true;
}

/*Test the end of token content at index i matches the content 
 of s, notice that this is to save the string allocation cost of
 using String's built-in endsWidth */

bool VTDNav::endsWith(int index, UCSChar *s) {
	tokenType type = getTokenType(index);
	size_t len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? (size_t) getTokenLength(index) & 0xffff
			: (size_t) getTokenLength(index);
	int offset = getTokenOffset(index);
	Long l1;
	size_t l, i, i2;
	int i1;
	bool b = (type == TOKEN_CHARACTER_DATA || type == TOKEN_ATTR_VAL);
	//int endOffset = offset + len;

	//       System.out.print("currentOffset :" + currentOffset);
	l = wcslen(s);
	if (l > len)
		return false;
	i2 = (size_t) getStringLength(index);
	if (l > i2)
		return false;
	i2 = i2 - l; // calculate the # of chars to be skipped
	// eat away first several chars
	for (i = 0; i < i2; i++) {
		if (b)
			l1 = getCharResolved(offset);
		else
			l1 = getChar(offset);
		offset += (int) (l1 >> 32);
	}
	//System.out.println(s);
	for (i = 0; i < l; i++) {
		if (b)
			l1 = getCharResolved(offset);
		else
			l1 = getChar(offset);
		i1 = s[i];
		if (i1 != (int) l1)
			return false;
		offset += (int) (l1 >> 32);
	}
	return true;
}

/*Test whether a given token contains s. notie that this function
 directly operates on the byte content of the token to avoid string creation */

bool VTDNav::contains(int index, UCSChar *s) {
	tokenType type = getTokenType(index);
	size_t len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);
	int offset = getTokenOffset(index);
	Long l1;
	size_t l, i;
	int i1;
	int endOffset = offset + len;

	bool b = (type == TOKEN_CHARACTER_DATA || type == TOKEN_ATTR_VAL);
	//       System.out.print("currentOffset :" + currentOffset);
	int gOffset = offset;
	l = wcslen(s);
	if (l > len)
		return false;
	//System.out.println(s);
	while (offset < endOffset) {
		gOffset = offset;
		for (i = 0; i < l && gOffset < endOffset; i++) {
			if (b)
				l1 = getCharResolved(gOffset);
			else
				l1 = getChar(gOffset);
			i1 = s[i];
			gOffset += (int) (l1 >> 32);
			if (i == 0)
				offset = gOffset;
			if (i1 != (int) l1)
				break;
		}
		if (i == l)
			return true;
	}
	return false;
}

/* Convert a token at the given index to a String and any lower case
 character will be converted to upper case, (entities and char
 references resolved).*/
UCSChar *VTDNav::toStringUpperCase(int index) {
	int offset, len;
	tokenType type = getTokenType(index);
	//UCSChar *s = NULL;

	if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS)
		len = getTokenLength(index) & 0xffff;
	else
		len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toRawStringUpperCase(offset, len);
}

/* Convert a token at the given index to a String and any upper case
 character will be converted to lower case, (entities and char
 references resolved).*/
UCSChar *VTDNav::toStringLowerCase(int index) {
	int offset, len;
	tokenType type = getTokenType(index);

	//UCSChar *s = NULL;
	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return toRawStringLowerCase(index);

	len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toStringLowerCase(offset, len);
}

/* Convert a token at the given index to a String and any lower case
 character will be converted to upper case, (entities and char
 references resolved for character data and attr val).*/
UCSChar *VTDNav::toRawStringUpperCase(int index) {
	int offset, len;
	tokenType type = getTokenType(index);
	//UCSChar *s = NULL;

	if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS)
		len = getTokenLength(index) & 0xffff;
	else
		len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toRawStringUpperCase(offset, len);
}

/* Convert a token at the given index to a String and any upper case
 character will be converted to lower case, (entities and char
 references resolved for character data and attr val).*/
UCSChar *VTDNav::toRawStringLowerCase(int index) {
	int offset, len;
	tokenType type = getTokenType(index);
	//UCSChar *s = NULL;

	if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS)
		len = getTokenLength(index) & 0xffff;
	else
		len = getTokenLength(index);

	offset = getTokenOffset(index);
	return toRawStringLowerCase(offset, len);
}

/* DupliateNav duplicates an instance of VTDNav but doesn't retain the original node position*/
VTDNav *VTDNav::duplicateNav() {
	VTDNav* vn1 = new VTDNav(rootIndex, encoding, ns, nestingLevel - 1, XMLDoc,
			bufLen, vtdBuffer, l1Buffer, l2Buffer, l3Buffer, docOffset, docLen,
			br);
	vn1->master = false;
	return vn1;
}
/* ClineNav duplicates an instance of VTDNav, also copies node position over */
VTDNav *VTDNav::cloneNav() {
	VTDNav* vn1 = new VTDNav(rootIndex, encoding, ns, nestingLevel - 1, XMLDoc,
			bufLen, vtdBuffer, l1Buffer, l2Buffer, l3Buffer, docOffset, docLen,
			br);
	vn1->master = false;
	vn1->atTerminal = atTerminal;
	vn1->LN = LN;
	if (context[0] != -1)
		memcpy(vn1->context, context, context[0]);
	//System.arraycopy(vn.context, 0, vn.context, 0, this.context[0] );
	else
		vn1->context[0] = -1;
	vn1->l1index = l1index;
	if (getCurrentDepth() > 1) {
		vn1->l2index = l2index;
		vn1->l2upper = l2upper;
		vn1->l2lower = l2lower;
	}
	if (getCurrentDepth() > 2) {
		vn1->l3lower = l3lower;
		vn1->l3index = l3index;
		vn1->l3upper = l3upper;
	}
	return vn1;
}

/* This method takes a vtd index, and recover its correspondin
 * node position, the index can only be of node type element,
 * document, attribute name, attribute value or character data,
 * or CDATA  */
void VTDNav::recoverNode(int index) {
	int d, type, t;
	//exception e;
	if (index < 0 || index >= vtdSize) {
		throw InvalidArgumentException("Invalid VTD index");
		//throw new NavException("Invalid VTD index");
	}

	type = getTokenType(index);

	if (//type == VTDNav.TOKEN_COMMENT ||
	//	type == VTDNav.TOKEN_PI_NAME ||
	type == TOKEN_PI_VAL || type == TOKEN_DEC_ATTR_NAME || type
			== TOKEN_DEC_ATTR_VAL || type == TOKEN_ATTR_VAL) {
		throw NavException("Invalid VTD index");
		//throw new NavException("Token type not yet supported");
	}
	// get depth
	d = getTokenDepth(index);
	// handle document node;	
	switch (d) {
	case -1:
		context[0] = -1;
		if (index != 0) {
			LN = index;
			atTerminal = true;
		} else
			atTerminal = false;

		return;
	case 0:
		context[0] = 0;
		if (index != rootIndex) {
			LN = index;
			atTerminal = true;
		} else
			atTerminal = false;
		return;
	}
	context[0] = d;
	if (type != TOKEN_STARTING_TAG) {
		LN = index;
		atTerminal = true;
	} else
		atTerminal = false;
	// search LC level 1
	recoverNode_l1(index);

	if (d == 1) {
		if (atTerminal && type > TOKEN_ATTR_NS)
			sync(1, index);
		return;
	}
	// search LC level 2
	recoverNode_l2(index);
	if (d == 2) {
		//resolveLC();
		if (atTerminal && type > TOKEN_ATTR_NS)
			sync(2, index);
		return;
	}
	// search LC level 3
	recoverNode_l3(index);
	if (d == 3) {
		//resolveLC();
		if (atTerminal && type > TOKEN_ATTR_NS)
			sync(3, index);
		return;
	}
	// scan backward
	if (type == TOKEN_STARTING_TAG) {
		context[d] = index;
	} else {
		int t = index - 1;
		while (!(getTokenType(t) == TOKEN_STARTING_TAG && getTokenDepth(t) == d)) {
			t--;
		}
		context[d] = t;
	}
	t = context[d] - 1;
	d--;
	while (d > 3) {
		while (!(getTokenType(t) == TOKEN_STARTING_TAG && getTokenDepth(t) == d)) {
			t--;
		}
		context[d] = t;
		d--;
	}
}

/*tokenType VTDNav::getTokenType(int index){
 return (tokenType) (((vtdBuffer->longAt(index) & VTDNav::MASK_TOKEN_TYPE) >> 60) & 0xf);
 }

 bool VTDNav::isElement(int index){
 return (((vtdBuffer->longAt(index) & VTDNav::MASK_TOKEN_TYPE) >> 60) & 0xf)
 == TOKEN_STARTING_TAG;
 }
 bool VTDNav::isElementOrDocument( int index){
 int i = 0;
 i= (int)(((vtdBuffer->longAt(index) & VTDNav::MASK_TOKEN_TYPE) >> 60) & 0xf);
 return (i == TOKEN_STARTING_TAG || i == TOKEN_DOCUMENT);
 }*/

Long VTDNav::getSiblingElementFragments(int i) {
	int so, len;
	Long l;
	BookMark *bm;
	if (i <= 0)
		throw InvalidArgumentException(
				" # of sibling can be less or equal to 0");
	// get starting char offset
	if (atTerminal == true)
		return -1L;
	// so is the char offset
	so = getTokenOffset(getCurrentIndex()) - 1;
	// char offset to byte offset conversion
	if (encoding >= FORMAT_UTF_16BE)
		so = so << 1;
	bm = new BookMark(this);
	bm->recordCursorPosition();
	while (i > 1 && toElement(NEXT_SIBLING)) {
		i--;
	}
	l = getElementFragment();
	len = (int) l + (int) (l >> 32) - so;
	if (i == 1 && toElement(NEXT_SIBLING)) {
	} else
		bm->setCursorPosition(this);
	delete (bm);
	return (((Long) len) << 32) | so;
}

int VTDNav::compareNormalizedTokenString2(int offset, int len, const UCSChar *s) {
	int i, i1, l, temp;
	Long l1, l2;
	//boolean b = false;
	// this.currentOffset = offset;
	int endOffset = offset + len;

	// System.out.print("currentOffset :" + currentOffset);
	l = wcslen(s);
	// System.out.println(s);
	for (i = 0; i < l && offset < endOffset;) {
		l1 = getCharResolved(offset);
		temp = (int) l1;
		l2 = (l1 >> 32);
		if (l2 <= 2 && isWS(temp))
			temp = ' ';
		i1 = s[i];
		if (i1 < temp)
			return 1;
		if (i1 > temp)
			return -1;
		i++;
		offset += (int) (l1 >> 32);
	}

	if (i == l && offset < endOffset)
		return 1;
	if (i < l && offset == endOffset)
		return -1;
	return 0;
	// return -1;
}

UCSChar* VTDNav::getPrefixString(int i) {
	if (ns == false)
		return NULL;

	tokenType type = getTokenType(i);
	if (type != TOKEN_ATTR_NAME && type != TOKEN_STARTING_TAG)
		return NULL;
	else {
		int offset = getTokenOffset(i);
		int preLen = getTokenLength(i) >> 16;
		if (preLen != 0)
			return toRawString(offset, preLen);
	}
	return NULL;

}

bool VTDNav::matchNormalizedTokenString2(int index, const UCSChar *s) {
	tokenType type = getTokenType(index);
	int len = (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type
			== TOKEN_ATTR_NS) ? getTokenLength(index) & 0xffff
			: getTokenLength(index);

	return compareNormalizedTokenString2(getTokenOffset(index), len, s) == 0;
}

UCSChar* VTDNav::toNormalizedString2(int index) {
	tokenType type = getTokenType(index);
	Long l;
	UCSChar *s;
	int ch, k;
	int len, offset, endOffset;
	if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
		return toRawString(index);

	len = getTokenLength(index);
	if (len == 0)
		return wcsdup(L"");
	offset = getTokenOffset(index);
	endOffset = len + offset - 1; // point to the last character
	try {
		s = new UCSChar[len + 1];
	} catch (std::bad_alloc &) {
		throw OutOfMemException(
				" string allocation failed in toNormalizedString2 ");
	}

	//boolean d = false;
	while (offset <= endOffset) {
		l = getCharResolved(offset);
		ch = (int) l;
		offset += (int) (l >> 32);
		if (isWS(ch) && (l >> 32) <= 2) {
			//d = true;
			s[k++] = (UCSChar) ' ';
			//sb.append(' ');
		} else {
			s[k++] = (UCSChar) ch;
			//sb.append((char) ch);
		}
	}
	return s;
}

ElementFragmentNs* VTDNav::getElementFragmentNs() {

	if (ns == false) {
		throw NavException(
				"getElementFragmentNS can only be called when ns is tured on");
		return NULL;
	} else {
		//fill the fib with integer 
		// first get the list of name space nodes 
		FastIntBuffer *fib = NULL;
		int i = 0;
		int count = 0;
		int type;

		int* ia = context;
		int d = ia[0]; // -1 for document node, 0 for root element;
		int c = getCurrentIndex2();
		int len = (c == 0 || c == rootIndex) ? 0 : (getTokenLength(c) & 0xffff); // get the length of qualified node
		int newSz;
		Long l;
		fib = new FastIntBuffer(3); // init size 8


		// put the neighboring ATTR_NS nodes into the array
		// and record the total # of them	     

		if (d > 0) { // depth > 0 every node except document and root element
			int k = getCurrentIndex2() + 1;
			if (k < vtdSize) {
				while (k < vtdSize) {
					type = getTokenType(k);
					if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS) {
						if (type == TOKEN_ATTR_NS) {
							fib->append(k);
							//System.out.println(" ns name ==>" + toString(k));
						}
					}
					k += 2;
					//type = getTokenType(vn,k);
				}

			}
			count = fib->size;
			d--;
			while (d >= 0) {
				// then search for ns node in the vinicity of the ancestor nodes
				if (d > 0) {
					// starting point
					k = ia[d] + 1;
				} else {
					// starting point
					k = rootIndex + 1;
				}
				if (k < vtdSize) {

					while (k < vtdSize) {
						type = getTokenType(k);
						if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS) {
							bool unique = true;
							if (type == TOKEN_ATTR_NS) {
								int z = 0;
								for (z = 0; z < fib->size; z++) {
									//System.out.println("fib size ==> "+fib.size());
									//if (fib->size==4);
									if (matchTokens(fib->intAt(z), this, k)) {
										unique = false;
										break;
									}

								}
								if (unique)
									fib->append(k);
							}
						}
						k += 2;
						//type = getTokenType(vn,k);
					}
				}
				d--;
			}
			// System.out.println("count ===> "+count);
			// then restore the name space node by shifting the array
			newSz = fib->size - count;
			for (i = 0; i < newSz; i++) {
				fib->modifyEntry(i, fib->intAt(i + count));
			}
			fib->resize(newSz);
		}

		l = getElementFragment();
		return new ElementFragmentNs(this, l, fib, len);
	}
}

void VTDNav::dumpState() {
	printf("l1 index ==>%d \n" + l1index);
	printf("l2 index ==>%d \n", l2index);
	printf("l2 lower ==>%d \n", l2lower);
	printf("l2 upper ==>%d \n", l2upper);
	printf("l3 index ==>%d \n", l3index);
	printf("l3 lower ==>%d \n", l3lower);
	printf("l3 upper ==>%d \n", l3upper);
}

void VTDNav::fillXPathString(FastIntBuffer* indexBuffer,
		FastIntBuffer *countBuffer) {
	int count = 0;
	int index = getCurrentIndex() + 1;
	int tokenType, depth, t = 0, length, i = 0;
	int dp = context[0];
	//int size = vtdBuffer.size;
	// store all text tokens underneath the current element node
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		depth = getTokenDepth(index);
		if (depth < dp || (depth == dp && tokenType == TOKEN_STARTING_TAG)) {
			break;
		}

		if (tokenType == TOKEN_CHARACTER_DATA || tokenType == TOKEN_CDATA_VAL) {
			length = getTokenLength(index);
			t += length;
			fib->append(index);
			if (length > VTDGen::MAX_TOKEN_LENGTH) {
				while (length > VTDGen::MAX_TOKEN_LENGTH) {
					length -= VTDGen::MAX_TOKEN_LENGTH;
					i++;
				}
				index += i + 1;
			} else
				index++;
			continue;
			//
		} else if (tokenType == TOKEN_ATTR_NAME || tokenType == TOKEN_ATTR_NS) {
			index = index + 2;
			continue;
		}
		index++;
	}
}

Long VTDNav::getOffsetBeforeTail() {
	Long l = getElementFragment();

	int offset = (int) l;
	int len = (int) (l >> 32);
	// byte to char conversion
	if (encoding >= FORMAT_UTF_16BE) {
		offset <<= 1;
		len <<= 1;
	}
	offset += len;
	if (getCharUnit(offset - 2) == '/')
		return 0xffffffff00000000LL | offset - 1;
	else {
		offset -= 2;
		while (getCharUnit(offset) != '/') {
			offset--;
		}
		return offset - 1;
	}
	//return 1;
}

bool VTDNav::toNode_LastChild() {
	int depth, index, tokenType, lastEntry, tmp;
	switch (context[0]) {
	case -1:
		index = vtdSize - 1;
		tokenType = getTokenType(index);
		depth = getTokenDepth(index);
		if (depth == -1) {
			switch (tokenType) {
			case TOKEN_COMMENT:
				LN = index;
				atTerminal = true;
				return true;
			case TOKEN_PI_VAL:
				LN = index - 1;
				atTerminal = true;
				return true;
			}
		}
		context[0] = 0;
		return true;

	case 0:
		if (l1Buffer->size != 0) {
			lastEntry = l1Buffer->upper32At(l1Buffer->size - 1);
			index = vtdSize - 1;
			while (index > lastEntry) {
				depth = getTokenDepth(index);
				if (depth == -1) {
					index--;
					continue;
				} else if (depth == 0) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_CHARACTER_DATA:
					case TOKEN_COMMENT:
					case TOKEN_CDATA_VAL:
						LN = index;
						atTerminal = true;
						l1index = l1Buffer->size - 1;
						return true;
					case TOKEN_PI_VAL:
						LN = index - 1;
						atTerminal = true;
						l1index = l1Buffer->size - 1;
						return true;
					default:
						return false;
					}
				} else {
					l1index = l1Buffer->size - 1;
					context[0] = 1;
					context[1] = lastEntry;
					return true;
				}
			}
			l1index = l1Buffer->size - 1;
			context[0] = 1;
			context[1] = lastEntry;
			return true;
		} else {
			index = vtdSize - 1;
			while (index > rootIndex) {
				depth = getTokenDepth(index);
				if (depth == -1) {
					index--;
					continue;
				}
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_CHARACTER_DATA:
				case TOKEN_COMMENT:
				case TOKEN_CDATA_VAL:
					LN = index;
					atTerminal = true;
					return true;
				case TOKEN_PI_VAL:
					LN = index - 1;
					atTerminal = true;
					return true;
				default:
					return false;
				}
			}
			return false;
		}

	case 1:
		if (l1Buffer->lower32At(l1index) != -1) {
			l2lower = l1Buffer->lower32At(l1index);
			tmp = l1index + 1;
			while (tmp < l1Buffer->size) {
				if (l1Buffer->lower32At(tmp) != -1) {
					l2upper = l1Buffer->lower32At(tmp) - 1;
					break;
				} else
					tmp++;
			}
			if (tmp == l1Buffer->size) {
				l2upper = l2Buffer->size - 1;
			}
			l2index = l2upper;
			index = vtdSize - 1;
			if (l1index != l1Buffer->size - 1) {
				index = l1Buffer->upper32At(l1index + 1) - 1;
			}
			tmp = l2Buffer->upper32At(l2index);
			// rewind and find the first node of depth 1
			while (index > tmp) {
				depth = getTokenDepth(index);
				if (depth < 1)
					index--;
				else if (depth == 1) {
					tokenType = getTokenType(index);
					if (tokenType == TOKEN_PI_VAL)
						LN = index - 1;
					else
						LN = index;
					atTerminal = true;
					//context[0]=1;
					return true;
				} else
					break;
			}
			context[0] = 2;
			context[2] = tmp;
			return true;

		} else {
			index = context[1] + 1;
			while (index < vtdSize) {
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_ATTR_NAME:
				case TOKEN_ATTR_NS:
					index += 2;
					break;
				default:
					goto loop1;
				}
			}
			loop1: if (index < vtdSize && getTokenDepth(index) == 1
					&& getTokenType(index) != TOKEN_STARTING_TAG) {
				lastEntry = index;
				index++;
				//scan forward
				while (index < vtdSize) {
					tokenType = getTokenType(index);
					depth = getTokenDepth(index);
					if (depth == 1) {
						switch (tokenType) {
						case TOKEN_CHARACTER_DATA:
						case TOKEN_COMMENT:
						case TOKEN_CDATA_VAL:
							lastEntry = index;
							index++;
							break;
						case TOKEN_PI_NAME:
							lastEntry = index;
							index += 2;
							break;
						default:
							goto loop2;
						}
					} else
						goto loop2;
				}
				loop2: LN = lastEntry;
				atTerminal = true;
				return true;
			} else
				return false;
		}

	case 2:
		if (l2Buffer->lower32At(l2index) != -1) {
			l3lower = l2Buffer->lower32At(l2index);
			tmp = l2index + 1;
			while (tmp < l2Buffer->size) {
				if (l2Buffer->lower32At(tmp) != -1) {
					l3upper = l2Buffer->lower32At(tmp) - 1;
					break;
				} else
					tmp++;
			}
			if (tmp == l2Buffer->size) {
				l3upper = l3Buffer->size - 1;
			}
			l3index = l3upper;
			index = vtdSize - 1;

			if (l1index != l1Buffer->size - 1) {
				index = l1Buffer->upper32At(l1index + 1) - 1;
			}

			if (l2index != l2Buffer->size - 1 && l2index != l2upper) {
				index = l2Buffer->upper32At(l2index + 1) - 1;
			}
			tmp = l3Buffer->intAt(l3index);
			// rewind and find the first node of depth 1
			while (index > tmp) {
				depth = getTokenDepth(index);
				if (depth < 2)
					index--;
				else if (depth == 2) {
					tokenType = getTokenType(index);
					if (tokenType == TOKEN_PI_VAL)
						LN = index - 1;
					else
						LN = index;
					atTerminal = true;
					//context[0]=1;
					return true;
				} else
					break;
			}
			context[0] = 3;
			context[3] = tmp;
			return true;
		} else {
			index = context[2] + 1;
			while (index < vtdSize) {
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_ATTR_NAME:
				case TOKEN_ATTR_NS:
					index += 2;
					break;
				default:
					goto loop3;
				}
			}
			loop3: if (index < vtdSize && getTokenDepth(index) == 2
					&& getTokenType(index) != TOKEN_STARTING_TAG) {
				lastEntry = index;
				index++;
				//scan forward
				while (index < vtdSize) {
					tokenType = getTokenType(index);
					depth = getTokenDepth(index);
					if (depth == 2) {
						switch (tokenType) {
						case TOKEN_CHARACTER_DATA:
						case TOKEN_COMMENT:
						case TOKEN_CDATA_VAL:
							lastEntry = index;
							index++;
							break;
						case TOKEN_PI_NAME:
							lastEntry = index;
							index += 2;
							break;
						default:
							goto loop4;
						}
					} else
						goto loop4;
				}
				loop4: LN = lastEntry;
				atTerminal = true;
				return true;
			} else
				return false;
		}
	default:
		index = context[context[0]] + 1;
		lastEntry = -1;
		atTerminal = false;
		while (index < vtdBuffer->size) {
			Long temp = vtdBuffer->longAt(index);
			tokenType = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0x0f;
			depth = getTokenDepth(index);
			switch (tokenType) {
			case TOKEN_STARTING_TAG:
				if (depth <= context[0]) {
					if (lastEntry != -1) {
						if (atTerminal) {
							LN = lastEntry;
						} else {
							context[0] += 1;
							context[context[0]] = lastEntry;
							atTerminal = false;
						}
						return true;
					} else
						return false;
				} else if (depth == (context[0] + 1)) {
					lastEntry = index;
					atTerminal = false;
				}
				index++;
				break;
			case TOKEN_ATTR_NAME:
			case TOKEN_ATTR_NS:
				index += 2;
				break;
			case TOKEN_CHARACTER_DATA:
			case TOKEN_COMMENT:
			case TOKEN_CDATA_VAL:
				if (depth < context[0]) {
					if (lastEntry != -1) {
						if (atTerminal) {
							LN = lastEntry;
						} else {
							context[0]++;
							context[context[0]] = lastEntry;
						}
						return true;
					} else
						return false;
				} else if (depth == (context[0])) {
					lastEntry = index;
					atTerminal = true;
				}
				index++;
				break;
			case TOKEN_PI_NAME:
				if (depth < context[0]) {
					if (lastEntry != -1) {
						if (atTerminal) {
							LN = lastEntry;
						} else {
							context[0]++;
							context[context[0]] = lastEntry;
						}
						return true;
					} else
						return false;
				} else if (depth == (context[0])) {
					lastEntry = index;
					atTerminal = true;
				}
				index += 2;
				break;
			}
			//index++;
		} // what condition
		if (lastEntry != -1) {
			if (atTerminal) {
				LN = lastEntry;
			} else {
				context[0]++;
				context[context[0]] = lastEntry;
			}
			return true;
		} else
			return false;
	}
}

bool VTDNav::toNode_PrevSibling() {
	int index, tokenType, depth, tmp;
	switch (context[0]) {
	case -1:
		if (atTerminal) {
			index = LN - 1;
			if (index > 0) {
				depth = getTokenDepth(index);
				if (depth == -1) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_PI_VAL:
						index--;
					case TOKEN_COMMENT:
						LN = index;
						return true;
					default:
						return false;
					}
				} else {
					context[0] = 0;
					atTerminal = false;
					return true;
				}
			} else {
				return false;
			}
		} else {
			return false;
		}

	case 0:
		if (atTerminal) {
			if (l1Buffer->size != 0) {
				// three cases
				if (LN < l1Buffer->upper32At(l1index)) {
					index = LN - 1;
					if (index > rootIndex) {
						tokenType = getTokenType(index);
						depth = getTokenDepth(index);
						if (depth == 0) {
							switch (tokenType) {
							case TOKEN_CHARACTER_DATA:
							case TOKEN_COMMENT:
							case TOKEN_CDATA_VAL:
								LN = index;
								return true;
							case TOKEN_PI_VAL:
								LN = index - 1;
								return true;
							}
						}
						if (l1index == 0)
							return false;
						l1index--;
						atTerminal = false;
						context[0] = 1;
						context[1] = l1Buffer->upper32At(l1index);
						return true;
					} else
						return false;
				} else {
					index = LN - 1;
					if (index > l1Buffer->upper32At(l1index)) {
						tokenType = getTokenType(index);
						depth = getTokenDepth(index);
						if (depth == 0) {
							switch (tokenType) {
							case TOKEN_CHARACTER_DATA:
							case TOKEN_COMMENT:
							case TOKEN_CDATA_VAL:
								LN = index;
								return true;
							case TOKEN_PI_VAL:
								LN = index - 1;
								return true;
							}
						}
					}
					atTerminal = false;
					context[0] = 1;
					context[1] = l1Buffer->upper32At(l1index);
					return true;
				}
			} else {
				index = LN - 1;
				if (index > rootIndex) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_PI_VAL:
						index--;
					case TOKEN_CHARACTER_DATA:
					case TOKEN_COMMENT:
					case TOKEN_CDATA_VAL:

						LN = index;
						atTerminal = true;
						context[0] = 0;
						return true;
					default:
						return false;
					}
				}
				return false;
			}

		} else {
			index = rootIndex - 1;
			if (index > 0) {
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_PI_VAL:
					index--;
				case TOKEN_COMMENT:
					LN = index;
					atTerminal = true;
					context[0] = -1;
					return true;
				default:
					return false;
				}
			} else {
				return false;
			}
		}
		//break;
	case 1:
		if (atTerminal) {
			if (l1Buffer->lower32At(l1index) != -1) {
				tmp = l2Buffer->upper32At(l2index);
				if (LN > tmp) {
					index = LN - 1;
					if (getTokenType(index) == TOKEN_PI_VAL) {
						index--;
					}
					if (getTokenDepth(index) == 1) {
						LN = index;
						return true;
					} else {
						atTerminal = false;
						context[0] = 2;
						context[2] = tmp;
						return true;
					}
				} else if (l2index != l2lower) {
					l2index--;
					atTerminal = false;
					context[0] = 2;
					context[2] = l2Buffer->upper32At(l2index);
					return true;
				} else {
					index = LN - 1;
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_PI_VAL:
						index--;
					case TOKEN_CHARACTER_DATA:
					case TOKEN_COMMENT:
					case TOKEN_CDATA_VAL:

						LN = index;
						atTerminal = true;
						context[0] = 1;
						return true;
					default:
						return false;
					}
				}
			} else {
				index = LN - 1;
				if (getTokenType(index) == TOKEN_PI_VAL)
					index--;
				if (index > context[1]) {
					tokenType = getTokenType(index);
					if (tokenType != TOKEN_ATTR_VAL) {
						LN = index;
						atTerminal = true;
						return true;
					} else
						return false;
				} else {
					return false;
				}
			}
		} else {
			index = context[1] - 1;
			tokenType = getTokenType(index);
			if (getTokenDepth(index) == 0 && tokenType != TOKEN_ATTR_VAL
					&& tokenType != TOKEN_STARTING_TAG) {
				if (tokenType == TOKEN_PI_VAL)
					index--;
				context[0] = 0;
				atTerminal = true;
				LN = index;
				return true;
			} else {
				// no more prev sibling element
				if (l1index != 0) {
					l1index--;
					context[1] = l1Buffer->upper32At(l1index);
					return true;
				} else
					return false;
			}
		}
		//break;
	case 2:
		if (atTerminal) {
			if (l2Buffer->lower32At(l2index) != -1) {
				tmp = l3Buffer->intAt(l3index);
				if (LN > tmp) {
					index = LN - 1;
					if (getTokenType(index) == TOKEN_PI_VAL) {
						index--;
					}
					if (getTokenDepth(index) == 2) {
						LN = index;
						return true;
					} else {
						atTerminal = false;
						context[0] = 3;
						context[3] = tmp;
						return true;
					}
				} else if (l3index != l3lower) {
					l3index--;
					atTerminal = false;
					context[0] = 3;
					context[3] = l3Buffer->intAt(l3index);
					return true;
				} else {
					index = LN - 1;
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_PI_VAL:
						index--;
					case TOKEN_CHARACTER_DATA:
					case TOKEN_COMMENT:
					case TOKEN_CDATA_VAL:

						LN = index;
						atTerminal = true;
						context[0] = 2;
						return true;
					default:
						return false;
					}
				}
			} else {
				index = LN - 1;
				if (getTokenType(index) == TOKEN_PI_VAL)
					index--;
				if (index > context[2]) {
					tokenType = getTokenType(index);
					if (tokenType != TOKEN_ATTR_VAL) {
						LN = index;
						atTerminal = true;
						return true;
					} else
						return false;
				} else {
					return false;
				}
			}
		} else {
			index = context[2] - 1;
			tokenType = getTokenType(index);
			if (getTokenDepth(index) == 1 && tokenType != TOKEN_ATTR_VAL
					&& tokenType != TOKEN_STARTING_TAG) {
				if (tokenType == TOKEN_PI_VAL)
					index--;
				context[0] = 1;
				atTerminal = true;
				LN = index;
				return true;
			} else {
				// no more prev sibling element
				if (l2index != l2lower) {
					l2index--;
					context[2] = l2Buffer->upper32At(l2index);
					return true;
				} else
					return false;
			}
		}
		//break;
	case 3:
		if (!atTerminal) {
			index = context[3] - 1;
			tokenType = getTokenType(index);
			if (getTokenDepth(index) == 2 && tokenType != TOKEN_ATTR_VAL
					&& tokenType != TOKEN_STARTING_TAG) {
				if (tokenType == TOKEN_PI_VAL)
					index--;
				context[0] = 2;
				atTerminal = true;
				LN = index;
				return true;
			} else {
				// no more prev sibling element
				if (l3index != l3lower) {
					l3index--;
					context[3] = l3Buffer->intAt(l3index);
					return true;
				} else
					return false;
			}
		}
		//break;
	default:
		if (atTerminal) {
			index = LN - 1;
			tmp = context[0] + 1;
		} else {
			index = context[context[0]] - 1;
			tmp = context[0];
		}
		while (index > context[tmp - 1]) {
			Long temp = vtdBuffer->longAt(index);
			tokenType = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0x0f;
			depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
			switch (tokenType) {
			case TOKEN_STARTING_TAG:

				/*if (depth < tmp) {
				 return false;
				 } else*/
				if (depth == tmp) {
					context[0] = depth;
					context[context[0]] = index;
					atTerminal = false;
					return true;
				} else
					index--;
				break;
			case TOKEN_ATTR_VAL:
				//case TOKEN_ATTR_NS:
				index -= 2;
				break;
			case TOKEN_CHARACTER_DATA:
			case TOKEN_COMMENT:
			case TOKEN_CDATA_VAL:
				//depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
				/*if (depth < tmp-1) {
				 return false;
				 } else*/
				if (depth == tmp - 1) {
					context[0] = tmp - 1;
					LN = index;
					atTerminal = true;
					return true;
				} else
					index--;
				break;
			case TOKEN_PI_VAL:
				//depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
				/*if (depth < context[0]) {
				 return false;
				 } else*/
				if (depth == (tmp - 1)) {
					context[0] = tmp - 1;
					LN = index - 1;
					atTerminal = true;
					return true;
				} else
					index -= 2;
				break;
			default:
				index--;
			}

		}
		return false;
	}
}

bool VTDNav::toNode(int dir) {
	int index, tokenType, depth, lastEntry, tmp;
	//count++;
	//System.out.println("count ==>"+ count);
	switch (dir) {
	case ROOT:
		if (context[0] != 0) {
			/*
			 * for (int i = 1; i <= context[0]; i++) { context[i] =
			 * 0xffffffff; }
			 */
			context[0] = 0;
		}
		atTerminal = false;
		//l1index = l2index = l3index = -1;
		return true;
	case PARENT:
		if (atTerminal == true) {
			atTerminal = false;
			return true;
		}
		if (context[0] > 0) {
			//context[context[0]] = context[context[0] + 1] =
			// 0xffffffff;
			context[context[0]] = -1;
			context[0]--;
			return true;
		} else if (context[0] == 0) {
			context[0] = -1; //to be compatible with XPath Data model
			return true;
		} else {
			return false;
		}
	case FIRST_CHILD:
		if (atTerminal)
			return false;
		switch (context[0]) {
		case -1:
			//starting with root element
			//scan backward, if there is a pi | comment node
			index = rootIndex - 1;

			while (index > 0) {
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_COMMENT:
					index--;
					break;
				case TOKEN_PI_VAL:
					index -= 2;
					break;
				default:
					goto loop1;
				}
			}
			loop1: index++; // points to
			if (index != rootIndex) {
				atTerminal = true;
				LN = index;
			} else {
				context[0] = 0;
			}
			return true;
		case 0:
			if (l1Buffer->size != 0) {
				index = l1Buffer->upper32At(0) - 1;
				//rewind
				while (index > rootIndex) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_CHARACTER_DATA:
					case TOKEN_COMMENT:
					case TOKEN_CDATA_VAL:
						index--;
						break;
					case TOKEN_PI_VAL:
						index -= 2;
						break;
					default:
						goto loop2;
					}
				}
				loop2: index++;
				l1index = 0;
				if (index == l1Buffer->upper32At(0)) {
					context[0] = 1;
					context[1] = l1Buffer->upper32At(0);
					atTerminal = false;
				} else {
					atTerminal = true;
					LN = index;
				}
				return true;

			} else {
				//get to the first non-attr node after the starting tag
				index = rootIndex + 1;
				while (index < vtdSize) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_ATTR_NAME:
					case TOKEN_ATTR_NS:
						index += 2;
						break;
					default:
						if (getTokenDepth(index) == 0) {
							atTerminal = true;
							LN = index;
							return true;
						} else
							return false;

					}
				}
				return false;
			}

		case 1:
			if (l1Buffer->lower32At(l1index) != -1) {
				// l2upper and l2lower
				l2lower = l1Buffer->lower32At(l1index);
				tmp = l1index + 1;
				l2upper = l2Buffer->size - 1;
				while (tmp < l1Buffer->size) {
					if (l1Buffer->lower32At(tmp) != -1) {
						l2upper = l1Buffer->lower32At(tmp) - 1;
						break;
					} else
						tmp++;
				}
				//if (tmp==l1Buffer.size){
				//	l2upper = l2Buffer.size-1;
				//}
				index = context[1] + 1;
				tmp = l2Buffer->upper32At(l2lower);
				while (index < tmp) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_ATTR_NAME:
					case TOKEN_ATTR_NS:
						index += 2;
						break;
					default:
						l2index = l2lower;
						atTerminal = true;
						LN = index;
						return true;
					}
				}
				l2index = l2lower;
				context[0] = 2;
				context[2] = index;
				return true;
			} else {
				index = context[1] + 1;
				while (index < vtdSize) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_ATTR_NAME:
					case TOKEN_ATTR_NS:
						index += 2;
						break;
					default:
						if (getTokenDepth(index) == 1 && getTokenType(index)
								!= TOKEN_STARTING_TAG) {
							atTerminal = true;
							LN = index;
							return true;
						} else
							return false;

					}
				}
				return false;
			}

		case 2:
			if (l2Buffer->lower32At(l2index) != -1) {
				// l2upper and l2lower
				l3lower = l2Buffer->lower32At(l2index);
				tmp = l2index + 1;
				l3upper = l3Buffer->size - 1;
				while (tmp < l2Buffer->size) {
					if (l2Buffer->lower32At(tmp) != -1) {
						l3upper = l2Buffer->lower32At(tmp) - 1;
						break;
					} else
						tmp++;
				}
				//if (tmp==l2Buffer.size){
				//	l3upper = l3Buffer.size-1;
				//}
				index = context[2] + 1;
				tmp = l3Buffer->intAt(l3lower);
				while (index < tmp) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_ATTR_NAME:
					case TOKEN_ATTR_NS:
						index += 2;
						break;
					default:
						l3index = l3lower;
						atTerminal = true;
						LN = index;
						return true;
					}
				}
				l3index = l3lower;
				context[0] = 3;
				context[3] = index;
				return true;
			} else {
				index = context[2] + 1;
				while (index < vtdSize) {
					tokenType = getTokenType(index);
					switch (tokenType) {
					case TOKEN_ATTR_NAME:
					case TOKEN_ATTR_NS:
						index += 2;
						break;
					default:
						if (getTokenDepth(index) == 2 && getTokenType(index)
								!= TOKEN_STARTING_TAG) {
							atTerminal = true;
							LN = index;
							return true;
						} else
							return false;

					}
				}
				return false;
			}
		default:
			index = context[context[0]] + 1;
			while (index < vtdBuffer->size) {
				Long temp = vtdBuffer->longAt(index);
				tokenType = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0x0f;
				switch (tokenType) {
				case TOKEN_STARTING_TAG:
					depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth <= context[0]) {
						return false;
					} else if (depth == (context[0] + 1)) {
						context[0] += 1;
						context[context[0]] = index;
						return true;
					} else
						throw new NavException("impossible condition");

				case TOKEN_ATTR_NAME:
				case TOKEN_ATTR_NS:
					index += 2;
					break;
				case TOKEN_CHARACTER_DATA:
				case TOKEN_COMMENT:
				case TOKEN_CDATA_VAL:
					depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth < context[0]) {
						return false;
					} else if (depth == (context[0])) {
						//System.out.println("inside to Node next sibling");
						LN = index;
						atTerminal = true;
						return true;
					} else
						throw new NavException("impossible condition");
				case TOKEN_PI_NAME:
					depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth < context[0]) {
						return false;
					} else if (depth == (context[0])) {
						LN = index;
						atTerminal = true;
						return true;
					} else
						throw new NavException("impossible condition");
				}
				//index++;
			} // what condition
			return false;
		}
	case LAST_CHILD:
		if (atTerminal)
			return false;
		return toNode_LastChild();

	case NEXT_SIBLING:
		switch (context[0]) {
		case -1:
			if (atTerminal) {
				index = LN;
				tokenType = getTokenType(index);
				switch (tokenType) {
				case TOKEN_PI_NAME:
					index += 2;
					break;
					//break loop2;
				case TOKEN_COMMENT:
					index++;
					break;
				}

				if (index < vtdSize) {
					tokenType = getTokenType(index);
					depth = getTokenDepth(index);
					if (depth == -1) {
						LN = index;
						return true;
					} else {
						atTerminal = false;
						context[0] = 0;
						return true;
						// depth has to be zero
					}
				} else
					return false;

			} else {
				return false;
			}
			//break;
		case 0:
			if (atTerminal) {
				index = LN;
				tokenType = getTokenType(LN);
				if (tokenType == TOKEN_ATTR_NAME)
					return false;
				//index++;
				if (l1Buffer->size != 0) {
					if (index < l1Buffer->upper32At(l1index)) {
						index++;
						if (tokenType == TOKEN_PI_NAME)
							index++;
						if (index <= l1Buffer->upper32At(l1index)) {
							if (index == l1Buffer->upper32At(l1index)) {
								atTerminal = false;
								context[0] = 1;
								context[1] = index;
								return true;
							}
							depth = getTokenDepth(index);
							if (depth != 0)
								return false;
							LN = index;
							atTerminal = true;
							return true;
						} else {
							return false;
						}
					} else if (l1index < l1Buffer->size - 1) { // whether l1index is the last entry in l1 buffer
						l1index++;
						if (tokenType == TOKEN_PI_NAME)
							index++;
						if (index <= l1Buffer->upper32At(l1index)) {
							if (index == l1Buffer->upper32At(l1index)) {
								atTerminal = false;
								context[0] = 1;
								context[1] = index;
								return true;
							}
							depth = getTokenDepth(index);
							if (depth != 0)
								return false;
							LN = index;
							atTerminal = true;
							return true;
						} else {
							return false;
						}
					} else {
						index++;
						if (tokenType == TOKEN_PI_NAME)
							index++;
						if (index < vtdSize) {
							depth = getTokenDepth(index);
							if (depth != 0)
								return false;
							LN = index;
							atTerminal = true;
							return true;
						} else {
							return false;
						}
					}
				} else {
					index++;
					if (tokenType == TOKEN_PI_NAME)
						index++;
					if (index < vtdSize) {
						depth = getTokenDepth(index);
						if (depth != 0)
							return false;
						LN = index;
						atTerminal = true;
						return true;
					} else {
						return false;
					}
				}

			} else {
				index = vtdSize - 1;
				depth = -2;
				// get to the end, then rewind
				while (index > rootIndex) {
					depth = getTokenDepth(index);
					if (depth == -1) {
						index--;
					} else
						break;
				}
				index++;
				if (index >= vtdSize)
					return false;
				else {
					context[0] = -1;
					LN = index;
					atTerminal = true;
					return true;
				}
			}
			//break;
		case 1:
			if (atTerminal) {
				tokenType = getTokenType(LN);
				if (tokenType == TOKEN_ATTR_NAME)
					return false;
				if (l1Buffer->lower32At(l1index) != -1) {
					if (LN < l2Buffer->upper32At(l2upper)) {
						tmp = l2Buffer->upper32At(l2index);
						index = LN + 1;
						if (tokenType == TOKEN_PI_NAME)
							index++;

						if (index < tmp) {
							LN = index;
							return true;
						} else {
							context[0] = 2;
							context[2] = tmp;
							atTerminal = false;
							return true;
						}
					} else {
						index = LN + 1;
						if (tokenType == TOKEN_PI_NAME)
							index++;
						if (index < vtdSize) {
							depth = getTokenDepth(index);
							if (depth == 1 && getTokenType(index)
									!= TOKEN_STARTING_TAG) {
								LN = index;
								atTerminal = true;
								return true;
							}
							return false;
						} else
							return false;
					}
				} else {
					index = LN + 1;
					if (tokenType == TOKEN_PI_NAME)
						index++;
					if (index < vtdSize) {
						depth = getTokenDepth(index);
						if (depth == 1 && getTokenType(index)
								!= TOKEN_STARTING_TAG) {
							LN = index;
							atTerminal = true;
							return true;
						}
						return false;
					} else {
						return false;
					}
				}
			} else {
				if (l1index != l1Buffer->size - 1) {
					// not the last one
					//rewind
					l1index++;
					index = lastEntry = l1Buffer->upper32At(l1index) - 1;
					while (getTokenDepth(index) == 0) {
						index--;
					}
					if (lastEntry == index) {
						atTerminal = false;
						context[0] = 1;
						context[1] = index + 1;
						return true;
					} else {
						atTerminal = true;
						context[0] = 0;
						LN = index + 1;
						return true;
					}
				} else {
					index = vtdSize - 1;
					while (index > l1Buffer->upper32At(l1index)
							&& getTokenDepth(index) <= 0) {
						index--;
					}

					if (index == vtdSize - 1) {
						if (getTokenDepth(index) == 0) {
							context[0] = 0;
							LN = index;
							atTerminal = true;
							return true;
						} else
							return false;
					}
					index++;
					if (getTokenDepth(index) == 0) {
						context[0] = 0;
						LN = index;
						atTerminal = true;
						return true;
					} else {
						return false;
					}
				}
			}

		case 2:
			if (atTerminal) {
				tokenType = getTokenType(LN);
				if (tokenType == TOKEN_ATTR_NAME)
					return false;
				if (l2Buffer->lower32At(l2index) != -1) {
					if (LN < l3Buffer->intAt(l3upper)) {
						tmp = l3Buffer->intAt(l3index);
						index = LN + 1;
						if (tokenType == TOKEN_PI_NAME)
							index++;

						if (index < tmp) {
							LN = index;
							return true;
						} else {
							context[0] = 3;
							context[3] = tmp;
							atTerminal = false;
							return true;
						}
					} else {
						index = LN + 1;
						if (tokenType == TOKEN_PI_NAME)
							index++;
						if (index < vtdSize) {
							depth = getTokenDepth(index);
							if (depth == 2 && getTokenType(index)
									!= TOKEN_STARTING_TAG) {
								LN = index;
								return true;
							}
							return false;
						}
						return false;
					}
				} else {
					index = LN + 1;
					if (tokenType == TOKEN_PI_NAME)
						index++;
					if (index < vtdSize) {
						depth = getTokenDepth(index);
						if (depth == 2 && getTokenType(index)
								!= TOKEN_STARTING_TAG) {
							LN = index;
							atTerminal = true;
							return true;
						}
						return false;
					} else {
						return false;
					}
				}
			} else {
				//l2index < l2upper
				if (l2index < l2upper) {
					tmp = l2Buffer->upper32At(l2index);
					l2index++;
					lastEntry = index = l2Buffer->upper32At(l2index) - 1;
					//rewind
					while (index > tmp) {
						if (getTokenDepth(index) == 1) {
							tokenType = getTokenType(index);
							switch (tokenType) {
							case TOKEN_CHARACTER_DATA:
							case TOKEN_COMMENT:
							case TOKEN_CDATA_VAL:
								index--;
								break;
							case TOKEN_PI_VAL:
								index = index - 2;
								break;
							default:
								goto loop3;
							}
						} else
							goto loop3;
					}
					loop3: if (index == lastEntry) {
						context[0] = 2;
						context[2] = index + 1;
						return true;
					}
					context[0] = 1;
					LN = index + 1;
					atTerminal = true;
					return true;
				} else {
					lastEntry = index = vtdSize - 1;
					if (l1index != l1Buffer->size - 1) {
						lastEntry = index = l1Buffer->upper32At(l1index + 1)
								- 1;
					}
					tmp = l2Buffer->upper32At(l2upper);// pointing to last level 2 element

					//rewind
					while (index > tmp) {
						if (getTokenDepth(index) < 2)
							index--;
						else
							break;
					}

					if (( /*lastEntry!=index &&*/getTokenDepth(index) == 1)) {
						LN = index;
						atTerminal = true;
						context[0] = 1;
						return true;
					}

					if (/*getTokenDepth(index+1)==1
					 && getTokenType(index+1)!= TOKEN_STARTING_TAG
					 &&index !=tmp+1*/
					lastEntry != index && getTokenDepth(index + 1) == 1) { //index has moved
						LN = index + 1;
						atTerminal = true;
						context[0] = 1;
						return true;
					}

					return false;
				}

			}
			//break;
		case 3:
			if (!atTerminal) {
				//l2index < l2upper
				if (l3index < l3upper) {
					//System.out.println(l3index+"  "+l3upper+" "+l3lower+" "+l3Buffer.size+" ");
					tmp = l3Buffer->intAt(l3index);
					l3index++;
					//lastEntry = index = vtdSize-1;
					//if (l3index <l3Buffer.size-1){
					lastEntry = index = l3Buffer->intAt(l3index) - 1;
					//}
					//rewind
					while (index > tmp) {
						if (getTokenDepth(index) == 2) {
							tokenType = getTokenType(index);
							switch (tokenType) {
							case TOKEN_CHARACTER_DATA:
							case TOKEN_COMMENT:
							case TOKEN_CDATA_VAL:
								index--;
								break;
							case TOKEN_PI_VAL:
								index = index - 2;
								break;
							default:
								goto loop4;
							}
						} else
							goto loop4;
					}
					loop4: if (index == lastEntry) {
						context[0] = 3;
						context[3] = index + 1;
						return true;
					}
					context[0] = 2;
					LN = index + 1;
					atTerminal = true;
					return true;
				} else {
					lastEntry = index = vtdSize - 1;

					if (l1index != l1Buffer->size - 1) {
						lastEntry = index = l1Buffer->upper32At(l1index + 1)
								- 1;
					}

					if (l2index != l2Buffer->size - 1 && l2index != l2upper) {
						lastEntry = index = l2Buffer->upper32At(l2index + 1)
								- 1;
					}
					// inser here
					tmp = l3Buffer->intAt(l3index);

					//rewind
					while (index > tmp) {
						if (getTokenDepth(index) < 3)
							index--;
						else
							break;
					}
					if ((/*lastEntry==index &&*/getTokenDepth(index) == 2)) {
						LN = index;
						atTerminal = true;
						context[0] = 2;
						return true;
					}

					if (lastEntry != index && getTokenDepth(index + 1) == 2) {
						LN = index + 1;
						atTerminal = true;
						context[0] = 2;
						return true;
					}

					return false;
				}

			}
			//break;
		default:
			if (atTerminal) {
				tokenType = getTokenType(LN);
				if (tokenType == TOKEN_ATTR_NAME)
					return false;
				index = LN + 1;
				tmp = context[0] + 1;
			} else {
				index = context[context[0]] + 1;
				tmp = context[0];
			}
			while (index < vtdSize) {
				Long temp = vtdBuffer->longAt(index);
				tokenType = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0x0f;
				depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
				switch (tokenType) {
				case TOKEN_STARTING_TAG:
					if (depth < tmp) {
						return false;
					} else if (depth == tmp) {
						context[0] = tmp;
						context[context[0]] = index;
						atTerminal = false;
						return true;
					} else
						index++;
					break;
				case TOKEN_ATTR_NAME:
				case TOKEN_ATTR_NS:
					index += 2;
					break;
				case TOKEN_CHARACTER_DATA:
				case TOKEN_COMMENT:
				case TOKEN_CDATA_VAL:
					//depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth < tmp - 1) {
						return false;
					} else if (depth == tmp - 1) {
						LN = index;
						context[0] = tmp - 1;
						atTerminal = true;
						return true;
					} else
						index++;
					break;
				case TOKEN_PI_NAME:
					//depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth < tmp - 1) {
						return false;
					} else if (depth == tmp - 1) {
						LN = index;
						context[0] = tmp - 1;
						atTerminal = true;
						return true;
					} else
						index += 2;
					break;
				default:
					index++;
				}

			}
			return false;
		}
	case PREV_SIBLING:
		return toNode_PrevSibling();
	default:
		throw new NavException("illegal navigation options");
	}
}

bool VTDNav::verifyNodeCorrectness() {
	if (atTerminal) {
		// check l1 index, l2 index, l2lower, l2upper, l3 index, l3 lower, l3 upper
		if (getTokenDepth(LN) != context[0])
			return false;
		switch (context[0]) {
		case -1:
			return true;
		case 0:
			//if (getTokenDepth(LN)!=0)
			//	return false;
			if (l1Buffer->size != 0) {
				if (l1index >= l1Buffer->size || l1index < 0)
					return false;
				if (l1index != l1Buffer->size - 1) {

					if (l1Buffer->upper32At(l1index) < LN)
						return false;
				}
				return true;
			} else
				return true;

		case 1:
			if (LN > context[1]) {
				//if (getTokenDepth(LN) != 1)
				//	return false;
				if (l1index < 0 || l1index > l1Buffer->size)
					return false;
				int i1, i2; // l2lower, l2upper and l2index
				i1 = l1Buffer->lower32At(l1index);
				if (i1 != -1) {
					if (i1 != l2lower)
						return false;
					int tmp = l1index + 1;
					i2 = l2Buffer->size - 1;
					while (tmp < l1Buffer->size) {
						if (l1Buffer->lower32At(tmp) != -1) {
							i2 = l1Buffer->lower32At(tmp) - 1;
							break;
						} else
							tmp++;
					}
					/*if (i1 !=l2lower)
					 return false;*/
					if (l2upper != i2)
						return false;
					if (l2index > l2upper || l2index < l2lower)
						return false;
					if (l2index != l2upper) {
						if (l2Buffer->upper32At(l2index) < LN)
							return false;
					}
					if (l2index != l2lower) {
						if (l2Buffer->upper32At(l2index - 1) > LN)
							return false;
					}
				}
				return true;
			} else
				return false;
		case 2:
			if (LN > context[2] && context[2] > context[1]) {
				//if (getTokenDepth(LN) != 2)
				//	return false;
				if (l1index < 0 || l1index > l1Buffer->size)
					return false;
				int i1, i2; //l2lower, l2upper and l2index
				i1 = l1Buffer->lower32At(l1index);
				if (i1 == -1)
					return false;
				if (i1 != l2lower)
					return false;
				int tmp = l1index + 1;
				i2 = l2Buffer->size - 1;
				while (tmp < l1Buffer->size) {
					if (l1Buffer->lower32At(tmp) != -1) {
						i2 = l1Buffer->lower32At(tmp) - 1;
						break;
					} else
						tmp++;
				}
				if (context[2] != l2Buffer->upper32At(l2index)) {
					return false;
				}
				if (l2index > l2upper || l2index < l2lower) {
					return false;
				}
				//l3
				i1 = l2Buffer->lower32At(l2index);
				if (i1 != -1) {
					if (l3lower != i1)
						return false;
					i2 = l3Buffer->size - 1;
					tmp = l2index + 1;

					while (tmp < l2Buffer->size) {
						if (l2Buffer->lower32At(tmp) != -1) {
							i2 = l2Buffer->lower32At(tmp) - 1;
							break;
						} else
							tmp++;
					}

					if (l3lower != i1)
						return false;

					if (l3upper != i2)
						return false;

					if (l3index < i1 || l3index > i2)
						return false;

					if (l3index != l3upper) {
						if (l3Buffer->intAt(l3index) < LN)
							return false;
					}
					if (l3index != l3lower) {
						if (l3Buffer->intAt(l3index - 1) > LN)
							return false;
					}
				}
				return true;
			} else
				return false;

		default:
			//if (getTokenDepth(LN) != 2)
			//	return false;
			if (l1index < 0 || l1index > l1Buffer->size)
				return false;
			int i1, i2; //l2lower, l2upper and l2index
			i1 = l1Buffer->lower32At(l1index);

			if (i1 == -1)
				return false;
			if (i1 != l2lower)
				return false;
			int tmp = l1index + 1;
			i2 = l2Buffer->size - 1;
			while (tmp < l1Buffer->size) {
				if (l1Buffer->lower32At(tmp) != -1) {
					i2 = l1Buffer->lower32At(tmp) - 1;
					break;
				} else
					tmp++;
			}
			if (context[2] != l2Buffer->upper32At(l2index)) {
				return false;
			}
			if (l2index > l2upper || l2index < l2lower) {
				return false;
			}
			//l3
			i1 = l2Buffer->lower32At(l2index);
			if (i1 == -1)
				return false;
			if (i1 != l3lower)
				return false;
			i2 = l3Buffer->size - 1;
			tmp = l2index + 1;

			while (tmp < l2Buffer->size) {
				if (l2Buffer->lower32At(tmp) != -1) {
					i2 = l2Buffer->lower32At(tmp) - 1;
					break;
				} else
					tmp++;
			}

			if (l3lower != i1)
				return false;

			if (l3upper != i2)
				return false;

			if (l3index < i1 || l3index > i2)
				return false;

			if (context[context[0]] > LN)
				return false;

			if (context[0] == 3) {
				if (l3index != l3upper) {
					if (l3Buffer->intAt(l3index) > LN)
						return false;
				}
				if (l3index + 1 <= l3Buffer->size - 1) {
					if (l3Buffer->intAt(l3index + 1) < LN) {
						return false;
					}
				}
			}

		}
		return true;

	} else {
		switch (context[0]) {
		case -1:
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		default:
			return true;
		}

	}
}

void VTDNav::sync(int depth, int index) {
	// assumption is that this is always at terminal
	int t = -1;
	switch (depth) {
	case -1:
		return;
	case 0:
		if (l1Buffer->size != 0) {
			if (l1index == -1)
				l1index = 0;

			if (index > l1Buffer->upper32At(l1Buffer->size - 1)) {
				l1index = l1Buffer->size - 1;
				return;
			}

			if (index > l1Buffer->upper32At(l1index)) {
				while (l1index < l1Buffer->size - 1 && l1Buffer->upper32At(
						l1index) < index) {
					l1index++;
				}
			} else {
				while (l1index > 0 && l1Buffer->upper32At(l1index - 1) > index) {
					l1index--;
				}
			}
			//assert(index<l1Buffer.upper32At(l1index));
		}
		break;
	case 1:
		if (l1Buffer->lower32At(l1index) != -1) {
			if (l2lower != l1Buffer->lower32At(l1index)) {
				l2lower = l2index = l1Buffer->lower32At(l1index);
				l2upper = l2Buffer->size - 1;
				int size = l1Buffer->size;
				for (int i = l1index + 1; i < size; i++) {
					int temp = l1Buffer->lower32At(i);
					if (temp != 0xffffffff) {
						l2upper = temp - 1;
						break;
					}
				}
				//l2upper = l1Buffer.lower32At(l1index);
			}

			if (index > l2Buffer->upper32At(l2index)) {
				while (l2index < l2upper && l2Buffer->upper32At(l2index)
						< index) {
					l2index++;
				}
			} else {
				while (l2index > l2lower && l2Buffer->upper32At(l2index - 1)
						> index) {
					l2index--;
				}
			}

			//assert(index<l2Buffer.upper32At(l2index));
		}

		break;
	case 2:
		if (l2Buffer->lower32At(l2index) != -1) {
			if (l3lower != l2Buffer->lower32At(l2index)) {
				l3index = l3lower = l2Buffer->lower32At(l2index);
				l3upper = l3Buffer->size - 1;
				int size = l2Buffer->size;
				for (int i = l2index + 1; i < size; i++) {
					int temp = l2Buffer->lower32At(i);
					if (temp != 0xffffffff) {
						l3upper = temp - 1;
						break;
					}
				}
			}
			if (index > l3Buffer->intAt(l3index)) {
				while (l3index < l3upper && l3Buffer->intAt(l3index) < index) {
					l3index++;
				}
			} else {
				while (l3index > l3lower && l3Buffer->intAt(l3index - 1)
						> index) {
					l3index--;
				}
			}

			//assert(index<l3Buffer.intAt(l3index));
		}
		break;
	default:
		if (l2Buffer->lower32At(l2index) != -1) {
			if (l3lower != l2Buffer->lower32At(l2index)) {
				l3index = l3lower = l2Buffer->lower32At(l2index);
				l3upper = l3Buffer->size - 1;
				int size = l2Buffer->size;
				for (int i = l2index + 1; i < size; i++) {
					int temp = l2Buffer->lower32At(i);
					if (temp != 0xffffffff) {
						l3upper = temp - 1;
						break;
					}
				}
			}
			//if (context[3]> l3Buffer.intAt(l3index)){
			while (context[3] != l3Buffer->intAt(l3index)) {
				l3index++;
			}
			//} else {
			//	while (context[3] != l3Buffer.intAt(l3index)){
			//		l3index--;
			//	}
			//}

			//assert(index<l3Buffer.intAt(l3index));
		}
		break;
	}
}

bool VTDNav::iterate_following_node() {
	int index = getCurrentIndex() + 1;
	int tokenType, depth = -2;
	//int size = vtdBuffer.size;
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
			index = index + 2;
			continue;
		case TOKEN_STARTING_TAG:
		case TOKEN_DOCUMENT:
			depth = getTokenDepth(index);
			context[0] = depth;
			if (depth > 0)
				context[depth] = index;
			if (depth < maxLCDepthPlusOne)
				resolveLC();
			atTerminal = false;
			return true;

		case TOKEN_CHARACTER_DATA:
		case TOKEN_CDATA_VAL:
		case TOKEN_COMMENT:
		case TOKEN_PI_NAME:
			depth = getTokenDepth(index);

			context[0] = depth;
			LN = index;
			atTerminal = true;
			sync(depth, index);
			return true;
		}
		index++;
	}
	return false;
}

bool VTDNav::iterate_preceding_node(int a[], int endIndex) {
	int index = getCurrentIndex() + 1;
	int tokenType, depth = -2;
	//int t,d;
	//int depth = getTokenDepth(index);
	//int size = vtdBuffer.size;
	while (index < endIndex) {
		tokenType = getTokenType(index);
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
			index = index + 2;
			continue;
		case TOKEN_STARTING_TAG:
			//case TOKEN_DOCUMENT:
			depth = getTokenDepth(index);
			if (depth > 0 && (index != a[depth])) {
				context[0] = depth;
				if (depth > 0)
					context[depth] = index;
				if (depth < maxLCDepthPlusOne)
					resolveLC();
				atTerminal = false;
				return true;
			} else {
				if (depth > 0)
					context[depth] = index;
				if (depth < maxLCDepthPlusOne)
					resolveLC();
				index++;
				atTerminal = false;
				continue;
			}

		case TOKEN_CHARACTER_DATA:
		case TOKEN_CDATA_VAL:
		case TOKEN_COMMENT:
		case TOKEN_PI_NAME:
			depth = getTokenDepth(index);

			context[0] = depth;
			LN = index;
			atTerminal = true;
			sync(depth, index);
			return true;
		}
		index++;
	}
	return false;
}

bool VTDNav::iterate_preceding(UCSChar *en, int a[], int endIndex) {
	int index = getCurrentIndex() + 1;
	int tokenType, depth = -2;
	//int depth = getTokenDepth(index);
	//int size = vtdBuffer.size;
	while (index < endIndex) {
		tokenType = getTokenType(index);
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
			index = index + 2;
			continue;
		case TOKEN_STARTING_TAG:
			//case TOKEN_DOCUMENT:
			depth = getTokenDepth(index);
			if (index != a[depth]) {
				if (wcscmp(en, L"*") || matchRawTokenString(index, en)) {
					context[0] = depth;
					if (depth > 0)
						context[depth] = index;
					if (depth < maxLCDepthPlusOne)
						resolveLC();
					atTerminal = false;
					return true;
				} else {
					context[depth] = index;
					index++;
					continue;
				}
			} else {
				context[depth] = index;
				index++;
				continue;
			}
		case TOKEN_CHARACTER_DATA:
		case TOKEN_CDATA_VAL:
		case TOKEN_COMMENT:
			index++;
			continue;
		case TOKEN_PI_NAME:
			index += 2;
			continue;
		}
		index++;
	}
	return false;
}

bool VTDNav::iterate_precedingNS(UCSChar* URL, UCSChar* ln, int a[],
		int endIndex) {
	int index = getCurrentIndex() - 1;
	int tokenType, depth = -2;
	//int depth = getTokenDepth(index);
	//int size = vtdBuffer.size;
	while (index < endIndex) {
		tokenType = getTokenType(index);
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
			index = index + 2;
			continue;
		case TOKEN_STARTING_TAG:
			//case TOKEN_DOCUMENT:
			depth = getTokenDepth(index);
			if (index != a[depth]) {
				context[0] = depth;
				if (depth > 0)
					context[depth] = index;
				if (matchElementNS(ln, URL)) {
					if (depth < maxLCDepthPlusOne)
						resolveLC();
					atTerminal = false;
					return true;
				} else {
					context[depth] = index;
					index++;
					continue;
				}
			} else {
				context[depth] = index;
				index++;
				continue;
			}
		case TOKEN_CHARACTER_DATA:
		case TOKEN_CDATA_VAL:
		case TOKEN_COMMENT:
			index++;
			continue;
		case TOKEN_PI_NAME:
			index += 2;
			continue;
		}
		index++;
	}
	return false;
}

bool VTDNav::iterateNode(int dp) {
	int index = getCurrentIndex() + 1;
	int tokenType, depth = -2;
	// int size = vtdBuffer.size;
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
			index = index + 2;
			continue;
		case TOKEN_STARTING_TAG:
		case TOKEN_DOCUMENT:
			depth = getTokenDepth(index);
			atTerminal = false;
			if (depth > dp) {
				context[0] = depth;
				if (depth > 0)
					context[depth] = index;
				if (dp < maxLCDepthPlusOne)
					resolveLC();
				return true;
			} else {
				return false;
			}
		case TOKEN_CHARACTER_DATA:
		case TOKEN_COMMENT:
		case TOKEN_PI_NAME:
		case TOKEN_CDATA_VAL:
			depth = getTokenDepth(index);

			if (depth >= dp) {
				sync(depth, index);
				LN = index;
				context[0] = depth;
				atTerminal = true;
				return true;
			}
			return false;
		default:
			index++;
		}
	}
	return false;
}

bool VTDNav::nodeToElement(int direction) {
	switch (direction) {
	case NEXT_SIBLING:
		switch (context[0]) {
		case 0:
			if (l1index != -1) {
				context[0] = 1;
				context[1] = l1Buffer->upper32At(l1index);
				atTerminal = false;
				return true;
			} else
				return false;
		case 1:
			if (l2index != -1) {
				context[0] = 2;
				context[2] = l2Buffer->upper32At(l2index);
				atTerminal = false;
				return true;
			} else
				return false;

		case 2:
			if (l3index != -1) {
				context[0] = 3;
				context[3] = l3Buffer->intAt(l3index);
				atTerminal = false;
				return true;
			} else
				return false;
		default:
			int index = LN + 1;
			int size = vtdBuffer->size;
			while (index < size) {
				Long temp = vtdBuffer->longAt(index);
				int token_type = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0xf;

				if (token_type == TOKEN_STARTING_TAG) {
					int depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					if (depth < context[0]) {
						return false;
					} else if (depth == (context[0])) {
						context[context[0]] = index;
						return true;
					}
				}
				index++;
			}
			return false;

		}
	case PREV_SIBLING:
		switch (context[0]) {
		case 0:
			if (l1index != -1 && l1index > 0) {
				l1index--;
				context[0] = 1;
				context[1] = l1Buffer->upper32At(l1index);
				atTerminal = false;
				return true;
			} else
				return false;
		case 1:
			if (l2index != -1 && l2index > l2lower) {
				l2index--;
				context[0] = 2;
				context[2] = l2Buffer->upper32At(l2index);
				atTerminal = false;
				return true;
			} else
				return false;
		case 2:
			if (l2index != -1 && l3index > l3lower) {
				l3index--;
				context[0] = 3;
				context[3] = l3Buffer->intAt(l3index);
				atTerminal = false;
				return true;
			} else
				return false;

		default:
			int index = LN - 1;
			while (index > context[context[0] - 1]) {
				// scan backforward
				Long temp = vtdBuffer->longAt(index);
				int token_type = (int) ((MASK_TOKEN_TYPE & temp) >> 60) & 0xf;

				if (token_type == TOKEN_STARTING_TAG) {
					int depth = (int) ((MASK_TOKEN_DEPTH & temp) >> 52);
					/*
					 * if (depth < context[0]) { return false; }
					 * else
					 */
					if (depth == (context[0])) {
						context[context[0]] = index;
						return true;
					}
				}
				index--;
			} // what condition
			return false;
		}
	}
	return false;
}

void VTDNav::loadCurrentNode() {
	currentNode->setCursorPosition();
}// ->setCursorPosition();}

void VTDNav::setCurrentNode() {
	if (currentNode == NULL) {
		currentNode = new BookMark(this);
	} else {
		currentNode->recordCursorPosition();//   ->recordCursorPosition();
	}
}

UCSChar* VTDNav::getXPathStringVal(int j, short mode) {
	int tokenType;
	int index = j + 1;
	int depth, t = 0, i = 0;
	int dp = getTokenDepth(j);
	//int size = vtdBuffer.size;
	// store all text tokens underneath the current element node
	while (index < vtdSize) {
		tokenType = getTokenType(index);
		depth = getTokenDepth(index);
		t = t + getTokenLength2(index);
		if (depth < dp || (depth == dp && tokenType == TOKEN_STARTING_TAG)) {
			break;
		}
		switch (tokenType) {
		case TOKEN_ATTR_NAME:
		case TOKEN_ATTR_NS:
		case TOKEN_PI_NAME:
			index = index + 2;
			continue;

		case TOKEN_CHARACTER_DATA:
		case TOKEN_CDATA_VAL:
			fib->append(index);
			index++;
			continue;
		}
		index++;
	}

	// calculate the total length
	UCSChar *sb = new UCSChar[t + 1];
	int os = 0;
	for (t = 0; t < fib->size; t++) {
		switch (mode) {
		case 0:
			os = _toString(sb, fib->intAt(t), os);
			break;
		case 1:
			os = _toStringUpperCase(sb, fib->intAt(t), os);
			break;
		case 2:
			os = _toStringLowerCase(sb, fib->intAt(t), os);
			break;
		default:
			throw new NavException("Invaild xpath string val mode");
		}

	}

	// clear the fib and return a string
	fib->clear();
	sb[os] = 0; // END Of STRING
	return sb;
}

int VTDNav::_toString(UCSChar *s, int index, int offset) {
	int len = getTokenLength2(index), k = offset;
	int os = getTokenOffset(index);
	int type = getTokenType(index);
	int endOffset = os + len;
	Long l;
	while (os < endOffset) {
		if (type!=TOKEN_CDATA_VAL)
			l = getCharResolved(os);
		else
			l = getChar(os);
		os += (int) (l >> 32);
		s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}

	return k;
}

int VTDNav::_toStringUpperCase(UCSChar *s, int index, int offset) {

	int len = getTokenLength2(index), k = offset;
	int os = getTokenOffset(index);
	int type = getTokenType(index);
	int endOffset = os + len;
	Long l;
	while (os < endOffset) {
		if (type!=TOKEN_CDATA_VAL)
			l = getCharResolved(os);
		else
			l = getChar(os);
		os += (int) (l >> 32);
		//s[k++] = (UCSChar)l; // java only support 16 bit unit code
		if ((int) l > 96 && (int) l < 123)
			s[k++] = (UCSChar)(l - 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l;

	}

	return k;
}

int VTDNav::_toStringLowerCase(UCSChar *s, int index, int offset) {
	int len = getTokenLength2(index), k = offset;
	int os = getTokenOffset(index);
	int type = getTokenType(index);
	int endOffset = os + len;
	Long l;
	while (os < endOffset) {
		if (type!=TOKEN_CDATA_VAL)
			l = getCharResolved(os);
		else
			l = getChar(os);
		os += (int) (l >> 32);
		if ((int) l > 64 && (int) l < 91)
			s[k++] = (UCSChar)(l + 32); // java only support 16 bit unit code
		else
			s[k++] = (UCSChar) l; // java only support 16 bit unit code
	}

	return k;
}

bool VTDNav::matchSubString(int os, int eos, int index, int t, UCSChar *s){
		int offset = os, endOffset=eos, type =t, c;Long l;
		int i=0;int len=wcslen(s);
		bool b=false;
		while(offset<endOffset){
			if (type==TOKEN_CHARACTER_DATA)
				l = getCharResolved(offset);
			else
				l = getChar(offset);
			c = (int)l;
			if (i<len-1 && c==s[i]){		
				offset += (int)(l>>32);
				i++;
			}else if(i==len-1)
				return true;
			else
				return false;				
		}
		index++;
		while(index<fib->size){		
			offset = getTokenOffset(fib->intAt(index));
			endOffset = offset + getTokenLength2(fib->intAt(index));
			type = getTokenType(fib->intAt(index));
			while(offset<endOffset){
				if (type==TOKEN_CHARACTER_DATA)
					l = getCharResolved(offset);
				else
					l = getChar(offset);
				c = (int)l;
				if (i<len && c==s[i]){		
					offset += (int)(l>>32);
					i++;
				}else if(i==len)
					return true;
				else
					return false;				
			}
			index++;
		}while(index<fib->size);
		if (i==len)
			return true;
		return false;
	}

UCSChar *VTDNav::toNormalizedXPathString(int j){
			// TODO Auto-generated method stub
		int tokenType;
		int index = j + 1;
		int depth, t=0;
		int dp = getTokenDepth(j);
		bool r = false;//default
		//int size = vtdBuffer.size;
		// store all text tokens underneath the current element node
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
		    depth = getTokenDepth(index);
		    
		    if (depth<dp || 
		    		(depth==dp && tokenType==TOKEN_STARTING_TAG)){
		    	break;
		    }
		    
		    if (tokenType==TOKEN_CHARACTER_DATA || tokenType==TOKEN_CDATA_VAL){
		    	//if (!match)
		    	t=t+getTokenLength2(index);
		    	fib->append(index);
		    	index++;
		    	continue;
		    } else if (tokenType==TOKEN_ATTR_NAME
			        || tokenType ==TOKEN_ATTR_NS
			        || tokenType ==TOKEN_PI_NAME){			  
			    index = index+2;
			    continue;
			}			
			index++;
		}
		//allocate string buffer
		UCSChar* sb = new UCSChar[t+1];
		t=0;
		//now create teh string, leading zeros/trailling zero stripped, multiple ws collapse into one
		index =0;
		int state=0;// start
		while(index<fib->size){
			int offset= getTokenOffset(fib->intAt(index));
			int len = getTokenLength2(fib->intAt(index));
			int endOS= offset + len;
			int c;
			Long l;
			
			int type = getTokenType(fib->intAt(index));
			if (type==TOKEN_CHARACTER_DATA){
				while(offset < endOS){
					l = getCharResolved(offset);
					c = (int)l;
					offset += (int)(l>>32);
					
					switch(state){
					case 0:
						if(isWS(c)){
							
						}else{
							sb[t++]=(char)c;
							
							state =1;
						}
						break;
						
					case 1:
						if(isWS(c)){
							sb[t++]=(char)' ';
							state =2;
						}else{
							sb[t++]=(char)c;
						}
						break;
						
					case 2:
						if (isWS(c)){
							
						}else{
							sb[t++]=((char)c);
							state = 1;
						}
						break;
					
					}
				}
			}else{
				while(offset < endOS){
					l = getChar(offset);
					c = (int)l;
					offset += (int)(l>>32);
					
					switch(state){
					case 0:
						if(isWS(c)){
							
						}else{
							sb[t++]=((char)c);
							state =1;
						}
						break;
					case 1:
						if(isWS(c)){
							sb[t++]=((char)' ');
							state =2;
						}else{
							sb[t++]=((char)c);
						}
						break;
					
					case 2: 
						if (isWS(c)){
							
						}else{
							sb[t++]=((char)c);
							state = 1;
						}
						break;
					
					}
				}
			}			
			
			index++;
			//System.out.println(sb.toString());
		}

		//put end of string character
		sb[t]=0;
		fib->clear();
		//String s =sb.toString();
		//System.out.println();
		return sb;
}

bool VTDNav::XPathStringVal_Contains(int j, UCSChar *s){
		int tokenType;
		int index = j + 1;
		int depth, t=0, i=0,offset, endOffset,len,type,c;
		Long l;
		bool result=false;
		int dp = getTokenDepth(j);
		//int size = vtdBuffer.size;
		// store all text tokens underneath the current element node
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
		    depth = getTokenDepth(index);
		    //t=t+getTokenLength2(index);
		    if (depth<dp || 
		    		(depth==dp && tokenType==TOKEN_STARTING_TAG)){
		    	break;
		    }
		    
		    if (tokenType==TOKEN_CHARACTER_DATA
		    		|| tokenType==TOKEN_CDATA_VAL){
		    	//length = getTokenLength2(index);
		    	//t += length;
		    	fib->append(index);
		    	index++;
		    	continue;
		    	//
		    } else if (tokenType==TOKEN_ATTR_NAME
			        || tokenType == TOKEN_ATTR_NS
			        || tokenType == TOKEN_PI_NAME){			  
			    index = index+2;
			    continue;
			}			
			index++;
		}
		
		index=0;
		while(index<fib->size){
			type = getTokenType(fib->intAt(index));
			offset = getTokenOffset(fib->intAt(index));
			len = getTokenLength(fib->intAt(index));
			endOffset = offset+len;
			while(offset<endOffset){
				if (type==TOKEN_CHARACTER_DATA)
					l = getCharResolved(offset);
				else
					l = getChar(offset);
				c = (int)l;
				if (c==s[0]&& matchSubString(offset, endOffset, index, type,s)){
					result=true;
					goto loop;
				}else
					offset += (int)(l>>32);
			}			
			index++;
		}
		
		loop:
		fib->clear();
		return result;
}

bool VTDNav::XPathStringVal_StartsWith(int j, UCSChar *s){
		int tokenType;
		int index = j + 1;
		int depth,length,i=0, offset, endOffset, len,len2=wcslen(s),c;
		Long l;
		int dp = getTokenDepth(j);
		bool r = false;//default
		//int size = vtdBuffer.size;
		// store all text tokens underneath the current element node
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
		    depth = getTokenDepth(index);
		    //t=t+getTokenLength2(index);
		    if (depth<dp ||
		    		(depth==dp && tokenType==TOKEN_STARTING_TAG)){
		    	break;
		    }
		    
		    if (tokenType==TOKEN_CHARACTER_DATA ){
		    	//if (!match)
		    	offset = getTokenOffset(index);
		    	len = getTokenLength2(index);
		    	endOffset = offset + len;
		    	while(offset<endOffset){
		    		l = getCharResolved(offset);
		    		c = (int)l;
		    		
		    		if (i< len2&& c == s[i]){
		    			offset += (int)(l>>32);
		    			i++;
		    		}else if (i==len2)
		    			return true;
		    		else
		    			return false;
		    		
		    	}
		    	index++;
		    	continue;
		    }else if( tokenType==TOKEN_CDATA_VAL){
		    	offset = getTokenOffset(index);
		    	len = getTokenLength2(index);
		    	endOffset = offset + len;
		    	while(offset<endOffset){
		    		l = getChar(offset);
		    		c = (int)l;
		    		
		    		if (i< len2&& c == s[i]){
		    			offset += (int)(l>>32);
		    			i++;
		    		}else if (i==len2)
		    			return true;
		    		else
		    			return false;
		    		
		    	}
		    	index++;
		    	continue;
		    }else if (tokenType==TOKEN_ATTR_NAME
			        || tokenType == TOKEN_ATTR_NS
			        || tokenType == TOKEN_PI_NAME){			  
			    
		    	index = index+2;
			    continue;
			}			
			index++;
		}
		return false;
}

bool VTDNav::XPathStringVal_EndsWith(int j, UCSChar *s){
		int tokenType;
		int index = j + 1,len=wcslen(s);
		int depth, t=0, i=0,d=0, offset,endOffset,type;
		bool b=false;
		Long l;
		int dp = getTokenDepth(j);
		//int size = vtdBuffer.size;
		// store all text tokens underneath the current element node
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
		    depth = getTokenDepth(index);
		    //t=t+getStringLength(index);
		    if (depth<dp || 
		    		(depth==dp && tokenType==TOKEN_STARTING_TAG)){
		    	break;
		    }
		    
		    if (tokenType==TOKEN_CHARACTER_DATA
		    		|| tokenType==TOKEN_CDATA_VAL){
		    	//length = getTokenLength2(index);
		    	//t += length;
		    	fib->append(index);
		    	index++;
		    	continue;
		    	//
		    } else if (tokenType==TOKEN_ATTR_NAME
			        || tokenType ==TOKEN_ATTR_NS
			        || tokenType ==TOKEN_PI_NAME){			  
			    index = index+2;
			    continue;
			}			
			index++;
		}
		//if (t<s.length())
		//	return false;
		for (i=fib->size-1;i!=0;i--){
			t+=getStringLength(fib->intAt(i));
			if (t>=len){
				d = t-len;//# of chars to be skipped
				break;
			}
		}
		
		if (i<-1)return false;
		type = getTokenType(fib->intAt(i));
		offset = getTokenOffset(fib->intAt(i));
		endOffset = offset+getTokenLength2(fib->intAt(i));
		for(j=0;j<d;j++){
			if (type==TOKEN_CHARACTER_DATA){
				l=getCharResolved(offset);
			}else
				l=getChar(offset);
			offset += (int)(l>>32);
		}
		b =matchSubString(offset, endOffset,i,type,s);
		fib->clear();
		return b;
}
