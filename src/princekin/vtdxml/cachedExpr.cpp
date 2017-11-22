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
#include "cachedExpr.h"
using namespace com_ximpleware;

CachedExpr::CachedExpr(Expr *e1):
e(e1),
cached(false),
ens(NULL),
count(0),
vn1(NULL),
es(NULL)
{
	
}

CachedExpr::~CachedExpr(){
	delete e;
	delete ens;
	delete vn1;
	delete es;
}

bool CachedExpr::evalBoolean(VTDNav *vn){
	if (cached){
		return eb;
	}else{
		eb = e->evalBoolean(vn);
		return eb;
	}
}

double CachedExpr::evalNumber(VTDNav *vn){
	if (cached){
		return en;
	}else{
		cached = true;
		en = e->evalNumber(vn);
		return en;
	}
}

int CachedExpr::evalNodeSet(VTDNav *vn){
	int i=-1;
		if (cached){
			if (count<ens->getSize()){
				i=ens->intAt(count);
				vn->recoverNode(i);
				count++;
				return i;
			}else
				return -1;

		}else{
			cached = true;
			
			if (ens==NULL){
				ens = new FastIntBuffer(8);//page size 64
			}
			//record node set
			while((i=e->evalNodeSet(vn))!=-1){
				ens->append(i);
			}
			e->reset(vn);
			if(ens->getSize()>0){
				i=ens->intAt(count);//count should be zero
				vn->recoverNode(i);
				count++;
				return i;
			}else
				return -1;
		}
}

UCSChar* CachedExpr::evalString(VTDNav *vn){
	if (cached){
		return es;
	}else{
		cached = true;
		es = e->evalString(vn);
		return es;
	}	
}

void CachedExpr::reset(VTDNav *vn){
	count = 0;
	if (e!=NULL && vn!=NULL)
		e->reset(vn);
}

void CachedExpr::toString(UCSChar *s){
	printf( "cached(");
	e->toString(s);
	printf(")");
}

bool CachedExpr::isNumerical(){
	return e->isNumerical();
}

bool CachedExpr::isNodeSet(){
	return e->isNodeSet();
}

bool CachedExpr::isString(){
	return e->isString();
}

bool CachedExpr::isBoolean(){
	return e->isBoolean();
}

bool CachedExpr::requireContextSize(){
	return e->requireContextSize();
}

void CachedExpr::setContextSize(int size){
	e->setContextSize(size);
}

void CachedExpr::setPosition(int pos){
	e->setPosition(pos);
}

int CachedExpr::adjust(int n){
	return e->adjust(n);
}

bool CachedExpr::isFinal(){
	return e->isFinal();
}
		
void CachedExpr::markCacheable(){
	e->markCacheable();
}

void CachedExpr::markCacheable2(){
	e->markCacheable2();
}

void CachedExpr::clearCache(){
	e->clearCache();
}
