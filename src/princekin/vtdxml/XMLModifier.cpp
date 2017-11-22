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
#include "XMLModifier.h"
#include <stdlib.h>
using namespace std;
using namespace com_ximpleware;

const static UByte ba1[] = {0x3e,0};
const static UByte ba2[] = {0x3c,0};
const static UByte ba3[] = {0,0x3e};
const static UByte ba4[] = {0,0x3c};

UByte* XMLModifier::doubleCapacity(UByte *b, size_t cap){
	UByte *t = new UByte[cap];
	memcpy(t,b,cap>>1);
	delete b;
	return t;
}
Long XMLModifier::getBytes_UTF8(UCSChar *s){
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

Long XMLModifier::getBytes_UTF16LE(UCSChar *s){
	size_t len = wcslen(s),i=0,k=0,capacity = max<size_t>(len,8)<<1;/*minimum size is 16 */
	int tmp,w1,w2;
	UByte *ba = new UByte[capacity<<1]; 
	if (ba == NULL)
		throw OutOfMemException("Byte allocation failed in getBytes_UTF_16LE");

	for (i=0;i<len;i++){
		if (s[i]<0x10000){
			ba[k]= s[i] & 0x00ff;
			ba[k+1]= (s[i] & 0xff00)>>8;
			k=k+2;
		} else if (s[i]<=0x10ffff){
			if (capacity -k<4){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			tmp = s[i]-0x10000;
			w1 = 0xd800 | (tmp & 0xffc00);
			w2 = 0xdc00 | (tmp & 0x3ff);
			ba[k] = (w1 & 0xff);
			ba[k+1] = (w1 & 0xff00) >> 8;
			ba[k+2] =(w2 & 0xff);
			ba[k+3] = (w2 & 0xff00) >> 8;

		}else 
			throw ModifyException("Invalid XML char for getBytes_UTF_16LE");
	}
	return ((Long)k<<32)|(Long)ba;
}
Long XMLModifier::getBytes_UTF16BE(UCSChar *s){
	size_t len = wcslen(s),i=0,k=0,capacity = max<size_t>(len,8)<<1;/*minimum size is 16 */
	int tmp,w1,w2;
	UByte *ba = new UByte[(capacity<<1)]; 
	if (ba == NULL)
		throw OutOfMemException("Byte allocation failed in getBytes_UTF_16BE");
	for (i=0;i<len;i++){
		if (s[i]<0x10000){
			ba[k]= (s[i] & 0xff00)>>8;
			ba[k+1]= s[i] & 0x00ff;
			k=k+2;
		} else if (s[i]<=0x10ffff){
			if (capacity -k<4){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			tmp = s[i]-0x10000;
			w1 = 0xd800 | (tmp & 0xffc00);
			w2 = 0xdc00 | (tmp & 0x3ff);
			ba[k] = (w1 & 0xff00) >> 8;
			ba[k+1] = (w1 & 0xff);
			ba[k+2] =(w2 & 0xff00) >> 8;
			ba[k+3] = (w2 & 0xff);
			k+=4;
		}else 
			throw ModifyException("Invalid XML char for getBytes_UTF_16BE");
	}
	return ((Long)k<<32)|(Long)ba;
}
Long XMLModifier::getBytes_ISO_8859_1(UCSChar *s){
	size_t len = wcslen(s),i=0;
	UByte *ba = new UByte[len];
	if (ba == NULL)
		throw OutOfMemException("byte allocation failed in getBytes_ISO_8859_1");
	for (i=0;i<len;i++){
		if (s[i]>0xff)
			throw ModifyException("Invalid char for ISO_8859_1");
		ba[i]=(UByte)s[i];
	}
	return ((Long)len<<32)|(Long)ba;
}
Long XMLModifier::getBytes_ASCII(UCSChar *s){
	size_t len = wcslen(s),i=0;
	UByte *ba = new UByte[len];
	if (ba == NULL)
		throw OutOfMemException("byte allocation failed in getBytes_ASCII");
	for (i=0;i<len;i++){
		if (s[i]>127)
			throw ModifyException("Invalid char for ASCII");
		ba[i]=(UByte)s[i];
	}
	return ((Long)len<<32)|(Long)ba;
}
void XMLModifier::check( ){
	int os1, os2, temp,i;
	int size =  flb->size;

	for (i=0;i<size;i++){
		os1 = flb->lower32At(i);
		os2 = flb->lower32At(i)+ (flb->upper32At(i)& 0x1fffffff)-1;
		if (i+1<size){
			temp = flb->lower32At(i+1);
			if (temp!= os1 && temp<=os2){
				throw ModifyException(
					"Invalid insertion/deletion condition detected in output()");
			}
		}
	}
}
void XMLModifier::sort( ){
	if (flb->size >0)
		quickSort(0,flb->size-1);
}

void XMLModifier::quickSort( int lo, int hi){
	int i=lo, j=hi; 
	Long h;
	Long o;
	int x= flb->lower32At( (lo+hi)/2);

	//  partition
	do
	{    
		while (flb->lower32At(i)<x) i++; 
		while (flb->lower32At(j)>x) j--;
		if (i<=j)
		{
			h= flb->longAt(i); 
			o = fob->longAt(i);
			flb->modifyEntry ( i,flb->longAt( j)); 
			fob->modifyEntry ( i,fob->longAt( j));
			flb->modifyEntry ( j,h);
			fob->modifyEntry ( j,o);
			i++; 
			j--;
		}
	} while (i<=j);

	//  recursion
	if (lo<j) quickSort( lo, j);
	if (i<hi) quickSort( i, hi);
}

void XMLModifier::insertBytesAt( int offset, Long lenPlusPointer){
	if (insertHash->isUnique(offset)==false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append( offset | MASK_INSERT_BYTE);
	fob->append( lenPlusPointer);
}
void XMLModifier::insertBytesAt2( int offset, Long lenPlusPointer){
	if (insertHash->isUnique(offset)==false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append( offset | MASK_INSERT_SEGMENT_BYTE);
	fob->append( lenPlusPointer);
}
void XMLModifier::insertBytesAt( int offset, ElementFragmentNs* ef){
	if (insertHash->isUnique(offset)==false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append(  offset | MASK_INSERT_FRAGMENT_NS);
	fob->append( (Long) ef);
}

XMLModifier::XMLModifier():
deleteHash(NULL),
insertHash(NULL),
flb(NULL),
fob(NULL),
md(NULL),
gbytes(NULL)
{
}
XMLModifier::XMLModifier(VTDNav *vn):
deleteHash(NULL),
insertHash(NULL),
flb(NULL),
fob(NULL),
md(NULL),
gbytes(NULL)
{
	bind(vn);
} 
XMLModifier::~XMLModifier(){

	int i=0;
	delete deleteHash;
	delete insertHash;

	if (fob != NULL)
		for (i=0;i<fob->size;i++){
			Long l = flb->longAt(i);
			if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
				//|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE
				)
				delete((UByte *)fob->lower32At(i));
			/*else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
			//freeElementFragmentNs((ElementFragmentNs *)lower32At(xm->fob,i));
			}*/
		}
		delete (flb);
		delete (fob);
}

void XMLModifier::bind(VTDNav *vn){
	md = vn;
	encoding = md->encoding;
	if (deleteHash !=NULL)
		delete (deleteHash);
	if (insertHash !=NULL)
		delete (insertHash);

	deleteHash = new IntHash(IntHash::determineHashWidth(vn->vtdSize));
	insertHash = new IntHash(IntHash::determineHashWidth(vn->vtdSize));

	flb = new FastLongBuffer();
	fob = new FastLongBuffer();
	switch(md->encoding){
		case FORMAT_ASCII:
			gbytes  = &XMLModifier::getBytes_ASCII;
			break;
		case FORMAT_ISO_8859_1:
			gbytes  = &XMLModifier::getBytes_ISO_8859_1;
			break;
		case FORMAT_UTF8:
			gbytes = &XMLModifier::getBytes_UTF8;
			break;
		case FORMAT_UTF_16LE:
			gbytes = &XMLModifier::getBytes_UTF16LE;
			break;
		case FORMAT_UTF_16BE:
			gbytes = &XMLModifier::getBytes_UTF16BE;
			break;
		default:
			throw ModifyException("Encoding not yet supported ");
	}
}
void XMLModifier::remove(){
	int i = md->getCurrentIndex();
	tokenType type = md->getTokenType(i);
	if (type==TOKEN_STARTING_TAG){
		Long l = md->getElementFragment();
		removeContent((int)l, (int)(l>>32));            
	} else if (type == TOKEN_ATTR_NAME 
		|| type==TOKEN_ATTR_NS){
			removeAttribute(i);
	} else {
		removeToken(i);
	}
}
void XMLModifier::removeToken(int i){
	    int type = md->getTokenType(i);
        int os,len;
		os = md->getTokenOffset(i);
		len =
			(type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_ATTR_NS)
			? md->getTokenLength(i) & 0xffff
			: md->getTokenLength(i);
        switch(type){
        	case TOKEN_CDATA_VAL:
        	    if (encoding < FORMAT_UTF_16BE)
        		    removeContent(os - 9, len + 12 );
        		else
        		    removeContent((os - 9)<<1,(len+12)<<1);
        		return;
        		 
        	case TOKEN_COMMENT:
           	    if (encoding< FORMAT_UTF_16BE)
           	        removeContent(os-4, len+7);
           	    else
           	        removeContent((os-4) << 1, (len+7) << 1);
           	    return;
        		
        	default:
    			if (encoding < FORMAT_UTF_16BE)
        	        removeContent(os, len);
        	    else
        	        removeContent((os) << 1, (len) << 1);
        	    return;        	    
        }
}
void XMLModifier::removeAttribute(int attrNameIndex){
	tokenType type = md->getTokenType(attrNameIndex);
	int os1,os2,len2;
	if (type != TOKEN_ATTR_NAME && type != TOKEN_ATTR_NS){
		throw ModifyException("token type should be attribute name");
	}
	os1 = md->getTokenOffset(attrNameIndex);
	os2 = md->getTokenOffset(attrNameIndex+1);
	len2 = md->getTokenLength(attrNameIndex+1);
	if ( md->encoding < FORMAT_UTF_16BE)
		removeContent(os1,os2+len2-os1+1); 
	else 
		removeContent(os1<<1,(os2+len2-os1+1)<<1); 
}
void XMLModifier::removeContent(int offset, int len){
	if (offset < md->docOffset || len > md->docLen 
		|| offset + len > md->docOffset + md->docLen){
			throw ModifyException("Invalid offset or length for removeContent");
	}
	if (deleteHash->isUnique(offset)==false){
		throw ModifyException("There can be only one deletion per offset value");
	}
	while(len > (1<<29)-1){
      	flb->append(((Long)((1<<29)-1))<<32 | offset | MASK_DELETE);
       	fob->append((Long)NULL);
       	len -= (1<<29)-1;
       	offset += (1<<29)-1;
    }
	flb->append(((Long)len)<<32 | offset | MASK_DELETE);
	fob->append((Long)NULL);
}

void XMLModifier::updateToken( int index, UCSChar *newContent){
	int offset, len, type;
	if (newContent==NULL){
		throw InvalidArgumentException("String newContent can't be null");
	}
	offset = md->getTokenOffset(index);
	//len = getTokenLength(xm->md,index);
	type = md->getTokenType(index);
	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? md->getTokenLength(index) & 0xffff
		: md->getTokenLength(index);
	// one insert
	switch(type){
			case TOKEN_CDATA_VAL:
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset-9,(this->*gbytes)(newContent));
				else 
					insertBytesAt((offset-9)<<1,(this->*gbytes)(newContent));
				break;
			case TOKEN_COMMENT:
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset-4,(this->*gbytes)(newContent));
				else 
					insertBytesAt((offset-4)<<1,(this->*gbytes)(newContent));
				break;

			default: 
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset,(this->*gbytes)(newContent));
				else 
					insertBytesAt(offset<<1,(this->*gbytes)(newContent));
	}
	/* one delete */
	removeToken(index);  
}
void XMLModifier::updateToken(int index, UByte *byteContent, int contentOffset, int contentLen){
	int offset, len;
	tokenType type;
	if (byteContent==NULL){
		throw InvalidArgumentException("byteContent can't be null");
	}
	offset = md->getTokenOffset(index);
	//len = getTokenLength(xm->md,index);
	type = md->getTokenType(index);

	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? md->getTokenLength(index) & 0xffff
		: md->getTokenLength(index);
	// one insert
	switch(type){
			case TOKEN_CDATA_VAL:
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset-9,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
				else 
					insertBytesAt((offset-9)<<1,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
				break;
			case TOKEN_COMMENT:
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset-4,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
				else 
					insertBytesAt((offset-4)<<1,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
				break;

			default: 
				if (md->encoding < FORMAT_UTF_16BE)
					insertBytesAt(offset,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
				else 
					insertBytesAt(offset<<1,(((Long)contentLen)<<32)|((int)byteContent+contentOffset));
	}
	/* one delete */
	removeToken(index);   
}
void XMLModifier::updateToken(int index, UByte *byteContent, int contentOffset, int contentLen, encoding_t src_encoding){
	if (src_encoding == encoding) {
		updateToken(index, byteContent, contentOffset, contentLen);
		return;
	}else{
		int offset,  len;
		tokenType type;
		Long bo;
		if (byteContent == NULL)
			throw InvalidArgumentException("newContentBytes can't be null");

		offset = md->getTokenOffset(index);
		//int len = md.getTokenLength(index);
		type = md->getTokenType(index);
		len = (type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS) ?
			md->getTokenLength(index) & 0xffff
			: md->getTokenLength(index);

		// one insert
		bo = Transcoder_transcode(byteContent,contentOffset,
			contentLen, src_encoding, encoding);

		switch (type) {
		case TOKEN_CDATA_VAL:
			if (encoding < FORMAT_UTF_16BE)
				insertBytesAt2( offset - 9, bo);
			else
				insertBytesAt2( (offset - 9) << 1, bo);
			break;
		case TOKEN_COMMENT:
			if (encoding < FORMAT_UTF_16BE)
				insertBytesAt2( offset - 4, bo);
			else
				insertBytesAt2( (offset - 4) << 1, bo);
			break;

		default:
			if (encoding < FORMAT_UTF_16BE)
				insertBytesAt2( offset, bo);
			else
				insertBytesAt2( offset << 1, bo);
		}
		// one delete
		removeToken( index);  
	}
}
void XMLModifier::updateToken(int index, VTDNav *vn1, int contentOffset, int contentLen){
	updateToken(index, vn1->XMLDoc,contentOffset, contentLen, vn1->encoding);
}

void XMLModifier::insertAfterElement(UCSChar *s){
	int startTagIndex = md->getCurrentIndex();
	tokenType type =  md->getTokenType(startTagIndex);
	int offset, len;
	Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	l = md->getElementFragment();
	offset = (int)l;
	len = (int)(l>>32);
	insertBytesAt(offset+len, (this->*gbytes)(s));
}
void XMLModifier::insertBeforeElement(UCSChar *s){
	int startTagIndex = md->getCurrentIndex();
	tokenType type =  md->getTokenType(startTagIndex);
	int offset;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	offset = md->getTokenOffset(startTagIndex)-1;
	
	if (encoding < FORMAT_UTF_16BE)
		insertBytesAt(offset,(this->*gbytes)(s));
	else
		insertBytesAt((offset)<<1,(this->*gbytes)(s));
}


void XMLModifier::insertAttribute(UCSChar *attr){
	int startTagIndex = md->getCurrentIndex(),offset,len;
	tokenType type = md->getTokenType(startTagIndex);
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	offset = md->getTokenOffset(startTagIndex);
	len = md->getTokenLength(startTagIndex)&0xffff;

	if (encoding < FORMAT_UTF_16BE)
		insertBytesAt(offset+len,(this->*gbytes)(attr));
	else
		insertBytesAt((offset+len)<<1,(this->*gbytes)(attr));
}
void XMLModifier::insertAfterHead(UCSChar *s){
	Long i = md->getOffsetAfterHead();
	if (i<0){
        insertBytesEnclosedAt((int)i-1,(this->*gbytes)(s));
		insertEndingTag(i);
		return;
	}
	insertBytesAt((int)i, (this->*gbytes)(s));
}

/* insert a byte array before the start of an element 
   the byte array won't get freed when calling XMLModifier's 
   resetXMLModifier() and freeXMLModifier() 
*/
void XMLModifier::insertAfterElement(UByte* ba, int arrayLen){
	int startTagIndex = md->getCurrentIndex();
	tokenType type =  md->getTokenType(startTagIndex);
	int offset, len;
	Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	l = md->getElementFragment();
	offset = (int)l;
	len = (int)(l>>32);
	insertBytesAt2(offset+len, (((Long)arrayLen)<<32)|(int)ba);
}

/*
Insert a byte of given length before the cursor element
*/
void XMLModifier::insertBeforeElement(UByte* ba, int arrayLen){
	int startTagIndex = md->getCurrentIndex();
	tokenType type =  md->getTokenType(startTagIndex);
	int offset;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	offset = md->getTokenOffset(startTagIndex)-1;
	
	if (encoding < FORMAT_UTF_16BE)
		insertBytesAt2(offset, (((Long)arrayLen)<<32)|(int)ba);
	else
		insertBytesAt2(offset<<1, (((Long)arrayLen)<<32)|(int)ba);
}
void XMLModifier::insertAfterHead(UByte* ba, int arrayLen){
	Long i =md->getOffsetAfterHead();
	if (i < 0){
        insertBytesEnclosedAt2((int)i-1,(((Long)arrayLen)<<32)|(int)ba);
		insertEndingTag(i);
		return;
	}
	insertBytesAt2((int)i, (((Long)arrayLen)<<32)|(int)ba);
}

void XMLModifier::insertAfterElement(UByte* ba, int contentOffset, int contentLen){
	int startTagIndex = md->getCurrentIndex();
	tokenType type =  md->getTokenType(startTagIndex);
	int offset,len;
	Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	//offset = getTokenOffset(xm->md,startTagIndex)-1;
	l = md->getElementFragment();
	offset = (int)l;
	len = (int)(l>>32);	
	insertBytesAt2(offset+len, (((Long)contentLen)<<32)|((int)ba+contentOffset));
}

void XMLModifier::insertBeforeElement(UByte* ba, int contentOffset, int contentLen){
	int startTagIndex = md->getCurrentIndex( );
	tokenType type =  md->getTokenType( startTagIndex);
	int offset;
	//Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	offset = md->getTokenOffset( startTagIndex) - 1;
	if (encoding < FORMAT_UTF_16BE)
		insertBytesAt2( offset, (((Long)contentLen)<<32)|((int)ba+contentOffset));
	else 
		insertBytesAt2( offset<<1, (((Long)contentLen)<<32)|((int)ba+contentOffset));
}

void XMLModifier::insertAfterHead(UByte* ba, int contentOffset, int contentLen){
	Long i =md->getOffsetAfterHead( );
	if (i<0){
        //throwException2(modify_exception,"Insertion failed");
		insertBytesEnclosedAt2((int)i-1,(((Long)contentLen)<<32)|((int)ba+contentOffset));
		insertEndingTag(i);
		return;
	}
	insertBytesAt2((int)i,(((Long)contentLen)<<32)|((int)ba+contentOffset));
}

void XMLModifier::insertBeforeElement(ElementFragmentNs *ef){
	int startTagIndex = md->getCurrentIndex( );
	tokenType type =  md->getTokenType( startTagIndex);
	int offset;
	//Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	offset = md->getTokenOffset( startTagIndex) - 1;
	if ( encoding < FORMAT_UTF_16BE)
		insertBytesAt(offset,ef);
	else 
		insertBytesAt(offset<<1,ef);
}

void XMLModifier::insertAfterElement(ElementFragmentNs *ef){
	int startTagIndex = md->getCurrentIndex( );
	tokenType type =  md->getTokenType( startTagIndex);
	int offset,len;
	Long l;
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("Token type is not a starting tag");
	}
	//offset = getTokenOffset(xm->md,startTagIndex)-1;
	l = md->getElementFragment();
	offset = (int)l;
	len = (int)(l>>32);
	insertBytesAt( offset+len, ef);	
}

void XMLModifier::insertAfterHead(ElementFragmentNs *ef){
	Long i =md->getOffsetAfterHead();
	if (i < 0){
        //throwException2(modify_exception,"Insertion failed");
		insertBytesEnclosedAt((int)i-1,ef);
		insertEndingTag(i);
		return;
	}
	insertBytesAt((int)i,ef);
}


void XMLModifier::insertAfterElement(encoding_t src_encoding, UByte* ba, int arrayLen){
	if(src_encoding ==  encoding){
		insertAfterElement(ba, arrayLen);
	}
	else {    
		int startTagIndex = md->getCurrentIndex( );
		int offset,len;
		tokenType type = md->getTokenType(startTagIndex);
		Long l;
		Long bo;
		if (type!=TOKEN_STARTING_TAG)
			throw ModifyException("Token type is not a starting tag");
		l = md->getElementFragment();
		offset = (int)l;
		len = (int)(l>>32);
		// transcoding logic
		bo = Transcoder_transcode(ba, 0, arrayLen, src_encoding,  encoding);
		//insertBytesAt(xm,offset+len,bo);
		insertBytesAt(offset+len, bo);
	}
}
void XMLModifier::insertBeforeElement(encoding_t src_encoding, UByte* ba, int arrayLen){
	if (src_encoding == encoding) {
		insertBeforeElement(ba, arrayLen);
	} else {
		int startTagIndex = md->getCurrentIndex( );
		int offset;
		tokenType type = md->getTokenType( startTagIndex);
		Long bo;
		if (type != TOKEN_STARTING_TAG)
			throw ModifyException("Token type is not a starting tag");

		offset = md->getTokenOffset(startTagIndex) - 1;
		bo = Transcoder_transcode(ba,0,arrayLen,src_encoding, encoding); 
		if ( encoding < FORMAT_UTF_16BE)
			insertBytesAt( offset,bo);
		else
			insertBytesAt( (offset) << 1, bo);
	}
}
void XMLModifier::insertAfterHead(encoding_t src_encoding, UByte* ba, int arrayLen){
	Long bo;
	Long i = md->getOffsetAfterHead();
	if (i < 0){
        bo = Transcoder_transcode(ba, 0, arrayLen, src_encoding, encoding);
		insertBytesAt((int)i-1, bo);
		insertEndingTag(i);
		return;
	}
	bo = Transcoder_transcode(ba, 0, arrayLen, src_encoding, encoding);
	//insertBytesAt(xm,offset+len,bo);
	insertBytesAt((int)i, bo);
}


void XMLModifier::insertAfterElement(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen){
	if (src_encoding == encoding) {
		insertAfterElement(ba,contentOffset,contentLen);
	} else {
		int startTagIndex = md->getCurrentIndex( );
		int offset, len, type = md->getTokenType( startTagIndex);
		Long l;
		Long bo;
		if (type != TOKEN_STARTING_TAG)
			throw ModifyException("Token type is not a starting tag");
		l = md->getElementFragment( );
		offset = (int) l;
		len = (int) (l >> 32);
		// transcode in here
		bo = Transcoder_transcode(ba, contentOffset, contentLen, src_encoding,  encoding);
		insertBytesAt( offset + len, bo);
	}
}

void XMLModifier::insertBeforeElement(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen){
	if (src_encoding == encoding) {
		insertBeforeElement(ba,contentOffset, contentLen);
	} else {
		int startTagIndex = md->getCurrentIndex();
		int offset; 
		tokenType type = md->getTokenType(startTagIndex);
		Long bo;
		if (type != TOKEN_STARTING_TAG)
			throw ModifyException("Token type is not a starting tag");

		offset = md->getTokenOffset(startTagIndex) - 1;
		// do transcoding here
		bo = Transcoder_transcode(ba,contentOffset,contentLen,src_encoding,  encoding);
		if (encoding < FORMAT_UTF_16BE)
			insertBytesAt(offset, bo);
		else
			insertBytesAt((offset) << 1, bo);
	}
}

void XMLModifier::insertAfterHead(encoding_t src_encoding, UByte* ba, int contentOffset, int contentLen){
	Long bo;
	Long i =md->getOffsetAfterHead();
	if (i < 0){
        bo = Transcoder_transcode(ba, contentOffset, contentLen, src_encoding, encoding);
		insertBytesEnclosedAt((int)i-1,bo);
		insertEndingTag(i);
        //throwException2(modify_exception,"Insertion failed");
		return;
	}
	bo = Transcoder_transcode(ba, contentOffset, contentLen, src_encoding, encoding);
	insertBytesAt((int)i, bo);
}


void XMLModifier::insertAfterElement(VTDNav *vn1, int contentOffset, int contentLen){
	insertAfterElement( vn1->encoding,vn1->XMLDoc,contentOffset,contentLen);
}
void XMLModifier::insertBeforeElement(VTDNav *vn1, int contentOffset, int contentLen){
	insertBeforeElement( vn1->encoding,vn1->XMLDoc,contentOffset, contentLen);
}
void XMLModifier::insertAfterHead(VTDNav *vn1, int contentOffset, int contentLen){
	insertAfterHead( vn1->encoding,vn1->XMLDoc,contentOffset,contentLen);
}

/*
void updateToken2(XMLModifier *xm, int index, UByte *newContentBytes, int len);
void insertAfterElement2(XMLModifier *xm, UByte *b, int len);
void insertBeforeElement2(XMLModifier *xm, UByte *b, int len);
void insertAttribute2(XMLModifier *xm, UByte *attr, int len);
*/

void XMLModifier::output(FILE *f){
	 Long l;
	 size_t k;
	 UByte *ba;
	 int t,start,len;
	if (f == NULL){
		throw InvalidArgumentException(
			"FILE can't be a NULL");
	}           
	t = md->vtdBuffer->lower32At(0);
	start = (t==0)?
		md->docOffset:32;
	len = (t==0)?
		md->docLen:(md->docLen-32);
	sort();
	check2();
	ba = md->XMLDoc;

	if ( flb->size==0){
		k=fwrite(md->XMLDoc+start,sizeof(UByte),len,f);
		if (k!=len){
			throw IOException("fwrite didn't complete");
		}
	}else if (md->encoding < FORMAT_UTF_16BE){
		int offset = start;
		int i;
		int inc=1;
		size_t t;
		for(i=0;i<flb->size;i=i+inc){
			if (i+1==flb->size){
				inc = 1;
			}
			else if (flb->lower32At(i)==flb->lower32At(i+1)){
				inc  = 2;
			} else 
				inc = 1;
			l = flb->longAt(i);
			if (inc == 1){                    
				if ((l & (~0x1fffffffffffffffLL)) == MASK_DELETE){
					t = flb->lower32At(i);
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					offset = t + (flb->upper32At(i) & 0x1fffffff);
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
					|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE){ 
					// insert
					t = flb->lower32At(i);
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					t = fob->upper32At(i);/* the length */
					k=fwrite((void *)fob->lower32At(i),sizeof(UByte),t,f);
					if (k!=t)
						throw IOException("fwrite didn't complete");                      
					offset= flb->lower32At(i);
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
					t = flb->lower32At(i);
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					((ElementFragmentNs*)fob->lower32At(i))->writeFragmentToFile(f,encoding);

					//writeFragmentToFile((ElementFragmentNs*)lower32At(xm->fob,i),f);

					offset= flb->lower32At(i);
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE_ENCLOSED
					|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED){
					t = flb->lower32At(i);
					
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					fwrite(">",sizeof(UByte),1,f);
					t = fob->upper32At(i);/* the length */
					k=fwrite((void *)fob->lower32At(i),sizeof(UByte),t,f);
					if (k!=t)
						throw IOException("fwrite didn't complete");   
					fwrite("<",sizeof(UByte),1,f);
					offset= flb->lower32At(i);
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED){
					
					t = flb->lower32At(i);
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					fwrite(">",sizeof(UByte),1,f);
					((ElementFragmentNs*)fob->lower32At(i))->writeFragmentToFile(f,encoding);
					fwrite("<",sizeof(UByte),1,f);
					//writeFragmentToFile((ElementFragmentNs*)lower32At(xm->fob,i),f);
					offset=flb->lower32At(i);
				} 
			} else {
				    Long k = flb->longAt(i+1),temp;
					int i1 = i,temp2,k1;
                    int i2 = i+1;
                    if ((l & (~0x1fffffffffffffffLL)) != MASK_DELETE){
                        temp = l;
                        l= k;
                        k = temp;
                        temp2 = i1;
                        i1 = i2;
                        i2 = temp2;
                    }									
					if ((l & (~0x1fffffffffffffffL)) == MASK_NULL) {
					} else {
						t = flb->lower32At(i1);
						k1=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
						if (k1!=t-offset)
							throw IOException("fwrite didn't complete");
						//os.write(ba,offset, flb.lower32At(i)-offset);
						//os.write((byte[])fob.objectAt(i+1));
						//t = upper32At(xm->fob,i+1);/* the length */

						if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
							|| (k & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE){
								/*t = lower32At(xm->flb,i+1);
								k=fwrite(xm->md->XMLDoc+offset,sizeof(UByte),t-offset,f);
								if (k!=t-offset)
								throwException2(io_exception,"fwrite didn't complete");*/
								/*os.write(ba,offset, flb.lower32At(i+1)-offset);*/
								t = fob->upper32At(i2);   /* the length */
								k=fwrite((void *)fob->lower32At(i2),sizeof(UByte),t,f);
								if (k!=t)
									throw IOException("fwrite didn't complete");  
								offset = flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff);
						}else if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
							/*t = lower32At(xm->flb,i+1);
							k=fwrite(xm->md->XMLDoc+offset,sizeof(UByte),t-offset,f);
							if (k!=t-offset)
							throwException2(io_exception,"fwrite didn't complete");*/
							((ElementFragmentNs*)fob->lower32At(i2))->writeFragmentToFile(f,encoding);
							offset = flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff);
						}else if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE_ENCLOSED
							|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED){ 
								/*os.write(ba,offset, flb.lower32At(i+1)-offset);*/
								fwrite(">",sizeof(UByte),1,f);
								t = fob->upper32At(i2);   /* the length */
								k=fwrite((void *)fob->lower32At(i2),sizeof(UByte),t,f);
								if (k!=t)
									throw IOException("fwrite didn't complete");  
								fwrite("<",sizeof(UByte),1,f);
								offset = flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff);
						}else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED) {
							fwrite(">",sizeof(UByte),1,f);
							((ElementFragmentNs*)fob->lower32At(i2))->writeFragmentToFile(f,encoding);
							fwrite("<",sizeof(UByte),1,f);
							offset = flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff);
						}
					}
			}
		}  
		t = start+len-offset;
		k=fwrite(md->XMLDoc+offset,sizeof(UByte),t,f);
		if (k!=t)
			throw IOException("fwrite didn't complete");
	}else{
		UByte* b1  = (UByte *)ba1;
		UByte* b2  = (UByte *)ba2;
		int offset = start;
		int i;
		int inc=1;
		size_t t;
		if (md->encoding == FORMAT_UTF_16BE){
			b1 =  (UByte *)ba3;
			b2 =  (UByte *)ba4;
		}

		for(i=0;i<flb->size;i=i+inc){
			if (i+1==flb->size){
				inc = 1;
			}
			else if (flb->lower32At(i)==flb->lower32At(i+1)){
				inc  = 2;
			} else 
				inc = 1;
			l = flb->longAt(i);
			if (inc == 1){                    
				if ((l & (~0x1fffffffffffffffLL)) == MASK_DELETE){
					t = flb->lower32At(i)<<1;
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					offset = t + ((flb->upper32At(i) & 0x1fffffff)<<1);
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
					|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE){ 
					// insert
					t = flb->lower32At(i)<<1;
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					t = fob->upper32At(i);/* the length */
					k=fwrite((void *)(fob->lower32At(i)),sizeof(UByte),t,f);
					if (k!=t)
						throw IOException("fwrite didn't complete");                      
					offset= flb->lower32At(i)<<1;
				}else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
					t = flb->lower32At(i)<<1;
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					((ElementFragmentNs*)fob->lower32At(i))->writeFragmentToFile(f,encoding);
					//writeFragmentToFile((ElementFragmentNs*)lower32At(xm->fob,i),f);

					offset= flb->lower32At(i)<<1;
				} else if ((l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE_ENCLOSED
					|| (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED){
						// insert
					t = flb->lower32At(i)<<1;
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					t = fob->upper32At(i);/* the length */
					fwrite(b1,sizeof(UByte),2,f);
					k=fwrite((void *)fob->lower32At(i),sizeof(UByte),t,f);
					if (k!=t)
						throw IOException("fwrite didn't complete"); 
					fwrite(b2,sizeof(UByte),2,f);                     
					offset= flb->lower32At(i)<<1;
				} else {
					t = flb->lower32At(i)<<1;
					fwrite(b1,sizeof(UByte),2,f);
					k=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k!=t-offset)
						throw IOException("fwrite didn't complete");
					fwrite(b2,sizeof(UByte),2,f);
					((ElementFragmentNs*)fob->lower32At(i))->writeFragmentToFile(f,encoding);
				}
			} else {
				    Long k = flb->longAt(i+1),temp;
					int i1 = i,temp2,k1;
                    int i2 = i+1;
                    if ((l & (~0x1fffffffffffffffLL)) != MASK_DELETE){
                        temp = l;
                        l= k;
                        k = temp;
                        temp2 = i1;
                        i1 = i2;
                        i2 = temp2;
                    }		

					if ((l & (~0x1fffffffffffffffL)) == MASK_NULL) {
					} else {
					
					t = flb->lower32At(i1)<<1;
					k1=fwrite(md->XMLDoc+offset,sizeof(UByte),t-offset,f);
					if (k1!=t-offset)
						throw IOException("fwrite didn't complete");
					//os.write(ba,offset, flb.lower32At(i)-offset);
					//os.write((byte[])fob.objectAt(i+1));
					//t = upper32At(xm->fob,i+1);/* the length */
								   
					if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
						|| (k & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE){
							/*t = lower32At(xm->flb,i+1);
							k=fwrite(xm->md->XMLDoc+offset,sizeof(UByte),t-offset,f);
							if (k!=t-offset)
							throwException2(io_exception,"fwrite didn't complete");*/
							/*os.write(ba,offset, flb.lower32At(i+1)-offset);*/
							t = fob->upper32At(i2);   /* the length */
							k=fwrite((void *)fob->lower32At(i2),sizeof(UByte),t,f);
							if (k!=t)
								throw IOException("fwrite didn't complete");  
							offset = (flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff))<<1;
					}else if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
						/*t = lower32At(xm->flb,i+1);
						k=fwrite(xm->md->XMLDoc+offset,sizeof(UByte),t-offset,f);
						if (k!=t-offset)
						throwException2(io_exception,"fwrite didn't complete");*/
						((ElementFragmentNs*)fob->lower32At(i2))->writeFragmentToFile(f,encoding);
						offset = (flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff)<<1);
					} else if ((k & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE_ENCLOSED
						|| (k & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED){
							t = fob->upper32At(i2);   /* the length */
							fwrite(b1,sizeof(UByte),2,f);
							k=fwrite((void *)fob->lower32At(i2),sizeof(UByte),t,f);
							fwrite(b2,sizeof(UByte),2,f);
							if (k!=t)
								throw IOException("fwrite didn't complete");  
							offset = (flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff))<<1;
					}else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED) {
						fwrite(b1,sizeof(UByte),2,f);
						((ElementFragmentNs*)fob->lower32At(i2))->writeFragmentToFile(f,encoding);
						fwrite(b2,sizeof(UByte),2,f);
						offset = (flb->lower32At(i1) + (flb->upper32At(i1) & 0x1fffffff))<<1;
					}
				}
			}
		}  
		t = start+len-offset;
		k=fwrite(md->XMLDoc+offset,sizeof(UByte),t,f);
		if (k!=t)
			throw IOException("fwrite didn't complete");
	}

}
	
void XMLModifier::output(char *fileName){
	FILE *f = NULL;
	f = fopen(fileName,"wb");
	
	if (f==NULL){
		throw InvalidArgumentException("fileName not valid");
	}
	output(f);
	fclose(f);
}

void XMLModifier::updateElementName(UCSChar* newElementName){
	int i = md->getCurrentIndex( );
	int type = md->getTokenType( i);
	int len,offset,temp;
	Long l;
	UByte* xml;
	encoding_t enc;
	
	if (type!=TOKEN_STARTING_TAG){
		throw ModifyException("You can only update a element name");
	}
	offset =  md->getTokenOffset(  i);
	len =  md->getTokenLength( i)& 0xffff;
	updateToken( i,newElementName);
	l =  md->getElementFragment( );
	enc =  md->getEncoding(  );
	xml =  md->XMLDoc;

	temp = (int)l+(int)(l>>32);
	if (enc < FORMAT_UTF_16BE) {
		//scan backwards for />
		//int temp = (int)l+(int)(l>>32);
		if (xml[temp - 2] == '/')
			return;
		//look for </
		temp--;
		while (xml[temp] != '/') {
			temp--;
		}
		insertBytesAt( temp + 1,  (this->*gbytes)(newElementName));
		//insertBytesAt(xm,offset-9,xm->gbytes(newContent))
		removeContent( temp + 1, len);
		return;
		//
	} else if (enc == FORMAT_UTF_16BE) {

		//scan backwards for />
		if (xml[temp - 3] ==  '/' && xml[temp - 4] == 0)
			return;

		temp-=2;
		while (!(xml[temp+1] == '/' && xml[temp ] == 0)) {
			temp-=2;
		}
		insertBytesAt( temp+2,  (this->*gbytes)(newElementName));
		removeContent( temp+2, len<<1);            
	} else {
		//scan backwards for />
		if (xml[temp - 3] == 0 && xml[temp - 4] == '/')
			return;

		temp-=2;
		while (!(xml[temp] == '/' && xml[temp+1 ] == 0) ) {
			temp-=2;
		}
		insertBytesAt( temp+2 ,  (this->*gbytes)(newElementName));
		removeContent( temp+2 , len<<1);
	}
}

void XMLModifier::reset(){	
		if (fob!= NULL){
			int i=0;
			for (i=0; i< flb->size;i++){
				// free byte segment and elementFragmentNs
				Long l = flb->longAt(i);
				if ( (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_BYTE
					//||(l & (~0x1fffffffffffffffLL)) == MASK_INSERT_SEGMENT_BYTE
					)
					free((void *)fob->lower32At(i));
				/*else if ( (l & (~0x1fffffffffffffffLL)) == MASK_INSERT_FRAGMENT_NS){
					//freeElementFragmentNs((ElementFragmentNs *)lower32At(xm->fob,i));
				}*/
			}
			fob->clear();
		}
		if ( flb!= NULL){
			flb->clear();
		}
		if ( insertHash!= NULL){
			insertHash->reset();
		}
		if ( deleteHash!= NULL){
			deleteHash->reset();
		}
	
}

void XMLModifier::insertBytesEnclosedAt(int offset, Long l){
	if (insertHash->isUnique(offset)== false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append( offset | MASK_INSERT_SEGMENT_BYTE_ENCLOSED);
	fob->append( l);
}

void XMLModifier::insertBytesEnclosedAt2(int offset, Long lenPlusPointer){
	if (insertHash->isUnique(offset)==false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append( offset | MASK_INSERT_BYTE_ENCLOSED);
	fob->append( lenPlusPointer);
}

void XMLModifier::insertBytesEnclosedAt( int offset, ElementFragmentNs* ef){
	if (insertHash->isUnique(offset)==false){
		throw ModifyException("There can be only one insertion per offset value");
	}
	flb->append( offset | MASK_INSERT_FRAGMENT_NS_ENCLOSED);
	fob->append((Long) ef);
}


void XMLModifier::insertEndingTag(Long l){
	int i = md->getCurrentIndex();
	int offset = md->getTokenOffset(i);
	int length = md->getTokenLength(i)&0xffff;
	UByte *xml =  md->XMLDoc;
	if (md->encoding < FORMAT_UTF_16BE)
		insertBytesAt2((int)l, (((Long)length)<<32)|((int)xml+offset));//xml,offset,length);//(((Long)contentLen)<<32)|((int)ba+contentOffset)
	else
		insertBytesAt2((int)l, (((Long)length<<1)<<32)|((int)xml+(offset<<1)));//xml,
}

/*void XMLModifier::insertBeforeTail(UByte *ba, Long l){
	  Long i = md->getOffsetBeforeTail();
        if (i<0){
            //throw new ModifyException("Insertion failed");
        	insertAfterHead(ba,l);
        	return;
        }
        insertBytesAt((int)i,ba,l);
}*/


void XMLModifier::insertBeforeTail(UByte *ba, int contentOffset, int contentLen){
	Long i = md->getOffsetBeforeTail();
	if (i<0){
		//throw new ModifyException("Insertion failed");
		insertAfterHead(ba, contentOffset, contentLen);
		return;
	}
	insertBytesAt2((int)i,(((Long)contentLen)<<32)|((int)ba+contentOffset));
}


void XMLModifier::insertBeforeTail(UByte *ba, int arrayLen){
	Long i = md->getOffsetBeforeTail();
	if (i<0){
		//throw new ModifyException("Insertion failed");
		// handle empty element case
		// <a/> would become <a>b's content</a>
		// so there are two insertions there
		insertAfterHead(ba, arrayLen);
		return;
	}

	insertBytesAt2((int)i,(((Long)arrayLen)<<32)|(int)ba);
}


void XMLModifier::insertBeforeTail(ElementFragmentNs *ef){
	Long i = md->getOffsetBeforeTail();
    if (i<0){
            //throw new ModifyException("Insertion failed");
        insertAfterHead(ef);
        return;
    }
	insertBytesAt((int)i, ef);
    //insertElementFragmentNsAt((int)i, ef);
}

/*void XMLModifier::insertBeforeTail(encoding_t src_encoding,UByte *ba, Long l){

}*/


void XMLModifier::insertBeforeTail(encoding_t src_encoding, UByte *ba, int contentOffset, int contentLen){
	 if(src_encoding == encoding){
            insertAfterHead(ba,contentOffset,contentLen);
        }else{
            Long i = md->getOffsetBeforeTail();
            if (i<0){
                //throw new ModifyException("Insertion failed");
            	insertAfterHead(src_encoding,ba,contentOffset, contentLen);
            	return;
            }
            Long bo = Transcoder_transcode(ba, contentOffset, contentLen, src_encoding,  encoding);
			if ( encoding < FORMAT_UTF_16BE)
				insertBytesAt( (int) i,bo);
			else
				insertBytesAt( (int) i<< 1, bo);
        }
}


void XMLModifier::insertBeforeTail(encoding_t src_encoding, UByte *ba, int arrayLen){
	 if(src_encoding == encoding){
		 insertBeforeTail(ba, arrayLen);
     }else{
		 Long i = md->getOffsetBeforeTail();
         if (i<0){
            //throw new ModifyException("Insertion failed");
          	insertAfterHead(src_encoding,ba,arrayLen);
           	return;
          }
         Long bo = Transcoder_transcode(ba,0,arrayLen,src_encoding, encoding); 
		if ( encoding < FORMAT_UTF_16BE)
			insertBytesAt( (int) i,bo);
		else
			insertBytesAt( (int) i<< 1, bo);        
	}
}


void XMLModifier::insertBeforeTail(UCSChar *s){
	Long i = md->getOffsetBeforeTail();
    if (i<0){
     //throw new ModifyException("Insertion failed");
     // handle empty element case
     // <a/> would become <a>b's content</a>
     // so there are two insertions there
    	insertAfterHead(s);
    	return;
    }

	if (encoding < FORMAT_UTF_16BE)
		insertBytesAt((int)i,(this->*gbytes)(s));
	else
		insertBytesAt(((int)i)<<1,(this->*gbytes)(s));
    
}

void XMLModifier::check2() {
    	int os1, os2, temp;
        int size = flb->size;
        for (int i=0;i<size;){
            os1 = flb->lower32At(i);
            os2 = flb->lower32At(i)+ (flb->upper32At(i)& 0x1fffffff)-1;            
            
            int z=1;
			while (i + z < size) {
				temp = flb->lower32At(i+z);
				if (temp==os1){
					if ((flb->upper32At(i+z)& 0x1fffffff)!=0) // not an insert
						os2=flb->lower32At(i+z)+ (flb->upper32At(i+z)& 0x1fffffff)-1; 
					z++;
				}
				else if (temp > os1 && temp <= os2) {
					int k= flb->lower32At(i+z)+ (flb->upper32At(i+z)& 0x1fffffff)-1;
					if (k>os2)
					// take care of overlapping conditions
					 throw new ModifyException
					  ("Invalid insertion/deletion condition detected between offset detected ");
					  // +os1 + " and offset "+os2);
					else
						flb->modifyEntry(i+z,(flb->longAt(i+z)& 0x1fffffffffffffffL)|MASK_NULL);
					//System.out.println(" hex ==> "+Long.toHexString(flb.longAt(k+z)));
					z++;
				} else
					break;
			}
            i+=z;
        }
    }
