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
#ifndef AL_H 
#define AL_H
#include "customTypes.h"
//#include <new>
//#include <iostream>

namespace com_ximpleware {
	
	class  ArrayList {
		friend class ContextBuffer;
		friend class FastIntBuffer;
		friend class FastLongBuffer;
	public:
		ArrayList();
		ArrayList(int initialCapacity);
		virtual ~ArrayList();
		static const int AL_GROW_INC =16;

		inline int add(void *element);
		void* get(int index){return storage[index];}
		int getSize(){return size;}

	private:
		int capacity;
		int size;
		void **storage;
		int addNew(void *element);
	};

	inline int ArrayList::add(void *element){
		//int t = 0,k=0;
		//void **v=NULL;
		if (size < capacity){
			storage[size] = element;
			size++;
			return size;
		}
		else
			return addNew(element);
	}

};

#endif
