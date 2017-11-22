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
#ifndef FUNC_EXPR_H
#define FUNC_EXPR_H
#include "expr.h"

namespace com_ximpleware{
	
	typedef enum FuncName {FN_LAST,
		FN_POSITION,
		FN_COUNT,
		FN_LOCAL_NAME,
		FN_NAMESPACE_URI,
		FN_NAME,
		FN_STRING,
		FN_CONCAT,
		FN_STARTS_WITH,
		FN_CONTAINS,
		FN_SUBSTRING_BEFORE,
		FN_SUBSTRING_AFTER,
		FN_SUBSTRING,
		FN_STRING_LENGTH,
		FN_NORMALIZE_SPACE,
		FN_TRANSLATE,
		FN_BOOLEAN,
		FN_NOT,
		FN_TRUE,
		FN_FALSE,
		FN_LANG,
		FN_NUMBER,
		FN_SUM,
		FN_FLOOR,
		FN_CEILING,
		FN_ROUND,
		/* 2.0 functions */
		FN_ABS,
		FN_ROUND_HALF_TO_EVEN,
		FN_ROUND_HALF_TO_ODD,
		FN_CODE_POINTS_TO_STRING,
		FN_COMPARE,
		FN_UPPER_CASE,
		FN_LOWER_CASE,
		FN_ENDS_WITH,
		FN_QNAME,
		FN_LOCAL_NAME_FROM_QNAME,
		FN_NAMESPACE_URI_FROM_QNAME,
		FN_NAMESPACE_URI_FOR_PREFIX,
		FN_RESOLVE_QNAME,
		FN_IRI_TO_URI,
		FN_ESCAPE_HTML_URI,
		FN_ENCODE_FOR_URI,
		FN_MATCH_NAME,
		FN_MATCH_LOCAL_NAME,
		FN_NOT_MATCH_NAME,
		FN_NOT_MATCH_LOCAL_NAME,
		FN_CURRENT,
		FN_GENERATE_ID,
		FN_FORMAT_NUMBER,
		FN_KEY,
		FN_ID,
		FN_DOCUMENT,
		FN_SYSTEM_PROPERTY,
		FN_ELEMENT_AVAILABLE,
		FN_FUNCTION_AVAILABLE
	} funcName;

	

	class FuncExpr: public Expr{
	public:
		FuncExpr(funcName fn, AList *a);
		virtual ~FuncExpr();
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
		bool checkArgumentCount();

	private:

		funcName opCode;
		AList *al;
		bool isNum;
		bool isBool;
		bool isStr;
		bool isNode;
		int contextSize;
		int position;
		int argCount1;
		int a;
		int state;
		VTDGen *vg;
		VTDNav *newVN, *xslVN;
		UCSChar *s;
		//int a;
		double sum( VTDNav *vn);
		int argCount();
		int count( VTDNav *vn);
		UCSChar *getLocalName( VTDNav *vn);
		UCSChar *getName( VTDNav *vn);
		UCSChar *getNameSpaceURI( VTDNav *vn);
		UCSChar *getString( VTDNav *vn);
		bool contains( VTDNav *vn);
		UCSChar* concat( VTDNav *vn);
		bool startsWith( VTDNav *vn);
		bool endsWith( VTDNav *vn);
		UCSChar* subString( VTDNav *vn);
		UCSChar* normalizeString( VTDNav *vn);
		UCSChar* subStringBefore( VTDNav *vn);
		UCSChar* subStringAfter( VTDNav *vn);
		UCSChar* translate( VTDNav *vn);
		bool isWS(UCSChar c);
		bool lang( VTDNav *vn, UCSChar* s);
		UCSChar* normalize(UCSChar *s);
		bool matchName(VTDNav *vn);
		bool matchLocalName(VTDNav *vn);
		//bool isElementAvailable(VTDNav *vn);
		
		UCSChar* formatNumber(VTDNav *vn);
		UCSChar* generateID(VTDNav *vn);
		int evalFirstArgumentListNodeSet( VTDNav *vn);
		int evalFirstArgumentListNodeSet2( VTDNav *vn);
		UCSChar* upperCase( VTDNav *vn);
		UCSChar* lowerCase( VTDNav *vn);
		double roundHalfToEven( VTDNav *vn);
		VTDNav *getNewNav(){return newVN;}
		UCSChar *fname();
		int getStringVal(VTDNav *vn,int i);
		UCSChar *getSystemProperty(VTDNav *vn){return createEmptyString();}
		bool isElementAvailable(VTDNav *vn){return false;}
		bool isFunctionAvailable(VTDNav *vn){return false;}
		UByte* doubleCapacity(UByte *b, size_t cap);
		Long getBytes_UTF8(UCSChar *s);
		//UCSChar *getStringVal(VTDNav *vn, int i);
	};
}

#endif
