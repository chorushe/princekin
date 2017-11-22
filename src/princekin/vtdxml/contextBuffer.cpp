/* 
 * Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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

#include "contextBuffer.h"
using namespace com_ximpleware;

ContextBuffer::ContextBuffer(int i) try :
al ( new ArrayList()),
capacity(0),
incSize(i),
n(10),
pageSize(1024),
r(1023),
size(0)
{

}
catch(std::bad_alloc&){
	throw OutOfMemException("array list allocation in context buffer failed");
}

ContextBuffer::ContextBuffer(int exp, int i) try :
al ( new ArrayList()),
capacity(0),
incSize(i),
n(10),
pageSize(1<<exp),
r(pageSize-1),
size(0)
{
}
catch(std::bad_alloc&){
	throw OutOfMemException("array list allocation in context buffer failed");
}

ContextBuffer::~ContextBuffer(){
	delete al;
	al = NULL;
}

bool ContextBuffer::load(int* output){
	int startingOffset, len, first_index, last_index,i;
	//exception e;
	if (size < incSize) {
        return false;
    }

    startingOffset = size - incSize;
    len = incSize;


    first_index = (startingOffset >> n);
    last_index = ((startingOffset + len) >>n);
    if (((startingOffset + len)& r)== 0) {
        last_index--;
    }

    if (first_index == last_index) {
        /* to see if there is a need to go across buffer boundry*/

		memcpy(output,
			((int *)al->get(first_index))+((startingOffset & r)),
			len<<2);
    } else {
        int int_array_offset = 0;
        for (i = first_index; i <= last_index; i++) {
            int* currentChunk = (int *) al->get(i);
            if (i == first_index) /* first section */
                {
					memcpy(output,
						currentChunk+ ((startingOffset&r)),
						(pageSize - (startingOffset & r))<<2);
              
                int_array_offset += pageSize - (startingOffset & r);
            } else if (i == last_index) /* last sections*/
                {
					memcpy(output+int_array_offset,
						currentChunk, 
						(len-int_array_offset)<<2);

            } else {
				memcpy(output+int_array_offset, currentChunk, (pageSize)<<2);
                int_array_offset += pageSize;
            }
        }
    }

    size -= incSize;
    return true;
}
void ContextBuffer::store(int* input){
	int *lastBuffer; 
	int lastBufferIndex;
	int i;
	int al_size = al->size;
    
	if (al_size == 0){
		//lastBuffer = (int *)malloc(sizeof(int)*cb->pageSize);
		try{
			lastBuffer = new int[pageSize];
		}catch(std::exception& ){
			throw OutOfMemException(" allocation of int* in ContextBuffer's store() failed");
		}
		/*if (lastBuffer == NULL){
			throwException2(out_of_mem,
				"Fail to increase the size of ArrayList of ContextBuffer");
		}*/
		al->add(lastBuffer);
		lastBufferIndex = 0;
        capacity = pageSize;
    } else {
		lastBufferIndex = min(size>>n, al_size-1);
		lastBuffer = (int *)al->get(lastBufferIndex);
    }

    if ((size + incSize) < capacity) {
		if (size + incSize < ((lastBufferIndex+1)<<n)){
			memcpy(lastBuffer+(size&r),input, incSize <<2);
		}else {
			int offset = pageSize - (size & r);
			int l = incSize - offset;
			int k = l>>n;
			int z;
			memcpy(lastBuffer+(size& r),input, 
				offset<<2);
			for (z=1;z<=k;z++){
				memcpy(al->get(lastBufferIndex+z),input+offset,pageSize<<2);
				offset += pageSize;
			}
			memcpy(al->get(lastBufferIndex+z),input+offset, (l & r)<<2);
		}

        size += incSize;
		return;
    } else /* new buffers needed */
        {
        /* compute the number of additional buffers needed*/
        int k =
			((incSize + size)>>n)+
			(((incSize + size)&r)>0? 1 : 0)-
			(capacity>>n);
		memcpy(lastBuffer+((size&r)),input, (capacity-size)<<2);

        for (i = 0; i < k; i++) {
            //int *newBuffer = (int *)malloc(cb->pageSize*sizeof(int));
			int *newBuffer = NULL;
			try{
				newBuffer = new int[pageSize];
			}catch (std::bad_alloc&){
				throw OutOfMemException("allocation of int* in ContextBuffer's store() failed");
			}
			
			/*if (newBuffer == NULL){
				throwException2(out_of_mem,
					"Fail to increase the size of ArrayList of ContextBuffer");
			}*/
            if (i < k - 1) {
                // full copy 
                //System.arraycopy(input, pageSize * i + capacity - size, newBuffer, 0, pageSize);
				memcpy(newBuffer, 
					input + ((pageSize*i + capacity - size)),
					pageSize<<2);
					
            } else {
				memcpy(newBuffer,
					input + (( (i << n) + capacity - size)), 
					(incSize + size - (i << n) - capacity)<<2);
            }
			al->add(newBuffer);
        }
        size += incSize;
        capacity += (k << n);
		}
}
void ContextBuffer::resize(){

}


