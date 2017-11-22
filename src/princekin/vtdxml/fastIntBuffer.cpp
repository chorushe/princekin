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

#include "fastIntBuffer.h"
using namespace com_ximpleware;

// Create FastIntBuffer with initial page size of 1024 ints
FastIntBuffer::FastIntBuffer() try:
al(new ArrayList()),
capacity(0),
exp(10),
pageSize(1<<10),
r((1<<10) -1),
size(0)
{
}
catch(std::bad_alloc&){
		throw OutOfMemException("array list allocation in FastIntBuffer's constructor failed");
}
// Create FastIntBuffer with initial page size of (1<<e) ints
FastIntBuffer::FastIntBuffer(int exp1) try:
al(new ArrayList()),
capacity(0),
exp(exp1),
pageSize(1<<exp1),
r((1<<exp1) -1),
size(0)
{

}
catch(std::bad_alloc&){
	throw OutOfMemException("array list allocation in FastIntBuffer's constructor failed");
}

FastIntBuffer::~FastIntBuffer(){
	delete al;
	al = NULL;
}

// Create FastIntBuffer with initial page size of (1<<e) ints
void FastIntBuffer::append(int i){
	//int lastBufferIndex;
	//int* lastBuffer = NULL;
	//int al_size = al->size;
    /*if (al_size == 0) {
		lastBuffer = new int[1<<exp];
        al->add((void*)lastBuffer);
        capacity = pageSize;
    } else {
		lastBufferIndex = min((size>>exp),al_size-1);
		lastBuffer = (int *)al->get(lastBufferIndex);
    }*/
	if (size < capacity) {
		((int *)al->get(size>>exp))[size & r] = i; 
		//lastBuffer[size & r] = i;
		size += 1;
	} else
	{
		//int *newBuffer = new int[1<<exp];
		int *newBuffer = NULL;
		try{
			newBuffer = new int[1<<exp];
		}catch(std::bad_alloc&){
			throw OutOfMemException("array list allocation in FastIntBuffer's append failed");
		}
		
		size++;
		capacity += pageSize;
		al->add((void *)newBuffer);
		newBuffer[0] = i;		
	}
}
// Append int array of length "len" to the end of fastIntBuffer
void FastIntBuffer::append(int *int_array, int len){
	int lastBufferIndex;
	int* lastBuffer=NULL;
	int al_size = al->size;
	if (int_array == NULL || len <0) {
		throw com_ximpleware::InvalidArgumentException("invalid argument for appendIntArray in FastIntBuffer");
	}

	if (al_size == 0){
		//lastBuffer = (int *)malloc(sizeof(int)<<fib->exp);
		try{
			lastBuffer = new int[1<<exp];
		}catch(std::bad_alloc&){
			throw OutOfMemException(" allocation of int* in FastIntBuffer's append() failed");
		}

		/*if(lastBuffer==NULL){
			throwException2(invalid_argument,
				"invalid argument for appendIntArray in FastIntBuffer");
		} */      
		al->add((void *)lastBuffer);
		lastBufferIndex = 0;
		capacity = pageSize;
    } else {
		lastBufferIndex = min((size>>exp),al_size-1);
		lastBuffer = (int *)al->get(lastBufferIndex);
    }

    if ((size + len) < capacity) {
		if (size + len <(lastBufferIndex+1)<<exp){
			memcpy(lastBuffer+(size & r), int_array, len<<2);
		} 
		else {
			int offset = pageSize -(size & r);
			int l = len - offset;
			int k = l>>exp;
			int z;
			memcpy(lastBuffer+(size & r),
				int_array,offset);
			for (z=1;z<=k;z++){
				memcpy(al->get(lastBufferIndex+z),
					int_array+offset, pageSize<<2);
				offset += pageSize;
			}
			memcpy(al->get(lastBufferIndex+z),
				int_array + offset, l & r);

		}
        size += len;
		return;
    } else /* new buffers needed*/
        {
		int i;
		int n = ((len + size)>>exp)
			+(((len + size) & r) > 0 ? 1:0)
			- (capacity >> exp);
        /* create these buffers*/
        /* add to bufferArrayList */
		memcpy(lastBuffer+(size & r), 
			int_array,
			capacity - size);

		for (i = 0; i < n; i++) {
			int *newBuffer = NULL;
			try{
				newBuffer = new int[1<<exp];
			}catch(std::bad_alloc&){
				throw OutOfMemException("array list allocation in FastIntBuffer's append failed");
			}
			if (i < n - 1) {
				memcpy(newBuffer,
					int_array + (i<<exp) + capacity - size,
					pageSize<<2);
			} else {
				memcpy(newBuffer,
					int_array + (i<<exp) + capacity - size,
					(len + size - capacity - (i<<exp))<<2);
			}
			al->add((void *)newBuffer);
		}
		size += len;
		capacity += (n << exp);
	}
}

int* FastIntBuffer::getIntArray(int offset, int len){
	int *result;
	int first_index, last_index;
	if (size <= 0 || 
		offset < 0 || 
		(offset+len)>size) {
			throw InvalidArgumentException("getIntArray in FastIntBuffer failed ");
	}
	//result = (int *)malloc(len*sizeof(int)); // allocate result array
	result = new int[len];
	
	first_index = offset >> exp;
	last_index = (offset + len)>>exp;
	if (((offset + len) & r) == 0){
		last_index--;
	}

	if (first_index == last_index) {
		memcpy(result,
			(int *)al->get(first_index)+ (offset & r),
			len<<2);
	} else {
		int int_array_offset = 0;
		int i;
		int *currentChunk;
		for (i = first_index; i <= last_index; i++) {
			currentChunk = (int *)al->get(i);
			if (i == first_index)
			{
				memcpy(result,
					currentChunk + (offset & r),
					(pageSize - (offset & r))<<2);
				int_array_offset += pageSize  - (offset & r);
			} else if (i == last_index) // last sections
			{
				memcpy(result + int_array_offset,
					currentChunk,
					(len - int_array_offset)<<2);

			} else {
				memcpy(result + int_array_offset,
					currentChunk,
					pageSize <<2);
				int_array_offset += pageSize;
			}
		}
	}
	return result;
}
// Get the int at the index position of FastIntBuffer
/*int FastIntBuffer::intAt(int index){
	if (index < 0 || index > size - 1) {
		throw InvalidArgumentException("invalid index range in FastIntBuffer's intAt()");
    }
	return ((int *) al->get(index>>exp))[index & r];
}*/
// Replace the value at the index position of FastIntBuffer 
// with newVal
/*void FastIntBuffer::modifyEntry(int index,int newVal){
	if (index < 0 || index > size - 1) {
		throw InvalidArgumentException("invalid index range in FastIntBuffer's modifyEntry()");
		
    }
	((int *) al->get(index>>exp))[index & r] = newVal;
}*/
// convert the content of FastIntBuffer to int *
int* FastIntBuffer::toIntArray(){
	if (size > 0) {
		int i;
		int array_offset = 0;
		//int *resultArray = (int *)malloc(sizeof(int)*fib->size);
		//int *resultArray = new int[size];
		int* resultArray = NULL;
		try{
			resultArray = new int[size];
		}catch(std::bad_alloc&){
			throw OutOfMemException("array list allocation in FastIntBuffer's modifyEntry() failed");
		}
			

		for (i = 0; i<al->size;i++)
		{
			/*System.arraycopy(
			(int[]) bufferArrayList.get(i),
			0,
			resultArray,
			array_offset,
			(i == (bufferArrayList.size() - 1)) ? (size& r) : pageSize);*/
			memcpy(resultArray + array_offset, 
				(int *) al->get(i),
				(i == al->size-1)? (size & r) : pageSize);
			//(i == (bufferArrayList.size() - 1)) ? size() % pageSize : pageSize);
			array_offset += pageSize;
		}
		return resultArray;
	}
	return NULL;
}

// reset the size of fastIntBuffer
bool FastIntBuffer::resize(int newSz){
	if (newSz <= capacity && newSz >=0){
		size = newSz;
		return true;
	}	 
	else
		return false;
}
