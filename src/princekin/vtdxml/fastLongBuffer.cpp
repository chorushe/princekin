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
#include "fastLongBuffer.h"

using namespace com_ximpleware;

// initialize FastLongBuffer with default page size of 1024 longs 
FastLongBuffer::FastLongBuffer() try:
al(new ArrayList()),
capacity(0),
exp(10),
pageSize(1<<10),
r(1023),
size(0)
{
	
}
catch(std::bad_alloc&){
		throw OutOfMemException("array list allocation in FastLongBuffer's constructor failed");
}


// initialize FastLongBuffer with page size of (1<<e) longs
FastLongBuffer::FastLongBuffer(int exp1) try:
al(new ArrayList()),
capacity(0),
exp(exp1),
pageSize(1<<exp1),
r((1<<exp1)-1),
size(0)
{
	
}
catch(std::bad_alloc&){
		throw OutOfMemException("array list allocation in FastLongBuffer's constructor failed");
}
// initialize FastLongBuffer with page size of (1<<e) longs and initial capciaty of c longs
FastLongBuffer::FastLongBuffer(int e, int c) try: 
al(new ArrayList(c)),
capacity(0),
exp(e),
pageSize(1<<e),
r((1<<exp)-1),
size(0)
{
}
catch(std::bad_alloc&){
		throw OutOfMemException("array list allocation in FastLongBuffer's constructor failed");
}

FastLongBuffer::~FastLongBuffer(){
	delete al;
}


// append a long array to the end of FastLongBuffer
void FastLongBuffer::append(Long* longArray, int len){
	Long *lastBuffer = NULL;
	int lastBufferIndex;
	int al_size = al->size;

	if (longArray == NULL || len <0) {
		throw InvalidArgumentException("invalid argument for appendLongArray /n");
    }
    /* no additional buffer space needed */
	if (al_size == 0)
	{
		lastBuffer = new Long[1<<exp];
		/*if (lastBuffer == NULL){
			throwException2(out_of_mem,
				" appendLongArray failed to allocate mem ");
		}*/
		al->add((void *)lastBuffer);
		lastBufferIndex = 0;
		capacity = pageSize;
	}else {
		lastBufferIndex = min((size>>exp),al_size-1);
		lastBuffer = (Long *)al->get(lastBufferIndex);
	}


	if ((size + len)< capacity){
		if (size + len <(lastBufferIndex+1)<<exp){
			memcpy(lastBuffer+(size & r), longArray, len<<3);
		} 
		else {
			int offset = pageSize -(size & r);
			int l = len - offset;
			int k = (l)>>exp;
			int z;
			memcpy(lastBuffer+(size & r),
				longArray,offset<<3);
			for (z=1;z<=k;z++){
				memcpy(al->get(lastBufferIndex+z),
					longArray+offset, pageSize<<3);
				offset += pageSize;
			}
			memcpy(al->get(lastBufferIndex+z),
				longArray + offset, (l & r)<<3);

		}
		size += len;
		return;
    } else
        {
		int i;
		Long *newBuffer = NULL;

		int n = ((len + size) >> exp) 
			+(((len + size) & r)> 0 ? 1 : 0)
			- (capacity >> exp);
       
		memcpy(lastBuffer + (size & r),
			longArray,
			(capacity - size)<<3);

        for (i = 0; i < n; i++) {
			//newBuffer = (Long *)malloc(sizeof(Long)<<flb->exp);
			newBuffer = new Long[1<<exp];
			/*if (newBuffer == NULL){
				throwException2(out_of_mem,
					" appendLongArray failed to allocate mem ");
			}*/
            if (i < n - 1) {
				memcpy( newBuffer,
					longArray + (i<<exp) + capacity - size,
					pageSize << 3);

            } else {
				memcpy(newBuffer,
					longArray + (i<<exp) + capacity - size,
					(len + size - (i<< exp) - capacity)<<3);
            }
			al->add(newBuffer);
        }
		size += len;
		capacity += (n << exp);
    }
}
// append a long to the end of FastLongBuffer
void FastLongBuffer::append(Long l){
	/*Long *lastBuffer = NULL;
	int lastBufferIndex;
	int al_size = al->size;

	if (al_size == 0) {
		lastBuffer = new Long[1<<exp];
		al->add((void *)lastBuffer);
		capacity = pageSize;
	}else{
		lastBufferIndex = min((size>>exp),al_size-1);
		lastBuffer = (Long *)al->get(lastBufferIndex);
	}*/

	if (size < capacity){
		((Long *)al->get(size>>exp))[size & r] = l; 
		//lastBuffer[size & r] = l;
		size ++;
	}else {
		Long* newBuffer = new Long[1<<exp];
		size ++;
		capacity += pageSize;
		al->add((void *)newBuffer);
		newBuffer[0]=l;
	}
}

// Return a selected chuck of long buffer as a long array.
Long* FastLongBuffer::getLongArray(int offset, int len){
	Long *result = NULL;
	int first_index, last_index;
	if (size <= 0 
		|| offset < 0 
		|| len <0 
		|| offset + len > size) {
			throw InvalidArgumentException("Invalid argument for getLongArray in FastLongBuffer");
		}
   
	//result = (Long *)malloc(sizeof(Long)*len);
	result = new Long[len];

	/*if (result == NULL){
		throwException2(out_of_mem,
			" getLongArray failed to allocate mem in FastLongBuffer");
	}*/
	first_index = offset >> exp;
	last_index = (offset+len) >> exp;

	if (((offset + len) & r)== 0){
		last_index--;
	}

    if (first_index == last_index) {
		memcpy(result, 
			(Long *)al->get(first_index) + (offset & r),
			len <<3);
    } else {
        int long_array_offset = 0;
		int i;
		Long *currentChunk;
        for (i = first_index; i <= last_index; i++) {
			currentChunk = (Long *)al->get(i);
            if (i == first_index)
                {
         
				memcpy(result,
					currentChunk + (offset & r),
					(pageSize - (offset& r))<<3);
				long_array_offset += pageSize - (offset & r);
            } else if (i == last_index)
                {
                memcpy(result + long_array_offset,
					currentChunk,
					(len - long_array_offset) <<3);


            } else {
                memcpy( result + long_array_offset,
					currentChunk,
					pageSize <<3 );
				long_array_offset += pageSize;
            }
		}
	}
		return result;
}

// get the long at the index position from FastLongBuffer
/*Long FastLongBuffer::longAt(int index){
	int pageNum,offset;
	if (index < 0 || index > size - 1) {
		throw InvalidArgumentException("invalid index range");
    }
	pageNum = (index >> exp);
    offset = index & r;
	return ((Long *)al->get(pageNum))[offset];
}*/

// get the lower 32 bits from the index position from FastLongBuffer
/*int FastLongBuffer::lower32At(int index){
	int pageNum,offset;
    if (index < 0 || index > size) {
		throw InvalidArgumentException(" invalid index range");
    }
    pageNum =  (index >> exp);
    offset = index & r;
	return (int)((Long *)al->get(pageNum))[offset];
}*/

// get the upper 32 bits from the index position from FastLongBuffer 
/*int FastLongBuffer::upper32At(int index){
		int pageNum, offset;
    if (index < 0 || index > size) {
		throw InvalidArgumentException(" invalid index range");
    }
    pageNum = (index >> exp);
    offset = index & r;
 	return (int) ((((Long *)al->get(pageNum))[offset] & (((Long)0xffffffffL)<<32))>>32);
}*/

// replace the entry at the index position of FastLongBuffer with l
/*void FastLongBuffer::modifyEntry(int index, Long l){
    if (index < 0 || index > size) {
		throw InvalidArgumentException("invalid index range");
    }
	((Long *)al->get(index>>exp))[index & r] = l;
}*/

// convert FastLongBuffer into a Long array 
Long* FastLongBuffer::toLongArray(){
	Long *resultArray = NULL;
	int i;
	int al_size= al->size;
    if (size > 0) {
	// if (size()>0) {
        //long[] resultArray = new long[size];
		int k;
        int array_offset = 0;
		//resultArray = (Long *)malloc(sizeof(Long)*flb->size);
		resultArray = new Long[size];


		/*if (resultArray == NULL){
			throwException2(out_of_mem,
				"toLongArray failed to allocate mem");
		}*/
        //copy all the content int into the resultArray

		k = al_size;
        //for (i = 0; i < bufferArrayList.size(); i++) {
		for (i=0; i< k;i++){
            /*System.arraycopy(
                (int[]) bufferArrayList.get(i),
                0,
                resultArray,
                array_offset,
                //(i == (bufferArrayList.size() - 1)) ? size - ((size>>exp)<<exp) : pageSize);
                (i == (bufferArrayList.size() - 1)) ? (size & r) : pageSize); */
			memcpy(resultArray + array_offset,
				(Long *)al->get(i),
				((i == (al_size - 1) ) ? ( size & r) : pageSize )<<3); 
            array_offset += pageSize;
        }
        return resultArray;
    }
    return NULL;
}
