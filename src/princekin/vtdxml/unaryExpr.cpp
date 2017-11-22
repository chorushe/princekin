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
#include "unaryExpr.h"
using namespace com_ximpleware;
UnaryExpr::UnaryExpr(opType op, Expr *e1):
e(e1),op(op)
{}
UnaryExpr::~UnaryExpr(){
  delete e;
}
bool UnaryExpr::evalBoolean(VTDNav *vn){
	return e->evalBoolean(vn);
}
double UnaryExpr::evalNumber(VTDNav *vn){
	return e->evalNumber(vn)*(-1);
}
int UnaryExpr::evalNodeSet(VTDNav *vn){
     throw XPathEvalException("unaryExpr can't eval to a node set!");
}
UCSChar* UnaryExpr::evalString(VTDNav *vn){
	UCSChar *string1 = e->evalString(vn);
	size_t len = wcslen(string1);
	//(UCSChar*) malloc((len+1)*sizeof(UCSChar));
	UCSChar *string =  new UCSChar[len+1]; 
	if (string == NULL) {
			throw OutOfMemException("allocate string failed in funcExpr's evalString()");
	}

	swprintf(string,64,L"-%ls",string1);

	delete(string1);
	return string;
}

void UnaryExpr::reset(VTDNav *vn){e->reset(vn);}
void UnaryExpr::toString(UCSChar* string){
	wprintf(L"-");
	e->toString(string);
}

bool UnaryExpr::isNumerical(){return true;}
bool UnaryExpr::isNodeSet(){return false;}
bool UnaryExpr::isString(){return false;}
bool UnaryExpr::isBoolean(){return false;}

bool UnaryExpr::requireContextSize(){return e->requireContextSize();}
void UnaryExpr::setContextSize(int size){e->setContextSize(size);}

void UnaryExpr::setPosition(int pos){e->setContextSize(pos);}
int UnaryExpr::adjust(int n){return 0;}
bool UnaryExpr::isFinal(){return e->isFinal();}
void UnaryExpr::markCacheable(){e->markCacheable();}
void UnaryExpr::markCacheable2(){e->markCacheable2();}
void UnaryExpr::clearCache(){e->clearCache();}
