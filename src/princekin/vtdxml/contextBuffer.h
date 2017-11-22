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
#ifndef CONTEXTBUFFER_H
#define CONTEXTBUFFER_H
#include "customTypes.h"
//#include <new>
//#include <iostream>
#include "arrayList.h"

namespace com_ximpleware {
	class ContextBuffer{
		friend class Expr;
		friend class FilterExpr;
		friend class UnionExpr;
		friend class PathExpr;
		friend class LocationPathExpr;
		friend class BinaryExpr;
		friend class VTDNav;
		friend class AutoPilot;
		friend class FuncExpr;
	private:
		ArrayList *al;
		int capacity;
		int incSize;
		int n;
		int pageSize;
		int r;
		int size;
	public:
		ContextBuffer(int i);
		ContextBuffer(int e, int i);
		virtual ~ContextBuffer();
		bool load(int* output);
		void store(int* input);
		void resize();
	};
}


#endif
