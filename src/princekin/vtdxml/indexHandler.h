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
#ifndef INDEX_HANDLER_H
#define INDEX_HANDLER_H

#include "customTypes.h"
#include <stdio.h>
//#include "vtdGen.h"

namespace com_ximpleware {
	class VTDGen;
    class FastLongBuffer;
	class FastIntBuffer;
	class IndexHandler{
	public:
		const static int OFFSET_ADJUSTMENT=32;
		static Long reverseLong(Long l);
		static int reverseInt(int i);


		static Long adjust(Long l,int i);
		static bool isLittleEndian();

		/*writeIndex writes VTD+XML into a file
		This function throws index_write_exception*/
		static bool _writeIndex_L3(Byte version, 
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
			FastIntBuffer *l3Buffer, 
			FILE *f);
		static bool _writeIndex_L5(Byte version, 
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
			FILE *f);

		/*writeSeparateIndex writes VTD index into a separate file from XML
		notice that this function assumes XML document byte starts from the begining
		This function throws index_write_exception*/
		static bool _writeSeparateIndex_L3(Byte version, 
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
			FILE *f);

		static bool _writeSeparateIndex_L5(Byte version, 
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
			FILE *f);

		/*readIndex loads VTD+XML into VTDGen*/
		static bool _readIndex(FILE *f, VTDGen *vg);

		static bool _readIndex(UByte *ba, int len, VTDGen *vg);

		static bool _readSeparateIndex(FILE *xml, int XMLSize, FILE *vtdIndex, VTDGen *vg);
	};
}
#endif
