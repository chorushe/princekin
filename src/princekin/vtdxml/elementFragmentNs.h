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
#ifndef ELEMENT_FRAGMENT_NS_H
#define ELEMENT_FRAGMENT_NS_H
//#include "customTypes.h"
#include "vtdNav.h"
//#include "fastIntBuffer.h"

namespace com_ximpleware {
	class ElementFragmentNs{
	private:
		Long l;
		FastIntBuffer *fib;
		int stLen;
		VTDNav* vn;
		//bool ws_init = false;
        

	public:
		
		ElementFragmentNs(VTDNav *vn, Long l1, FastIntBuffer *fib, int len);
		virtual ~ElementFragmentNs();
		int getFragmentSize();
		int getFragmentSize(encoding_t t);

		Long getOffsetLeng();
		UByte* toFragmentBytes();
		UByte* toFragmentBytes(encoding_t dest_encoding);
		void writeFragmentToFile(FILE *f);
		void writeFragmentToFile(FILE *f, encoding_t dest_encoding);
		static UByte ws[5];
	
	};
};
#endif