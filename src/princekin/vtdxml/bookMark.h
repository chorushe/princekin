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
#ifndef BM_H
#define BN_H

#include "customTypes.h"
#include "vtdNav.h"

namespace com_ximpleware{
	class BookMark{
	private:	
		int* ba;
		int ba_len;
		VTDNav *vn1;
	public:
		BookMark();
		BookMark(VTDNav *vn);
		virtual ~BookMark();
		void unbind();
		void bind(VTDNav *vn);
		VTDNav* getNav();
		bool setCursorPosition( VTDNav *vn);
		bool setCursorPosition();
		/**
		* Record the cursor position
		* This method is implemented to be lenient on loading in
		* that it can load nodes from any VTDNav object
		* if vn is null, return false
		*/
		bool recordCursorPosition( VTDNav *vn);
		/**
		* Record cursor position of the VTDNav object as embedded in the
		* bookmark
		*/
		bool recordCursorPosition();
		/**
		* Compare the bookmarks to ensure they represent the same
		* node in the same VTDnav instance
		*/
		bool equal(BookMark *bm2);
		/**
		* Returns the hash code which is a unique integer for every node
		*/
		int hashCode();
		/**
		* Compare the bookmarks to ensure they represent the same
		* node in the same VTDnav instance
		*/
		bool deepEqual(BookMark *bm2);

		 /**
		  * Compare the node positions of two bookMarks
		  * @param bm1
		  * @return
		  */
		bool compare(BookMark *bm2);
	};
}

#endif