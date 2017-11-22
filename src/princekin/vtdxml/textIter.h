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
#ifndef TEXT_ITER_H
#define TEXT_ITER_H
#include "customTypes.h"
#include "vtdNav.h"

namespace com_ximpleware{

	class TextIter{
	private:
		int prevLocation;
		int depth;
		int index;
		VTDNav *vn;
		int lcIndex;
		int lcLower;
		int lcUpper;
		int sel_type;
		UCSChar *piName;

		int increment(int sp);
		/* Test whether a give token type is a TEXT.*/
		bool isText(int i);		
		
		int handleDocumentNode();
		int handleLevel0();
		int handleLevel1();
		int handleLevel2();
		int _handleLevel2();
		int handleLevel3();
		int handleLevel4();
		int handleDefault();

	public:
		/* create TextIter */
		TextIter();
		/* free TextIter */
		virtual ~TextIter();
		/* Obtain the current navigation position and element info from VTDNav.
		* So one can instantiate it once and use it for many different elements */
		void touch(VTDNav *vn);
		/* Get the index vals for the text nodes in document order.*/
		int getNext();
		/* Ask textIter to return character data or CDATA nodes*/
		void selectText();
		/*  Ask textIter to return comment nodes*/
		void selectComment();
		/* Ask TextIter to return processing instruction name 
		* no value */
		void selectPI0();
		/* Ask TextIter to return processing instruction of 
		given name */
		void selectPI1(UCSChar *piName);
	};


}

#endif
