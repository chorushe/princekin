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
#include "pathExpr.h"
using namespace com_ximpleware;

PathExpr::PathExpr(Expr *f, LocationPathExpr *l) try :
fe(f),
lpe(l),
evalState(0),
ih( new IntHash())
{}
catch (std::bad_alloc&){
	throw OutOfMemException("Allocation failed for PathExpr");
}

PathExpr::~PathExpr(){
	delete ih;
	delete fe;
	delete lpe;
	ih=NULL;
	fe=NULL;
	lpe=NULL;
}
bool PathExpr::evalBoolean(VTDNav *vn){
	bool b = false;
	int size;
	vn->push2();
	/* record teh stack size*/
	size = vn->contextBuf2->size;
    try{
		b = (evalNodeSet(vn) != -1);
	}catch (...){
	}
	/*rewind stack*/
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return b;
}

double PathExpr::evalNumber(VTDNav *vn){
	double d1 = 0.0;
	double d=d1/d1;
	int a = -1;
	vn->push2();
	int size = vn-> contextBuf2->size;
	try {
		a = evalNodeSet(vn);
		if (a != -1) {
			int t = vn->getTokenType(a);
			if (t == TOKEN_ATTR_NAME) {
				d = vn->parseDouble(a+1);
			} else if (t == TOKEN_STARTING_TAG || t ==TOKEN_DOCUMENT) {
				UCSChar *s = vn->getXPathStringVal(), *s1;
				d  = wcstod(s,&s1);
				delete s;
			}else if (t == TOKEN_PI_NAME) {
				if (a+1 < vn->vtdSize || vn->getTokenType(a+1)==TOKEN_PI_VAL)
					//s = vn.toString(a+1); 	
					d = vn->parseDouble(a+1);                	
			}else 
				d = vn->parseDouble(a);
		}
	} catch (NavException&) {

	}
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	//return s;
	return d;
}

int PathExpr::evalNodeSet(VTDNav *vn){
	int a;
	while (true) {
		switch (evalState) {
		case 0: /*this state is teh initial state;*/
			a = fe->evalNodeSet(vn);
			if (a == -1){
				evalState =4;
			}
			else
				evalState = 1;
			break;
		case 1: /* fe returns valid value, then iterate the locationPath*/
			vn->push2();
			a = lpe->evalNodeSet(vn);
			if (a == -1) {
				lpe->reset(vn);
				evalState = 3;
			} else {
				evalState = 2;
				if (isUnique(a))
					return a;
			}
			break;
		case 2:
			a = lpe->evalNodeSet(vn);
			if (a == -1) {
				lpe->reset( vn);
				evalState = 3;
			} else{
				if (isUnique(a))
					return a;
				//return a;
			}
			break;
		case 3:
			vn->pop2();
			a = fe->evalNodeSet(vn);
			if (a == -1)
				evalState = 4;
			else{
			    vn->push2();
				evalState = 2;
			}
			break;
		case 4:
			return -1;
		default:
			throw OtherException("Invalid state evaluating PathExpr");
		}
	}
}

UCSChar* PathExpr::evalString(VTDNav *vn){
	UCSChar *s = NULL;	
	int a = -1;
	vn->push2();
    int size = vn->contextBuf2->size;
     
	try {
         a = evalNodeSet(vn);
		 if (a != -1) {
			 int t = vn->getTokenType(a);
			 switch(t){
			 case TOKEN_STARTING_TAG:
			 case TOKEN_DOCUMENT:
				 s = vn->getXPathStringVal();
				 break;
			 case TOKEN_ATTR_NAME:
				 s = vn->toString(a + 1);
				 break;
			 case TOKEN_PI_NAME:
				 //if (a + 1 < vn.vtdSize
				 //		|| vn.getTokenType(a + 1) == VTDNav.TOKEN_PI_VAL)
				 s = vn->toString(a + 1);
				 break;
			 default:
				 s = vn->toString(a);
				 break;
			 }				
		 }else{
			 s=wcsdup(L"");
		 }
        } catch (NavException&) {

        }
        vn->contextBuf2->size = size;
        reset(vn);
        vn->pop2();
        return s;
}

void PathExpr::reset(VTDNav *vn){
	fe->reset(vn);
	lpe->reset(vn);
	ih->reset();
	evalState = 0;
}

void PathExpr::toString(UCSChar *string){	
	wprintf(L"(");
	fe->toString(string);
	wprintf(L")/");
	lpe->toString(string);
}

bool PathExpr::isNumerical(){return false;}
bool PathExpr::isNodeSet(){return true;}
bool PathExpr::isString(){return false;}
bool PathExpr::isBoolean(){return false;}

bool PathExpr::requireContextSize(){return false;}
void PathExpr::setContextSize(int size){}

void PathExpr::setPosition(int pos){}
int PathExpr::adjust(int n){
	int i=fe->adjust(n);
	lpe->adjust(n);
	if (ih!=NULL && i==ih->e)
	{}else{
		delete(ih);
		ih = new IntHash(i);
	}
	return i;
}

bool PathExpr::isFinal(){
	return fe->isFinal();
}
		
void PathExpr::markCacheable(){
	fe->markCacheable();
	lpe->markCacheable();
}

void PathExpr::markCacheable2(){
	fe->markCacheable2();
	lpe->markCacheable2();
}

void PathExpr::clearCache(){
	fe->clearCache();
	lpe->clearCache();
}
