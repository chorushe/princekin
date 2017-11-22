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
#include "numberExpr.h"
using namespace com_ximpleware;

NumberExpr::NumberExpr(double d):
dval(d){
}
NumberExpr::~NumberExpr(){
}
bool NumberExpr::evalBoolean(VTDNav *vn){
	if (dval == 0
		|| dval!=dval)
		return false;
	return true;
}
double NumberExpr::evalNumber(VTDNav *vn){
	return dval;
}
int NumberExpr::evalNodeSet(VTDNav *vn){
	throw XPathEvalException(
		"numberExpr can't eval to a node set!");
	return -1;
}

UCSChar* NumberExpr::evalString(VTDNav *vn){
	bool b = false;
	double d = 0;
	UCSChar *tmp;
	if (dval != dval){
		tmp = wcsdup(L"NaN");
		b = true;
	}
	else if ( dval == 1/d){
		tmp = wcsdup(L"Infinity");
		b= true;
	}
	else if (dval == -1/d){
		tmp = wcsdup(L"-Infinity");
		b = true;
	}	
	else
	tmp = new UCSChar[1<<8];

	//if (tmp == NULL) {
	//	throw std::bad_alloc("string allocation in evalString_ne failed ");
	//}
	if (b)
		return tmp;
    if (dval == (Long) dval){
		swprintf(tmp, 64, L"%d",(Long) dval);
	} else {
		swprintf(tmp, 64, L"%f", dval);
	}
	return tmp;
}


void NumberExpr::reset(VTDNav *vn){}

void NumberExpr::toString(UCSChar *string){
	if (dval == (Long)dval){
		wprintf(L"%d",(Long)dval);
	}else
		wprintf(L"%f",dval);
}


bool NumberExpr::isNumerical(){return true;}

bool NumberExpr::isNodeSet(){return false;}

bool NumberExpr::isString(){return false;}

bool NumberExpr::isBoolean(){return false;}

bool NumberExpr::requireContextSize(){
	return false;
}

void NumberExpr::setContextSize(int size){}

void NumberExpr::setPosition(int pos){
}
bool NumberExpr::isFinal(){return true;}


/*bool NumberExpr::isConstant(){return true;}*/
int NumberExpr::adjust(int n){
	return 0;
}