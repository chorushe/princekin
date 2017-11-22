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
#include "intHash.h"
using namespace com_ximpleware;

IntHash::IntHash()try:
storage(new FastIntBuffer*[1<<0]),
m1(0),
m2((~m1) & 0xffffffff),
pse(ih_pageSizeE),
hw(1),
maxDepth(0),
e(0)
{
	//IntHash(0);
	int i=0;
	/* initialize everything to null */
	for (i=0;i<hw;i++){
		storage[i]= NULL;
	}
}
catch(std::bad_alloc&){
	throw OutOfMemException("allocation failure in IntHash's constructor");
}

IntHash::IntHash(int hashWidthExpo) try:
storage(new FastIntBuffer*[1<<hashWidthExpo]),
m1((1<<hashWidthExpo)-1),
m2((~m1) & 0xffffffff),
pse(ih_pageSizeE),
hw(1<<hashWidthExpo),
maxDepth(0)
{
	int i=0;
	/* initialize everything to null */
	for (i=0;i<hw;i++){
		storage[i]= NULL;
	}
}
catch(std::bad_alloc&){
	throw OutOfMemException("allocation failure in IntHash's constructor");
}

IntHash::~IntHash(){
	int i=0;

	for (i=0;i<=maxDepth;i++){
		delete (FastIntBuffer*)storage[i];
	}
	delete (FastIntBuffer**)storage;
	storage = NULL;
}

int IntHash::determineHashWidth(int i){
	if (i<(1<<8))
		return 3;
	if (i<(1<<9))
		return 4;
	if (i<(1<<10))
		return 5;
	if (i<(1<<11))
		return 6;
	if (i<(1<<12))
		return 7;
	if (i<(1<<13))
		return 8;
	if (i<(1<<14))
		return 9;
	if (i<(1<<15))
		return 10;
	if (i<(1<<16))
		return 11;
	if (i<(1<<17))
		return 12;
	if (i<(1<<18))
		return 13;
	if (i<(1<<19))
		return 14;
	if (i<(1<<20))
		return 15;
	if (i<(1<<21))
		return 16;
	if (i<(1<<22))
		return 17;
	if (i<(1<<23))
		return 18;
	if (i<(1<<25))
		return 19;
	if (i<(1<<27))
		return 20;
	if (i<(1<<29))
		return 21;
	return 22;
}
//void freeIntHash(IntHash *ih);
bool IntHash::isUnique(int i){
	int j,size;
	int temp = i & m1;
	if (temp>maxDepth){
		maxDepth = temp;
	}

	if (storage[temp]==NULL) {
		//ih->storage[temp]= createFastIntBuffer2(ih->pse);
		try{
			storage[temp]= new FastIntBuffer(pse);
		}catch(std::bad_alloc& ){
			throw OutOfMemException("allocation failed in IntHash's isUnique()");
		}
		/*if (ih->storage[temp]==NULL) {
		throwException2(out_of_mem,
		"FastIntBuffer allocation failed ");
		}*/
		storage[temp]->append(i);
		return true;
	}
	else{
		size = ((FastIntBuffer *)storage[temp])->size;
		for (j=0;j<size;j++){
			//if (i == storage[temp]->intAt(j)){
			if (i == ((FastIntBuffer *)storage[temp])->intAt(j)){
				return false;
			}
		}
		((FastIntBuffer *)storage[temp])->append(i);
		return true;
	}
}

void IntHash::reset(){
	int i=0;
	for (i=0;i<=maxDepth;i++){
		if (storage[i]!=NULL){
			storage[i]->clear();
		}
	}
}
