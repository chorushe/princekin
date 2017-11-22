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
#include "elementFragmentNs.h"
#include "transcoder.h"


using namespace com_ximpleware;


UByte ElementFragmentNs::ws[5]={0,' ',0,'-',0};
//ElementFragmentNs::ws[2]= 0;
//ElementFragmentNs::ws[4] = 0;
//ElementFragmentNs::ws[1]=' ';
//ElementFragmentNs::ws[3]='-';

ElementFragmentNs::ElementFragmentNs(VTDNav *vn, Long l1, FastIntBuffer *fib, int len):
l (l1),
fib(fib),
stLen(len),
vn(vn)
{
	
}

ElementFragmentNs::~ElementFragmentNs(){
	delete fib;
}

int ElementFragmentNs::getFragmentSize(){
		//int encoding = vn.encoding;
	int len = (int) (l >> 32);
	int i = 0;
	if (stLen != 0)
		for (i = 0; i < fib->getSize(); i++) {
			int k = fib->intAt(i);
			if (vn->encoding < FORMAT_UTF_16BE){
				len += (vn->getTokenLength(k) & 0xffff)
					+ vn->getTokenLength( k + 1) + 4;
			} else {
				len += ((vn->getTokenLength(k) & 0xffff)
					+ vn->getTokenLength( k + 1) + 4)<<1;
			}
		}
		return len;
}
int ElementFragmentNs::getFragmentSize(encoding_t dest_encoding){
	        if (vn->encoding == dest_encoding)
            return getFragmentSize();
		else{
        int i /*, src_encoding= vn->encoding*/;
		UByte *ba = vn->XMLDoc;
        int len = Transcoder_getOutLength(ba, (int)l, (int)(l >>32 ), vn->encoding, dest_encoding );
        
        if (stLen != 0)
            for (i = 0; i < fib->getSize(); i++) {
                int k = fib->intAt(i);
                if (vn->encoding < FORMAT_UTF_16BE ){                    
                    len += Transcoder_getOutLength(ba, vn->getTokenOffset(k),
                            (vn->getTokenLength(k) & 0xffff), vn->encoding,
                            dest_encoding)
                            + Transcoder_getOutLength(ba, vn->getTokenOffset( k+1),
                                    vn->getTokenLength(k + 1), vn->encoding,
                                    dest_encoding) + ((dest_encoding<FORMAT_UTF_16BE)?4:8);
                }else {
                    len += Transcoder_getOutLength(ba, vn->getTokenOffset(k)<<1,
                            (vn->getTokenLength(k) & 0xffff)<<1, vn->encoding,
                            dest_encoding)
                            + Transcoder_getOutLength(ba, vn->getTokenOffset(k+1)<<1,
                                    vn->getTokenLength(k + 1)<<1, vn->encoding,
                                    dest_encoding) + ((dest_encoding<FORMAT_UTF_16BE)?4:8);                
                }
            }
        return len;   
		}
}

Long ElementFragmentNs::getOffsetLeng(){
	return l;
}

UByte* ElementFragmentNs::toFragmentBytes(){
	//UByte  *ba = (UByte *)malloc(getFragmentSize(ef));
	UByte *ba = new UByte[getFragmentSize()];
	if (ba == NULL){
		throw OutOfMemException("byte array allocation failed in toFragmentBytes");
	} else {
		int os = (int)l;
		int len = (int)(l>>32);
		int os1 = 0;
		encoding_t enc;
		int temp,i;
		int tos =0,tlen=0;
		UByte *xml = vn->XMLDoc;
		if (stLen==0){
			//System.arraycopy(xml,os,ba,0,len);
			memcpy(ba,xml+os,len);
			return ba;
		}
		enc = vn->encoding;
		temp = 0;

		switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: temp= (stLen+1)<<1; break;
			default:
				temp = stLen+1;
		}            
		//System.arraycopy(xml,os,ba,0,temp);
		memcpy(ba,xml+os,temp);
		//namespace compensation
		os1 += temp;

		
		for (i = 0; i < fib->getSize(); i++) {
			//System.out.println("i ==>"+fib.intAt(i));
			switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws,0,ba,os1,2);
				memcpy(ba+os1,ws,2);
				os1 += 2;
				tos = vn->getTokenOffset(fib->intAt(i))<<1;
				tlen= (vn->getTokenLength(fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);

				os1 +=tlen;
				// write a 0 and =
				// System.arraycopy(ws,2,ba,os1,2);
				memcpy(ba+os1,ws+2,2);
				os1	+= 2;
				tos = (vn->getTokenOffset( fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength( fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws,1,ba,os1,2);
				memcpy(ba+os1,ws+1,2);
				os1 += 2;
				tos = vn->getTokenOffset( fib->intAt(i))<<1;
				tlen= (vn->getTokenLength( fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				// 	 write a = and 0
				//System.arraycopy(ws,3,ba,os1,2);
				memcpy(ba+os1,ws+3,2);
				os1	+= 2;
				tos = (vn->getTokenOffset(fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength(fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				break;
			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				memcpy(ba+os1,ws+1,1);
				os1 ++;
				tos = vn->getTokenOffset( fib->intAt(i));
				tlen = (vn->getTokenLength(fib->intAt(i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				// 	 write a = 
				//System.arraycopy(ws, 3, ba, os1, 1);
				memcpy(ba+os1,ws+3,1);

				os1 ++;
				tos = vn->getTokenOffset( fib->intAt(i) + 1) - 1 ;
				tlen = (vn->getTokenLength( fib->intAt(i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
			}
		}
		//System.out.println(new String(ba));
		switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: 
				temp= (stLen+1)<<1; break;
			default:
				temp = stLen+1;
		}  
		//System.arraycopy(xml, os + temp, ba, os1, len - temp);
		memcpy(ba+os1,xml+os+temp, len-temp);
		//System.out.println(new String(ba));
		return ba;
	}
}

UByte* ElementFragmentNs::toFragmentBytes(encoding_t dest_encoding){
		if (dest_encoding == vn->encoding){
		return toFragmentBytes();
	}else{
		// find out how long the output bytes are
		//UByte *ba = malloc(getFragmentSize2(ef,dest_encoding));
		UByte *ba = new UByte[getFragmentSize(dest_encoding)];
		int i,tos,tlen,os = (int)l;
		int len = (int)(l>>32);
		int os1 = 0;
		encoding_t  enc;
		int temp=0, outPosition=0;
		UByte* xml = vn->XMLDoc;
		if (stLen==0){
			Transcoder_transcodeAndFill(xml,ba,os,len,vn->encoding,dest_encoding);
			return ba;
		}
		enc = vn->encoding;

		// transcode and fill the bytes
		switch (enc) {
		case FORMAT_UTF_16BE:
		case FORMAT_UTF_16LE:
			temp = (stLen + 1) << 1;
			break;
		default:
			temp = stLen + 1;
		}
		// transcode starting length
		outPosition = Transcoder_transcodeAndFill2(outPosition, 
			xml, ba, os, temp, vn->encoding, dest_encoding);

		//System.arraycopy(xml, os, ba, 0, temp);

		//namespace compensation
		os1 += temp;

		tos = 0, tlen = 0;
		for (i = 0; i < fib->getSize(); i++) {
			//System.out.println("i ==>"+fib.intAt(i));
			switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws, 0, ba, os1, 2);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws,ba,0,2, vn->encoding, dest_encoding);
				os1 += 2;
				tos = vn->getTokenOffset( fib->intAt(i)) << 1;
				tlen = (vn->getTokenLength(fib->intAt(i)) & 0xffff) << 1;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml,ba,tos,tlen, vn->encoding, dest_encoding);
				os1 += tlen;
				// write a 0 and =
				//System.arraycopy(ws, 2, ba, os1, 2);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws,ba,2,2, vn->encoding, dest_encoding);
				os1 += 2;
				tos = (vn->getTokenOffset(fib->intAt(i) + 1) - 1) << 1;
				tlen = ((vn->getTokenLength(fib->intAt(i) + 1) & 0xffff) + 2) << 1;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml, ba, tos, tlen, vn->encoding, dest_encoding);
				os1 += tlen;
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws, 1, ba, os1, 2);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws, ba, 1, 2, vn->encoding, dest_encoding);
				os1 += 2;
				tos = vn->getTokenOffset(fib->intAt(i)) << 1;
				tlen = (vn->getTokenLength(fib->intAt(i)) & 0xffff) << 1;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml, ba, tos, tlen, vn->encoding, dest_encoding);
				os1 += tlen;
				// 	 write a = and 0
				//System.arraycopy(ws, 3, ba, os1, 2);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws,ba,3,2, vn->encoding, dest_encoding);
				os1 += 2;
				tos = (vn->getTokenOffset(fib->intAt(i) + 1) - 1) << 1;
				tlen = ((vn->getTokenLength(fib->intAt(i) + 1) & 0xffff) + 2) << 1;
				// System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml, ba, tos, tlen, vn->encoding, dest_encoding);
				os1 += tlen;
				break;
			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws,ba,1,1, vn->encoding, dest_encoding);
				os1++;
				tos = vn->getTokenOffset( fib->intAt(i));
				tlen = (vn->getTokenLength( fib->intAt(i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml, ba, tos, tlen,vn->encoding, dest_encoding);
				os1 += tlen;
				// 	 write a =
				//System.arraycopy(ws, 3, ba, os1, 1);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					ws,ba,3,1, vn->encoding, dest_encoding);
				os1++;
				tos = vn->getTokenOffset(fib->intAt(i) + 1) - 1;
				tlen = (vn->getTokenLength( fib->intAt(i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				outPosition = Transcoder_transcodeAndFill2(outPosition,
					xml, ba, tos, tlen, vn->encoding, dest_encoding);
				os1 += tlen;
			}
		}
		//System.out.println(new String(ba));
		switch (enc) {
		case FORMAT_UTF_16BE:
		case FORMAT_UTF_16LE:
			temp = (stLen + 1) << 1;
			break;
		default:
			temp = stLen + 1;
		}
		//System.arraycopy(xml, os + temp, ba, os1, len - temp);
		outPosition = Transcoder_transcodeAndFill2(outPosition,
			xml, ba, os + temp, len - temp, vn->encoding, dest_encoding);
		//System.out.println(new String(ba));
		return ba;
	}
}
void ElementFragmentNs::writeFragmentToFile(FILE *f){
	int os = (int)l;
	size_t len = (size_t)(l>>32);
	/*int os1 = 0;*/
	encoding_t enc;
	size_t temp;
	int i;
	size_t temp2;
	int tos =0;
	size_t tlen=0;
	UByte *xml = vn->XMLDoc;
	if (stLen==0){
		//System.arraycopy(xml,os,ba,0,len);
		//memcpy(ba,xml,sizeof(UByte),len);
		temp2=fwrite(xml+os,sizeof(UByte),len,f);
		if (temp2<len){
			throw IOException("fwrite incomplete in writeToFile");
		}
		return;
	}
	enc = vn->getEncoding();
	temp = 0;

	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: temp= (size_t)((stLen+1)<<1); break;
			default:
				temp = (size_t)(stLen+1);
	}            
	//System.arraycopy(xml,os,ba,0,temp);
	//memcpy(ba,xml,sizeof(UByte),temp);
	temp2 = fwrite(xml+os,sizeof(UByte),temp,f); 
	if (temp2<temp){
		throw IOException("fwrite incomplete in writeToFile");
	}
	//namespace compensation
	


	for (i = 0; i < fib->getSize(); i++) {
		//System.out.println("i ==>"+fib.intAt(i));
		switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws,0,ba,os1,2);
				//memcpy(ba+os1,ws,sizeof(UByte),2);
				temp2 = fwrite(ws,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset(fib->intAt(i))<<1;
				tlen= (vn->getTokenLength(fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,sizeof(UByte),tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// write a 0 and =
				// System.arraycopy(ws,2,ba,os1,2);
				//memcpy(ba+os1,ws+2,2);
				temp2 = fwrite(ws+2,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = (vn->getTokenOffset( fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength( fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws,1,ba,os1,2);
				//memcpy(ba+os1,ws+1,2);
				temp2 = fwrite(ws+1,sizeof(UByte),2,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset( fib->intAt(i))<<1;
				tlen= (vn->getTokenLength( fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// 	 write a = and 0
				//System.arraycopy(ws,3,ba,os1,2);
				//memcpy(ba+os1,ws+3,2);
				temp2 = fwrite(ws+3,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = (vn->getTokenOffset(fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength(fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}				
				break;

			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				//memcpy(ba+os1,ws+1,1);
				temp2 = fwrite(ws+1,sizeof(UByte),1,f);
				if (temp2<1){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset( fib->intAt(i));
				tlen = (vn->getTokenLength(fib->intAt(i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// 	 write a = 
				//System.arraycopy(ws, 3, ba, os1, 1);
				//memcpy(ba+os1,ws+3,1);
				temp2 = fwrite(ws+3,sizeof(UByte),1,f);
				if (temp2<1){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset( fib->intAt(i) + 1) - 1 ;
				tlen = (vn->getTokenLength( fib->intAt(i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
		}
	}
	//System.out.println(new String(ba));
	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: 
				temp= (stLen+1)<<1; break;
			default:
				temp = stLen+1;
	}  
	//System.arraycopy(xml, os + temp, ba, os1, len - temp);
	//memcpy(ba+os1,xml+os+temp, len-temp);
	temp2 = fwrite(xml+os+temp,sizeof(UByte),len-temp,f);
	if (temp2<len-temp){
		throw IOException("fwrite incomplete in writeToFile");
	}
}
void ElementFragmentNs::writeFragmentToFile(FILE *f, encoding_t dest_encoding){
	int os = (int)l;
	size_t len = (size_t)(l>>32);
	/*int os1 = 0;*/
	encoding_t enc;
	size_t temp;
	int i;
	size_t temp2;
	int tos =0;
	size_t tlen=0;
	UByte *xml = vn->XMLDoc;
	if (stLen==0){
		//System.arraycopy(xml,os,ba,0,len);
		//memcpy(ba,xml,sizeof(UByte),len);
		temp2=fwrite(xml+os,sizeof(UByte),len,f);
		if (temp2<len){
			throw IOException("fwrite incomplete in writeToFile");
		}
		return;
	}
	enc = vn->getEncoding();
	temp = 0;

	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: temp= (size_t)((stLen+1)<<1); break;
			default:
				temp = (size_t)(stLen+1);
	}            
	//System.arraycopy(xml,os,ba,0,temp);
	//memcpy(ba,xml,sizeof(UByte),temp);
	temp2 = fwrite(xml+os,sizeof(UByte),temp,f); 
	if (temp2<temp){
		throw IOException("fwrite incomplete in writeToFile");
	}
	//namespace compensation
	


	for (i = 0; i < fib->getSize(); i++) {
		//System.out.println("i ==>"+fib.intAt(i));
		switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws,0,ba,os1,2);
				//memcpy(ba+os1,ws,sizeof(UByte),2);
				temp2 = fwrite(ws,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset(fib->intAt(i))<<1;
				tlen= (vn->getTokenLength(fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,sizeof(UByte),tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// write a 0 and =
				// System.arraycopy(ws,2,ba,os1,2);
				//memcpy(ba+os1,ws+2,2);
				temp2 = fwrite(ws+2,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = (vn->getTokenOffset(fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength(fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws,1,ba,os1,2);
				//memcpy(ba+os1,ws+1,2);
				temp2 = fwrite(ws+1,sizeof(UByte),2,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset(fib->intAt(i))<<1;
				tlen= (vn->getTokenLength(fib->intAt(i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// 	 write a = and 0
				//System.arraycopy(ws,3,ba,os1,2);
				//memcpy(ba+os1,ws+3,2);
				temp2 = fwrite(ws+3,sizeof(UByte),2,f);
				if (temp2<2){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = (vn->getTokenOffset(fib->intAt(i)+1)-1)<<1;
				tlen= ((vn->getTokenLength(fib->intAt(i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}				
				break;

			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				//memcpy(ba+os1,ws+1,1);
				temp2 = fwrite(ws+1,sizeof(UByte),1,f);
				if (temp2<1){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset(fib->intAt(i));
				tlen = (vn->getTokenLength(fib->intAt(i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				// 	 write a = 
				//System.arraycopy(ws, 3, ba, os1, 1);
				//memcpy(ba+os1,ws+3,1);
				temp2 = fwrite(ws+3,sizeof(UByte),1,f);
				if (temp2<1){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
				tos = vn->getTokenOffset(fib->intAt(i) + 1) - 1 ;
				tlen = (vn->getTokenLength(fib->intAt(i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throw IOException("fwrite incomplete in writeToFile");
				}
				
		}
	}
	//System.out.println(new String(ba));
	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: 
				temp= (stLen+1)<<1; break;
			default:
				temp = stLen+1;
	}  
	//System.arraycopy(xml, os + temp, ba, os1, len - temp);
	//memcpy(ba+os1,xml+os+temp, len-temp);
	temp2 = fwrite(xml+os+temp,sizeof(UByte),len-temp,f);
	if (temp2<len-temp){
		throw IOException("fwrite incomplete in writeToFile");
	}
}
