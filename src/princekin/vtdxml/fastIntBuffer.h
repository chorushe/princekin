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
#ifndef INTBUFFER_H
#define INTBUFFER_H
#include "arrayList.h"
//#include "elementFragmentNs.h"
//#include "indexHandler.h"
//#include "fastLongBuffer.h"

namespace com_ximpleware {

	class FastIntBuffer {
		friend class IndexHandler;
		friend class VTDNav;
		friend class VTDNav_L5;
		friend class VTDGen;
		friend class IntHash;
		friend class elementFragmentNs;
		friend class TextIter;
		friend class BinaryExpr;
		friend class AutoPilot;
		friend class NodeRecorder;
	private:
		ArrayList *al;
		int capacity;
		int exp;
		int pageSize;
		int r;
		int size;

	public:
		// Create FastIntBuffer with initial page size of 1024 ints
		FastIntBuffer();
		// Create FastIntBuffer with initial page size of (1<<e) ints
		FastIntBuffer(int exp);
		virtual ~FastIntBuffer();

		// Create FastIntBuffer with initial page size of (1<<e) ints
		void append(int i);
		// Append int array of length "len" to the end of fastIntBuffer
		void append(int *i, int len);
		// Get the capacity of FastIntBuffer
		int getCapacity(){return capacity;}
		// Get the page size of FastIntBuffer
		int getPageSize(){return pageSize;}
		// Get the int array corresponding to content of FastIntBuffer 
		// with the starting offset and len
		int *getIntArray(int offset, int len);
		// Get the int at the index position of FastIntBuffer
		//inline int intAt(int index);

		inline int intAt(int index);
		// Replace the value at the index position of FastIntBuffer 
		// with newVal
		void modifyEntry(int index,int newVal);
		// convert the content of FastIntBuffer to int *
		int* toIntArray();
		// set the buffer size to zero, capacity untouched,
		void clear() { size = 0;}
		// getSize
		int getSize(){return size;}
		// reset the size of fastIntBuffer
		bool resize(int newSz);
		
	};
	inline void FastIntBuffer::modifyEntry(int index,int newVal){
		if (index < 0 || index > size - 1) {
			throw InvalidArgumentException("invalid index range in FastIntBuffer's modifyEntry()");
		}
		((int *) al->get(index>>exp))[index & r] = newVal;
	}
	// Get the int at the index position of FastIntBuffer
	inline int FastIntBuffer::intAt(int index){
	if (index < 0 || index > size - 1) {
		throw InvalidArgumentException("invalid index range in FastIntBuffer's intAt()");
    }
	return ((int *) al->get(index>>exp))[index & r];
}
};

#endif 