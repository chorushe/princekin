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
#include "indexHandler.h"
#include "fastIntBuffer.h"
#include "fastLongBuffer.h"
#include "vtdGen.h"

using namespace com_ximpleware;

Long IndexHandler::reverseLong(Long l){
	Long t = (((l & -0x0100000000000000LL) >> 56) & 0xffLL)
		| ((l & 0xff000000000000LL) >> 40) 
		| ((l & 0xff0000000000LL) >> 24) 
		| ((l & 0xff00000000LL) >> 8) 
		| ((l & 0xff000000LL) << 8) 
		| ((l & 0xff0000LL) << 24) 
		| ((l & 0xff00LL) << 40) 
		| ((l & 0xffLL) << 56);
	return t;
}

int IndexHandler::reverseInt(int i){
	int t = (((i & -0x01000000) >> 24) & 0xff) 
		| ((i & 0xff0000) >> 8) 
		| ((i & 0xff00) << 8) 
		| ((i & 0xff) << 24);
	return t;
}

bool IndexHandler::isLittleEndian(){
	int a = 0xaabbccdd;
	if ( ((Byte *)(&a))[0] == (Byte)0xdd)
		return true;
	return false;
}
/*writeIndex writes VTD+XML into a file
  This function throws index_write_exception*/
bool IndexHandler::_writeIndex_L3(Byte version, 
				int encodingType, 
				bool ns, 
				bool byteOrder, 
				int nestDepth, 
				int LCLevel, 
				int rootIndex, 
				UByte* xmlDoc, 
				int docOffset, 
				int docLen, 
				FastLongBuffer* vtdBuffer, 
				FastLongBuffer* l1Buffer, 
				FastLongBuffer* l2Buffer, 
				FastIntBuffer* l3Buffer, 
				FILE *f){
					int i;
					Byte ba[4];
					Long l;
					bool littleEndian = isLittleEndian();
			if (xmlDoc == NULL 
				|| docLen <= 0 
				|| vtdBuffer == NULL
				|| l1Buffer == NULL 
				|| l2Buffer == NULL 
				|| l3Buffer == NULL
				|| f == NULL)
			{
				throw InvalidArgumentException("writeIndex's argument invalid");
				return false;	
			}

			if (vtdBuffer->size == 0){
				throw IndexWriteException("vTDBuffer can't be zero in size");
			}
			
			//UPGRADE_TODO: Class 'java.io.DataOutputStream' was converted to 'System.IO.BinaryWriter' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataOutputStream'"
			//System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
			// first 4 bytes
			
			ba[0] = (Byte) version; // version # is 1 
			ba[1] = (Byte) encodingType;
            if (littleEndian == false)
                ba[2] = (Byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (Byte)(ns ? 0xc0 : 0x80);
			ba[3] = (Byte) nestDepth;
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			// second 4 bytes
			ba[0] = 0;
			ba[1] = 4;
			ba[2] = (Byte) ((rootIndex & 0xff00) >> 8);
			ba[3] = (Byte) (rootIndex & 0xff);
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			// 2 reserved 32-bit words set to zero
			ba[1] = ba[2] = ba[3] = 0;
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			// write XML doc in bytes	
			l = docLen;
			if (fwrite((UByte*) (&l), 1 , 8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
            //dos.Write(xmlDoc, docOffset, docLen);
			if (fwrite((UByte*)(xmlDoc+docOffset), 1, docLen,f)!=(size_t)docLen){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			//dos.Write(xmlDoc, docOffset, docLen);
			// zero padding to make it integer multiple of 64 bits
			if ((docLen & 0x07) != 0)
			{
				int t = (((docLen >> 3) + 1) << 3) - docLen;
				for (; t > 0; t--){
					if (fwrite(ba,1,1,f)!=1){
						throw IndexWriteException( "fwrite error occurred");
						return false;
					};
				}
			}
			// write VTD
            
			//dos.Write((long)vtdBuffer.size());
			l = vtdBuffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			for (i = 0; i < vtdBuffer->size; i++)
			{
				l = vtdBuffer->longAt(i);
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
			}
			// write L1 
			//dos.Write((long)l1Buffer.size());
			l = l1Buffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			for (i = 0; i < l1Buffer->size; i++)
			{
				l = l1Buffer->longAt(i);
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
			}
			// write L2
			l = l2Buffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			for (i = 0; i < l2Buffer->size; i++)
			{
				l = l2Buffer->longAt(i);
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
			}
			// write L3
			l = l3Buffer->size;
			if (fwrite((UByte*) &l, 1 , 8,f)!=8){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			for (i = 0; i < l3Buffer->size; i++)
			{
				int s = l3Buffer->intAt(i);
				if (fwrite((UByte*)&s, 1 , 4, f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
			}
			// pad zero if # of l3 entry is odd
			if ((l3Buffer->size & 1) != 0){
				if (fwrite(ba,1,1,f)!=1){
						throw IndexWriteException("fwrite error occurred");
						return false;
					};
			}
			//fclose(f);
			return true;
}
/*readIndex loads VTD+XML into VTDGen
  this function throws index_read_exception*/
bool IndexHandler::_readIndex(FILE *f, VTDGen *vg){
	int intLongSwitch;
	int endian;
	Byte ba[4];
	int LCLevel;
	Long l;
	int size;
	Byte *XMLDoc;
	bool littleEndian = isLittleEndian();
	if (f == NULL || vg == NULL)
	{
		throw InvalidArgumentException("Invalid argument(s) for readIndex()");
		return false;
	}
	
	// first byte
	if (fread(ba,1,1,f) != 1 ){
		throw IndexReadException("fread error occurred");
		return false;
	}
    
	if (ba[0]!=1){
		throw IndexReadException("version # error");
		return false;
	}
	// no check on version number for now
	// second byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}

	vg->encoding = (encoding_t)ba[0];
	
	// third byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}
	if ((ba[0] & 0x80) != 0)
		intLongSwitch = 1;
	//use ints
	else
		intLongSwitch = 0;
	if ((ba[0] & 0x40) != 0)
		vg->ns = true;
	else
		vg->ns = false;
	if ((ba[0] & 0x20) != 0)
		endian = 1;
	else
		endian = 0;

	// fourth byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}
	vg->VTDDepth = ba[0];

	// 5th and 6th byte
	if (fread(ba,1,2,f) != 2){
		throw IndexReadException("fread error occurred");
		return false;
	}
	LCLevel = (((int) ba[0]) << 8) | ba[1];
	if (LCLevel != 4 &&  LCLevel !=6)
	{
		throw IndexReadException("LC levels must be at least 3");
		return false;
	}
	if (LCLevel ==4)    
		vg->shallowDepth = true;
    else
		vg->shallowDepth = false;

	// 7th and 8th byte
	if (fread(ba,1,2,f) != 2){
		throw IndexReadException("fread error occurred");
		return false;
	}
	vg->rootIndex = (((int) ba[0]) << 8) | ba[1];

	// skip a long
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	
	// read XML size
	if ((littleEndian && (endian == 0))
		|| ((littleEndian == false) && (endian == 1)))
		size = (int)l;
	else
		size = (int)reverseLong(l);


	// read XML bytes
	try{
		XMLDoc = new Byte[size];
	}catch(std::bad_alloc&){
		throw OutOfMemException("Byte array allocation failed");
		return false;
	}
	if (fread(XMLDoc,1,size,f)!= (size_t)size){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//dis.Read(XMLDoc,0,size);
	if ((size & 0x7) != 0)
	{
		int t = (((size >> 3) + 1) << 3) - size;
		while (t > 0)
		{
			if (fread(ba,1,1,f) != 1){
				throw IndexReadException("fread error occurred");
				return false;
			}
			t--;
		}
	}

	vg->setDoc((UByte *)XMLDoc,size);

	if ( (littleEndian && (endian == 0))
		|| (littleEndian == false && endian == 1))
	{
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		vtdSize = (int) l;
		while (vtdSize > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->VTDBuffer->append(l);
			vtdSize--;
		}
		// read L1 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l1Size = (int) l;
		while (l1Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l1Buffer->append(l);
			l1Size--;
		}
		// read L2 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l2Size = (int) l;
		while (l2Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l2Buffer->append(l);
			l2Size--;
		}
		// read L3 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l3Size = (int) l;
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l3Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(size);
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append((int)(l >> 32));
					l3Size--;
				}
			}
		}else{
			while (l3Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l3Buffer->append(l);
				l3Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l4Size = (int) l;
			while (l4Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l4Buffer->append(l);
				l4Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l5Size = (int) l;
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(size);
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append((int)(l >> 32));
					l5Size--;
				}
			}
		}
	}
	else
	{
		// read vtd records
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		vtdSize = (int) reverseLong(l);
		while (vtdSize > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->VTDBuffer->append(reverseLong(l));
			vtdSize--;
		}
		// read L1 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l1Size = (int) reverseLong(l);
		while (l1Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l1Buffer->append(reverseLong(l));
			l1Size--;
		}
		// read L2 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l2Size = (int) reverseLong(l);
		while (l2Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l2Buffer->append(reverseLong(l));
			l2Size--;
		}
		// read L3 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l3Size = (int) reverseLong(l);
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l3Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(reverseInt(size));
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(reverseInt((int) (l >> 32)));
					l3Size--;
				}
			}
		}else{
			while (l3Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l3Buffer->append(reverseLong(l));
				l3Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l4Size = (int) reverseLong(l);
			while (l4Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l4Buffer->append(reverseLong(l));
				l4Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l5Size = (int)reverseLong(l);
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(reverseInt(size));
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(reverseInt((int)(l >> 32)));
					l5Size--;
				}
			}
		}
	}
	//fclose(f);
	return true;
	
}

bool IndexHandler::_readIndex(UByte *ba, int len, VTDGen *vg){
	int intLongSwitch;
	int endian;
	int LCLevel;
	Long l;
	int size,adj;
	int count;
	int t=0;
	bool littleEndian = isLittleEndian();
	if (ba == NULL || vg == NULL)
	{
		throw InvalidArgumentException("Invalid argument(s) for readIndex()");
		return false;
	}
	
	// first byte
	if (len < 32){
		throw IndexReadException("Invalid Index error");
		return false;
	}	

	vg->encoding = (encoding_t)ba[1];
	adj = OFFSET_ADJUSTMENT;
	if (vg->encoding >= FORMAT_UTF_16BE){
		adj = OFFSET_ADJUSTMENT >> 1;
	}
	
	if ((ba[2] & 0x80) != 0)
		intLongSwitch = 1;
	//use ints
	else
		intLongSwitch = 0;
	if ((ba[2] & 0x40) != 0)
		vg->ns = true;
	else
		vg->ns = false;
	if ((ba[2] & 0x20) != 0)
		endian = 1;
	else
		endian = 0;

	// fourth byte
	
	vg->VTDDepth = ba[3];

	// 5th and 6th byte
	
	LCLevel = (((int) ba[4]) << 8) | ba[5];
	if (LCLevel != 4 &&  LCLevel !=6)
	{
		throw IndexReadException("LC levels must be at least 3");
		return false;
	}
	if (LCLevel ==4)    
		vg->shallowDepth = true;
    else
		vg->shallowDepth = false;
	// 7th and 8th byte
	vg->rootIndex = (((int) ba[6]) << 8) | ba[7];

	//Console.WriteLine(" l ==>" + l);
	l=((Long*)(ba+24))[0];
	//Console.WriteLine(" l ==>" + l);
	
	// read XML size
	if ((littleEndian && (endian == 0))
		|| ((littleEndian == false) && (endian == 1)))
		size = (int)l;
	else
		size = (int)reverseLong(l);

	if (len < 32+size){
		throw IndexReadException("Invalid Index error");
		return false;
	}
	vg->setDoc(ba,len,32,size);
	//dis.Read(XMLDoc,0,size);
	if ((size & 0x7) != 0)
	{
		t = (((size >> 3) + 1) << 3) - size;
	}

	count = 32 + size +t;
	if ( (littleEndian && (endian == 0))
		|| (littleEndian == false && endian == 1))
	{
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		vtdSize = (int) l;
		while (vtdSize > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->VTDBuffer->append(adjust(l,adj));
			vtdSize--;
		}
		// read L1 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l1Size = (int) l;
		while (l1Size > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->l1Buffer->append(l);
			l1Size--;
		}
		// read L2 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l2Size = (int) l;
		while (l2Size > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->l2Buffer->append(l);
			l2Size--;
		}
		// read L3 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l3Size = (int) l;
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l3Size > 0)
				{
					int i;
					if (len < count+4){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					i = ((int*)(ba+count))[0];
					count+=4;
					vg->l3Buffer->append(i);
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (len < count+8){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					l = ((Long*)(ba+count))[0];
					count+=8;
					vg->l3Buffer->append((int)(l >> 32));
					l3Size--;
				}
			}
		}else{
			while (l3Size > 0)
			{
				if (len < count+8){
					throw IndexReadException("Invalid Index error");
					return false;
				}
				l = ((Long*)(ba+count))[0];
				count+=8;
				vg->_l3Buffer->append(l);
				l3Size--;
			}
			// read L4 LC records
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			int l4Size = (int) l;
			while (l4Size > 0)
			{
				if (len < count+8){
					throw IndexReadException("Invalid Index error");
					return false;
				}
				l = ((Long*)(ba+count))[0];
				count+=8;
				vg->_l4Buffer->append(l);
				l4Size--;
			}

			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			int l5Size = (int) l;

			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					int i;
					if (len < count+4){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					i = ((int*)(ba+count))[0];
					count+=4;
					vg->_l5Buffer->append(i);
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (len < count+8){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					l = ((Long*)(ba+count))[0];
					count+=8;
					vg->_l5Buffer->append((int)(l >> 32));
					l5Size--;
				}
			}
		}
	}
	else
	{
		// read vtd records
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		vtdSize = (int) reverseLong(l);
		while (vtdSize > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->VTDBuffer->append(adjust(reverseLong(l),adj));
			vtdSize--;
		}
		// read L1 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l1Size = (int) reverseLong(l);
		while (l1Size > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->l1Buffer->append(reverseLong(l));
			l1Size--;
		}
		// read L2 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l2Size = (int) reverseLong(l);
		while (l2Size > 0)
		{
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			vg->l2Buffer->append(reverseLong(l));
			l2Size--;
		}
		// read L3 LC records
		if (len < count+8){
			throw IndexReadException("Invalid Index error");
			return false;
		}
		l = ((Long*)(ba+count))[0];
		count+=8;
		l3Size = (int) reverseLong(l);
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				int i;
				while (l3Size > 0)
				{
					if (len < count+4){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					i = ((int*)(ba+count))[0];
					count+=4;
					vg->l3Buffer->append(reverseInt(i));
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (len < count+8){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					l = ((Long*)(ba+count))[0];
					count+=8;
					vg->l3Buffer->append(reverseInt((int) (l >> 32)));
					l3Size--;
				}
			}
		}else {
			while (l3Size > 0)
			{
				if (len < count+8){
					throw IndexReadException("Invalid Index error");
					return false;
				}
				l = ((Long*)(ba+count))[0];
				count+=8;
				vg->_l3Buffer->append(reverseLong(l));
				l3Size--;
			}
			// read L4 LC records
			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			int l4Size = (int) l;
			while (l4Size > 0)
			{
				if (len < count+8){
					throw IndexReadException("Invalid Index error");
					return false;
				}
				l = ((Long*)(ba+count))[0];
				count+=8;
				vg->_l4Buffer->append(reverseLong(l));
				l4Size--;
			}

			if (len < count+8){
				throw IndexReadException("Invalid Index error");
				return false;
			}
			l = ((Long*)(ba+count))[0];
			count+=8;
			int l5Size = (int) l;

			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					int i;
					if (len < count+4){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					i = ((int*)(ba+count))[0];
					count+=4;
					vg->_l5Buffer->append(reverseInt(i));
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (len < count+8){
						throw IndexReadException("Invalid Index error");
						return false;
					}
					l = ((Long*)(ba+count))[0];
					count+=8;
					vg->_l5Buffer->append((int)reverseLong(l >> 32));
					l5Size--;
				}
			}
		}
	}
	//fclose(f);
	return true;
}
Long IndexHandler::adjust(Long l,int i){
       Long l1 = (l & 0xffffffffLL)+ i;
       Long l2 = l & 0xffffffff00000000LL;
       return l1|l2;   
}

bool IndexHandler::_writeSeparateIndex_L3(Byte version, 
				int encodingType, 
				bool ns, 
				bool byteOrder, 
				int nestDepth, 
				int LCLevel, 
				int rootIndex, 
				//UByte* xmlDoc, 
				int docOffset, 
				int docLen, 
				FastLongBuffer *vtdBuffer, 
				FastLongBuffer *l1Buffer, 
				FastLongBuffer *l2Buffer, 
				FastIntBuffer *l3Buffer, 
				FILE *f){
					int i;
					Byte ba[4];
					Long l;
					bool littleEndian = isLittleEndian();
			if (docLen <= 0 
				|| vtdBuffer == NULL
				|| l1Buffer == NULL 
				|| l2Buffer == NULL 
				|| l3Buffer == NULL
				|| f == NULL)
			{
				throw InvalidArgumentException("writeSeparateIndex's argument invalid");
				return false;	
			}

			if (vtdBuffer->size == 0){
				throw IndexWriteException("vTDBuffer can't be zero in size");
			}
			
			//UPGRADE_TODO: Class 'java.io.DataOutputStream' was converted to 'System.IO.BinaryWriter' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataOutputStream'"
			//System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
			// first 4 bytes
			
			ba[0] = (Byte) version; // version # is 2 
			ba[1] = (Byte) encodingType;
            if (littleEndian == false)
                ba[2] = (Byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (Byte)(ns ? 0xc0 : 0x80);
			ba[3] = (Byte) nestDepth;
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			// second 4 bytes
			ba[0] = 0;
			ba[1] = 4;
			ba[2] = (Byte) ((rootIndex & 0xff00) >> 8);
			ba[3] = (Byte) (rootIndex & 0xff);
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			// 2 reserved 32-bit words set to zero
			ba[1] = ba[2] = ba[3] = 0;
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			// write XML doc in bytes	
			l = docLen;
			if (fwrite((UByte*) (&l), 1 , 8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}

			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			if (fwrite(ba,1,4,f)!=4){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}

            //dos.Write(xmlDoc, docOffset, docLen);
			/*if (fwrite((UByte*)(xmlDoc+docOffset), 1, docLen,f)!=docLen){
				throwException2(index_write_exception, "fwrite error occurred");
				return false;
			}*/
			//dos.Write(xmlDoc, docOffset, docLen);
			// zero padding to make it integer multiple of 64 bits
			/*if ((docLen & 0x07) != 0)
			{
				int t = (((docLen >> 3) + 1) << 3) - docLen;
				for (; t > 0; t--){
					if (fwrite(ba,1,1,f)!=1){
						throwException2(index_write_exception, "fwrite error occurred");
						return false;
					};
				}
			}*/
			// write VTD
            
			//dos.Write((long)vtdBuffer.size());
			l = vtdBuffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			for (i = 0; i < vtdBuffer->size; i++)
			{
				l = vtdBuffer->longAt(i);
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
			}
			// write L1 
			//dos.Write((long)l1Buffer.size());
			l = l1Buffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			for (i = 0; i < l1Buffer->size; i++)
			{
				l = l1Buffer->longAt(i);
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
			}
			// write L2
			l = l2Buffer->size;
			if (fwrite((UByte*) &l, 1 ,8,f)!=8){
				throw IndexWriteException( "fwrite error occurred");
				return false;
			}
			for (i = 0; i < l2Buffer->size; i++)
			{
				l = l2Buffer->longAt(i);
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
			}
			// write L3
			l = l3Buffer->size;
			if (fwrite((UByte*) &l, 1 , 8,f)!=8){
				throw IndexWriteException("fwrite error occurred");
				return false;
			}
			for (i = 0; i < l3Buffer->size; i++)
			{
				int s = l3Buffer->intAt(i);
				if (fwrite((UByte*)&s, 1 , 4, f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
			}
			// pad zero if # of l3 entry is odd
			if ((l3Buffer->size & 1) != 0){
				if (fwrite(ba,1,1,f)!=1){
						throw IndexWriteException("fwrite error occurred");
						return false;
					};
			}
			//fclose(f);
			return true;

}

bool IndexHandler::_readSeparateIndex(FILE *xml, int XMLSize, FILE *f, VTDGen *vg){
	int intLongSwitch;
	int endian;
	Byte ba[4];
	int LCLevel;
	Long l;
	int size;
	Byte *XMLDoc;
	bool littleEndian = isLittleEndian();
	if (f == NULL || vg == NULL)
	{
		throw InvalidArgumentException("Invalid argument(s) for readIndex()");
		return false;
	}
	
	// first byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}
	if (ba[0]!=2){
		throw IndexReadException("version # error");
		return false;
	}

	// no check on version number for now
	// second byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}

	vg->encoding =(encoding_t) ba[0];
	
	// third byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}
	if ((ba[0] & 0x80) != 0)
		intLongSwitch = 1;
	//use ints
	else
		intLongSwitch = 0;
	if ((ba[0] & 0x40) != 0)
		vg->ns = true;
	else
		vg->ns = false;
	if ((ba[0] & 0x20) != 0)
		endian = 1;
	else
		endian = 0;

	// fourth byte
	if (fread(ba,1,1,f) != 1){
		throw IndexReadException("fread error occurred");
		return false;
	}
	vg->VTDDepth = ba[0];

	// 5th and 6th byte
	if (fread(ba,1,2,f) != 2){
		throw IndexReadException("fread error occurred");
		return false;
	}
	LCLevel = (((int) ba[0]) << 8) | ba[1];
	if (LCLevel != 4 &&  LCLevel !=6)
	{
		throw IndexReadException("LC levels must be at least 3");
		return false;
	}
	if (LCLevel ==4)    
		vg->shallowDepth = true;
    else
		vg->shallowDepth = false;
	// 7th and 8th byte
	if (fread(ba,1,2,f) != 2){
		throw IndexReadException("fread error occurred");
		return false;
	}
	vg->rootIndex = (((int) ba[0]) << 8) | ba[1];

	// skip a long
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	// read XML size
	if ((littleEndian && (endian == 0))
		|| ((littleEndian == false) && (endian == 1)))
		size = (int)l;
	else
		size = (int)reverseLong(l);
	
	if (size!= XMLSize){
		throw IndexReadException( "XML size mismatch");
	}

	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//Console.WriteLine(" l ==>" + l);
	if (fread(&l,1,8,f) != 8){
		throw IndexReadException("fread error occurred");
		return false;
	}

	// read XML bytes the assumption is that there is no offset shift
	// in XML bytes
	XMLDoc = new Byte[size];
	/*if (XMLDoc == NULL){
		throwException2(out_of_mem,"Byte array allocation failed");
		return false;
	}*/
	if (fread(XMLDoc,1,size,xml)!= (size_t)size){
		throw IndexReadException("fread error occurred");
		return false;
	}
	//dis.Read(XMLDoc,0,size);
	/*if ((size & 0x7) != 0)
	{
		int t = (((size >> 3) + 1) << 3) - size;
		while (t > 0)
		{
			if (fread(ba,1,1,xml) != 1){
				throwException2(index_read_exception,"fread error occurred");
				return false;
			}
			t--;
		}
	}*/

	vg->setDoc((UByte*)XMLDoc,size);

	if ( (littleEndian && (endian == 0))
		|| (littleEndian == false && endian == 1))
	{
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		l=0;
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		vtdSize = (int) l;
		while (vtdSize > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->VTDBuffer->append(l);
			vtdSize--;
		}
		// read L1 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l1Size = (int) l;
		while (l1Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l1Buffer->append(l);
			l1Size--;
		}
		// read L2 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l2Size = (int) l;
		while (l2Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l2Buffer->append( l);
			l2Size--;
		}
		// read L3 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l3Size = (int) l;
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l3Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(size);
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append((int)(l >> 32));
					l3Size--;
				}
			}
		}else{
			while (l3Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l3Buffer->append(l);
				l3Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l4Size = (int) l;
			while (l4Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l4Buffer->append(l);
				l4Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l5Size = (int) l;
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(size);
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append((int)(l >> 32));
					l5Size--;
				}
			}
		}
	}
	else
	{
		// read vtd records
		int vtdSize,l1Size,l2Size,l3Size;
		// read vtd records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		vtdSize = (int) reverseLong(l);
		while (vtdSize > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->VTDBuffer->append(reverseLong(l));
			vtdSize--;
		}
		// read L1 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l1Size = (int) reverseLong(l);
		while (l1Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l1Buffer->append(reverseLong(l));
			l1Size--;
		}
		// read L2 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l2Size = (int) reverseLong(l);
		while (l2Size > 0)
		{
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			vg->l2Buffer->append( reverseLong(l));
			l2Size--;
		}
		// read L3 LC records
		if (fread(&l,1,8,f) != 8){
			throw IndexReadException("fread error occurred");
			return false;
		}
		l3Size = (int) reverseLong(l);
		if (vg->shallowDepth){
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l3Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(reverseInt(size));
					l3Size--;
				}
			}
			else
			{
				while (l3Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->l3Buffer->append(reverseInt((int) (l >> 32)));
					l3Size--;
				}
			}
		}else{
			while (l3Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l3Buffer->append(reverseLong(l));
				l3Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l4Size = (int) reverseLong(l);
			while (l4Size > 0)
			{
				if (fread(&l,1,8,f) != 8){
					throw IndexReadException("fread error occurred");
					return false;
				}
				vg->_l4Buffer->append(reverseLong(l));
				l4Size--;
			}
			if (fread(&l,1,8,f) != 8){
				throw IndexReadException("fread error occurred");
				return false;
			}
			int l5Size = (int)reverseLong(l);
			if (intLongSwitch == 1)
			{
				//l3 uses ints
				while (l5Size > 0)
				{
					if (fread(&size,1,4,f) != 4){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(reverseInt(size));
					l5Size--;
				}
			}
			else
			{
				while (l5Size > 0)
				{
					if (fread(&l,1,8,f) != 8){
						throw IndexReadException("fread error occurred");
						return false;
					}
					vg->_l5Buffer->append(reverseInt((int)(l >> 32)));
					l5Size--;
				}
			}
		}
	}
	//fclose(f);
	return true;
	
}

bool IndexHandler::_writeSeparateIndex_L5(Byte version, 
			int encodingType, 
			bool ns, 
			bool byteOrder, 
			int nestDepth, 
			int LCLevel, 
			int rootIndex, 
			//UByte* xmlDoc, 
			int docOffset, 
			int docLen, 
			FastLongBuffer *vtdBuffer, 
			FastLongBuffer *l1Buffer, 
			FastLongBuffer *l2Buffer, 
			FastLongBuffer *l3Buffer, 
			FastLongBuffer *l4Buffer, 
			FastIntBuffer *l5Buffer,
			FILE *f){
				int i;
				Byte ba[4];
				Long l;
				bool littleEndian = isLittleEndian();
				if (docLen <= 0 
					|| vtdBuffer == NULL
					|| l1Buffer == NULL 
					|| l2Buffer == NULL 
					|| l3Buffer == NULL
					|| f == NULL)
				{
					throw InvalidArgumentException("writeSeparateIndex's argument invalid");
					return false;	
				}

				if (vtdBuffer->size == 0){
					throw IndexWriteException("vTDBuffer can't be zero in size");
				}

				//UPGRADE_TODO: Class 'java.io.DataOutputStream' was converted to 'System.IO.BinaryWriter' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataOutputStream'"
				//System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
				// first 4 bytes

				ba[0] = (Byte) version; // version # is 2 
				ba[1] = (Byte) encodingType;
				if (littleEndian == false)
					ba[2] = (Byte)(ns ? 0xe0 : 0xa0); // big endien
				else
					ba[2] = (Byte)(ns ? 0xc0 : 0x80);
				ba[3] = (Byte) nestDepth;
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				// second 4 bytes
				ba[0] = 0;
				ba[1] = 4;
				ba[2] = (Byte) ((rootIndex & 0xff00) >> 8);
				ba[3] = (Byte) (rootIndex & 0xff);
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				// 2 reserved 32-bit words set to zero
				ba[1] = ba[2] = ba[3] = 0;
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				// write XML doc in bytes	
				l = docLen;
				if (fwrite((UByte*) (&l), 1 , 8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}

				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}

				//dos.Write(xmlDoc, docOffset, docLen);
				/*if (fwrite((UByte*)(xmlDoc+docOffset), 1, docLen,f)!=docLen){
				throwException2(index_write_exception, "fwrite error occurred");
				return false;
				}*/
				//dos.Write(xmlDoc, docOffset, docLen);
				// zero padding to make it integer multiple of 64 bits
				/*if ((docLen & 0x07) != 0)
				{
				int t = (((docLen >> 3) + 1) << 3) - docLen;
				for (; t > 0; t--){
				if (fwrite(ba,1,1,f)!=1){
				throwException2(index_write_exception, "fwrite error occurred");
				return false;
				};
				}
				}*/
				// write VTD

				//dos.Write((long)vtdBuffer.size());
				l = vtdBuffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				for (i = 0; i < vtdBuffer->size; i++)
				{
					l = vtdBuffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException( "fwrite error occurred");
						return false;
					}
				}
				// write L1 
				//dos.Write((long)l1Buffer.size());
				l = l1Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l1Buffer->size; i++)
				{
					l = l1Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 ,8,f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				// write L2
				l = l2Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l2Buffer->size; i++)
				{
					l = l2Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				// write L3
				l = l3Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l3Buffer->size; i++)
				{
					l = l3Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}

				// write L4
				l = l4Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l4Buffer->size; i++)
				{
					l = l4Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}

				// write L5
				l = l5Buffer->size;
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				for (i = 0; i < l5Buffer->size; i++)
				{
					int s = l5Buffer->intAt(i);
					if (fwrite((UByte*)&s, 1 , 4, f)!=4){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				// pad zero if # of l3 entry is odd
				if ((l5Buffer->size & 1) != 0){
					if (fwrite(ba,1,1,f)!=1){
						throw IndexWriteException("fwrite error occurred");
						return false;
					};
				}
				//fclose(f);
				return true;
}

bool IndexHandler::_writeIndex_L5(Byte version, 
			int encodingType, 
			bool ns, 
			bool byteOrder, 
			int nestDepth, 
			int LCLevel, 
			int rootIndex, 
			UByte* xmlDoc, 
			int docOffset, 
			int docLen, 
			FastLongBuffer *vtdBuffer, 
			FastLongBuffer *l1Buffer, 
			FastLongBuffer *l2Buffer, 
			FastLongBuffer *l3Buffer, 
			FastLongBuffer *l4Buffer, 
			FastIntBuffer *l5Buffer,
			FILE *f){
							
				int i;
				Byte ba[4];
				Long l;
				bool littleEndian = isLittleEndian();
				if (xmlDoc == NULL 
					|| docLen <= 0 
					|| vtdBuffer == NULL
					|| l1Buffer == NULL 
					|| l2Buffer == NULL 
					|| l3Buffer == NULL
					|| l4Buffer == NULL
					|| l5Buffer == NULL
					|| LCLevel != 5
					|| f == NULL)
				{
					throw InvalidArgumentException("writeIndex's argument invalid");
					return false;	
				}

				if (vtdBuffer->size == 0){
					throw IndexWriteException("vTDBuffer can't be zero in size");
				}

				//UPGRADE_TODO: Class 'java.io.DataOutputStream' was converted to 'System.IO.BinaryWriter' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataOutputStream'"
				//System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
				// first 4 bytes

				ba[0] = (Byte) version; // version # is 1 
				ba[1] = (Byte) encodingType;
				if (littleEndian == false)
					ba[2] = (Byte)(ns ? 0xe0 : 0xa0); // big endien
				else
					ba[2] = (Byte)(ns ? 0xc0 : 0x80);
				ba[3] = (Byte) nestDepth;
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				// second 4 bytes
				ba[0] = 0;
				ba[1] = 6;
				ba[2] = (Byte) ((rootIndex & 0xff00) >> 8);
				ba[3] = (Byte) (rootIndex & 0xff);
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				// 2 reserved 32-bit words set to zero
				ba[1] = ba[2] = ba[3] = 0;
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				if (fwrite(ba,1,4,f)!=4){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				// write XML doc in bytes	
				l = docLen;
				if (fwrite((UByte*) (&l), 1 , 8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				//dos.Write(xmlDoc, docOffset, docLen);
				if (fwrite((UByte*)(xmlDoc+docOffset), 1, docLen,f)!=(size_t)docLen){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				//dos.Write(xmlDoc, docOffset, docLen);
				// zero padding to make it integer multiple of 64 bits
				if ((docLen & 0x07) != 0)
				{
					int t = (((docLen >> 3) + 1) << 3) - docLen;
					for (; t > 0; t--){
						if (fwrite(ba,1,1,f)!=1){
							throw IndexWriteException( "fwrite error occurred");
							return false;
						};
					}
				}
				// write VTD

				//dos.Write((long)vtdBuffer.size());
				l = vtdBuffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				for (i = 0; i < vtdBuffer->size; i++)
				{
					l = vtdBuffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException( "fwrite error occurred");
						return false;
					}
				}
				// write L1 
				//dos.Write((long)l1Buffer.size());
				l = l1Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				for (i = 0; i < l1Buffer->size; i++)
				{
					l = l1Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 ,8,f)!=8){
						throw IndexWriteException( "fwrite error occurred");
						return false;
					}
				}
				// write L2
				l = l2Buffer->size;
				if (fwrite((UByte*) &l, 1 ,8,f)!=8){
					throw IndexWriteException("fwrite error occurred");
					return false;
				}
				for (i = 0; i < l2Buffer->size; i++)
				{
					l = l2Buffer->longAt(i);
					if (fwrite((UByte*) &l, 1 , 8,f)!=8){
						throw IndexWriteException( "fwrite error occurred");
						return false;
					}
				}
				// write L3
				l = l3Buffer->size;
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l3Buffer->size; i++)
				{
					Long s = l3Buffer->longAt(i);
					if (fwrite((UByte*)&s, 1 , 8, f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				// write L4
				l = l4Buffer->size;
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l4Buffer->size; i++)
				{
					Long s = l4Buffer->longAt(i);
					if (fwrite((UByte*)&s, 1 , 8, f)!=8){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				
				// write L5
				l = l5Buffer->size;
				if (fwrite((UByte*) &l, 1 , 8,f)!=8){
					throw IndexWriteException( "fwrite error occurred");
					return false;
				}
				for (i = 0; i < l5Buffer->size; i++)
				{
					int s = l5Buffer->intAt(i);
					if (fwrite((UByte*)&s, 1 , 4, f)!=4){
						throw IndexWriteException("fwrite error occurred");
						return false;
					}
				}
				// pad zero if # of l5 entry is odd
				if ((l5Buffer->size & 1) != 0){
					if (fwrite(ba,1,1,f)!=1){
						throw IndexWriteException("fwrite error occurred");
						return false;
					};
				}
				//fclose(f);
				return true;
}