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
#ifndef UNARY_EXPR_H
#define UNARY_EXPR_H
#include "expr.h"

namespace com_ximpleware{
	class UnaryExpr: public Expr{
	private:
		Expr *e;
		opType op;
	public:
		UnaryExpr(opType op, Expr *e1);
		virtual ~UnaryExpr();
		bool evalBoolean(VTDNav *vn);
		double evalNumber(VTDNav *vn);
		int evalNodeSet(VTDNav *vn);
		UCSChar* evalString(VTDNav *vn);

		void reset(VTDNav *vn);
		void toString(UCSChar* string);

		bool isNumerical();
		bool isNodeSet();
		bool isString();
		bool isBoolean();

		bool requireContextSize();
		void setContextSize(int size);

		void setPosition(int pos);
		int adjust(int n);
		bool isFinal();
		void markCacheable();
		void markCacheable2();
		void clearCache();
	};

}
#endif