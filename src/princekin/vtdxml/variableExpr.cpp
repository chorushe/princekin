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
#include "variableExpr.h"
using namespace com_ximpleware;

VariableExpr::VariableExpr(UCSChar *s, Expr *e):
exprName(s),exprVal(e)
{
}
VariableExpr::~VariableExpr(){
	delete exprName;
	delete exprVal;
}
bool VariableExpr::evalBoolean(VTDNav *vn){
	return exprVal->evalBoolean(vn);
}
double VariableExpr::evalNumber(VTDNav *vn){
	return exprVal->evalNumber(vn);
}
int VariableExpr::evalNodeSet(VTDNav *vn){
	return exprVal->evalNodeSet(vn);
}
UCSChar* VariableExpr::evalString(VTDNav *vn){
	return exprVal->evalString(vn);
}

void VariableExpr::reset(VTDNav *vn){
	exprVal->reset(vn);
}
void VariableExpr::toString(UCSChar* string){
	//wprintf(L"$");
	wprintf(L"$%ls",exprName);
}

bool VariableExpr::isNumerical(){
	return exprVal->isNumerical();
}
bool VariableExpr::isNodeSet(){
	return exprVal->isNodeSet();
}
bool VariableExpr::isString(){
	return exprVal->isString();
}
bool VariableExpr::isBoolean(){
	return exprVal->isBoolean();
}

bool VariableExpr::requireContextSize(){
	return exprVal->requireContextSize();
}
void VariableExpr::setContextSize(int size){
	exprVal->setContextSize(size);
}

void VariableExpr::setPosition(int pos){
	exprVal->setPosition(pos);
}
int VariableExpr::adjust(int n){
	return exprVal->adjust(n);
}

bool VariableExpr::isFinal(){return exprVal->isFinal();}
void VariableExpr::markCacheable(){exprVal->markCacheable();}
void VariableExpr::markCacheable2(){exprVal->markCacheable2();}
void VariableExpr::clearCache(){exprVal->clearCache();}