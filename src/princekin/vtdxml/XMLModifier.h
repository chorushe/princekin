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
#ifndef XMLMODIFIER_H
#define XMLMODIFIER_H
#include "vtdGen.h"
#include "elementFragmentNs.h"
#include "intHash.h"
#include "transcoder.h"

namespace com_ximpleware {
	class XMLModifier {
		
	public:
		
		const static Long MASK_DELETE = 0x00000000000000000LL;
		const static Long MASK_INSERT_SEGMENT_BYTE  =0x2000000000000000LL;
		const static Long MASK_INSERT_BYTE =0x4000000000000000LL;
		const static Long MASK_INSERT_SEGMENT_STRING =0x6000000000000000LL;
		const static Long MASK_INSERT_STRING =0x8000000000000000LL;
		const static Long MASK_INSERT_FRAGMENT_NS  =0xa000000000000000LL;
		const static Long MASK_INSERT_SEGMENT_BYTE_ENCLOSED = 0x6000000000000000LL;
		const static Long MASK_INSERT_BYTE_ENCLOSED = 0x8000000000000000LL;
		const static Long MASK_INSERT_FRAGMENT_NS_ENCLOSED = 0xe000000000000000LL;
		const static Long MASK_NULL =  0xc000000000000000LL; //1100

		XMLModifier();
		XMLModifier(VTDNav *vn);
		~XMLModifier();
		void bind(VTDNav *md);
		void remove();
		void removeToken(int i);
		void removeAttribute(int attrNameIndex);
		void removeContent(int offset, int len);

		void updateToken( int index, UCSChar *newContent);
		void updateToken(int index, UByte *byteContent, int contentOffset, int contentLen);
		void updateToken(int index, UByte *byteContent, int contentOffset, int contentLen, encoding_t src_encoding);
		void updateToken(int index, VTDNav *vn, int contentOffset, int contentLen);

		void insertAfterElement(UCSChar *s);
		void insertBeforeElement(UCSChar *s);
		void insertAttribute(UCSChar *attr);
		void insertAfterHead(UCSChar *attr);

		void insertAfterElement(UByte* ba, int arrayLen);
		void insertAfterElement(char* ba,int arrayLen){
			insertAfterElement((UByte *)ba,arrayLen);
		}
		void insertBeforeElement(UByte* ba, int arrayLen);
		void insertBeforeElement(char* ba,int arrayLen){
			insertBeforeElement((UByte *)ba,arrayLen);
		}
		void insertAfterHead(UByte* ba, int arrayLen);
		void insertAfterHead(char* ba,int arrayLen){
			insertAfterHead((UByte *)ba,arrayLen);
		}

		void insertAfterElement(UByte* ba, int contentOffset, int contentLen);
		void insertAfterElement(char* ba, int contentOffset, int contentLen){
			insertAfterElement((UByte *)ba, contentOffset,contentLen);
		}
		void insertBeforeElement(UByte* ba, int contentOffset, int contentLen);
		void insertBeforeElement(char* ba, int contentOffset, int contentLen){
			insertBeforeElement((UByte *)ba, contentOffset,contentLen);
		}
		void insertAfterHead(UByte* ba, int contentOffset, int contentLen);
		void insertAfterHead(char* ba, int contentOffset, int contentLen){
			insertAfterHead((UByte *)ba, contentOffset,contentLen);
		}
		void insertBeforeElement(ElementFragmentNs *ef);
		void insertAfterElement(ElementFragmentNs *ef);
		void insertAfterHead(ElementFragmentNs *ef);


		void insertAfterElement(encoding_t src_encoding, UByte* ba, int arrayLen);
		void insertAfterElement(encoding_t src_encoding, char* ba, int arrayLen){
			insertAfterElement(src_encoding, (UByte *) ba, arrayLen);
		}
		void insertBeforeElement(encoding_t src_encoding, UByte* ba, int arrayLen);
		void insertBeforeElement(encoding_t src_encoding, char* ba, int arrayLen){
			insertBeforeElement(src_encoding, (UByte *) ba, arrayLen);
		}
		void insertAfterHead(encoding_t src_encoding, UByte* ba, int arrayLen);
		void insertAfterHead(encoding_t src_encoding, char* ba, int arrayLen){
			insertAfterHead(src_encoding, (UByte *) ba, arrayLen);
		}

		void insertAfterElement(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen);
		void insertBeforeElement(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen);
		void insertAfterHead(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen);

		
		void insertAfterElement(encoding_t src_encoding, char* ba, int contentOffset, int contentLen){
			insertAfterElement(  src_encoding, (UByte*) ba, contentOffset, contentLen);
		}
		void insertBeforeElement(encoding_t src_encoding, char* ba, int contentOffset, int contentLen){
			insertBeforeElement( src_encoding, (UByte*) ba, contentOffset, contentLen);
		}
		void insertAfterHead(encoding_t src_encoding, char* ba, int contentOffset, int contentLen){
			insertAfterHead(  src_encoding, (UByte*) ba, contentOffset, contentLen);
		}


		void insertAfterElement(VTDNav *vn1, int contentOffset, int contentLen);
		void insertBeforeElement(VTDNav *vn1, int contentOffset, int contentLen);
		void insertAfterHead(VTDNav *vn1, int contentOffset, int contentLen);

		///2.11----------------------
		void insertBeforeTail(VTDNav *vn1, int contentOffset, int contentLen){
			insertBeforeTail(vn1->encoding,vn1->XMLDoc,contentOffset, contentLen); 
		}
		/*void insertBeforeTail(VTDNav *vn1, Long l){
			insertBeforeTail(vn1->encoding, vn1->XMLDoc, l);
		}*/
		
		/*void insertBeforeTail(UByte *ba, Long l);
		void insertBeforeTail(char *ba, Long l){
			insertBeforeTail((UByte *)ba, l);
		}*/
		
		void insertBeforeTail(UByte *ba, int contentOffset, int contentLen);
		void insertBeforeTail(char *ba, int contentOffset, int contentLen){
			insertBeforeTail((UByte *)ba, contentOffset, contentLen);
		}

		void insertBeforeTail(UByte *ba, int arrayLen);
		void insertBeforeTail(char *ba, int arrayLen){
			insertBeforeTail((UByte *)ba, arrayLen);
		}

		void insertBeforeTail(ElementFragmentNs *ef);

		/*void insertBeforeTail(encoding_t src_encoding,UByte *ba, Long l);
		void insertBeforeTail(encoding_t src_encoding,char *ba, Long l){
			insertBeforeTail(src_encoding, (UByte *)ba, l);
		}*/

		void insertBeforeTail(encoding_t src_encoding, UByte *ba, int contentOffset, int contentLen);
		void insertBeforeTail(encoding_t src_encoding,char *ba,int contentOffset, int contentLen){
			insertBeforeTail(src_encoding, (UByte *)ba, contentOffset, contentLen);
		}

		void insertBeforeTail(encoding_t src_encoding, UByte *ba, int arrayLen);
		void insertBeforeTail(encoding_t src_encoding, char *ba, int arrayLen){
			insertBeforeTail(src_encoding, (UByte *)ba, arrayLen);
		}

		void insertBeforeTail(UCSChar *s);

		/*
		void updateToken2(XMLModifier *xm, int index, UByte *newContentBytes, int len);
		void insertAfterElement2(XMLModifier *xm, UByte *b, int len);
		void insertBeforeElement2(XMLModifier *xm, UByte *b, int len);
		void insertAttribute2(XMLModifier *xm, UByte *attr, int len);
		*/

		void output(FILE *f);
		void output(char *fileName);

		void updateElementName(UCSChar* elementName);

		void reset();

	private:
		
		typedef Long (XMLModifier::*getBytes)(UCSChar *s);
		encoding_t encoding;
		IntHash *deleteHash;
		IntHash *insertHash;
		FastLongBuffer *flb; /* lower 32 bit offset, upper 29 bits
						 length, upper 3 bits */
		FastLongBuffer *fob; /*lower 32 bits the object pointer,
						 upper 32 bits the length of the byte array*/
		VTDNav *md; /*master document*/
		getBytes gbytes;

		Long getBytes_UTF8(UCSChar *s);
		Long getBytes_UTF16LE(UCSChar *s);
		Long getBytes_UTF16BE(UCSChar *s);
		Long getBytes_ISO_8859_1(UCSChar *s);
		Long getBytes_ASCII(UCSChar *s);

		void check( );
		void sort( );
		void quickSort( int lo, int hi);
		void insertBytesAt( int offset, Long l);
		void insertBytesAt2( int offset, Long lenPlusPointer);
		void insertBytesAt( int offset, ElementFragmentNs* ef);
		UByte *doubleCapacity(UByte *b, size_t cap);

		void insertBytesEnclosedAt(int offset, Long l);
		void insertBytesEnclosedAt2(int offset, Long lenPlusPointer);
		void insertBytesEnclosedAt( int offset, ElementFragmentNs* ef);

		void insertEndingTag(Long l);
		void check2();
		
		
	};
}

#endif