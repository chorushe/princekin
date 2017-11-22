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
#ifndef LONGBUFFER_H
#define LONGBUFFER_H
#include "arrayList.h"
#include "indexHandler.h"
namespace com_ximpleware {
        class VTDNav;
        class VTDGen;
        class IndexHandler;
	    class FastLongBuffer{
		friend class IndexHandler;
		friend class VTDNav;
		friend class VTDNav_L5;
		friend class VTDGen;
		friend class TextIter;
		friend class XMLModifier;

	private:
		ArrayList *al;
		int capacity;
		int exp;
		int pageSize;
		int r;
		int size;


	public:

		// initialize FastLongBuffer with default page size of 1024 longs 
		FastLongBuffer();
		// initialize FastLongBuffer with page size of (1<<e) longs
		FastLongBuffer(int e);

		// initialize FastLongBuffer with page size of (1<<e) longs and initial capciaty of c longs
		FastLongBuffer(int e, int c);
		virtual ~FastLongBuffer();
		// append a long array to the end of FastLongBuffer
		void append(Long* longArray, int len);
		// append a long to the end of FastLongBuffer
		void append(Long);
		// get the capacity of FastLongBuffer
		int getCapacity(){return capacity;}
		// Return a selected chuck of long buffer as a long array.
		Long* getLongArray(int offset, int len);
		// get the page size of FastLongBuffer
		int getPageSize(){return pageSize;}
		// get the # of entries in FastLongBuffer
		int getSize(){return size;}
		// get the long at the index position from FastLongBuffer
		Long longAt(int index);
		// get the lower 32 bits from the index position from FastLongBuffer
		int lower32At(int index);
		// get the upper 32 bits from the index position from FastLongBuffer 
		int upper32At(int index);
		// replace the entry at the index position of FastLongBuffer with l
		void modifyEntry(int index, Long l);
		// convert FastLongBuffer into a Long array 
		Long* toLongArray();
		// set the buffer size to zero, capacity untouched,
		void clear() { size = 0;}

		bool resize(int newSz){
			if (newSz <= capacity && newSz >=0){	
				size = newSz;
				return true; 
			}
			else	
				return false;  
		}
	};

	inline	Long FastLongBuffer::longAt(int index){
		int pageNum,offset;
		if (index < 0 || index > size - 1) {
			throw InvalidArgumentException("invalid index range");
		}
		pageNum = (index >> exp);
		offset = index & r;
		return ((Long *)al->get(pageNum))[offset];
	}

	inline int FastLongBuffer::lower32At(int index){
		int pageNum,offset;
		if (index < 0 || index > size) {
			throw InvalidArgumentException(" invalid index range");
		}
		pageNum =  (index >> exp);
		offset = index & r;
		return (int)((Long *)al->get(pageNum))[offset];
	}

	inline int FastLongBuffer::upper32At(int index){
		int pageNum, offset;
		if (index < 0 || index > size) {
			throw InvalidArgumentException(" invalid index range");
		}
		pageNum = (index >> exp);
		offset = index & r;
		return (int) ((((Long *)al->get(pageNum))[offset] & (((Long)0xffffffffL)<<32))>>32);
	}

	inline void FastLongBuffer::modifyEntry(int index, Long l){
		if (index < 0 || index > size) {
			throw InvalidArgumentException("invalid index range");
		}
		((Long *)al->get(index>>exp))[index & r] = l;
	}
}

#endif
