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
#ifndef BINARY_Expr_H
#define BINARY_Expr_H
#include "expr.h"

namespace com_ximpleware{
	class BinaryExpr: public Expr{
	public:
		static const int BUF_SZ_EXP = 7; 
		BinaryExpr(Expr *e1, opType op1, Expr *e2);
		virtual ~BinaryExpr();
		bool evalBoolean(VTDNav *vn);
		double evalNumber(VTDNav *vn);
		int evalNodeSet(VTDNav *vn);
		UCSChar* evalString(VTDNav *vn);

		void reset(VTDNav *vn);
		void toString(UCSChar *s);

		bool isNumerical();
		bool isNodeSet();
		bool isString();
		bool isBoolean();

		bool requireContextSize();
		void setContextSize(int size);

		void setPosition(int pos);
		int adjust(int n);

	private:
		bool computeComp( opType op,VTDNav *vn);
		bool compNumericalNodeSet(  VTDNav *vn, opType op);
		bool compNodeSetNumerical(  VTDNav *vn, opType op);
		bool compStringNodeSet(  VTDNav *vn, opType op);
		bool compNodeSetString(  VTDNav *vn, opType op);
		bool compNodeSetNodeSet(  VTDNav *vn, opType op);
		bool compNumbers( double d1, double d2, opType op);
		bool compareVNumber1( int i, VTDNav *vn, double d, opType op);
		bool compareVNumber2( int i, VTDNav *vn, double d, opType op);
		int getStringVal(VTDNav *vn, int i);
		bool compareVString1(int k, VTDNav *vn, UCSChar *s, opType op);
		bool compareVString2(int k, VTDNav *vn, UCSChar *s, opType op);
		bool compareVV(int k,  VTDNav *vn, int j,opType op);
		bool compEmptyNodeSet(opType op, UCSChar *s);

		bool isFinal();
		
		void markCacheable();
		void markCacheable2();
		void clearCache();

		Expr *left;
		opType op;
		Expr *right;
		FastIntBuffer *fib1;
		bool isBool;
		bool isNum;
	};
}

#endif
