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
/*This file defines XML char validation lookup table and various functions */
#pragma once
#ifndef XMLCHAR_H
#define XMLCHAR_H
//#include "customTypes.h"
namespace com_ximpleware{
#define LENGTH(x,y) (sizeof(x)/sizeof(y))


 extern int MASK_VALID;/*= 0x01;*/
 extern int MASK_SPACE; /*= 0x01<<1;*/
 extern int MASK_NAME_START; /*= 0x01<<2;*/
 extern int MASK_NAME; /*= 0x01<<3;*/
 extern int MASK_PUBID; /*= 0x01<<4;*/
 extern int MASK_CONTENT; /*= 0x01<<5;*/
 extern int MASK_NCNAME_START; /*= 0x01 << 6;*/
 extern int MASK_NCNAME;  /* = 0x01 << 7;*/

extern unsigned char CHARS[1<<16];// = unsigned Byte[1<<16];

extern char Character[1<<8]; // = 

extern char isReady;// = FALSE;

extern char isCharacterReady;

extern void XMLChar_init();

void init_Character();
   /**
     * Returns true if the specified character is a supplemental character.
     *
     * @param c The character to check.
     */
    extern char XMLChar_isSupplemental(int c);

    /**
     * Returns true the supplemental character corresponding to the given
     * surrogates.
     *
     * @param h The high surrogate.
     * @param l The low surrogate.
     */
    extern int XMLChar_isSupplementalChar(char h, char l) ;

    /**
     * Returns the high surrogate of a supplemental character
     *
     * @param c The supplemental character to "split".
     */
    extern  unsigned short XMLChar_highSurrogate(int c);

    /**
     * Returns the low surrogate of a supplemental character
     *
     * @param c The supplemental character to "split".
     */
    extern  unsigned short XMLChar_lowSurrogate(int c);

    /**
     * Returns whether the given character is a high surrogate
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isHighSurrogate(int c);
    /**
     * Returns whether the given character is a low surrogate
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isLowSurrogate(int c);


    /**
     * Returns true if the specified character is valid. This method
     * also checks the surrogate character range from 0x10000 to 0x10FFFF.
     * <p>
     * If the program chooses to apply the mask directly to the
     * <code>CHARS</code> array, then they are responsible for checking
     * the surrogate character range.
     *
     * @param c The character to check.
     */
    extern char XMLChar_isValidChar(int c);

    /**
     * Returns true if the specified character is invalid.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isInvalidChar(int c);

    /**
     * Returns true if the specified character can be considered content.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isContentChar(int c);

    /**
     * Returns true if the specified character can be considered markup.
     * Markup characters include '&lt;', '&amp;', and '%'.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isMarkupChar(int c) ;

    /**
     * Returns true if the specified character is a space character
     * as defined by production [3] in the XML 1.0 specification.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isSpaceChar(int c);

    /**
     * Returns true if the specified character is a valid name start
     * character as defined by production [5] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isNameStartChar(int c) ;

    /**
     * Returns true if the specified character is a valid name
     * character as defined by production [4] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isNameChar(int c) ;

    /**
     * Returns true if the specified character is a valid NCName start
     * character as defined by production [4] in Namespaces in XML
     * recommendation.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isNCNameStart(int c);

    /**
     * Returns true if the specified character is a valid NCName
     * character as defined by production [5] in Namespaces in XML
     * recommendation.
     *
     * @param c The character to check.
     */
    extern  char XMLChar_isNCName(int c) ;

    /**
     * Returns true if the specified character is a valid Pubid
     * character as defined by production [13] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    extern char XMLChar_isPubid(int c);

	extern int Character_digit(int ch, int radix);

    /*
     * [5] Name ::= (Letter | '_' | ':') (NameChar)*
     */
    /**
     * Check to see if a string is a valid Name according to [5]
     * in the XML 1.0 Recommendation
     *
     * @param name string to check
     * @return true if name is a valid Name
     */
/*    inline  static bool isValidName(const string& name)
	{
        if(name.empty() || (!isNameStart(name[0]))) return false;

        for (int i = 1; i < name.length(); i++ )
           if(!isName(name[i])) return false;

		return true;
    } // isValidName(String):char
*/

    /*
     * from the namespace rec
     * [4] NCName ::= (Letter | '_') (NCNameChar)*
     */
    /**
     * Check to see if a string is a valid NCName according to [4]
     * from the XML Namespaces 1.0 Recommendation
     *
     * @param name string to check
     * @return true if name is a valid NCName
     */
/*    inline  static bool isValidNCName(const string& ncName)
	{
        if (ncName.empty() ||(!isNCNameStart(ncName[0]))) return false;

        for (int i = 1; i < ncName.length(); i++ )
           if(!isNCName(ncName[i])) return false;

		return true;
    } // isValidNCName(String):char
*/
    /*
     * [7] Nmtoken ::= (NameChar)+
     */
    /**
     * Check to see if a string is a valid Nmtoken according to [7]
     * in the XML 1.0 Recommendation
     *
     * @param nmtoken string to check
     * @return true if nmtoken is a valid Nmtoken
     */
/*  inline  static bool isValidNmtoken(const string& nmtoken) {
        if(nmtoken.empty()) return false;

        for (int i = 0; i < nmtoken.length(); i++ )
           if(!isName(nmtoken[i])) return false;

		return true;
    } // isValidName(String):char
*/
}

inline int com_ximpleware::Character_digit(int ch, int radix){
	int i;

	if (com_ximpleware::isCharacterReady == 0)
		init_Character();

	i = com_ximpleware::Character[ch & 0xff];
	if (i > radix - 1){
		return -1;
	}
	return com_ximpleware::Character[ch&0xff];
}


/**
* Returns 1 if the specified character is a supplemental character.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isSupplemental(int c) {
	return (c >= 0x10000 && c <= 0x10FFFF);
}

/**
* Returns 1 the supplemental character corresponding to the given
* surrogates.
*
* @param h The high surrogate.
* @param l The low surrogate.
*/
inline int com_ximpleware::XMLChar_isSupplementalChar(char h, char l) {
	return (h - 0xD800) * 0x400 + (l - 0xDC00) + 0x10000;
}

/**
* Returns the high surrogate of a supplemental character
*
* @param c The supplemental character to "split".
*/
inline unsigned short com_ximpleware::XMLChar_highSurrogate(int c) {
	return (unsigned short) (((c - 0x00010000) >> 10) + 0xD800);
}

/**
* Returns the low surrogate of a supplemental character
*
* @param c The supplemental character to "split".
*/
inline unsigned short com_ximpleware::XMLChar_lowSurrogate(int c) {
	return (unsigned short) (((c - 0x00010000) & 0x3FF) + 0xDC00);
}

/**
* Returns whether the given character is a high surrogate
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isHighSurrogate(int c) {
	return (0xD800 <= c && c <= 0xDBFF);
}

/**
* Returns whether the given character is a low surrogate
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isLowSurrogate(int c) {
	return (0xDC00 <= c && c <= 0xDFFF);
}


/**
* Returns 1 if the specified character is valid. This method
* also checks the surrogate character range from 0x10000 to 0x10FFFF.
* <p>
* If the program chooses to apply the mask directly to the
* <code>CHARS</code> array, then they are responsible for checking
* the surrogate character range.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isValidChar(int c) {
	return (c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_VALID) != 0) ||
		(0x10000 <= c && c <= 0x10FFFF);
} // isValid(int):char

/**
* Returns 1 if the specified character is invalid.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isInvalidChar(int c) {
	return !XMLChar_isValidChar(c);
} // isInvalid(int):char

/**
* Returns 1 if the specified character can be considered content.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isContentChar(int c) {
	return (c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_CONTENT) != 0) ||
		(0x10000 <= c && c <= 0x10FFFF);
} // isContent(int):char

/**
* Returns 1 if the specified character can be considered markup.
* Markup characters include '&lt;', '&amp;', and '%'.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isMarkupChar(int c) {
	return c == '<' || c == '&' || c == '%';
} // isMarkup(int):char

/**
* Returns 1 if the specified character is a space character
* as defined by production [3] in the XML 1.0 specification.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isSpaceChar(int c) {
	return c <= 0x20 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_SPACE) != 0;
} // isSpace(int):char

/**
* Returns 1 if the specified character is a valid name start
* character as defined by production [5] in the XML 1.0
* specification.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isNameStartChar(int c) {
	return c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_NAME_START) != 0;
} // isNameStart(int):char

/**
* Returns 1 if the specified character is a valid name
* character as defined by production [4] in the XML 1.0
* specification.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isNameChar(int c) {
	return c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_NAME) != 0;
} // isName(int):char

/**
* Returns 1 if the specified character is a valid NCName start
* character as defined by production [4] in Namespaces in XML
* recommendation.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isNCNameStart(int c) {
	return c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_NCNAME_START) != 0;
} // isNCNameStart(int):char

/**
* Returns 1 if the specified character is a valid NCName
* character as defined by production [5] in Namespaces in XML
* recommendation.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isNCName(int c) {
	return c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_NCNAME) != 0;
} // isNCName(int):char

/**
* Returns 1 if the specified character is a valid Pubid
* character as defined by production [13] in the XML 1.0
* specification.
*
* @param c The character to check.
*/
inline char com_ximpleware::XMLChar_isPubid(int c) {
	return c < 0x10000 && (com_ximpleware::CHARS[c] & com_ximpleware::MASK_PUBID) != 0;
} // isPubid(int):char

#endif
