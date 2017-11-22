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
#include "arrayList.h"
#include <cstdlib>
using namespace com_ximpleware;
int ArrayList::addNew(void *element){
	int t = capacity + AL_GROW_INC,k=0;
	void **v = (void **)std::malloc( sizeof(void *)*t); //new void*[t];			
	for (k=0;k<size;k++)
	{
		v[k] = storage[k]; // copy content
	}
	for (k=size;k<capacity;k++){
		v[k] = NULL;			// the remaining ones set to NULL
	}
	v[size]=element;
	capacity = t;
	size++;
	//delete[] storage;
	std::free(storage);
	storage = v;
	return size;
}
ArrayList::ArrayList():
capacity (16), size(0)
{	
	//storage = new void*[10];
	storage = (void **)std::malloc( sizeof(void *)<<4); // initial capacity of 16;
	for (int i=0;i<10;i++){
		storage[i] = NULL;
	}
    //return al;
}

ArrayList::ArrayList(int initialCapacity):
capacity (initialCapacity), size(0)
{
	//storage = new void*[initialCapacity];
	storage = (void **)std::malloc( sizeof(void *)*capacity); 
	for (int i=0;i<initialCapacity;i++)
		storage[i] = NULL;
}

ArrayList::~ArrayList(){
	for (int i=0;i<size;i++)
		std::free(storage[i]); 
	std::free(storage);
	storage = NULL;
}


