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
#include "filterExpr.h"
#include "cachedExpr.h"
using namespace com_ximpleware;

FilterExpr::FilterExpr(Expr *e1, Predicate *pr):
e(e1),
p(pr),
out_of_range(false)
{
	pr->fe = this;
}
	
FilterExpr::~FilterExpr(){
	delete e;
	delete p;
	e=NULL;
	p=NULL;
}

bool FilterExpr::evalBoolean(VTDNav *vn){
	bool a = false;
	int size;
	vn->push2();
	//record stack size
	size = vn->contextBuf2->size;
	try{
		a = (evalNodeSet(vn) != -1);
	}catch (XPathEvalException&){
	}
	//rewind stack
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return a;
}

double FilterExpr::evalNumber(VTDNav *vn){

	//String s = "";
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

		/////////////////
	/*double d = 0.0;
	int a = getStringIndex(vn);
	try{
		if (a!=-1) return vn->parseDouble(a);
	}catch (NavException&){
	}
	return 0/d;*/
}

int FilterExpr::evalNodeSet(VTDNav *vn){
	int i,a;
	if (first_time && p->requireContext){
		first_time = false;
		i = 0;
		e->adjust(vn->vtdSize);
		while(e->evalNodeSet(vn)!=-1)
			i++;
		p->setContextSize_p(i);
		reset2(vn);
	}
	a = e->evalNodeSet(vn);
	if (out_of_range)
		return -1;
	while (a!=-1){
		if (p->eval_p(vn)==true){
			//p.reset();
			return a;
		}else {
			//p.reset();
			a = e->evalNodeSet(vn);
		}
	}
	return -1;
}

UCSChar* FilterExpr::evalString(VTDNav *vn){

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

void FilterExpr::reset(VTDNav *vn){
	reset2(vn);
	//vn.contextStack2.size = stackSize;
	//position = 1;
	first_time = true;
}
void FilterExpr::reset2(VTDNav *vn){
	out_of_range = false;
	e->reset(vn);
	p->reset_p(vn);
}
void FilterExpr::toString(UCSChar* string){
	wprintf(L"(");
	e->toString(string);
	wprintf(L")");
	p->toString_p(string);
}

bool FilterExpr::isNumerical(){return false;}

bool FilterExpr::isNodeSet(){return true;}

bool FilterExpr::isString(){return false;}

bool FilterExpr::isBoolean(){return false;}

bool FilterExpr::requireContextSize(){
	return e->requireContextSize();
}

void FilterExpr::setContextSize(int size){
	e->setContextSize(size);
}

void FilterExpr::setPosition(int pos){
	e->setPosition(pos);
}

int FilterExpr::adjust(int n){
	return e->adjust(n);
}

bool FilterExpr::isFinal(){
	return e->isFinal();
}
		
void FilterExpr::markCacheable(){
	e->markCacheable();
	if (p->e!=NULL){
		if (p->e->isFinal()&&p->e->isNodeSet()){
			CachedExpr *ce = new CachedExpr(p->e);
			p->e= ce;
		}
		p->e->markCacheable2();
	}
}

void FilterExpr::markCacheable2(){
	e->markCacheable2();
	if (p->e!=NULL){
		if (p->e->isFinal()&&p->e->isNodeSet()){
			CachedExpr *ce = new CachedExpr(p->e);
			p->e= ce;
		}
		p->e->markCacheable2();
	}
}

void FilterExpr::clearCache(){
	e->clearCache();
	if (p->e!=NULL){
		p->e->clearCache();
	}
}
