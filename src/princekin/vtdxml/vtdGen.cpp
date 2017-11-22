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
#include "vtdGen.h"
#include "XMLChar.h"
#include "UTF8Char.h"
#include "decoder.h"
extern "C" {
   // Get declaration for f(int i, char c, float x)
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <sys/stat.h>
}

//using namespace com_ximpleware_char;
using namespace com_ximpleware;
//done!!
//int  VTDGen::entityIdentifier(){
int  VTDGen::entityIdentifier(){

	int ch = getChar();
	int val = 0;
	switch (ch) {
			case '#' :
				ch = getChar();
				if (ch == 'x') {
					while (true) {
						ch = getChar();
						if (ch >= '0' && ch <= '9') {
							val = (val << 4) + (ch - '0');
						} else if (ch >= 'a' && ch <= 'f') {
							val = (val << 4) + (ch - 'a' + 10);
						} else if (ch >= 'A' && ch <= 'F') {
							val = (val << 4) + (ch - 'A' + 10);
						} else if (ch == ';') {
							return val;
						} else{
							/*throwException( parse_exception,0,
							"Parse exception in entityIdentifier",
							"Errors in char reference: Illegal char following &#x.");*/
							throw ParseException("Parse exception in entityIdentifier \n"\
								"Errors in char reference: Illegal char following &#x.");
						}
					}
				} else {
					while (true) {
						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							break;
						} else{
							throw ParseException(
								"Parse exception in entityIdentifier \n"\
								"Errors in char reference: Illegal char following &#.");
						}
						ch = getChar();
					}
				}
				if (!XMLChar_isValidChar(val)) {
					throw ParseException(
						"Parse exception in entityIdentifier \n" \
						"Errors in entity reference: Invalid XML char.");
				}
				return val;
				//break;

			case 'a' :
				ch = getChar();
				if (ch == 'm') {
					if (getChar() == 'p' && getChar() == ';') {
						return '&';
					} else{
						throw ParseException(
							"Parse exception in entityIdentifier \n"\
							"Errors in Entity: Illegal builtin reference");
					}
				} else if (ch == 'p') {
					if (getChar() == 'o'
						&& getChar() == 's'
						&& getChar() == ';') {
							//System.out.println(" entity for ' ");
							return '\'';
					} else{
						throw ParseException(
							"Parse exception in entityIdentifier \n"\
							"Errors in Entity: Illegal builtin reference");
					}
				} else{
					throw ParseException(
						"Parse exception in entityIdentifier \n" \
						"Errors in Entity: Illegal builtin reference");
				}

			case 'q' :
				if (getChar() == 'u'
					&& getChar() == 'o'
					&& getChar() == 't'
					&& getChar() == ';') {
						return '"';
				} else{
					throw ParseException(
						"Parse exception in entityIdentifier \n"\
						"Errors in Entity: Illegal builtin reference");
				}
			case 'l' :
				if (getChar() == 't' && getChar() == ';') {
					return '<';
				} else{
					throw ParseException(
						"Parse exception in entityIdentifier \n"\
						"Errors in Entity: Illegal builtin reference");
				}

			case 'g' :
				if (getChar() == 't' && getChar() == ';') {
					return '>';
				} else{
					throw ParseException(
						"Parse exception in entityIdentifier \n"\
						"Errors in Entity: Illegal builtin reference");
				}
			default :
				throw ParseException(
					"Parse exception in entityIdentifier \n"\
					"Errors in Entity: Illegal builtin reference");
	}
	return 0;
}
void VTDGen::printLineNumber(){
		int so = docOffset;
	int lineNumber = 0;
	int lineOffset = 0;
	//int end = offset;

	if (encoding < FORMAT_UTF_16BE) {
		while (so <= offset-1) {
			if (XMLDoc[so] == '\n') {
				lineNumber++;
				lineOffset = so;
			}
			//lineOffset++;
			so++;
		}
		lineOffset = offset - lineOffset;
	} else if (encoding == FORMAT_UTF_16BE) {
		while (so <= offset-2) {
			if (XMLDoc[so + 1] == '\n' && XMLDoc[so] == 0) {
				lineNumber++;
				lineOffset = so;
			}
			so += 2;
		}
		lineOffset = (offset - lineOffset) >> 1;
	} else {
		while (so <= offset-2) {
			if (XMLDoc[so] == '\n' && XMLDoc[so + 1] == 0) {
				lineNumber++;
				lineOffset = so;
			}
			so += 2;
		}
		lineOffset = (offset - lineOffset) >> 1;
	}
	//return "\nLine Number: " + lineNumber + " Offset: " + lineOffset;
	printf("\nLine Number: %d  Offset: %d \n",lineNumber+1, lineOffset-1);
}
//done!!
//done!!
bool VTDGen::skip4OtherEncoding(int ch1){
	UByte ch = XMLDoc[offset];
	int temp;
	switch (encoding)
	{
	case FORMAT_ISO_8859_2:
		temp = iso_8859_2_decode(ch);
		break;

	case FORMAT_ISO_8859_3:
		temp = iso_8859_3_decode(ch);
		break;

	case FORMAT_ISO_8859_4:
		temp = iso_8859_4_decode(ch);
		break;
	case FORMAT_ISO_8859_5:
		temp = iso_8859_5_decode(ch);
		break;
	case FORMAT_ISO_8859_6:
		temp = iso_8859_6_decode(ch);
		break;

	case FORMAT_ISO_8859_7:
		temp = iso_8859_7_decode(ch);
		break;

	case FORMAT_ISO_8859_8:
		temp = iso_8859_8_decode(ch);
		break;

	case FORMAT_ISO_8859_9:
		temp = iso_8859_9_decode(ch);
		break;

	case FORMAT_ISO_8859_10:
		temp = iso_8859_10_decode(ch);
		break;

	case FORMAT_ISO_8859_11:
		temp = iso_8859_11_decode(ch);
		break;

	case FORMAT_ISO_8859_13:
		temp = iso_8859_13_decode(ch);
		break;

	case FORMAT_ISO_8859_14:
		temp = iso_8859_14_decode(ch);
		break;

	case FORMAT_ISO_8859_15:
		temp = iso_8859_15_decode(ch);
		break;

	case FORMAT_WIN_1250:
		temp = windows_1250_decode(ch);
		break;

	case FORMAT_WIN_1251:
		temp = windows_1251_decode(ch);
		break;

	case FORMAT_WIN_1252:
		temp = windows_1252_decode(ch);
		break;

	case FORMAT_WIN_1253:
		temp = windows_1253_decode(ch);
		break;

	case FORMAT_WIN_1254:
		temp = windows_1254_decode(ch);
		break;

	case FORMAT_WIN_1255:
		temp = windows_1255_decode(ch);
		break;

	case FORMAT_WIN_1256:
		temp = windows_1256_decode(ch);
		break;

	case FORMAT_WIN_1257:
		temp = windows_1257_decode(ch);
		break;

	case FORMAT_WIN_1258:
		temp = windows_1258_decode(ch);
		break;

	default:
		throw ParseException(
			"Parse exception in skipChar\n"\
			"Unknown encoding");	
	}

	if (temp == ch1)
	{
		offset++;
		return true;
	}
	else
		return false;
}
//done!
int VTDGen::handleOtherEncoding(){
	UByte ch = XMLDoc[offset++];
	switch (encoding)
	{

	case FORMAT_ISO_8859_2:
		return iso_8859_2_decode(ch);

	case FORMAT_ISO_8859_3:
		return iso_8859_3_decode(ch);

	case FORMAT_ISO_8859_4:
		return iso_8859_4_decode(ch);

	case FORMAT_ISO_8859_5:
		return iso_8859_5_decode(ch);

	case FORMAT_ISO_8859_6:
		return iso_8859_6_decode(ch);

	case FORMAT_ISO_8859_7:
		return iso_8859_7_decode(ch);

	case FORMAT_ISO_8859_8:
		return iso_8859_8_decode(ch);

	case FORMAT_ISO_8859_9:
		return iso_8859_9_decode(ch);

	case FORMAT_ISO_8859_10:
		return iso_8859_10_decode(ch);

	case FORMAT_ISO_8859_11:
		return iso_8859_11_decode(ch);

	case FORMAT_ISO_8859_13:
		return iso_8859_13_decode(ch);

	case FORMAT_ISO_8859_14:
		return iso_8859_14_decode(ch);

	case FORMAT_ISO_8859_15:
		return iso_8859_15_decode(ch);

	case FORMAT_WIN_1250:
		return windows_1250_decode(ch);

	case FORMAT_WIN_1251:
		return windows_1251_decode(ch);

	case FORMAT_WIN_1252:
		return windows_1252_decode(ch);

	case FORMAT_WIN_1253:
		return windows_1253_decode(ch);

	case FORMAT_WIN_1254:
		return windows_1254_decode(ch);

	case FORMAT_WIN_1255:
		return windows_1255_decode(ch);

	case FORMAT_WIN_1256:
		return windows_1256_decode(ch);

	case FORMAT_WIN_1257:
		return windows_1257_decode(ch);

	case FORMAT_WIN_1258:
		return windows_1258_decode(ch);
	default:
		throw ParseException(
			"Parse exception in parse() \n"\
			"Unknown encoding");		
	}
	return 0;
}
//done
int VTDGen::handle_16le(){
	int temp,val;
	temp = XMLDoc[offset + 1] << 8 | XMLDoc[offset];
	if (temp < 0xd800 || temp > 0xdfff) { // check for low surrogate
		offset += 2;
		return temp;
	} else {
		if(temp<0xd800 || temp>0xdbff){
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 16 LE encoding error: should never happen");
		}
		val = temp;
		temp = XMLDoc[offset + 3] << 8 | XMLDoc[offset + 2];
		if (temp < 0xdc00 || temp > 0xdfff) {
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 16 LE encoding error: should never happen");		
		}
		val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;
		offset += 4;
		return val;
	}
}
//done
int VTDGen::handle_16be(){
	int temp,val;
	temp = XMLDoc[offset] << 8 | XMLDoc[offset + 1];
	if ((temp < 0xd800)
		|| (temp > 0xdfff)) { // not a high surrogate
			offset += 2;
			return temp;
	} else {
		if(temp<0xd800 || temp>0xdbff){
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 16 BE encoding error: should never happen");
		}
		val = temp;
		temp = XMLDoc[offset + 2] << 8 | XMLDoc[offset + 3];
		if (temp < 0xdc00 || temp > 0xdfff) {
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 16 BE encoding error: should never happen");
		}
		val = ((val - 0xd800) <<10) + (temp - 0xdc00) + 0x10000;
		offset += 4;
		return val;
	}
}

int VTDGen::handle_utf8(int temp){
	int c,d,a,i;
	int val;
	switch (UTF8Char_byteCount(temp)) { 
			case 2 :
				c = 0x1f;
				d = 6;
				a = 1;
				break;
			case 3 :
				c = 0x0f;
				d = 12;
				a = 2;
				break;
			case 4 :
				c = 0x07;
				d = 18;
				a = 3;
				break;
			case 5 :
				c = 0x03;
				d = 24;
				a = 4;
				break;
			case 6 :
				c = 0x01;
				d = 30;
				a = 5;
				break;
			default :
				throw ParseException(
			"Parse exception in parse() \n"\
					"UTF 8 encoding error: should never happen");			
	}
	val = (temp & c) << d;
	i = a - 1;
	while (i >= 0) {
		temp = XMLDoc[offset + a - i];
		if ((temp & 0xc0) != 0x80){
			throw ParseException(
			"Parse exception in parse() \n"\
					"UTF 8 encoding error: should never happen");	
		}
		val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
		i--;
	}
	offset += a + 1;
	return val;
}
//done
void VTDGen::matchISOEncoding(){
	if ((skipChar('s')
		|| skipChar('S'))
		&& (skipChar('o')
		|| skipChar('O'))
		&& skipChar('-')
		&& skipChar('8')
		&& skipChar('8')
		&& skipChar('5')
		&& skipChar('9')
		&& skipChar('-')
		) {
			if (encoding != FORMAT_UTF_16LE
				&& encoding	!= FORMAT_UTF_16BE) {
					if (must_utf_8){
						throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch from UTF-8");
					}
					if(skipChar('1')){
						if (skipChar(ch_temp)){
							encoding = FORMAT_ISO_8859_1;
							_writeVTD(
								temp_offset,
								10,
								TOKEN_DEC_ATTR_VAL,
								depth);
							return;
						} else if (skipChar('0')){
							encoding = FORMAT_ISO_8859_10;
							iso_8859_10_chars_init();
							_writeVTD(
								temp_offset,
								11,
								TOKEN_DEC_ATTR_VAL,
								depth);
						}else if (skipChar('1')){
							encoding = FORMAT_ISO_8859_11;
							iso_8859_11_chars_init();
							_writeVTD(
								temp_offset,
								11,
								TOKEN_DEC_ATTR_VAL,
								depth);
						}else if (skipChar('3')){
							encoding = FORMAT_ISO_8859_13;
							iso_8859_13_chars_init();
							_writeVTD(
								temp_offset,
								11,
								TOKEN_DEC_ATTR_VAL,
								depth);
						}else if (skipChar('4')){
							encoding = FORMAT_ISO_8859_14;
							iso_8859_14_chars_init();
							_writeVTD(
								temp_offset,
								11,
								TOKEN_DEC_ATTR_VAL,
								depth);
						}else if (skipChar('5')){
							encoding = FORMAT_ISO_8859_15;
							iso_8859_15_chars_init();
							_writeVTD(
								temp_offset,
								11,
								TOKEN_DEC_ATTR_VAL,
								depth);
						}
						else
							throwInvalidEncodingException ();		
					}else if(skipChar('2')){						
						encoding = FORMAT_ISO_8859_2;
						iso_8859_2_chars_init();
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('3')){						
						encoding = FORMAT_ISO_8859_3;
						iso_8859_3_chars_init();
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('4')){						
						encoding = FORMAT_ISO_8859_4;
						iso_8859_4_chars_init();
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('5')){						
						encoding = FORMAT_ISO_8859_5;
						iso_8859_5_chars_init();
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('6')){							
						encoding = FORMAT_ISO_8859_6;
						iso_8859_6_chars_init();							
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('7')){						
						encoding = FORMAT_ISO_8859_7;
						iso_8859_7_chars_init();								
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}else if(skipChar('8')){						
						encoding = FORMAT_ISO_8859_8;
						iso_8859_8_chars_init();								
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}else if(skipChar('9')){						
						encoding = FORMAT_ISO_8859_9;
						iso_8859_9_chars_init();
						_writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else 
						throwInvalidEncodingException();
					if (skipChar(ch_temp))
						return;
			} else{
				throw ParseException(
			"Parse exception in getChar \n"\
							"XML decl error: Can't switch encoding to ISO-8859");
			}

	}
	throwInvalidEncodingException ();
}
void VTDGen::matchCPEncoding(){
		if ((skipChar('p')
		|| skipChar('P'))
		&& skipChar('-')
		&& skipChar('1')
		&& skipChar('2')
		&& skipChar('5')) {
			if (encoding != FORMAT_UTF_16LE
				&& encoding	!= FORMAT_UTF_16BE) {
					if (must_utf_8){
						throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch from UTF-8");
					}
					if(skipChar('0')){						
						encoding = FORMAT_WIN_1250;
						windows_1250_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('1')){						
						encoding = FORMAT_WIN_1251;
						windows_1251_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('2')){						
						encoding = FORMAT_WIN_1252;
						windows_1252_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('3')){						
						encoding = FORMAT_WIN_1253;
						windows_1253_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('4')){						
						encoding = FORMAT_WIN_1250;
						windows_1250_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('5')){						
						encoding = FORMAT_WIN_1255;
						windows_1255_chars_init();
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('6')){							
						encoding = FORMAT_WIN_1256;
						windows_1256_chars_init();							
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);

					}else if(skipChar('7')){						
						encoding = FORMAT_WIN_1257;
						windows_1257_chars_init();								
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}else if(skipChar('8')){						
						encoding = FORMAT_WIN_1258;
						windows_1258_chars_init();							
						_writeVTD(
							temp_offset,
							6,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}
					else
						throwInvalidEncodingException ();
					if (skipChar(ch_temp))
						return;
			}
			else{
				throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch encoding to ISO-8859");
			}
	}
	throwInvalidEncodingException ();
}
void VTDGen::matchWindowsEncoding(){
	
	if ((skipChar('i')
		|| skipChar('I'))
		&&(skipChar('n')
		|| skipChar('N'))
		&&(skipChar('d')
		|| skipChar('D'))
		&&(skipChar('o')
		|| skipChar('O'))
		&&(skipChar('w')
		|| skipChar('W'))
		&&(skipChar('s')
		|| skipChar('S'))
		&& skipChar('-')
		&& skipChar('1')
		&& skipChar('2')
		&& skipChar('5')) {
			if (encoding != FORMAT_UTF_16LE
				&& encoding	!= FORMAT_UTF_16BE) {
					if (must_utf_8){
						throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch from UTF-8");
					}
					if(skipChar('0')){						
						encoding = FORMAT_WIN_1250;
						windows_1250_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('1')){						
						encoding = FORMAT_WIN_1251;
						windows_1251_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('2')){						
						encoding = FORMAT_WIN_1252;
						windows_1252_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('3')){						
						encoding = FORMAT_WIN_1253;
						windows_1253_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('4')){						
						encoding = FORMAT_WIN_1250;
						windows_1250_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('5')){						
						encoding = FORMAT_WIN_1255;
						windows_1255_chars_init();
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);
					}else if(skipChar('6')){							
						encoding = FORMAT_WIN_1256;
						windows_1256_chars_init();							
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);

					}else if(skipChar('7')){						
						encoding = FORMAT_WIN_1257;
						windows_1257_chars_init();								
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}else if(skipChar('8')){						
						encoding = FORMAT_WIN_1258;
						windows_1258_chars_init();							
						_writeVTD(
							temp_offset,
							12,
							TOKEN_DEC_ATTR_VAL,
							depth);												
					}
					else
						throwInvalidEncodingException ();
					if (skipChar(ch_temp))
						return;
			}
			else{
				throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch encoding to ISO-8859");
			}

	}
	throwInvalidEncodingException ();
}
void VTDGen::matchUTFEncoding(){
		if ((skipChar('s')
		|| skipChar('S'))){
			if (skipChar('-')
				&& (skipChar('a')
				|| skipChar('A'))
				&& (skipChar('s')
				|| skipChar('S'))
				&& (skipChar('c')
				|| skipChar('C'))
				&& (skipChar('i')
				|| skipChar('I'))
				&& (skipChar('i')
				|| skipChar('I'))
				&& skipChar(ch_temp)) {
					if (singleByteEncoding) {
							if (must_utf_8){
								throw ParseException(
			"Parse exception in parse() \n"\
									"Can't switch from UTF-8");
							}
							encoding = FORMAT_ASCII;							
							if (singleByteEncoding){
									_writeVTD(
										temp_offset,
										5,
										TOKEN_DEC_ATTR_VAL,
										depth);
							}
							else{
								_writeVTD(
									temp_offset
									>> 1,
									5,
									TOKEN_DEC_ATTR_VAL,
									depth);
							}
							return;

					} else
					{		
						throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch encoding to US-ASCII");
					}
			} else{		
				throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Invalid Encoding");
			}
	}
	if ((skipChar('t')
		|| skipChar('T'))
		&& (skipChar('f')
		|| skipChar('F'))
		&& skipChar('-')) {
			if (skipChar('8')
				&& skipChar(ch_temp)) {
					if (singleByteEncoding){
						writeVTD(
							temp_offset,
							5,
							TOKEN_DEC_ATTR_VAL,
							depth);

						return;
					} 
					else
						throw ParseException(
						"Parse exception in parse() \n"\
						"XML decl error: Can't switch encoding to UTF-8");								
			}
			if (skipChar('1')
				&& skipChar('6')) {
					if (skipChar(ch_temp)) {
						if (!singleByteEncoding) {
							if (BOM_detected == false){
								throw ParseException(
									"Parse exception in parse() \n"\
									"BOM not detected for UTF-16");
							}

							_writeVTD(
								temp_offset
								>> 1,
								6,
								TOKEN_DEC_ATTR_VAL,
								depth);

								return;
						}

					throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Can't switch encoding to UTF-16");
					} else if (
						(skipChar('l')
						|| skipChar('L'))
						&& (skipChar('e')
						|| skipChar('E'))
						&& skipChar(ch_temp)) {
							if (encoding
								== FORMAT_UTF_16LE) {
									writeVTD(
										temp_offset
										>> 1,
										7,
										TOKEN_DEC_ATTR_VAL,
										depth);
									return;
							}
							throw ParseException(
								"Parse exception in parse() \n"\
								"XML decl error: Can't switch encoding to UTF-16LE");					
					} else if (
						(skipChar('b')
						|| skipChar('B'))
						&& (skipChar('e')
						|| skipChar('E'))
						&& skipChar(ch_temp)) {
							if (encoding
								== FORMAT_UTF_16BE) {

									writeVTD(
										temp_offset
										>> 1,
										7,
										TOKEN_DEC_ATTR_VAL,
										depth);

									return;
							}
							throw ParseException(
			"Parse exception in parse() \n"\
								"XML decl error: Can't switch encoding to UTF-16BE");				
					}
					throw ParseException(
			"Parse exception in parse() \n"\
							"XML decl error: Invalid encoding");
			}
	}
}
//done
bool VTDGen::skipUTF8(int temp,int ch){
		int val, a,c,d,i;
	switch (UTF8Char_byteCount(temp)) { 
			case 2 :
				c = 0x1f;
				d = 6; // 
				a = 1; //
				break;
			case 3 :
				c = 0x0f;
				d = 12;
				a = 2;
				break;
			case 4 :
				c = 0x07;
				d = 18;
				a = 3;
				break;
			case 5 :
				c = 0x03;
				d = 24;
				a = 4;
				break;
			case 6 :
				c = 0x01;
				d = 30;
				a = 5;
				break;
			default :
				throw ParseException(
			"Parse exception in parse() \n"\
							"UTF 8 encoding error: should never happen");
	}
	val = (temp & c) << d;
	i = a - 1;
	while (i >= 0) {
		temp = XMLDoc[offset + a - i];
		if ((temp & 0xc0) != 0x80){
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 8 encoding error: should never happen");
		}
		val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
		i--;
	}

	if (val == ch) {
		offset += a + 1;
		return true;
	} else {
		return false;
	}
}
//done
bool VTDGen::skip_16be(int ch){
		int temp,val;
	temp = XMLDoc[offset] << 8 | XMLDoc[offset + 1];
	if ((temp < 0xd800)
		|| (temp >= 0xdc00)) { // not a high surrogate
			//offset += 2;
			if (temp == ch) {
				offset += 2;
				return true;
			} else
				return false;
	} else {
		val = temp;
		temp = XMLDoc[offset + 2] << 8 | XMLDoc[offset + 3];
		if (temp < 0xdc00 || temp > 0xdfff) {
			// has to be a low surrogate here{
			throw ParseException(
			"Parse exception in parse() \n"\
							"UTF 16 BE encoding error: should never happen");
		}
		val = ((val - 0xd800) << 10) + (temp - 0xdc00) + 0x10000;

		if (val == ch) {
			offset += 4;
			return true;
		} else
			return false;
	}
}
//done
bool VTDGen::skip_16le(int ch){
		int temp,val;
	temp = XMLDoc[offset + 1] << 8 | XMLDoc[offset];
	if (temp < 0xdc00 || temp > 0xdfff) { // check for low surrogate
		if (temp == ch) {
			offset += 2;
			return true;
		} else {
			return false;
		}
	} else {
		val = temp;
		temp = XMLDoc[offset + 3] << 8 | XMLDoc[offset + 2];
		if (temp < 0xd800 || temp > 0xdc00) {
			// has to be high surrogate
			// has to be a low surrogate here{
			throw ParseException(
			"Parse exception in parse() \n"\
				"UTF 16 LE encoding error: should never happen");
		}
		val = ((temp - 0xd800)<<10) + (val - 0xdc00) + 0x10000;

		if (val == ch) {
			offset += 4;
			return true;
		} else
			return false;
	}
}
//done
inline int VTDGen::getCharAfterSe(){
		int n = 0;
	int temp; //offset saver
	while (true) {
		n = getChar();
		if (!XMLChar_isSpaceChar(n)) {
			if (n != '&')
				return n;
			else {
				temp = offset;
				if (!XMLChar_isSpaceChar(entityIdentifier())) {
					offset = temp; // rewind
					return '&';
				}
			}
		}
	}
}

//done
inline void VTDGen::writeVTD(int offset, int length, tokenType token_type, int depth){
	VTDBuffer->append(((Long) ((token_type << 28)
		| ((depth & 0xff) << 20) | length) << 32)
		| offset);
	// remember VTD depth start from zero
	if (token_type == TOKEN_STARTING_TAG) {
		switch (depth) {
				case 0 :
					//rootIndex = VTDBuffer.size() - 1;
					rootIndex = VTDBuffer->size - 1;
					break;
				case 1 :
					if (last_depth == 1) {

						l1Buffer->append(
							((Long) last_l1_index << 32) | 0xffffffffLL);

					} else if (last_depth == 2) {

						l2Buffer->append(
							((Long) last_l2_index << 32) | 0xffffffffLL);

					}
					last_l1_index = VTDBuffer->size - 1;
					last_depth = 1;
					break;
				case 2 :
					if (last_depth == 1) {
						l1Buffer->append(
							((Long) last_l1_index << 32) + l2Buffer->size);

					} else if (last_depth == 2) {
						l2Buffer->append(
							((Long) last_l2_index << 32) | 0xffffffffLL);

					}
					last_l2_index = VTDBuffer->size - 1;
					last_depth = 2;
					break;

				case 3 :
					l3Buffer->append( VTDBuffer->size - 1);
					if (last_depth == 2) {
						l2Buffer->append(
							((Long)last_l2_index << 32) + l3Buffer->size - 1);
					}

					last_depth = 3;
					break;
				default :					
					break;
		}
	} 
}
//done
void VTDGen::finishUp(){
	if (shallowDepth){
		if (last_depth == 1) {
			l1Buffer->append(((Long) last_l1_index << 32) | 0xffffffffLL);
		} else if (last_depth == 2) {
			l2Buffer->append(((Long) last_l2_index << 32) | 0xffffffffLL);
		}
	}else{
		if (last_depth == 1) {
			l1Buffer->append(((Long) last_l1_index << 32) | 0xffffffffLL);
		} else if (last_depth == 2) {
			l2Buffer->append(((Long) last_l2_index << 32) | 0xffffffffLL);
		}else if (last_depth == 3) {
			_l3Buffer->append(((Long) last_l3_index << 32) | 0xffffffffLL);
		}else if (last_depth == 4) {
			_l4Buffer->append(((Long) last_l4_index << 32) | 0xffffffffLL);
		}
	}
}

//done!
void VTDGen::decide_encoding(){
		if (docLen==0)
	        throw ParseException(
			"Parse exception in parse() \n"\
			"Document is zero sized ");
	if (XMLDoc[offset] == (UByte) -2) {
		increment = 2;
		if (XMLDoc[offset + 1] == (UByte)-1) {
			offset += 2;
			encoding = FORMAT_UTF_16BE;
			BOM_detected = true;
		} else{
			throw ParseException(
			"Parse exception in parse() \n"\
				"Unknown Character encoding: should be 0xff 0xfe");
		}
	} else if (XMLDoc[offset] == (UByte)-1) {
		increment = 2;
		if (XMLDoc[offset + 1] == (UByte)-2) {
			offset += 2;
			encoding = FORMAT_UTF_16LE;
			BOM_detected = true;
		} else{
			throw ParseException(
			"Parse exception in parse() \n"\
				"Unknown Character encoding");
		}
	} else if (XMLDoc[offset] == (UByte)-17) {
		if (XMLDoc[offset+1] == (UByte)-69 && XMLDoc[offset+2]==(UByte)-65){
			offset +=3;
			must_utf_8= true;
		}
		else {
			throw ParseException(
			"Parse exception in parse() \n"\
				"Unknown Character encoding: not UTF-8");
		}

	} else if (XMLDoc[offset] == 0) {
		if (XMLDoc[offset+1] == 0x3c 
			&& XMLDoc[offset+2] == 0 
			&& XMLDoc[offset+3] == 0x3f){
				encoding = FORMAT_UTF_16BE;
				increment = 2;
		}
		else{
			throw ParseException(
			"Parse exception in parse() \n"\
				"Unknown Character encoding: not UTF-16BE");
		}
	} else if (XMLDoc[offset] == 0x3c) {
		if (XMLDoc[offset+1] == 0 
			&& XMLDoc[offset+2] == 0x3f 
			&& XMLDoc[offset+3] == 0x0){
				increment = 2;
				encoding = FORMAT_UTF_16LE;
		}
	}

	if (encoding < FORMAT_UTF_16BE) {
		if (ns == true) {
			if (((Long) offset + docLen) >= (1U << 30)){
				throw ParseException(
			"Parse exception in parse() \n"\
					"Other error: file size too large >=1G ");
			}
		}else {
			if (((Long) offset + docLen) >= (1U << 31)){
				throw ParseException(
			"Parse exception in parse() \n"\
					"Other error: file size too large >=2G ");
			}
		}
	} else {
		if ((unsigned int)(offset - 2 + docLen) >= (1U<< 31)){
			throw ParseException(
			"Parse exception in parse() \n"\
				"Other error: file size too large");
		}
	}
	if(encoding >= FORMAT_UTF_16BE){
		singleByteEncoding = false;
	}
}
//done
VTDGen::pState VTDGen::process_end_pi(){
	int length1;
	pState parser_state= (pState) 0;
	ch = getChar();
	if (XMLChar_isNameStartChar(ch)) {
		if ((ch == 'x' || ch == 'X')
			&& (skipChar('m') || skipChar('M'))
			&& (skipChar('l') && skipChar('L'))) {
				//temp_offset = offset;
				ch = getChar();
				if (XMLChar_isSpaceChar(ch) || ch == '?'){		
					throw ParseException(
						"Parse exception in parse() \n"\
						"Error in PI: [xX][mM][lL] not a valid PI target");
				}
		}

		while (true) {
			//ch = getChar();
			if (!XMLChar_isNameChar(ch)) {
				break;
			}
			ch = getChar();
		}

		length1 = offset - temp_offset - increment;
		if (singleByteEncoding){
			if (length1 > MAX_TOKEN_LENGTH){
				throw ParseException(
					"Parse exception in parse() \n"\
					"Token Length Error: PI_NAME length too long");
			}
			_writeVTD(
				temp_offset,
				length1,
				TOKEN_PI_NAME,
				depth);
		}
		else{
			if (length1 > (MAX_TOKEN_LENGTH<<1)){
				throw ParseException(
					"Parse exception in parse() \n"\
					"Token Length Error: PI_NAME length too long");
			}
			_writeVTD(
				temp_offset >> 1,
				length1 >> 1,
				TOKEN_PI_NAME,
				depth);
		}
		//length1 = 0;
		temp_offset = offset;
		if (XMLChar_isSpaceChar(ch)) {
			ch = getCharAfterS();

			while (true) {
				if (XMLChar_isValidChar(ch)) {
					if (ch == '?') {
						if (skipChar('>')) {
							parser_state = STATE_DOC_END;
							break;
						} else{		
							throw ParseException(
								"Parse exception in parse() \n"\
								"Error in PI: invalid termination sequence");
						}
					}
				} else{		
					throw ParseException(
						"Parse exception in parse() \n"\
						"Error in PI: Invalid char in PI val");
				}
				ch = getChar();
			}
			length1 = offset - temp_offset - (increment<<1);
			if (encoding < FORMAT_UTF_16BE){
				if (length1 > MAX_TOKEN_LENGTH){
					throw ParseException(
						"Parse exception in parse() \n"\
						"Token Length Error: PI_VAL length too long");
				}
				writeVTD(
					temp_offset,
					length1,
					TOKEN_PI_VAL,
					depth);
			}
			else{
				if (length1 > (MAX_TOKEN_LENGTH << 1)){
					throw ParseException(
						"Parse exception in parse() \n"\
						"Token Length Error: PI_VAL length too long");
				}
				writeVTD(
					temp_offset >> 1,
					length1 >> 1,
					TOKEN_PI_VAL,
					depth);
			}
		} else {
			if (singleByteEncoding){
				_writeVTD(
					(temp_offset),
					0,
					TOKEN_PI_NAME,
					depth);
			}
			else{
				_writeVTD(
					(temp_offset) >> 1,
					0,
					TOKEN_PI_NAME,
					depth);
			}
			if ((ch == '?') && skipChar('>')) {
				parser_state = STATE_DOC_END;
			} else{		
				throw ParseException(
					"Parse exception in parse() \n"\
					"Error in PI: invalid termination sequence");
			}
		}
	} else{		
		throw ParseException(
			"Parse exception in parse() \n"\
			"Error in PI: invalid char in PI target");
	}
	return parser_state;
}


//done
VTDGen::pState VTDGen::process_end_comment(){
	int length1;
	pState parser_state= (pState)0;
	while (true) {
		ch = getChar();
		if (XMLChar_isValidChar(ch)) {
			if (ch == '-' && skipChar('-')) {
				length1 =
					offset - temp_offset - (increment<<1);
				break;
			}
		} else{		
			throw ParseException(
				"Parse exception in parse() \n"\
				"Error in comment: Invalid Char");
		}
		/*throw new ParseException(
		"Error in comment: Invalid Char"
		+ formatLineNumber());*/
	}
	if (getChar() == '>') {
		//System.out.println(" " + temp_offset + " " + length1 + " comment " + depth);
		if (singleByteEncoding){
			writeVTDText(
				temp_offset,
				length1,
				TOKEN_COMMENT,
				depth);
		}
		else
		{
			writeVTDText(
				temp_offset >> 1,
				length1 >> 1,
				TOKEN_COMMENT,
				depth);
		}
		//length1 = 0;
		parser_state = STATE_DOC_END;
		return parser_state;
	}		
	throw ParseException(
		"Parse exception in parse() \n"\
		"Error in comment: '-->' expected");
	return (pState)0;
}
//done!
VTDGen::pState VTDGen::process_comment(){
	int length1;
	pState parser_state= (pState)0;
	while (true) {
		ch = getChar();
		if (XMLChar_isValidChar(ch)) {
			if (ch == '-' && skipChar('-')) {
				length1 =
					offset - temp_offset - (increment<<1);
				break;
			}
		} else{		
			throw ParseException(
				"Parse exception in parse() \n"\
				"Error in comment: Invalid Char");
		}
	}
	if (getChar() == '>') {
		if (singleByteEncoding){
			writeVTDText(
				temp_offset,
				length1,
				TOKEN_COMMENT,
				depth);
		}
		else{
			writeVTDText(
				temp_offset >> 1,
				length1 >> 1,
				TOKEN_COMMENT,
				depth);
		}
		temp_offset = offset;
		ch = getCharAfterSe();
		if (ch == '<') {
			if (ws)
				addWhiteSpaceRecord();
			parser_state = STATE_LT_SEEN;
		} else if (XMLChar_isContentChar(ch)) {
			parser_state = STATE_TEXT;
		} else if (ch == '&') {
			entityIdentifier();
			parser_state = STATE_TEXT;
		} else if (ch == ']') {
			if (skipChar(']')) {
				while (skipChar(']')) {
				}
				if (skipChar('>')){		
					throw ParseException(
						"Parse exception in parse() \n"\
						"Error in text content: ]]> in text content");
				}
			}
			parser_state = STATE_TEXT;
		}
		else{		
			throw ParseException(
				"Parse exception in parse() \n"\
				"XML decl error");
		}
		return parser_state;
	} else{		
		throw ParseException(
			"Parse exception in parse() \n"\
			"Error in comment: Invalid terminating sequence");
		return (pState) 0;
	}
}
//done
VTDGen::pState VTDGen::process_doc_type(){
	int length1;
	pState parser_state= (pState)0;
	int	z = 1;

	while (true) {
		ch = getChar();
		if (XMLChar_isValidChar(ch)) {
			if (ch == '>')
				z--;
			else if (ch == '<')
				z++;
			if (z == 0)
				break;
		} else{		
			throw ParseException(
				"Parse exception in parse() \n"\
				"Error in DOCTYPE: Invalid char");
		}
	}
	length1 = offset - temp_offset - increment;
	if (singleByteEncoding){
		if (length1 > MAX_TOKEN_LENGTH){
			throw ParseException(
				"Parse exception in parse() \n"\
				"Token Length Error: DTD_VAL length too long");
		}
		_writeVTD(
			temp_offset,
			length1,
			TOKEN_DTD_VAL,
			depth);
	}
	else{
		if (length1 > (MAX_TOKEN_LENGTH<<1)){
			throw ParseException(
				"Parse exception in parse() \n"\
				"Token Length Error: DTD_VAL length too long");
		}
		_writeVTD(
			temp_offset >> 1,
			length1 >> 1,
			TOKEN_DTD_VAL,
			depth);
	}
	ch = getCharAfterS();
	if (ch == '<') {
		parser_state = STATE_LT_SEEN;
	} else{		
		throw ParseException(
			"Parse exception in parse() \n"\
			"Other Error: Invalid char in xml");
	}
	return parser_state;
}

//done!
VTDGen::pState VTDGen::process_cdata(){
	int length1;
	pState parser_state=(pState)0;
	while (true) {
		ch = getChar();
		if (XMLChar_isValidChar(ch)) {
			if (ch == ']' && skipChar(']')) {
				while (skipChar(']'));
				if (skipChar('>')) {
					break;
				} 
			}
		} else{		
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Error in CDATA: Invalid Char");
		}
	}
	length1 = offset - temp_offset - increment - (increment<<1);
	if (singleByteEncoding){
		writeVTDText(
			temp_offset,
			length1,
			TOKEN_CDATA_VAL,
			depth);
	}
	else{
		writeVTDText(
			temp_offset >> 1,
			length1 >> 1,
			TOKEN_CDATA_VAL,
			depth);
	}
	temp_offset = offset;
	ch = getCharAfterSe();
	if (ch == '<') {
		if (ws) 
			addWhiteSpaceRecord();
		parser_state = STATE_LT_SEEN;
	} else if (XMLChar_isContentChar(ch)) {
		//temp_offset = offset-1;
		parser_state = STATE_TEXT;
	} else if (ch == '&') {
		//temp_offset = offset-1;
		entityIdentifier();
		parser_state = STATE_TEXT;
	} else if (ch == ']') {
		//temp_offset = offset-1;
		if (skipChar(']')) {
			while (skipChar(']')) {
			}
			if (skipChar('>')){		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"Error in text content: ]]> in text content");
			}
		}
		parser_state = STATE_TEXT;
	}
	else{		
		throw ParseException(
			"Parse exception in parse()\n"\
			"Other Error: Invalid char in xml");
	}
	return parser_state;
}


//done!
VTDGen::pState VTDGen::process_pi_val(){
	//int length1;
	pState parser_state=(pState)0;
	if (!XMLChar_isSpaceChar(ch)){		
		throw ParseException(	
			"Parse exception in parse()"\
			"Other Error: Invalid char in xml");
	}
	temp_offset = offset;
	ch = getChar();
	while (true) {
		if (XMLChar_isValidChar(ch)) {
			if (ch == '?'){
				if (skipChar('>')) {
					break;
				} /*else{		
					throw ParseException(	
						"Parse exception in parse() \n"\
						"Error in PI: invalid termination sequence for PI");
				}*/
			}
		} else{		
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Errors in PI: Invalid char in PI val");
		}
		ch = getChar();
	}
	length1 = offset - temp_offset - (increment<<1);
	if (singleByteEncoding){
		if (length1 > MAX_TOKEN_LENGTH){
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Token Length Error: PI_VAL length too long");
		}
		_writeVTD(temp_offset, length1, TOKEN_PI_VAL, depth);
	}
	else{
		if (length1 > (MAX_TOKEN_LENGTH << 1)){
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Token Length Error: PI_VAL length too long");
		}
		_writeVTD(temp_offset >> 1,
			length1 >> 1,
			TOKEN_PI_VAL,
			depth);
	}
	temp_offset = offset;
	ch = getCharAfterS();
	if (ch == '<') {
		if (ws) 
			addWhiteSpaceRecord();
		parser_state = STATE_LT_SEEN;
	} else if (XMLChar_isContentChar(ch)) {
		parser_state = STATE_TEXT;
	} else if (ch == '&') {
		entityIdentifier();
		parser_state = STATE_TEXT;
	} else if (ch == ']') {
		if (skipChar(']')) {
			while (skipChar(']')) {
			}
			if (skipChar('>')){		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"Error in text content: ]]> in text content");
			}
		}
		parser_state = STATE_TEXT;
	}
	else{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"Error in text content: Invalid char");
	}
	return parser_state;
}
//done!
VTDGen::pState VTDGen::process_pi_tag(){	
	//int length1;
	pState parser_state=(pState)0;
	while (true) {
		ch = getChar();
		if (!XMLChar_isNameChar(ch))
			break;
	}

	length1 = offset - temp_offset - increment;
	if (singleByteEncoding){
		if (length1 > MAX_TOKEN_LENGTH){
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Token Length Error: PI_TAG length too long");
		}
		_writeVTD(
			(temp_offset),
			length1,
			TOKEN_PI_NAME,
			depth);
	}
	else{													
		if (length1 > (MAX_TOKEN_LENGTH << 1)){
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Token Length Error: PI_TAG length too long");
		}
		_writeVTD(
			(temp_offset) >> 1,
			(length1 >> 1),
			TOKEN_PI_NAME,
			depth);
	}
	//temp_offset = offset;
	//if (XMLChar_isSpaceChar(ch)) {
	//	ch = getChar();
	//}
	if (ch == '?') {
		if (singleByteEncoding){
			_writeVTD(
				(temp_offset),
				0,
				TOKEN_PI_VAL,
				depth);
		}
		else{
			_writeVTD(
				(temp_offset) >> 1,
				0,
				TOKEN_PI_VAL,
				depth);
		}
		if (skipChar('>')) {
			temp_offset = offset;
			ch = getCharAfterS();
			if (ch == '<') {
				if (ws) 
					addWhiteSpaceRecord();
				parser_state = STATE_LT_SEEN;
			} else if (XMLChar_isContentChar(ch)) {
				parser_state = STATE_TEXT;
			} else if (ch == '&') {
				//has_amp = true;
				entityIdentifier();
				parser_state = STATE_TEXT;
			} else if (ch == ']') {
				if (skipChar(']')) {
					while (skipChar(']')) {
					}
					if (skipChar('>')){		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"Error in text content: ]]> in text content");
					}
				}
				parser_state = STATE_TEXT;
			}else
			{		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"Error in text content: Invalid char");
			}
			return parser_state;
		} else
		{		
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Error in PI: invalid termination sequence");
		}
	}
	parser_state = STATE_PI_VAL;
	return parser_state;
}
//done!
VTDGen::pState VTDGen::process_dec_attr(){

	pState parser_state=(pState)0;
	if (ch == 'v'
		&& skipChar('e')
		&& skipChar('r')
		&& skipChar('s')
		&& skipChar('i')
		&& skipChar('o')
		&& skipChar('n')) {
			ch = getCharAfterS();
			if (ch == '=') {
				if (singleByteEncoding){
					_writeVTD(
						temp_offset - 1,
						7,
						TOKEN_DEC_ATTR_NAME,
						depth);
				}
				else{
					_writeVTD(
						(temp_offset - 2) >> 1,
						7,
						TOKEN_DEC_ATTR_NAME,
						depth);
				}
			} 
			else
			{		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"XML decl error: Invalid char");
			}
	} else{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"XML decl error: should be version");
	}
	ch_temp = getCharAfterS();
	if (ch_temp != '\'' && ch_temp != '"')
	{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"XML decl error: Invalid char to start attr name");
	}
	temp_offset = offset;
	/* support 1.0 or 1.1*/
	if (skipChar('1')
		&& skipChar('.')
		&& (skipChar('0') || skipChar('1'))) {
			if (encoding < FORMAT_UTF_16BE){
				writeVTD(
					temp_offset,
					3,
					TOKEN_DEC_ATTR_VAL,
					depth);
			}
			else{
				writeVTD(
					temp_offset >> 1,
					3,
					TOKEN_DEC_ATTR_VAL,
					depth);
			}
	} else
	{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"XML decl error: Invalid version(other than 1.0 or 1.1) detected");
	}
	if (!skipChar(ch_temp))
	{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"XML decl error: version not terminated properly");
	}
	ch = getChar();
	//? space or e 
	if (XMLChar_isSpaceChar(ch)) {
		ch = getCharAfterS();
		temp_offset = offset - increment;
		if (ch == 'e') {
			if (skipChar('n')
				&& skipChar('c')
				&& skipChar('o')
				&& skipChar('d')
				&& skipChar('i')
				&& skipChar('n')
				&& skipChar('g')) {
					ch = getChar();
					if (XMLChar_isSpaceChar(ch))
						ch = getCharAfterS();
					if (ch == '=') {
						if (encoding < FORMAT_UTF_16BE){
							writeVTD(
								temp_offset,
								8,
								TOKEN_DEC_ATTR_NAME,
								depth);
						}
						else{
							writeVTD(
								temp_offset >> 1,
								8,
								TOKEN_DEC_ATTR_NAME,
								depth);
						}
					} else{		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid char");
					}
					ch_temp = getCharAfterS();
					if (ch_temp != '"' && ch_temp != '\'')
					{		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid char to start attr name");
					}
					temp_offset = offset;
					ch = getChar();
					switch (ch) {
					case 'a' :
					case 'A' :{
						if ((skipChar('s')
							|| skipChar('S'))
							&& (skipChar('c')
							|| skipChar('C'))
							&& (skipChar('i')
							|| skipChar('I'))
							&& (skipChar('i')
							|| skipChar('I'))
							&& skipChar(ch_temp)) {
								if (encoding != FORMAT_UTF_16LE
									&& encoding
									!= FORMAT_UTF_16BE) {
										if (must_utf_8){
											throw ParseException(	
												"Parse exception in parse() \n"\
												"Can't switch from UTF-8");
										}
										encoding = FORMAT_ASCII;

										if (encoding
											< FORMAT_UTF_16BE){
												writeVTD(
													temp_offset,
													5,
													TOKEN_DEC_ATTR_VAL,
													depth);
										}
										else{
											writeVTD(
												temp_offset >> 1,
												5,
												TOKEN_DEC_ATTR_VAL,
												depth);
										}
										break;
								} else
								{		
									throw ParseException(	
										"Parse exception in parse() \n"\
										"XML decl error: Can't switch encoding to ASCII");
								}

						}

						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid Encoding");
							  }

					case 'c':
					case 'C': 
						matchCPEncoding();
						break;
					case 'i' :
					case 'I' :
						matchISOEncoding();
						break;
					case 'u' :
					case 'U' :
						matchUTFEncoding();
						break;
					case 'w':
					case 'W':
						matchWindowsEncoding();
						break;
					default :
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid Encoding");
						break;						
					}
					ch = getChar();
					if (XMLChar_isSpaceChar(ch))
						ch = getCharAfterS();
					temp_offset = offset - increment;
			} else{		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"XML decl Error: Invalid char");
			}
		}

		if (ch == 's') {
			if (skipChar('t')
				&& skipChar('a')
				&& skipChar('n')
				&& skipChar('d')
				&& skipChar('a')
				&& skipChar('l')
				&& skipChar('o')
				&& skipChar('n')
				&& skipChar('e')) {

					ch = getCharAfterS();
					if (ch != '='){		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid char");
					}
					if (encoding < FORMAT_UTF_16BE){
						writeVTD(
							temp_offset,
							10,
							TOKEN_DEC_ATTR_NAME,
							depth);
					}
					else{
						writeVTD(
							temp_offset >> 1,
							10,
							TOKEN_DEC_ATTR_NAME,
							depth);
					}
					ch_temp = getCharAfterS();
					temp_offset = offset;
					if (ch_temp != '"' && ch_temp != '\''){		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: Invalid char to start attr name");
					}
					ch = getChar();
					if (ch == 'y') {
						if (skipChar('e')
							&& skipChar('s')
							&& skipChar(ch_temp)) {
								if (encoding < FORMAT_UTF_16BE)
									writeVTD(
									temp_offset,
									3,
									TOKEN_DEC_ATTR_VAL,
									depth);
								else
									writeVTD(
									temp_offset >> 1,
									3,
									TOKEN_DEC_ATTR_VAL,
									depth);
						} else{		
							throw ParseException(	
								"Parse exception in parse() \n"\
								"XML decl error: invalid val for standalone");
						}
					} else if (ch == 'n') {
						if (skipChar('o')
							&& skipChar(ch_temp)) {
								if (encoding < FORMAT_UTF_16BE)
									writeVTD(
									temp_offset,
									2,
									TOKEN_DEC_ATTR_VAL,
									depth);
								else
									writeVTD(
									temp_offset >> 1,
									2,
									TOKEN_DEC_ATTR_VAL,
									depth);
						} else{		
							throw ParseException(	
								"Parse exception in parse() \n"\
								"XML decl error: invalid val for standalone");
						}
					} else{		
						throw ParseException(	
							"Parse exception in parse() \n"\
							"XML decl error: invalid val for standalone");
					}
			} else{		
				throw ParseException(	
					"Parse exception in parse() \n"\
					"XML decl error");
			}
			ch = getChar();
			if (XMLChar_isSpaceChar(ch))
				ch = getCharAfterS();
		}
	}

	if (ch == '?' && skipChar('>')) {
		temp_offset = offset;
		ch = getCharAfterS();
		if (ch == '<') {
			parser_state = STATE_LT_SEEN;
		} else{		
			throw ParseException(	
				"Parse exception in parse() \n"\
				"Other Error: Invalid Char in XML");
		}
	} else{		
		throw ParseException(	
			"Parse exception in parse() \n"\
			"XML decl Error: Invalid termination sequence");
	}
	return parser_state;
}
//void throwInvalidEncodingException();
//done!
VTDGen::pState VTDGen::process_start_doc(){
	int c = getChar();
	if (c == '<') {
		temp_offset = offset;
		/* xml decl has to be right after the start of the document*/
		if (skipChar('?')
			&& (skipChar('x') || skipChar('X'))
			&& (skipChar('m') || skipChar('M'))
			&& (skipChar('l') || skipChar('L'))) {
				if (skipChar(' ')
					|| skipChar('\t')
					|| skipChar('\n')
					|| skipChar('\r')) {
						ch = getCharAfterS();
						temp_offset = offset;
						return STATE_DEC_ATTR_NAME;
				} else if (skipChar('?')){
					throw ParseException(	
						"Parse exception in parse() \n"\
						"Error in XML decl: premature ending");
				}
		}
		offset = temp_offset;
		return STATE_LT_SEEN;
	}else if (c==' '||c=='\n'||c=='\r'||c=='\t'){ 
		if (getCharAfterS()=='<'){ 
			return STATE_LT_SEEN;; 
		} 
	}                 


	throw ParseException(	
		"Parse exception in parse() \n"\
		"Other Error: XML not starting properly");
	return (pState)0;
}
//done!
VTDGen::pState VTDGen::process_end_doc(){
	ch = getCharAfterS();
	if (ch == '<') {
		if (skipChar('?')) {
			temp_offset = offset;
			return STATE_END_PI;
		} else if (
			skipChar('!')
			&& skipChar('-')
			&& skipChar('-')) {
				temp_offset = offset;
				return STATE_END_COMMENT;
		}
	}
	/*printf("**********************\n");
	printf(" char is %i \n", ch);
	printf("**********************\n");*/
	throw ParseException(	
		"Parse exception in parse() \n"\
		"Other Error: XML not terminated properly");
	return (pState)0;
}
//done!
VTDGen::pState VTDGen::process_qm_seen(){
	temp_offset = offset;
	ch = getChar();
	if (XMLChar_isNameStartChar(ch)) {
		if ((ch == 'x' || ch == 'X')
			&& (skipChar('m') || skipChar('M'))
			&& (skipChar('l') || skipChar('L'))) {
				ch = getChar();
				if (ch == '?'
					|| XMLChar_isSpaceChar(ch)){
						throw ParseException(	
							"Parse exception in parse() \n"\
							"Error in PI: [xX][mM][lL] not a valid PI targetname");
				}
				offset = getPrevOffset();
		}
		return STATE_PI_TAG;
	}
	throw ParseException(	
		"Parse exception in parse() \n"\
		"Other Error: First char after <? invalid");
	return (pState)0;
}
//done!
VTDGen::pState VTDGen::process_ex_seen(){
	bool hasDTD = false;
	pState parser_state;
	ch = getChar();
	switch (ch) {
					case '-' :
						if (skipChar('-')) {
							temp_offset = offset;
							parser_state = STATE_COMMENT;
							break;
						} else
						{		
							throw ParseException(	
								"Parse exception in parse() \n"\
								"Error in comment: Invalid char sequence to start a comment");
						}
					case '[' :
						if (skipChar('C')
							&& skipChar('D')
							&& skipChar('A')
							&& skipChar('T')
							&& skipChar('A')
							&& skipChar('[')
							&& (depth != -1)) {
								temp_offset = offset;
								parser_state = STATE_CDATA;
								break;
						} else {
							if (depth == -1){
								throw ParseException(	
									"Parse exception in parse() \n"\
									"Error in CDATA: Invalid char sequence for CDATA");
							}
							throw ParseException(	
								"Parse exception in parse() \n"\
								"Error in CDATA: Invalid char sequence for CDATA");
						}

					case 'D' :
						if (skipChar('O')
							&& skipChar('C')
							&& skipChar('T')
							&& skipChar('Y')
							&& skipChar('P')
							&& skipChar('E')
							&& (depth == -1)
							&& !hasDTD) {
								hasDTD = true;
								temp_offset = offset;
								parser_state = STATE_DOCTYPE;
								break;
						} else {
							if (hasDTD == true){
								throw ParseException(	
									"Parse exception in parse() \n"\
									"Error for DOCTYPE: Only DOCTYPE allowed");
							}
							if (depth != -1){
								throw ParseException(	
									"Parse exception in parse() \n"\
									"Error for DOCTYPE: DTD at wrong place");
							}

							throw ParseException(	
								"Parse exception in parse() \n"\
								"Error for DOCTYPE: Invalid char sequence for DOCTYPE");
						}
					default :
						throw ParseException(	
							"Parse exception in parse() \n"\
							"Other Error: Unrecognized char after <!");
	}
	return parser_state;
}
//done!
void VTDGen::addWhiteSpaceRecord(){
	if (depth > -1) {
		int length1 = offset - increment - temp_offset;
		if (length1 != 0){
			if (singleByteEncoding)
				writeVTDText(temp_offset, length1, 
				TOKEN_CHARACTER_DATA, depth);
			else
				writeVTDText( temp_offset >> 1,length1 >> 1,
				TOKEN_CHARACTER_DATA, depth);
		}
	}
}
//done!
VTDGen::VTDGen() try: 
ns(false),
VTDDepth(0),
last_depth(0),
last_l1_index(0),
last_l2_index(0),
last_l3_index(0),
offset(0),
ws(false),
prev_offset(0),
rootIndex(0),
XMLDoc(NULL),
docLen(0),
bufLen(0),
LcDepth(3),
//singleByteEncoding(true),
shallowDepth(true),
VTDBuffer(NULL),
l1Buffer(NULL),
l2Buffer(NULL),
l3Buffer(NULL),
_l3Buffer(NULL),
_l4Buffer(NULL),
_l5Buffer(NULL),
nsBuffer1(new FastIntBuffer(4)),
nsBuffer2(new FastLongBuffer(4)),
nsBuffer3(new FastLongBuffer(4)),
currentElementRecord(0),
br(false),
stateTransfered(true),
endOffset(0),
tag_stack(new Long[TAG_STACK_SIZE]),
attr_name_array(new Long[ATTR_NAME_ARRAY_SIZE]),
prefixed_attr_name_array(new Long[ATTR_NAME_ARRAY_SIZE]),
prefix_URL_array(new int[ATTR_NAME_ARRAY_SIZE]),
anaLen(ATTR_NAME_ARRAY_SIZE),
panaLen(ATTR_NAME_ARRAY_SIZE)

{
	//Long* l = NULL;
	//Long* ts = NULL;
	//VTDGen * = NULL;

	//l = (Long*) malloc(ATTR_NAME_ARRAY_SIZE*sizeof(Long));
	
	/*if (l==NULL){
	throwException2(out_of_mem,
	"VTDGen allocation failed ");
	return NULL;
	}*/
	//ts = (Long*) malloc(TAG_STACK_SIZE * sizeof(Long));
	
	/*if (ts==NULL) {
	free(l);
	throwException2(out_of_mem,
	"VTDGen allocation failed ");
	return NULL;
	}*/

	/* = (VTDGen *)malloc(sizeof(VTDGen));
	if (==NULL){
	free(l);
	free(ts);
	throwException2(out_of_mem,
	"VTDGen allocation failed ");
	return NULL;
	}*/
	//anaLen = ATTR_NAME_ARRAY_SIZE;
	//attr_name_array = l;
	//tag_stack = ts;

	//VTDDepth = 0;

	//VTDBuffer =	l1Buffer = 	l2Buffer = NULL;
	//l3Buffer = NULL;
	//bufLen =docLen = docLen = last_depth = 0;
	//last_l3_index = last_l2_index = last_l1_index = 0;
	//XMLDoc = NULL;
	//rootIndex = endOffset= 0;
	//ns= false;
	//offset = prev_offset =0;
	//stateTransfered = true; // free VTDGen won't free all location cache and VTD buffer

	//br = false;
	//ws = false;
}
catch(std::bad_alloc&){
		throw OutOfMemException("array list allocation in FastLongBuffer's constructor failed");
}
//done!
VTDGen::~VTDGen(){
	delete[] attr_name_array;
	delete[] prefix_URL_array;
	delete[] prefixed_attr_name_array;
	delete[] tag_stack;
	delete nsBuffer1;
	delete nsBuffer2;
	delete nsBuffer3;
	if (stateTransfered == false || br == true){
		//free(XMLDoc);
		delete VTDBuffer;
		delete l1Buffer;
		delete l2Buffer;
		delete l3Buffer;
		delete _l3Buffer;
		delete _l4Buffer;
		delete _l5Buffer;
	}
}
//done!
void VTDGen::clear(){	
	if (br == false){
		if (stateTransfered == false){
			//free(XMLDoc);
			delete VTDBuffer;
			delete l1Buffer;
			delete l2Buffer;
			delete l3Buffer;
			delete _l3Buffer;
			delete _l4Buffer;
			delete _l5Buffer;
		}
		VTDBuffer = NULL;
		l1Buffer = NULL;
		l2Buffer = NULL;
		l3Buffer = NULL;
		_l3Buffer = NULL;
		_l4Buffer = NULL;
		_l5Buffer = NULL;
		XMLDoc = NULL;
	}

	last_depth = last_l1_index = 
		last_l2_index = last_l3_index = last_l4_index=0;
	offset = temp_offset = 0;
	rootIndex = 0;

	depth = -1;
	increment = 1;
	BOM_detected = false;
	must_utf_8 = false;
	ch = ch_temp = 0;
	encoding = FORMAT_UTF8;
}

VTDNav* VTDGen::getNav(){
	VTDNav *vn;
	if (shallowDepth)

		vn= new VTDNav(rootIndex,
		encoding,
		ns,
		VTDDepth,
		XMLDoc,
		bufLen,
		VTDBuffer,
		l1Buffer,
		l2Buffer,
		l3Buffer,
		docOffset,
		docLen,
		br);
	else
		vn= new VTDNav_L5(rootIndex,
		encoding,
		ns,
		VTDDepth,
		XMLDoc,
		bufLen,
		VTDBuffer,
		l1Buffer,
		l2Buffer,
		_l3Buffer,
		_l4Buffer,
		_l5Buffer,
		docOffset,
		docLen,
		br);
	//l1Buffer = l2Buffer  = _l3Buffer=_l4Buffer  = VTDBuffer= NULL;
	//_l5Buffer= l3Buffer = NULL;
	stateTransfered = true;
	vn->master = true;
	clear();
	return vn;
}
void VTDGen::parse(bool ns1){
	//int length1 = 0, length2 = 0;
	//int attr_count = 0 /*, ch = 0, ch_temp = 0*/;
	/*int prev_ch = 0, prev2_ch = 0; */
	volatile pState parser_state = STATE_DOC_START;
	/*bool has_amp = false; */ 
	//bool is_ns = false;
	//bool unique;
	//bool unequal;
	bool helper = false;
	bool default_ns = false;
	bool isXML = false;
	/*bool BOM_detected = false;*/
	/*bool must_utf_8 = false; */
	Long x;
	/*char char_temp; //holds the ' or " indicating start of attr val */
	int sos = 0, sl = 0;
	length1 = length2=0;
	XMLChar_init();
	ns = ns1;
	is_ns = false;
	encoding = FORMAT_UTF8;
	singleByteEncoding = true;
	attr_count = prefixed_attr_count = 0;

	/* first check first 2 bytes BOM to determine if encoding is UTF16*/
	decide_encoding();

	/* enter the main finite state machine */
	try {
		_writeVTD(0,0,TOKEN_DOCUMENT,depth);
		while (true) {
			switch (parser_state) {

					case STATE_LT_SEEN : 
						temp_offset = offset;
						ch = getChar();
						if (XMLChar_isNameStartChar(ch)) {
							depth++;
							parser_state = STATE_START_TAG;
						} else {
							switch (ch) {
					case '/' :
						parser_state = STATE_END_TAG;
						break;
					case '?' :
						parser_state = process_qm_seen();
						break;
					case '!' : 
						parser_state = process_ex_seen();
						break;
					default :throw ParseException(
								 "Parse exception in parse() \n"\
								 "Other Error: Invalid char after <");
							}
						}
						break;

					case STATE_START_TAG : 
						do {
							ch = getChar();
							if (XMLChar_isNameChar(ch)) {
								if (ch == ':') {
									length2 = offset - temp_offset - increment;
									if (ns && checkPrefix2(temp_offset,length2))
										throw ParseException("xmlns can't be an element prefix ");
								}
							} else
								break;
						} while (true);
						length1 = offset - temp_offset - increment;
						x = ((Long) length1 << 32) + temp_offset;
						tag_stack[depth] = x;
						if (depth > MAX_DEPTH) {
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Other Error: Depth exceeds MAX_DEPTH");
						}
						if (depth > VTDDepth)
							VTDDepth = depth;
						if (singleByteEncoding){
							if (length2>MAX_PREFIX_LENGTH 
								|| length1 > MAX_QNAME_LENGTH){
									throw ParseException(
								 "Parse exception in parse() \n"\
										"Token Length Error: Starting tag prefix or qname length too long");
							}
							if (this->shallowDepth)
								writeVTD(
								(temp_offset),
								(length2 << 11) | length1,
								TOKEN_STARTING_TAG,
								depth);
							else
								writeVTD_L5(
								(temp_offset),
								(length2 << 11) | length1,
								TOKEN_STARTING_TAG,
								depth);
						}
						else{
							if ((length2>(MAX_PREFIX_LENGTH<<1)) 
								|| (length1 > (MAX_QNAME_LENGTH <<1))){
									throw ParseException(
								 "Parse exception in parse() \n"\
										"Token Length Error: Starting tag prefix or qname length too long");
							}
							if (shallowDepth)							
								writeVTD(								
								(temp_offset) >> 1,
								(length2 << 10) | (length1 >> 1),
								TOKEN_STARTING_TAG,
								depth);
							else
								writeVTD_L5(
								(temp_offset) >> 1,
								(length2 << 10) | (length1 >> 1),
								TOKEN_STARTING_TAG,
								depth);
						}
						if (ns) {
							if (length2!=0){
								length2 += increment;
								currentElementRecord = (((Long)((length2<<16)|length1))<<32) 
								| temp_offset;
							} else
								currentElementRecord = 0;
						
							if (depth <= nsBuffer1->size - 1) {
								int t;
								nsBuffer1->resize(depth);
								t= nsBuffer1->intAt(depth-1)+1;
								nsBuffer2->resize(t);
								nsBuffer3->resize(t);
							}
						}
						length2 = 0;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS();
							if (XMLChar_isNameStartChar(ch)) {
								temp_offset = getPrevOffset();
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}
						helper = true;
						if (ch == '/') {
							depth--;
							helper = false;
							ch = getChar();
						}
						if (ch == '>') {
							if (ns){
								nsBuffer1->append(nsBuffer3->size-1);
								if (currentElementRecord !=0)
									qualifyElement();
							}
							if (depth != -1) {
								temp_offset = offset;
								ch = getCharAfterS(); // consume WSs
								if (ch == '<') {
									if (ws) 
										addWhiteSpaceRecord();
									parser_state = STATE_LT_SEEN;
									if (skipChar('/')) {
										if (helper){
											length1 =
												offset
												- temp_offset
												- (increment<<1);

											if (singleByteEncoding)
												writeVTDText(
												(temp_offset),
												length1,
												TOKEN_CHARACTER_DATA,
												depth);
											else
												writeVTDText(
												(temp_offset) >> 1,
												(length1 >> 1),
												TOKEN_CHARACTER_DATA,
												depth);

										}
										//offset += length1;
										parser_state = STATE_END_TAG;
										break;
									}
								} else if (XMLChar_isContentChar(ch)) {

									parser_state = STATE_TEXT;
								} else {
									
									parser_state = STATE_TEXT;
									handleOtherTextChar2(ch);
								}
								
							} else {
								parser_state = STATE_DOC_END;
							}
							break;
						}

						throw ParseException(
								 "Parse exception in parse() \n"\
							"Starting tag Error: Invalid char in starting tag");

					case STATE_END_TAG :
						temp_offset = offset;

						sos = (int) tag_stack[depth];
						sl = (int) (tag_stack[depth] >> 32);
						offset = temp_offset + sl;
						if (offset >= endOffset){
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Premature EOF reached, XML document incomplete");
						}

						if (memcmp(XMLDoc+temp_offset, XMLDoc+sos, sl)!=0){
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Ending tag error: Start/ending tag mismatch");
						}

						depth--;
						ch = getCharAfterS();

						if (ch!='>'){
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Ending tag error: Invalid char in end tag");

						}


						if (depth != -1) {
							temp_offset = offset;
							ch = getCharAfterS();
							if (ch == '<'){
								if (ws) 
									addWhiteSpaceRecord();
								parser_state = STATE_LT_SEEN;
							}
							else if (XMLChar_isContentChar(ch)) {
								parser_state = STATE_TEXT;
							} else {
									
									parser_state = STATE_TEXT;
									handleOtherTextChar2(ch);
							}
						} else
							parser_state = STATE_DOC_END;
						break;
					case STATE_ATTR_NAME :

						if (ch == 'x') {
							if (skipChar('m')
								&& skipChar('l')
								&& skipChar('n')
								&& skipChar('s')) {
									ch = getChar();
									if (ch == '='
										|| XMLChar_isSpaceChar(ch)) {
											is_ns = true; //break;
											default_ns = true;
									} else if (ch ==':'){
										is_ns = true;
										default_ns = false;
									}
							}
						}
						do {
							if (XMLChar_isNameChar(ch)) {
								if (ch == ':') {
									length2 = offset - temp_offset - increment;
								}
								ch = getChar();
							} else
								break;
						}while (true);
						length1 = getPrevOffset() - temp_offset;
						if (is_ns && ns){
							// make sure postfix isn't xmlns
							if (!default_ns){
								if ((increment==1 && (length1-length2-1 == 5))
										|| (increment==2 && (length1-length2-2==10)))
									disallow_xmlns(temp_offset+length2+increment);
							    
							// if the post fix is xml, signal it
							    if ((increment==1 && (length1-length2-1 == 3))
										|| (increment==2 && (length1-length2-2==6)))
							    	isXML = matchXML(temp_offset+length2+increment);
							}
						}
						checkAttributeUniqueness();
						//unique = true;
						/*for (i = 0; i < attr_count; i++) {
							int prevLen;
							unequal = false;
							prevLen = (int) attr_name_array[i];
							if (length1 == prevLen) {
								int prevOffset =
									(int) (attr_name_array[i] >> 32);
								for (j = 0; j < prevLen; j++) {
									if (XMLDoc[prevOffset + j]
									!= XMLDoc[temp_offset + j]) {
										unequal = true;
										break;
									}
								}
							} else
								unequal = true;
							unique = unique && unequal;
						}
						if (!unique && attr_count != 0){		
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Error in attr: Attr name not unique");
						}
						unique = true;
						if (attr_count < anaLen) {
							attr_name_array[attr_count] =
								((Long) (temp_offset) << 32) + length1;
							attr_count++;
						} else 
						{
							Long* temp_array = attr_name_array;
							attr_name_array = 
								(Long *)malloc(sizeof(Long)*
								(attr_count + ATTR_NAME_ARRAY_SIZE));

							if (attr_name_array == NULL){
								throw ParseException(
								 "Parse exception in parse() \n"\
									"alloc mem for attr_name_array_failed");
							}
							anaLen = attr_count + ATTR_NAME_ARRAY_SIZE;

							for (i = 0; i < attr_count; i++) {
								attr_name_array[i] = temp_array[i];
							}
							attr_name_array[attr_count] =
								((Long) (temp_offset) << 32) + length1;
							attr_count++;
						}*/

						if (is_ns) {
							if (singleByteEncoding){
								if (length2>MAX_PREFIX_LENGTH 
									|| length1 > MAX_QNAME_LENGTH){
										throw ParseException(
								 "Parse exception in parse() \n"\
											"Token Length Error: Attr NS prefix or qname length too long");
								}

								_writeVTD(temp_offset,
									(length2 << 11) | length1,
									TOKEN_ATTR_NS,
									depth);

							}
							else{
								if (length2>(MAX_PREFIX_LENGTH<<1) 
									|| length1 >(MAX_QNAME_LENGTH<<1)){
										throw ParseException(
								 "Parse exception in parse() \n"\
											"Token Length Error: Attr NS prefix or qname length too long");
								}

								_writeVTD(temp_offset >> 1,
									(length2 << 10) | (length1 >> 1),
									TOKEN_ATTR_NS,
									depth);
							}

							if (ns) {								
								//unprefixed xmlns are not recorded
								if (length2 != 0 && !isXML) {
									//nsBuffer2.append(VTDBuffer.size() - 1);
									Long l = ((Long) ((length2 << 16) | length1)) << 32
										| temp_offset;
									nsBuffer3->append(l); // byte offset and byte
									// length
								}
							}
							//is_ns = false;
						} else {
							if (singleByteEncoding){
								if (length2>MAX_PREFIX_LENGTH 
									|| length1 > MAX_QNAME_LENGTH){
										throw ParseException(
								 "Parse exception in parse() \n"\
											"Token Length Error: Attr name prefix or qname length too long");
								}
								_writeVTD(temp_offset,
									(length2 << 11) | length1,
									TOKEN_ATTR_NAME,
									depth);

							}
							else {
								if (length2> (MAX_PREFIX_LENGTH <<1)
									|| length1 > (MAX_QNAME_LENGTH<<1)){
										throw ParseException(
								 "Parse exception in parse() \n"\
											"Token Length Error: Attr name prefix or qname length too long");
								}

								_writeVTD(temp_offset >> 1,
									(length2 << 10) | (length1 >> 1),
									TOKEN_ATTR_NAME,
									depth);

							}
						}
						length2 = 0;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS();
						}
						if (ch != '='){		
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Error in attr: invalid char");
						}
						ch_temp = getCharAfterS();
						if (ch_temp != '"' && ch_temp != '\''){		
							throw ParseException(
								 "Parse exception in parse() \n"\
								"Error in attr: invalid char (should be ' or \" )");
						}
						temp_offset = offset;
						parser_state = STATE_ATTR_VAL;
						break;

					case STATE_ATTR_VAL :
						 do {
							ch = getChar();
							if (XMLChar_isValidChar(ch) && ch != '<') {
								if (ch == ch_temp)
									break;
								if (ch == '&') {
									if (!XMLChar_isValidChar(entityIdentifier())) {		
									throw ParseException(
								 "Parse exception in parse() \n"\
											"Error in attr: Invalid XML char");
									}
								}

							} else {		
								throw ParseException(
								 "Parse exception in parse() \n"\
									"Error in attr: Invalid XML char");
							}
						}while (true);

						length1 = offset - temp_offset - increment;

						if (ns && is_ns){
							int t;
							if (!default_ns && length1==0){
								throw ParseException(" non-default ns URL can't be empty");
									//+formatLineNumber());								
							}
							//identify nsURL return 0,1,2
							t= identifyNsURL(temp_offset, length1);
							if (isXML){//xmlns:xml
								if (t!=1)
									//URL points to "http://www.w3.org/XML/1998/namespace"
									throw ParseException("xmlns:xml can only point to"\
									"\"http://www.w3.org/XML/1998/namespace\"" );
									//+ formatLineNumber());

							} else {
								if (!default_ns)
									nsBuffer2->append(((Long)temp_offset<<32) | length1);
								if (t!=0){		
									if (t==1)
										throw ParseException("namespace declaration can't point to"\
										" \"http://www.w3.org/XML/1998/namespace\"" );
										//+ formatLineNumber());
									throw ParseException("namespace declaration can't point to"\
										" \"http://www.w3.org/2000/xmlns/\"" );
										//+ formatLineNumber());	
								}
							}							
							// no ns URL points to 
							//"http://www.w3.org/2000/xmlns/"

							// no ns URL points to  
							//"http://www.w3.org/XML/1998/namespace"
						}
						if (singleByteEncoding){
							if (length1 > MAX_TOKEN_LENGTH){
								throw ParseException(
								 "Parse exception in parse() \n"\
									"Token Length Error: ATTR_VAL length too long");
							}
							_writeVTD(temp_offset,
								length1,
								TOKEN_ATTR_VAL,
								depth);
						}
						else{
							if (length1 > (MAX_TOKEN_LENGTH << 1)){
								throw ParseException(
								 "Parse exception in parse() \n"\
									"Token Length Error: ATTR_VAL length too long");
							}
							_writeVTD(temp_offset >> 1,
								length1 >> 1,
								TOKEN_ATTR_VAL,
								depth);
						}
						isXML = false;
						is_ns = false;
						ch = getChar();
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS();
							if (XMLChar_isNameStartChar(ch)) {
								temp_offset = offset - increment;
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}

						helper = true;
						if (ch == '/') {
							depth--;
							helper = false;
							ch = getChar();
						}

						if (ch == '>') {							
							if (ns){
								nsBuffer1->append(nsBuffer3->size-1);
								if (prefixed_attr_count>0)
									qualifyAttributes();
								if (prefixed_attr_count > 1)
									checkQualifiedAttributeUniqueness();
								if (currentElementRecord!=0)
									qualifyElement();
								prefixed_attr_count=0;
							}
							attr_count = 0;
							if (depth != -1) {
								temp_offset = offset;
								ch = getCharAfterSe();
								if (ch == '<') {
									if (ws) 
										addWhiteSpaceRecord();
									parser_state = STATE_LT_SEEN;
									if (skipChar('/')) {
										if (helper){
											length1 =
												offset
												- temp_offset
												- (increment<<1);

											if (singleByteEncoding)
												writeVTDText((temp_offset),
												length1,
												TOKEN_CHARACTER_DATA,
												depth);
											else
												writeVTDText(
												(temp_offset) >> 1,
												(length1 >> 1),
												TOKEN_CHARACTER_DATA,
												depth);

										}
										//offset += length1;
										parser_state = STATE_END_TAG;
										break;
									}
								} else if (XMLChar_isContentChar(ch)) {
									parser_state = STATE_TEXT;
								} else{
									handleOtherTextChar2(ch);
									parser_state = STATE_TEXT;
								}
								
							} else {
								parser_state = STATE_DOC_END;
							}
							break;
						}

						throw ParseException(
								 "Parse exception in parse() \n"\
							"Starting tag Error: Invalid char in starting tag");

					case STATE_TEXT :
						if (depth == -1){throw ParseException(
								 "Parse exception in parse() \n"\
							"Error in text: Char data at the wrong place");
						}
						 do {
							ch = getChar();
							if (XMLChar_isContentChar(ch)) {
							} else if (ch == '<') {
								break;
							} else 
								handleOtherTextChar(ch);
							ch = getChar();
							if (XMLChar_isContentChar(ch)) {
							} else if (ch == '<') {
								break;
							} else 
								handleOtherTextChar(ch);
						} while (true);
						length1 = offset - increment - temp_offset;						
						if (singleByteEncoding){
							writeVTDText(temp_offset,
								length1,
								TOKEN_CHARACTER_DATA,
								depth);
						}
						else{
							writeVTDText(temp_offset >> 1,
								length1 >> 1,
								TOKEN_CHARACTER_DATA,
								depth);
						}
						parser_state = STATE_LT_SEEN;
						break;

					case STATE_DOC_START :
						parser_state = process_start_doc();
						break;

					case STATE_DOC_END :
						parser_state = process_end_doc();
						break;

					case STATE_PI_TAG :
						parser_state = process_pi_tag();
						break;

					case STATE_PI_VAL :
						parser_state = process_pi_val();
						break;

					case STATE_DEC_ATTR_NAME :
						parser_state = process_dec_attr();
						break;

					case STATE_COMMENT :
						parser_state = process_comment();
						break;

					case STATE_CDATA :
						parser_state = process_cdata();
						break;

					case STATE_DOCTYPE :
						parser_state = process_doc_type();
						break;
					case STATE_END_COMMENT :
						parser_state = process_end_comment();
						break;
					case STATE_END_PI :										  

						parser_state = process_end_pi();
						break;

					

					default :	
						throw ParseException(
								 "Parse exception in parse() \n"\
							"Other error: invalid parser state");
			}
		}
	} 
	catch (ParseException& e) {
		if (parser_state != STATE_DOC_END){
			printf("Parsing error => %s  ",e.getMessage());
			printLineNumber();
			//printf("\n Last Offset val ===> %d \n",offset);
			throw e;
		}
		finishUp();
	}
}

UByte* VTDGen::doubleCapacity(UByte *b, size_t cap){
	UByte *t = new UByte[cap];
	memcpy(t,b,cap>>1);
	delete b;
	return t;
}
Long VTDGen::getBytes_UTF8(UCSChar *s){
	/* < 0x7f  1 byte*/
	/* < 0x7ff  2 bytes */
	/* < 0xffff  3 bytes */
	/* < 0x 10ffff  4 bytes*/
	size_t len = wcslen(s),i=0,k=0,capacity = max<size_t>(len,8);/*minimum size is 8 */
	UByte *ba = new UByte[capacity]; 
	if (ba == NULL)
		throw OutOfMemException("Byte allocation failed in getBytes_UTF_8");
	for (i=0;i<len;i++){
		if (s[i]<= 0x7f){
			if (capacity -k<1){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]=(UByte)s[i];
			k++;
		}else if (s[i] <=0x7ff){
			if (capacity -k<2){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0x7c0) >> 6) | 0xc0;
			ba[k+1] = (s[i] & 0x3f) | 0x80;
			k += 2;
		}else if (s[i] <=0xffff){
			if (capacity -k<3){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0xf000) >> 12) | 0xe0;
			ba[k+1] = ((s[i] & 0xfc) >> 6) | 0x80;
			ba[k+2] = (s[i] & 0x3f) | 0x80;
			k += 3;
		}else if (s[i] <=0x10ffff){
			if (capacity -k<4){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0x1c0000) >> 18) | 0xf0;
			ba[k+1] = ((s[i] & 0x3f0) >> 12) | 0x80;
			ba[k+2] = ((s[i] & 0xfc) >> 6) | 0x80;
			ba[k+3] = (s[i] & 0x3f) | 0x80;
			k += 4;
		}else
			throw ModifyException("Invalid XML char for getBytes_UTF_8");
	}
	return ((Long)k<<32)|(Long)ba;
}
bool VTDGen::parseFile(bool ns, const UCSChar* fileName){	
	FILE *f = NULL;
	//exception e;
	int status,len;
	UByte *ba=NULL;
	struct stat buffer;
	char *fileName2 = (char *)getBytes_UTF8((UCSChar *)fileName);
	f = fopen(fileName2,"rb");
	delete fileName2;//delete it after fopen
	if (f==NULL){
		//throwException2(invalid_argument,"fileName not valid");
		return false;
	}
	status = stat(fileName2,&buffer);
	if (status !=0){
		fclose(f);
		//throw ParseException("error occurred in parseFile");
		return false;
	}
	len = buffer.st_size;
	ba = new UByte[len];
	if (ba == NULL){
		fclose(f);
		//throwException2(out_of_mem,"error occurred in parseFile");
		return false;
	}
	if (fread(ba,1,len,f)!=(size_t)len){
		fclose(f);
		return false;
	}
	setDoc(ba,len);
	try{
		parse(ns);
	}catch(ParseException& e){
		delete[] ba;
		clear();
		fclose(f);
		printf("%s\n",e.what());
		//printf("%s\n",e.sub_msg);
		//throwException2(out_of_mem,"error occurred in parseFile");
		return false;
	}
	
	fclose(f);
	return true;
}

bool VTDGen::parseFile(bool ns, const char* fileName){
		FILE *f = NULL;
	//exception e;
	int status,len;
	UByte *ba=NULL;
	struct stat buffer;
	f = fopen(fileName,"rb");
	if (f==NULL){
		//throwException2(invalid_argument,"fileName not valid");
		return false;
	}
	status = stat(fileName,&buffer);
	if (status !=0){
		fclose(f);
		//throw ParseException("error occurred in parseFile");
		return false;
	}
	len = buffer.st_size;
	ba = new UByte[len];
	if (ba == NULL){
		fclose(f);
		//throwException2(out_of_mem,"error occurred in parseFile");
		return false;
	}
	if (fread(ba,1,len,f)!=(size_t)len){
		fclose(f);
		return false;
	}
	setDoc(ba,len);
	try{
		parse(ns);
	}catch(ParseException& e){
		delete[] ba;
		clear();
		fclose(f);
		printf("%s\n",e.what());
		//printf("%s\n",e.sub_msg);
		//throwException2(out_of_mem,"error occurred in parseFile");
		return false;
	}
	
	fclose(f);
	return true;
}

//done!!
void VTDGen::setDoc(UByte *ba, int len){
	setDoc(ba, len, 0, len);
}
//done!
void VTDGen::setDoc(UByte *ba, int len, int os, int docLen1){
	int a,i1=8,i2=9,i3=11;
	br = false;
	depth = -1;
	increment = 1;
	BOM_detected = false;
	must_utf_8 = false;
	ch = ch_temp = 0;
	temp_offset = 0;
	XMLDoc = ba;
	docOffset = offset = os;
	docLen = docLen1;
	bufLen =len;
	endOffset = os + docLen;
	last_depth = last_l3_index = last_l2_index = last_l1_index = last_l4_index;
	currentElementRecord = 0;
	nsBuffer1->clear();
	nsBuffer2->clear();
	nsBuffer3->clear();
	if (shallowDepth){
		if (docLen <= 1024) {		
			a = 6; i1=5; i2=5;i3=5;
		} else if (docLen <= 4096 * 2){
			a = 7; i1=6; i2=6; i3=6;
		}else if (docLen <=1024*16){
			a =8; i1 = 7;i2=7;i3=7;
		}
		else if (docLen <= 1024 * 16 * 4) {
			a = 11; i2= 8; i3=8;
		} else if (docLen <= 1024 * 256) {		
			a = 12;
		} else {	
			a = 15;
		}

		if (stateTransfered == false && VTDBuffer != NULL){
			delete VTDBuffer;
			delete l1Buffer;
			delete l2Buffer;
			delete l3Buffer;		
		}
		VTDBuffer = new FastLongBuffer(a, len>>(a+1)); 
		l1Buffer = new FastLongBuffer(i1);
		l2Buffer = new FastLongBuffer(i2); 
		l3Buffer = new FastIntBuffer(i3);
	}else{
		int i1 = 7, i2 = 9, i3 = 11, i4 = 11, i5 = 11;
			if (docLen <= 1024) {
				// a = 1024; //set the floor
				a = 6;
				i1 = 5;
				i2 = 5;
				i3 = 5;
				i4 = 5;
				i5 = 5;
			} else if (docLen <= 4096) {
				a = 7;
				i1 = 6;
				i2 = 6;
				i3 = 6;
				i4 = 6;
				i5 = 6;
			} else if (docLen <= 1024 * 16) {
				a = 8;
				i1 = 7;
				i2 = 7;
				i3 = 7;
			} else if (docLen <= 1024 * 16 * 4) {
				// a = 2048;
				a = 11;
				i2 = 8;
				i3 = 8;
				i4 = 8;
				i5 = 8;
			} else if (docLen <= 1024 * 256) {
				// a = 1024 * 4;
				a = 12;
				i1 = 8;
				i2 = 9;
				i3 = 9;
				i4 = 9;
				i5 = 9;
			} else if (docLen <= 1024 * 1024) {
				// a = 1024 * 4;
				a = 12;
				i1 = 8;
				i3 = 10;
				i4 = 10;
				i5 = 10;
			} else {
				// a = 1 << 15;
				a = 15;
				i1 = 8;
			}
			if (VTDBuffer != NULL && stateTransfered == false){
				delete VTDBuffer;
				delete l1Buffer;
				delete l2Buffer;
				delete _l3Buffer;
				delete _l4Buffer;
				delete _l5Buffer;
			}
			VTDBuffer = new FastLongBuffer(a, len >> (a + 1));
			l1Buffer = new FastLongBuffer(i1);
			l2Buffer = new FastLongBuffer(i2);
			_l3Buffer = new FastLongBuffer(i3);
			_l4Buffer = new FastLongBuffer(i4);
			_l5Buffer = new FastIntBuffer(i5);

	}

	stateTransfered = false;
}
//done
void VTDGen::setDoc_BR(UByte *ba, int len){
	setDoc(ba,len,0,len);
}
//done
void VTDGen::setDoc_BR(UByte *ba, int len, int os, int docLen){
	int a,i1=8,i2=9,i3=11;
	br = true;
	depth = -1;
	increment = 1;
	BOM_detected = false;
	must_utf_8 = false;
	ch = ch_temp = 0;
	temp_offset = 0;
	XMLDoc = ba;
	docOffset = offset = os;
	docLen = docLen;
	bufLen = len;
	endOffset = os + docLen;
	last_depth = last_l3_index = last_l2_index = last_l1_index;
	currentElementRecord = 0;
	nsBuffer1->clear();
	nsBuffer2->clear();
	nsBuffer3->clear();
	if (shallowDepth){
		if (VTDBuffer == NULL){
			if (docLen <= 1024) {
				a = 6; i1=5; i2=5;i3=5;
			} else if (docLen <= 4096 * 2){
				a = 7; i1=6; i2=6; i3=6;
			}
			else if (docLen <=1024*16){
				a =8; i1 = 7;i2=7;i3=7;
			}
			else if (docLen <= 1024 * 16 * 4) {
				a = 10;
			} else if (docLen <= 1024 * 256) {
				a = 12;
			} else {
				a = 15;
			}

			VTDBuffer = new FastLongBuffer(a, len>>(a+1)); 
			l1Buffer = new FastLongBuffer(i1);
			l2Buffer = new FastLongBuffer(i2); 
			l3Buffer = new FastIntBuffer(i3);
		}
		else {
			VTDBuffer->clear();
			l1Buffer->clear();
			l2Buffer->clear();
			l3Buffer->clear();
		}
	}else{
					int i1 = 8, i2 = 9, i3 = 11, i4 = 11, i5 = 11;
			if (docLen <= 1024) {
				// a = 1024; //set the floor
				a = 6;
				i1 = 5;
				i2 = 5;
				i3 = 5;
				i4 = 5;
				i5 = 5;
			} else if (docLen <= 4096) {
				a = 7;
				i1 = 6;
				i2 = 6;
				i3 = 6;
				i4 = 6;
				i5 = 6;
			} else if (docLen <= 1024 * 16) {
				a = 8;
				i1 = 7;
				i2 = 7;
				i3 = 7;
			} else if (docLen <= 1024 * 16 * 4) {
				// a = 2048;
				a = 11;
				i2 = 8;
				i3 = 8;
				i4 = 8;
				i5 = 8;
			} else if (docLen <= 1024 * 256) {
				// a = 1024 * 4;
				a = 12;
				i1 = 8;
				i2 = 9;
				i3 = 9;
				i4 = 9;
				i5 = 9;
			} else if (docLen <= 1024 * 1024) {
				// a = 1024 * 4;
				a = 12;
				i1 = 8;
				i3 = 10;
				i4 = 10;
				i5 = 10;
			} else {
				// a = 1 << 15;
				a = 15;
				i1 = 8;
			}
			if (VTDBuffer == NULL) {
				VTDBuffer = new FastLongBuffer(a, len >> (a + 1));
				l1Buffer = new FastLongBuffer(i1);
				l2Buffer = new FastLongBuffer(i2);
				_l3Buffer = new FastLongBuffer(i3);
				_l4Buffer = new FastLongBuffer(i4);
				_l5Buffer = new FastIntBuffer(i5);
			} else {
				VTDBuffer->size = 0;
				l1Buffer->size = 0;
				l2Buffer->size = 0;
				_l3Buffer->size = 0;
				_l4Buffer->size = 0;
				_l5Buffer->size = 0;
			}

	}
	stateTransfered = false;
}
VTDNav* VTDGen::loadIndex(FILE *f){
	clear();
	delete[] XMLDoc;
	if (IndexHandler::_readIndex(f,this))
	    return getNav();
	else 
		return NULL;
}

VTDNav* VTDGen::loadIndex(UByte* ba,int len){
	clear();
	delete[] XMLDoc;
	if (IndexHandler::_readIndex(ba,len,this))
	return getNav();
	else return NULL;
}

bool VTDGen::writeIndex(FILE *f){
	if (shallowDepth)
	return IndexHandler::_writeIndex_L3(1, 
                encoding, 
                ns, 
                true, 
                VTDDepth, 
                3, 
                rootIndex, 
                XMLDoc, 
                docOffset, 
                docLen, 
                VTDBuffer, 
                l1Buffer, 
                l2Buffer, 
                l3Buffer, 
                f);
	else
		return IndexHandler::_writeIndex_L5(1, 
                encoding, 
                ns, 
                true, 
                VTDDepth, 
                5, 
                rootIndex, 
                XMLDoc, 
                docOffset, 
                docLen, 
                VTDBuffer, 
                l1Buffer, 
                l2Buffer, 
                _l3Buffer,
				_l4Buffer,
				_l5Buffer,
                f);
} 
bool VTDGen::writeIndex(const char *fileName){
	FILE *f = NULL;
	bool b = false;
	f = fopen(fileName,"wb");
	
	if (f==NULL){
		throw InvalidArgumentException("fileName not valid");
		//return false;
	}
	b = writeIndex(f);
	fclose(f);
	return b;
}
Long VTDGen::getIndexSize(){
	int size;
	if ( (docLen & 7)==0)
		size = docLen;
	else
		size = ((docLen >>3)+1)<<3;

	size += (VTDBuffer->size<<3)+
		(l1Buffer->size<<3)+
		(l2Buffer->size<<3);

	if ((l3Buffer->size & 1) == 0){ //even
		size += l3Buffer->size<<2;
	} else {
		size += (l3Buffer->size+1)<<2; //odd
	}
	return size+64;
}
void VTDGen::writeSeparateIndex(char *vtdIndexFile){
	FILE *f = NULL;
	//Boolean b = FALSE;
	f = fopen(vtdIndexFile,"wb");
	
	if (f==NULL){
		throw InvalidArgumentException("fileName not valid");
		//return FALSE;
	}

	IndexHandler::_writeSeparateIndex_L3( (Byte)2, 
                encoding, 
                ns, 
                true, 
                VTDDepth, 
                3, 
                rootIndex, 
                //vg->XMLDoc, 
                docOffset, 
                docLen, 
                VTDBuffer, 
                l1Buffer, 
                l2Buffer, 
                l3Buffer, 
                f);
	
	fclose(f);
}
VTDNav* VTDGen::loadSeparateIndex(char *XMLFile, char *VTDIndexFile){
	FILE *vf = NULL, *xf=NULL;
	bool b = false;
	struct stat s;
	unsigned int xsize; 

	vf = fopen(VTDIndexFile,"rb");
	xf = fopen(XMLFile, "rb");
	stat(XMLFile,&s);
	xsize = (unsigned int) s.st_size;

	if (xf==NULL||vf==NULL){
		throw InvalidArgumentException("fileName not valid");
		//return NULL;
	}
	// clear internal state of vg
	clear();
	delete[] XMLDoc;

	//get xml file size
	b=IndexHandler::_readSeparateIndex(xf, xsize, vf, this);
	if (b==true){
		return getNav();
	}else {
		return NULL;
	}
}

//done!
int VTDGen::getPrevOffset(){
	int prevOffset = offset;
	switch (encoding) {
			case FORMAT_UTF8 :
				do {
					prevOffset--;
				} while (XMLDoc[prevOffset] >= 128 && 
					((XMLDoc[prevOffset] & 0xc0) == 0x80));
				return prevOffset;
			case FORMAT_ASCII :
			case FORMAT_ISO_8859_1 :
			case FORMAT_ISO_8859_2:
			case FORMAT_ISO_8859_3:
			case FORMAT_ISO_8859_4:
			case FORMAT_ISO_8859_5:
			case FORMAT_ISO_8859_6:
			case FORMAT_ISO_8859_7:
			case FORMAT_ISO_8859_8:
			case FORMAT_ISO_8859_9:
			case FORMAT_ISO_8859_10:
			case FORMAT_WIN_1250:
			case FORMAT_WIN_1251:
			case FORMAT_WIN_1252:
			case FORMAT_WIN_1253:
			case FORMAT_WIN_1254:
			case FORMAT_WIN_1255:
			case FORMAT_WIN_1256:
			case FORMAT_WIN_1257:
			case FORMAT_WIN_1258:

				return offset - 1;
			case FORMAT_UTF_16LE :
				if (XMLDoc[ offset - 2] < 0xDC00
					|| XMLDoc[ offset - 2] > 0xDFFFF) {
						return offset - 2;
				} else
					return offset - 4;
			case FORMAT_UTF_16BE :
				if (XMLDoc[ offset - 1] < 0xDC00
					|| XMLDoc[ offset - 1] > 0xDFFFF) {
						return offset - 2;
				} else
					return offset - 4;
			default :
				throw ParseException(
					"Parse exception in parse()\n"\
					"Other Error: Should never happen");	
				return 0;
	}
}


void VTDGen::qualifyAttributes(){
	int i1= nsBuffer3->size-1;
		int j= 0,i=0;
		// two cases:
		// 1. the current element has no prefix, look for xmlns
		// 2. the current element has prefix, look for xmlns:something
		while(j<prefixed_attr_count){		
			int preLen = (int)((prefixed_attr_name_array[j] & 0xffff0000LL)>>16);
			int preOs = (int) (prefixed_attr_name_array[j]>>32);
			//System.out.println(new String(XMLDoc, preOs, preLen)+"===");
			i = i1;
			while(i>=0){
				int t = nsBuffer3->upper32At(i);
				// with prefix, get full length and prefix length
				if ( (t&0xffff) - (t>>16) == preLen+increment){
					// doing byte comparison here
					int os =nsBuffer3->lower32At( i)+(t>>16)+increment;
					//System.out.println(new String(XMLDoc, os, preLen)+"");
					int k=0;
					for (;k<preLen;k++){
						//System.out.println(i+" "+(char)(XMLDoc[os+k])+"<===>"+(char)(XMLDoc[preOs+k]));
						if (XMLDoc[os+k]!=XMLDoc[preOs+k])
							break;
					}
					if (k==preLen){
						break; // found the match
					}
				}
				/*if ( (nsBuffer3.upper32At(i) & 0xffff0000) == 0){
					return;
				}*/
				i--;
			}
			if (i<0)
				throw ParseException("Name space qualification Exception: prefixed attribute not qualified\n");
						//+formatLineNumber(preOs));
			else
				prefix_URL_array[j] = i;
			j++;
			// no need to check if xml is the prefix
		}
}
void VTDGen::qualifyElement(){
	int i= nsBuffer3->size-1;
	// two cases:
	// 1. the current element has no prefix, look for xmlns
	// 2. the current element has prefix, look for xmlns:something

	int preLen = (int)((currentElementRecord & 0xffff000000000000LL)>>48);
	int preOs = (int)currentElementRecord;
	while(i>=0){
		int t = nsBuffer3->upper32At(i);
		// with prefix, get full length and prefix length
		if ( (t&0xffff) - (t>>16) == preLen){
			// doing byte comparison here
			int os = nsBuffer3->lower32At(i)+(t>>16)+increment;
			int k=0;
			for (;k<preLen-increment;k++){
				if (XMLDoc[os+k]!=XMLDoc[preOs+k])
					break;
			}
			if (k==preLen-increment)
				return; // found the match
		}
		/*if ( (nsBuffer3.upper32At(i) & 0xffff0000) == 0){
		return;
		}*/
		i--;
	}
	// no need to check if xml is the prefix
	if (checkPrefix(preOs, preLen))
		return;


	// print line # column# and full element name
	throw ParseException(
			"Name space qualification Exception: Element not qualified\n");
			//	+formatLineNumber((int)currentElementRecord));
}
bool VTDGen::matchXML(int byte_offset){
			// TODO Auto-generated method stub
		if (encoding<FORMAT_UTF_16BE){
			 if (XMLDoc[byte_offset]=='x'
					&& XMLDoc[byte_offset+1]=='m'
					&& XMLDoc[byte_offset+2]=='l')
					return true;		
		}else{
			 if (encoding==FORMAT_UTF_16LE){
				if (XMLDoc[byte_offset]=='x' && XMLDoc[byte_offset+1]==0
					&& XMLDoc[byte_offset+2]=='m' && XMLDoc[byte_offset+3]==0
					&& XMLDoc[byte_offset+4]=='l' && XMLDoc[byte_offset+5]==0)
					return true;
			} else {
				if (XMLDoc[byte_offset]== 0 && XMLDoc[byte_offset+1]=='x'
						&& XMLDoc[byte_offset+2]==0 && XMLDoc[byte_offset+3]=='m'
						&& XMLDoc[byte_offset+4]==0 && XMLDoc[byte_offset+5]=='l')
						return true;
			}
		}		
		return false;
}
bool VTDGen::matchURL(int bos1, int len1, int bos2, int len2){
		Long l1,l2;
		int i1=bos1, i2=bos2, i3=bos1+len1,i4=bos2+len2;
		//System.out.println("--->"+new String(XMLDoc, bos1, len1)+" "+new String(XMLDoc,bos2,len2));
		while(i1<i3 && i2<i4){
			l1 = _getCharResolved(i1);
			l2 = _getCharResolved(i2);
			if ((int)l1!=(int)l2)
				return false;
			i1 += (int)(l1>>32);
			i2 += (int)(l2>>32);
		}
		if (i1==i3 && i2==i4)
			return true;
		return false;
}
int VTDGen::identifyNsURL(int byte_offset, int length){
		UCSChar* URL1 = (UCSChar *)L"2000/xmlns/";
		UCSChar* URL2 = (UCSChar *)L"http://www.w3.org/XML/1998/namespace";
		Long l;
		int i,t,g=byte_offset+length;
		int os=byte_offset;
		if (length <29
				|| (increment==2 && length<58) )
			return 0;
		
		for (i=0; i<18 && os<g; i++){
			l = _getCharResolved(os);
			//System.out.println("char ==>"+(char)l);
			if (URL2[i]!= (int)l)
				return 0;
			os += (int)(l>>32);
		}
		
		//store offset value 
		t = os;
		
		for (i=0;i<11 && os<g;i++){
			l = _getCharResolved(os);
			if (URL1[i]!= (int)l)
				break;
			os += (int)(l>>32);
		}
		if (os == g)
			return 2;
		
		//so far a match
		os = t;
		for (i=18;i<36 && os<g;i++){
			l = _getCharResolved(os);
			if (URL2[i]!= (int)l)
				return 0;
			os += (int)(l>>32);
		}
		if (os==g)
			return 1;
			
		return 0;
}
inline int VTDGen::getCharUnit(int byte_offset){
	return (encoding <= 2)
		? XMLDoc[byte_offset] & 0xff
		: (encoding < FORMAT_UTF_16BE)
		? decode(byte_offset):(encoding == FORMAT_UTF_16BE)
		? (((int)XMLDoc[byte_offset])
		<< 8 | XMLDoc[byte_offset+1])
		: (((int)XMLDoc[byte_offset + 1])
		<< 8 | XMLDoc[byte_offset]);
}
inline void VTDGen::disallow_xmlns(int byte_offset){
			// TODO Auto-generated method stub
		if (encoding<FORMAT_UTF_16BE){
			 if (XMLDoc[byte_offset]=='x'
					&& XMLDoc[byte_offset+1]=='m'
					&& XMLDoc[byte_offset+2]=='l'
					&& XMLDoc[byte_offset+3]=='n'
					&& XMLDoc[byte_offset+4]=='s')
					throw ParseException(
							"xmlns as a ns prefix can't be re-declared");
					//formatLineNumber(byte_offset));
			
		}else{
			 if (encoding==FORMAT_UTF_16LE){
				if (XMLDoc[byte_offset]=='x' && XMLDoc[byte_offset+1]==0
					&& XMLDoc[byte_offset+2]=='m' && XMLDoc[byte_offset+3]==0
					&& XMLDoc[byte_offset+4]=='l' && XMLDoc[byte_offset+5]==0
					&& XMLDoc[byte_offset+6]=='n' && XMLDoc[byte_offset+7]==0
					&& XMLDoc[byte_offset+8]=='s' && XMLDoc[byte_offset+9]==0)
					throw ParseException(
							"xmlns as a ns prefix can't be re-declared");
			} else {
				if (XMLDoc[byte_offset]== 0 && XMLDoc[byte_offset+1]=='x'
						&& XMLDoc[byte_offset+2]==0 && XMLDoc[byte_offset+3]=='m'
						&& XMLDoc[byte_offset+4]==0 && XMLDoc[byte_offset+5]=='l'
						&& XMLDoc[byte_offset+6]==0 && XMLDoc[byte_offset+7]=='n'
						&& XMLDoc[byte_offset+8]==0 && XMLDoc[byte_offset+9]=='s')
						throw ParseException("xmlns as a ns prefix can't be re-declared");
								//+formatLineNumber(offset));
			}
		}		
}
void VTDGen::checkQualifiedAttributeUniqueness(){
			// TODO Auto-generated method stub
		int  preLen1,os1,postLen1,URLLen1,URLOs1, 
			 preLen2, os2,postLen2, URLLen2, URLOs2,k,i,j;
		for (i=0;i<prefixed_attr_count;i++){
			preLen1 = (int)((prefixed_attr_name_array[i] & 0xffff0000LL)>>16);
			postLen1 = (int) ((prefixed_attr_name_array[i] & 0xffffLL))-preLen1-increment;
			os1 = (int) (prefixed_attr_name_array[i]>>32) + preLen1+increment;
			URLLen1 = nsBuffer2->lower32At(prefix_URL_array[i]);
			URLOs1 =  nsBuffer2->upper32At(prefix_URL_array[i]);
			for (j=i+1;j<prefixed_attr_count;j++){
				// prefix of i matches that of j
				preLen2 = (int)((prefixed_attr_name_array[j] & 0xffff0000LL)>>16);
				postLen2 = (int) ((prefixed_attr_name_array[j] & 0xffffL))-preLen2-increment;
				os2 = (int)(prefixed_attr_name_array[j]>>32) + preLen2 + increment;
				//System.out.println(new String(XMLDoc,os1, postLen1)
				//	+" "+ new String(XMLDoc, os2, postLen2));
				if (postLen1 == postLen2){
					k=0;
					/*for (;k<postLen1;k++){
					//System.out.println(i+" "+(char)(XMLDoc[os+k])+"<===>"+(char)(XMLDoc[preOs+k]));
						if (XMLDoc[os1+k]!=XMLDoc[os2+k])
							break;
					}*/
					if (memcmp(XMLDoc+os1, XMLDoc+os2,postLen1)==0){
							// found the match
						URLLen2 = nsBuffer2->lower32At(prefix_URL_array[j]);
						URLOs2 =  nsBuffer2->upper32At(prefix_URL_array[j]);
						//System.out.println(" URLOs1 ===>" + URLOs1);
						//System.out.println("nsBuffer2 ===>"+nsBuffer2.longAt(i)+" i==>"+i);
						//System.out.println("URLLen2 "+ URLLen2+" URLLen1 "+ URLLen1+" ");
						if (matchURL(URLOs1, URLLen1, URLOs2, URLLen2))
							throw ParseException(" qualified attribute names collide ");
							//	+ formatLineNumber(os2));
				    }
				}				
			}
			//System.out.println("======");
		}
}
bool VTDGen::checkPrefix(int os, int len){
			//int i=0;
		if (encoding < FORMAT_UTF_16BE){
			if (len==4	&&	XMLDoc[os]=='x'
				&& XMLDoc[os+1]=='m' && XMLDoc[os+2]=='l'){
				return true;
			}
		}else if (encoding == FORMAT_UTF_16BE){
			if (len==8	&&	XMLDoc[os]==0 && XMLDoc[os+1]=='x'
				&& XMLDoc[os+2]==0 && XMLDoc[os+3]=='m' 
				&& XMLDoc[os+4]==0 && XMLDoc[os+5]=='l'){
				return true;
			}
		}else {
			if (len==8	&&	XMLDoc[os]=='x' && XMLDoc[os+1]==0
				&& XMLDoc[os+2]=='m' && XMLDoc[os+3]==0 
				&& XMLDoc[os+4]=='l' && XMLDoc[os+5]==0){
				return true;
			}
		}
		return false;
}
bool VTDGen::checkPrefix2(int os, int len){
			//int i=0;
		if (encoding < FORMAT_UTF_16BE){
			if ( len==5 && XMLDoc[os]=='x'
				&& XMLDoc[os+1]=='m' && XMLDoc[os+2]=='l'
				&& XMLDoc[os+3]=='n' && XMLDoc[os+4]=='s'){
				return true;
			}
		}else if (encoding == FORMAT_UTF_16BE){
			if ( len==10 && XMLDoc[os]==0 && XMLDoc[os+1]=='x'
				&& XMLDoc[os+2]==0 && XMLDoc[os+3]=='m' 
				&& XMLDoc[os+4]==0 && XMLDoc[os+5]=='l'
				&& XMLDoc[os+6]==0 && XMLDoc[os+7]=='n' 
				&& XMLDoc[os+8]==0 && XMLDoc[os+9]=='s'		
			){
				return true;
			}
		}else {
			if ( len==10 && XMLDoc[os]=='x' && XMLDoc[os+1]==0
				&& XMLDoc[os+2]=='m' && XMLDoc[os+3]==0 
				&& XMLDoc[os+4]=='l' && XMLDoc[os+5]==0
				&& XMLDoc[os+6]=='n' && XMLDoc[os+3]==0 
				&& XMLDoc[os+8]=='s' && XMLDoc[os+5]==0				
			){
				return true;
			}
		}
		return false;
}
void VTDGen::checkAttributeUniqueness(){
	bool unique = true;
	bool unequal;
	//int prevLen;
	int i;
	for (i = 0; i < attr_count; i++) {
		int prevLen = (int) attr_name_array[i];
		unequal = false;	
		if (length1 == prevLen) {
			int prevOffset =
				(int) (attr_name_array[i] >> 32);
			if (memcmp(XMLDoc+prevOffset,XMLDoc+temp_offset,prevLen)!=0)
				unequal = true;
			/*for (j = 0; j < prevLen; j++) {
				if (XMLDoc[prevOffset + j]
				!= XMLDoc[temp_offset + j]) {
					unequal = true;
					break;
				}
			}*/
		} else
			unequal = true;
		unique = unique && unequal;
	}
	if (!unique && attr_count != 0){		
		throw ParseException("Parse exception in parse(). Error in attr: Attr name not unique");
	}
	unique = true;
	if (attr_count < anaLen) {
		attr_name_array[attr_count] =
			((Long) (temp_offset) << 32) + length1;
		attr_count++;
	} else 
	{
		Long* temp_array = attr_name_array;
		attr_name_array = 
			(Long *)malloc(sizeof(Long)*
			(attr_count + ATTR_NAME_ARRAY_SIZE));

		if (attr_name_array == NULL){
			throw ParseException("Parse exception in parse(). alloc mem for attr_name_array_failed");
		}
		anaLen = attr_count + ATTR_NAME_ARRAY_SIZE;

		for (i = 0; i < attr_count; i++) {
			attr_name_array[i] = temp_array[i];
		}
		attr_name_array[attr_count] =
			((Long) (temp_offset) << 32) + length1;
		attr_count++;
	}
		// insert prefix attr node into the prefixed_attr_name array
		// xml:something will not be inserted
		//System.out.println(" prefixed attr count ===>"+prefixed_attr_count);
		//System.out.println(" length2 ===>"+length2);
		if (ns && !is_ns && length2!=0 ){
			if ((increment==1 && length2 ==3 && matchXML(temp_offset))
					|| (increment==2 &&length2 ==6 &&  matchXML(temp_offset))){
				return;
			}
			else if (prefixed_attr_count < panaLen){
				prefixed_attr_name_array[prefixed_attr_count] =
					((Long) (temp_offset) << 32) | (length2<<16)| length1;
				prefixed_attr_count++;
			}else {
				Long* temp_array1 = prefixed_attr_name_array;
				prefixed_attr_name_array =(Long *)malloc(sizeof(Long)*(prefixed_attr_count + ATTR_NAME_ARRAY_SIZE));
				panaLen = prefixed_attr_count+ATTR_NAME_ARRAY_SIZE;
					//new long[prefixed_attr_count + ATTR_NAME_ARRAY_SIZE];
				prefix_URL_array = (int *)malloc(sizeof(int)*(prefixed_attr_count + ATTR_NAME_ARRAY_SIZE));
					//new int[prefixed_attr_count + ATTR_NAME_ARRAY_SIZE];
				memcpy(prefixed_attr_name_array,temp_array1,prefixed_attr_count);
				//System.arraycopy(temp_array1, 0, prefixed_attr_name_array, 0, prefixed_attr_count);
				//System.arraycopy(temp_array1, 0, prefixed_attr_val_array, 0, prefixed_attr_count)
				/*for (int i = 0; i < attr_count; i++) {
					attr_name_array[i] = temp_array[i];
				}*/
				prefixed_attr_name_array[prefixed_attr_count] =
					((Long) (temp_offset) << 32) | (length2<<16)| length1;
				prefixed_attr_count++;
			}
		}
}
inline Long VTDGen::_getCharResolved(int byte_offset){

	int ch = 0;
	int val = 0;
	Long inc = 2<<(increment-1);
	Long l = _getChar(byte_offset);

	ch = (int)l;

	if (ch != '&')
		return l;

	// let us handle references here
	//currentOffset++;
	byte_offset+=increment;
	ch = getCharUnit(byte_offset);
	byte_offset+=increment;
	switch (ch) {
				case '#' :

					ch = getCharUnit(byte_offset);

					if (ch == 'x') {
						while (true) {
							byte_offset+=increment;
							inc+=increment;
							ch = getCharUnit(byte_offset);

							if (ch >= '0' && ch <= '9') {
								val = (val << 4) + (ch - '0');
							} else if (ch >= 'a' && ch <= 'f') {
								val = (val << 4) + (ch - 'a' + 10);
							} else if (ch >= 'A' && ch <= 'F') {
								val = (val << 4) + (ch - 'A' + 10);
							} else if (ch == ';') {
								inc+=increment;
								break;
							} 
						}
					} else {
						while (true) {
							ch = getCharUnit(byte_offset);
							byte_offset+=increment;
							inc+=increment;
							if (ch >= '0' && ch <= '9') {
								val = val * 10 + (ch - '0');
							} else if (ch == ';') {
								break;
							} 						
						}
					}
					break;

				case 'a' :
					ch = getCharUnit(byte_offset);
					if (encoding<FORMAT_UTF_16BE){
						if (ch == 'm') {
							if (getCharUnit(byte_offset + 1) == 'p'
								&& getCharUnit(byte_offset + 2) == ';') {
									inc = 5;
									val = '&';
							} 
						} else if (ch == 'p') {
							if (getCharUnit(byte_offset + 1) == 'o'
								&& getCharUnit(byte_offset + 2) == 's'
								&& getCharUnit(byte_offset + 3) == ';') {
									inc = 6;
									val = '\'';
							} 
						} 
					}else{
						if (ch == 'm') {
							if (getCharUnit(byte_offset + 2) == 'p'
								&& getCharUnit(byte_offset + 4) == ';') {
									inc = 10;
									val = '&';
							} 
						} else if (ch == 'p') {
							if (getCharUnit(byte_offset + 2) == 'o'
								&& getCharUnit(byte_offset + 4) == 's'
								&& getCharUnit(byte_offset + 6) == ';') {
									inc = 12;
									val = '\'';
							} 
						} 
					}
					break;

				case 'q' :

					if (encoding<FORMAT_UTF_16BE){
						if (getCharUnit(byte_offset) == 'u'
							&& getCharUnit(byte_offset + 1) == 'o'
							&& getCharUnit(byte_offset + 2) == 't'
							&& getCharUnit(byte_offset + 3) ==';') {
								inc = 6;
								val = '\"';
						} }
					else{
						if (getCharUnit(byte_offset) == 'u'
							&& getCharUnit(byte_offset + 2) == 'o'
							&& getCharUnit(byte_offset + 4) == 't'
							&& getCharUnit(byte_offset + 6) ==';') {
								inc = 12;
								val = '\"';
						}
					}
					break;
				case 'l' :
					if (encoding<FORMAT_UTF_16BE){
						if (getCharUnit(byte_offset) == 't'
							&& getCharUnit(byte_offset + 1) == ';') {
								//offset += 2;
								inc = 4;
								val = '<';
						} 
					}else{
						if (getCharUnit(byte_offset) == 't'
							&& getCharUnit(byte_offset + 2) == ';') {
								//offset += 2;
								inc = 8;
								val = '<';
						} 
					}
					break;
				case 'g' :
					if (encoding<FORMAT_UTF_16BE){
						if (getCharUnit(byte_offset) == 't'
							&& getCharUnit(byte_offset + 1) == ';') {
								inc = 4;
								val = '>';
						} 
					}else {
						if (getCharUnit(byte_offset) == 't'
							&& getCharUnit(byte_offset + 2) == ';') {
								inc = 8;
								val = '>';
						} 
					}
					break;
	}

	//currentOffset++;
	return val | (inc << 32);
}
inline Long VTDGen::_getChar(int byte_offset){
	int c;
	switch (encoding) {
			case FORMAT_ASCII :
				c =  XMLDoc[offset];
				if (c=='\r' && XMLDoc[offset+1]=='\n')
					return (2LL<<32)|'\n';
				return (1LL<<32)|c;
			case FORMAT_ISO_8859_1 :
				c = XMLDoc[offset];
				if (c=='\r' && XMLDoc[offset+1]=='\n')
					return (2LL<<32)|'\n';
				return (1LL<<32)|c;
			case FORMAT_UTF8 :
				c = XMLDoc[offset];
			   if (c>=0){
				if (c == '\r') {
					if (XMLDoc[offset + 1] == '\n') {
						return '\n'|(2LL<<32);
					} else {
						return '\n'|(1LL<<32);
					}
				}
				//currentOffset++;
				return c|(1LL<<32);
			}				
			return _handle_utf8(c,offset);

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				return _handle_16be( offset);

			case FORMAT_UTF_16LE :
				return _handle_16le( offset);

			default :
				return _handleOtherEncoding(offset);

	}
}
int VTDGen::decode(int byte_offset){
	    int b = XMLDoc[byte_offset];
	switch(encoding){
		case FORMAT_ISO_8859_2: return iso_8859_2_decode(b);
		case FORMAT_ISO_8859_3: return iso_8859_3_decode(b);
		case FORMAT_ISO_8859_4: return iso_8859_4_decode(b);
		case FORMAT_ISO_8859_5: return iso_8859_5_decode(b);
		case FORMAT_ISO_8859_6: return iso_8859_6_decode(b);
		case FORMAT_ISO_8859_7: return iso_8859_7_decode(b);
		case FORMAT_ISO_8859_8: return iso_8859_8_decode(b);
		case FORMAT_ISO_8859_9: return iso_8859_9_decode(b);
		case FORMAT_ISO_8859_10: return iso_8859_10_decode(b);
		case FORMAT_ISO_8859_11: return iso_8859_11_decode(b);
		case FORMAT_ISO_8859_13: return iso_8859_13_decode(b);
		case FORMAT_ISO_8859_14: return iso_8859_14_decode(b);
		case FORMAT_ISO_8859_15: return iso_8859_15_decode(b);
		
		case FORMAT_WIN_1250: return windows_1250_decode( b);
		case FORMAT_WIN_1251: return windows_1251_decode( b);
		case FORMAT_WIN_1252: return windows_1252_decode( b);
		case FORMAT_WIN_1253: return windows_1253_decode( b);
		case FORMAT_WIN_1254: return windows_1254_decode( b);
		case FORMAT_WIN_1255: return windows_1255_decode( b);
		case FORMAT_WIN_1256: return windows_1256_decode( b);
		case FORMAT_WIN_1257: return windows_1257_decode( b);
		case FORMAT_WIN_1258: return windows_1258_decode( b);   
	}
	return 0;
}
Long VTDGen::_handle_16be(int byte_offset){
	Long val; 

	int temp =
		((XMLDoc[offset ] & 0xff)	<< 8) 
		|(XMLDoc[offset + 1]& 0xff);
	if ((temp < 0xd800)
		|| (temp > 0xdfff)) { // not a high surrogate
			if (temp == '\r') {
				if (XMLDoc[offset  + 3] == '\n'
					&& XMLDoc[offset + 2] == 0) {

						return '\n'|(4LL<<32);
				} else {
					return '\n'|(2LL<<32);
				}
			}
			//currentOffset++;
			return temp| (2LL<<32);
	} else {
		val = temp;
		temp =
			((XMLDoc[offset + 2] & 0xff)
			<< 8) | (XMLDoc[offset+ 3] & 0xff);
		val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;
		//currentOffset += 2;
		return val | (4LL<<32);
	}
}
Long VTDGen::_handle_16le(int byte_offset){
	// implement UTF-16LE to UCS4 conversion
	int val, temp =
		(XMLDoc[offset + 1 ] & 0xff)
		<< 8 | (XMLDoc[offset] & 0xff);
	if (temp < 0xdc00 || temp > 0xdfff) { // check for low surrogate
		if (temp == '\r') {
			if (XMLDoc[offset + 2] == '\n'
				&& XMLDoc[offset + 3] == 0) {
					return '\n' | (4LL<<32) ;
			} else {
				return '\n' | (2LL<<32);
			}
		}
		return temp | (2LL<<32);
	} else {
		val = temp;
		temp =
			(XMLDoc[offset + 3]&0xff)
			<< 8 | (XMLDoc[offset + 2] & 0xff);
		val = ((temp - 0xd800)<<10) + (val - 0xdc00) + 0x10000;

		return val | (4LL<<32);
	}
}
Long VTDGen::_handle_utf8(int temp,int offset){
	int c=0, d=0, a=0,i; 
	Long val;
	switch (UTF8Char_byteCount((int)temp & 0xff)) {
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
	}

	val = (temp & c) << d;
	i = a - 1;
	while (i >= 0) {
		temp = XMLDoc[offset + a - i];
		val = val | ((temp & 0x3f) << ((i << 2) + (i << 1)));
		i--;
	}
	//currentOffset += a + 1;
	return val | (((Long)(a+1))<<32);
}
Long VTDGen::_handleOtherEncoding(int byte_offset){
	    int b = XMLDoc[byte_offset];
		if (b=='\r' && XMLDoc[byte_offset+1]=='\n')
				return (2LL<<32)|'\n';
	switch(encoding){
		case FORMAT_ISO_8859_2: return iso_8859_2_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_3: return iso_8859_3_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_4: return iso_8859_4_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_5: return iso_8859_5_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_6: return iso_8859_6_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_7: return iso_8859_7_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_8: return iso_8859_8_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_9: return iso_8859_9_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_10: return iso_8859_10_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_11: return iso_8859_11_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_13: return iso_8859_13_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_14: return iso_8859_14_decode(b)|(1LL<<32);
		case FORMAT_ISO_8859_15: return iso_8859_15_decode(b)|(1LL<<32);
		
		case FORMAT_WIN_1250: return windows_1250_decode( b)|(1LL<<32);
		case FORMAT_WIN_1251: return windows_1251_decode( b)|(1LL<<32);
		case FORMAT_WIN_1252: return windows_1252_decode( b)|(1LL<<32);
		case FORMAT_WIN_1253: return windows_1253_decode( b)|(1LL<<32);
		case FORMAT_WIN_1254: return windows_1254_decode( b)|(1LL<<32);
		case FORMAT_WIN_1255: return windows_1255_decode( b)|(1LL<<32);
		case FORMAT_WIN_1256: return windows_1256_decode( b)|(1LL<<32);
		case FORMAT_WIN_1257: return windows_1257_decode( b)|(1LL<<32);
		case FORMAT_WIN_1258: return windows_1258_decode( b)|(1LL<<32);   
	}
	return 0LL;
}
void VTDGen::handleOtherTextChar2(int ch){
	if (ch == '&') {
		//has_amp = true;	
		if (!XMLChar_isValidChar(entityIdentifier()))
			throw ParseException(
			"Error in text content: Invalid char in text content ");
		//+ formatLineNumber());
		//parser_state = STATE_TEXT;
	}  else if (ch == ']') {
		if (skipChar(']')) {
			while (skipChar(']')) {
			}
			if (skipChar('>'))
				throw ParseException(
				"Error in text content: ]]> in text content");
			//+ formatLineNumber());
		}	
	} else
		throw ParseException(
		"Error in text content: Invalid char in text content ");
	//+ formatLineNumber());	
}
void VTDGen::handleOtherTextChar(int ch){
	if (ch == '&') {
		//has_amp = true;
		//temp_offset = offset;
		entityIdentifier();
		//parser_state = STATE_TEXT;
	} else if (ch == ']') {
		if (skipChar(']')) {
			while (skipChar(']')) {
			}
			if (skipChar('>'))
				throw ParseException(
				"Error in text content: ]]> in text content");
			//+ formatLineNumber());
		}
		//parser_state = STATE_TEXT;
	}else
		throw ParseException(
		"Error in text content: Invalid char");
		//+ formatLineNumber());
}


inline void VTDGen::writeVTDText(int offset, int length, tokenType token_type, int depth){
	if (length > MAX_TOKEN_LENGTH) {
		int k;
		int r_offset = offset;
		Long l = ((Long)((token_type << 28)
			| ((depth & 0xff) << 20) 
			| MAX_TOKEN_LENGTH) << 32);

		for (k = length; k > MAX_TOKEN_LENGTH; k = k - MAX_TOKEN_LENGTH) {
			VTDBuffer->append( l | r_offset);
			r_offset += MAX_TOKEN_LENGTH;
		}
		VTDBuffer->append(((Long) ((token_type << 28)
			| ((depth & 0xff) << 20) | k) << 32)
			| r_offset);
	} else {
		VTDBuffer->append(((Long) ((token_type << 28)
			| ((depth & 0xff) << 20) | length) << 32)
			| offset);
	}
}

void VTDGen::writeVTD_L5(int offset, int length, tokenType token_type, int depth){
	VTDBuffer->append(((Long) ((token_type << 28)
		| ((depth & 0xff) << 20) | length) << 32)
		| offset);
	switch (depth) {
			case 0:
				rootIndex = VTDBuffer->size - 1;
				break;
			case 1:
				if (last_depth == 1) {
					l1Buffer->append(((Long) last_l1_index << 32) | 0xffffffffLL);
				} else if (last_depth == 2) {
					l2Buffer->append(((Long) last_l2_index << 32) | 0xffffffffLL);
				} else if (last_depth ==3) {
					_l3Buffer->append(((Long) last_l3_index << 32) | 0xffffffffLL);
				} else if (last_depth ==4){
					_l4Buffer->append(((Long) last_l4_index << 32) | 0xffffffffLL);
				}
				last_l1_index = VTDBuffer->size - 1;
				last_depth = 1;
				break;
			case 2:
				if (last_depth == 1) {
					l1Buffer->append(((Long) last_l1_index << 32)
						+ l2Buffer->size);
				} else if (last_depth == 2) {
					l2Buffer->append(((Long) last_l2_index << 32) | 0xffffffffLL);
				} else if (last_depth ==3) {
					_l3Buffer->append(((Long) last_l3_index << 32) | 0xffffffffLL);
				} else if (last_depth ==4){
					_l4Buffer->append(((Long) last_l4_index << 32) | 0xffffffffLL);
				}
				last_l2_index = VTDBuffer->size - 1;
				last_depth = 2;
				break;

			case 3:
				/*if (last_depth == 1) {
				l1Buffer.append(((long) last_l1_index << 32)
				+ l2Buffer.size);
				} else*/ 
				if (last_depth == 2) {
					l2Buffer->append(((Long) last_l2_index << 32) 
						+ _l3Buffer->size);
				} else if (last_depth ==3) {
					_l3Buffer->append(((Long) last_l3_index << 32) | 0xffffffffLL);
				} else if (last_depth ==4){
					_l4Buffer->append(((Long) last_l4_index << 32) | 0xffffffffLL);
				}
				last_l3_index = VTDBuffer->size - 1;
				last_depth = 3;
				break;

			case 4:
				/*if (last_depth == 1) {
				l1Buffer.append(((long) last_l1_index << 32)
				+ l2Buffer.size);
				} else if (last_depth == 2) {
				l2Buffer.append(((long) last_l2_index << 32) | 0xffffffffL);
				} else*/ 
				if (last_depth ==3) {
					_l3Buffer->append(((Long) last_l3_index << 32) 
						+ _l4Buffer->size);
				} else if (last_depth ==4){
					_l4Buffer->append(((Long) last_l4_index << 32) | 0xffffffffLL);
				}
				last_l4_index = VTDBuffer->size - 1;
				last_depth = 4;
				break;
			case 5:
				_l5Buffer->append(VTDBuffer->size - 1);
				if (last_depth == 4) {
					_l4Buffer->append(((Long) last_l4_index << 32)
						+ _l5Buffer->size - 1);
				}
				last_depth = 5;
				break;
	}
}

inline void VTDGen::_writeVTD(int offset, int length, tokenType token_type, int depth){
	VTDBuffer->append(((Long) ((token_type << 28)
		| ((depth & 0xff) << 20) | length) << 32)
		| offset);		
}

/*void VTDGen::selectLcDepth(int i){
	if (i!=3 &&i!=5)
		throw ParseException("LcDepth can only take the value of 3 or 5");
	if (i==5)
		shallowDepth = false;
}*/

VTDNav* VTDGen::loadIndex(const char* fileName){
	FILE *f = NULL;
	//exception e;
	//int status,len;
	UByte *ba=NULL;
	//struct stat buffer;
	f = fopen(fileName,"rb");
	if (f==NULL){
		//throwException2(invalid_argument,"fileName not valid");
		return false;
	}
	VTDNav *vn=loadIndex(f);
	fclose(f);
	return vn;
	
}


inline int VTDGen::getChar(){
	int temp = 0;
	if (offset >= endOffset){
		throw EOFException(
			"Parse exception in getChar \n"\
			"Premature EOF reached, XML document incomplete");			
	}
	switch (encoding) {
	case FORMAT_ASCII :
		temp = XMLDoc[offset];
		if (temp<128){
			offset++;
			return temp;
		}else
			throw ParseException("Parse exception in getChar \n"\
								 "Invalid Char for ASCII encoding");
	case FORMAT_ISO_8859_1 :
		temp = XMLDoc[offset];
		offset++;
		return temp;
	case FORMAT_UTF8 :

		temp = XMLDoc[offset];
		if (temp <128) {
			offset++;
			return temp;
		}
		//temp = temp & 0xff;
		return handle_utf8(temp);


	case FORMAT_UTF_16BE :
		// implement UTF-16BE to UCS4 conversion
		return handle_16be();

	case FORMAT_UTF_16LE :
		return handle_16le();


	default :
		return handleOtherEncoding();
	}


	//done

}
inline int VTDGen::getCharAfterS(){
	int n, k;
	n = k = 0;
	do{
		n = getChar();
		if (n == ' ' || n == '\n' || n == '\t' || n == '\r') {
		} else
			return n;
		n = getChar();
		if (n == ' ' || n == '\n' || n == '\t' || n == '\r') {
		} else
			return n;
	}while (true);
}
//done
inline bool VTDGen::skipChar(int ch){
	int temp = 0;
	if (offset >= endOffset){
		throw ParseException(
			"Parse exception in parse() \n"\
			"Premature EOF reached, XML document incomplete");
	}	
	switch (encoding) {
	case FORMAT_ASCII :
		temp = XMLDoc[offset];
		if (temp>127){
			throw ParseException(
				"Parse exception in parse() \n"\
				"Invalid char for ASCII encoding");
		}
		if (ch == temp) {
			offset++;
			return true;
		} else {
			return false;
		}
	case FORMAT_ISO_8859_1 :
		temp = XMLDoc[offset];
		if (temp == ch) {
			offset++;
			return true;
		} else {
			return false;
		}
	case FORMAT_UTF8 :
		temp = XMLDoc[offset];
		if (temp <128) {
			if (ch == temp) {
				offset++;
				return true;
			} else {
				return false;
			}
		}
		return skipUTF8(temp,ch);

	case FORMAT_UTF_16BE :
		return skip_16be(ch);

	case FORMAT_UTF_16LE :
		return skip_16le(ch);

	default :
		return skip4OtherEncoding(ch);
	}
}
