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
#include "literalExpr.h"
using namespace com_ximpleware;

LiteralExpr::LiteralExpr(UCSChar *st):
s(st){
}

inline LiteralExpr::~LiteralExpr(){
	delete s;
	s=NULL;
}

inline bool LiteralExpr::isNumerical(){return false;}
inline bool LiteralExpr::isNodeSet(){return false;}
inline bool LiteralExpr::isString(){return true;}
inline bool LiteralExpr::isBoolean(){return false;}

void LiteralExpr::reset(VTDNav *vn){}
void LiteralExpr::toString(UCSChar* string){
	wprintf(L"\"");
	wprintf(L"%ls",s);
	wprintf(L"\"");
}

bool LiteralExpr::evalBoolean(VTDNav *vn){
	size_t len = wcslen(s);
	return len != 0;
}

double LiteralExpr::evalNumber(VTDNav *vn){
	double d  = 0, result;
	UCSChar *temp;
	if (wcslen(s)==0)
		return d/d;
	result = wcstod(s,&temp);
	while(*temp!=0){
		if ( *temp == L' '
			|| *temp == L'\n'
			|| *temp == L'\t'
			|| *temp == L'\r'){
				temp++;
			}
		else
			return d/d; //NaN
	}
	return result;
}
int LiteralExpr::evalNodeSet(VTDNav *vn){		
	throw XPathEvalException("LiteralExpr can't eval to a node set!");
}
UCSChar* LiteralExpr::evalString(VTDNav *vn){return wcsdup(s);}
bool LiteralExpr::requireContextSize(){return false;}
void LiteralExpr::setContextSize(int size){}

void LiteralExpr::setPosition(int pos){}
int LiteralExpr::adjust(int n) {return 0;}

bool LiteralExpr::isFinal(){return true;}


/*bool LiteralExpr::isConstant(){return true;}*/



char* com_ximpleware::getAxisString(axisType at) {

	switch(at){
		case AXIS_CHILD0:
		case AXIS_CHILD : return (char*)"child::";
		case AXIS_DESCENDANT0 : return (char*)"descendant::";
		case AXIS_DESCENDANT : return (char*)"descendant::";
		case AXIS_PARENT :	return (char*)"parent::";
		case AXIS_ANCESTOR :	return (char*)"ancestor::";
		case AXIS_FOLLOWING_SIBLING0 :	return (char*)"following-sibling::";
		case AXIS_FOLLOWING_SIBLING :	return (char*)"following-sibling::";
		case AXIS_PRECEDING_SIBLING0 :	return (char*)"preceding-sibling::";
		case AXIS_PRECEDING_SIBLING :	return (char*)"preceding-sibling::";
		case AXIS_FOLLOWING0 :	return (char*)"following::";
		case AXIS_FOLLOWING :	return (char*)"following::";
		case AXIS_PRECEDING0 :	return (char*)"preceding::";
		case AXIS_PRECEDING :	return (char*)"preceding::";
		case AXIS_ATTRIBUTE :	return (char*)"attribute::";
		case AXIS_NAMESPACE :	return (char*)"namespace::";
		case AXIS_SELF :	return (char*)"self::";
		case AXIS_DESCENDANT_OR_SELF0 :	return (char*)"descendant-or-self::";
		case AXIS_DESCENDANT_OR_SELF :	return (char*)"descendant-or-self::";
		default :	return (char*)"ancestor-or-self::";
	}
}

