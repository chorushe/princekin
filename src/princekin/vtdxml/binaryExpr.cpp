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
#include "binaryExpr.h"
#include "cachedExpr.h"
using namespace com_ximpleware;

BinaryExpr::BinaryExpr(Expr *e1, opType op1, Expr *e2):
left(e1),
op(op1),
right(e2),
fib1(NULL)
{
	switch(op){
	 	case OP_ADD:
		case OP_SUB:
		case OP_MULT:
		case OP_DIV:
		case OP_MOD: isNum = true; isBool = false; break;
		case OP_OR :
		case OP_AND:
		case OP_EQ:
		case OP_NE:
		case OP_LE:
		case OP_GE:
		case OP_LT:
		default: isNum= false; isBool = true;
	}
}
BinaryExpr::~BinaryExpr(){
	delete left;
	delete right;
	delete fib1;
	left = right = NULL;
	fib1 = NULL;
}
bool BinaryExpr::evalBoolean(VTDNav *vn){
	double dval;
	switch(op){
			case OP_OR: return left->evalBoolean(vn)
						 || right->evalBoolean(vn);
			case OP_AND:return left->evalBoolean(vn)
						 && right->evalBoolean(vn);
			case OP_EQ:
			case OP_NE:
			case OP_LE:
			case OP_GE:
			case OP_LT:
			case OP_GT:  return computeComp(op,vn);
			default: dval = evalNumber(vn);
				if (dval ==-0.0 || dval ==+0.0 || (dval!=dval))
					return false;
				return true;
	}
}
double BinaryExpr::evalNumber(VTDNav *vn){
	switch(op){
			case OP_ADD: return left->evalNumber(vn) + right->evalNumber(vn);
			case OP_SUB: return left->evalNumber(vn) - right->evalNumber(vn);
			case OP_MULT:return left->evalNumber(vn) * right->evalNumber(vn);
			case OP_DIV: return left->evalNumber(vn) / right->evalNumber(vn);
			case OP_MOD: return fmod(left->evalNumber(vn), right->evalNumber(vn));
			default	: if (evalBoolean(vn) == true)
						  return 1;
				return 0;

	}
}
int BinaryExpr::evalNodeSet(VTDNav *vn){
	throw XPathEvalException("can't evaluate nodeset on a binary expr");
	//return -1;
}
UCSChar* BinaryExpr::evalString(VTDNav *vn){
	double n = 0.0;
	bool b = false;
	UCSChar *tmp;
	if(isNumerical()){
		double d = evalNumber(vn);
		if (d != d){
			tmp = wcsdup(L"NaN");
			b= true;
		}
		else if ( d == 1/n){
			tmp = wcsdup(L"Infinity");
			b = true;
		}
		else if (d == -1/n){
			tmp = wcsdup(L"-Infinity");
			b  = true;
		}else
		tmp = new UCSChar[1<<8];

		if (tmp == NULL) {
			throw OutOfMemException("string allocation in evalString_be failed ");
		}
		if (b)
			return tmp;
		if (d == (Long) d){
			swprintf(tmp,64,L"%d",(Long) d);
		} else {
			swprintf(tmp,64,L"%f", d);
		}
		return tmp;
	} else {
		bool b = evalBoolean(vn);
		if (b)
			tmp= wcsdup(L"true");
		else
			tmp= wcsdup(L"false");
		if (tmp == NULL){
			throw OutOfMemException("String allocation failed in evalString_be");
		}
		return tmp;

	}
}

void BinaryExpr::reset(VTDNav *vn){
	left->reset(vn);
	right->reset(vn);
}
void BinaryExpr::toString(UCSChar *string){
	wprintf(L"(");
	left->toString(string);
	switch(op){
			case OP_ADD: wprintf(L" + "); break;
			case OP_SUB: wprintf(L" - "); break;
			case OP_MULT: wprintf(L" * "); break;
			case OP_DIV: wprintf(L" / "); break;
			case OP_MOD: wprintf(L" mod "); break;
			case OP_OR : wprintf(L" or ");break;
			case OP_AND: wprintf(L" and "); break;
			case OP_EQ: wprintf(L" = "); break;
			case OP_NE: wprintf(L" != "); break;
			case OP_LE: wprintf(L" <= "); break;
			case OP_GE: wprintf(L" >= "); break;
			case OP_LT: wprintf(L" < "); break;
			default: wprintf(L" > "); break;
	}
	right->toString(string);
	wprintf(L")");}

bool BinaryExpr::isNumerical(){return isNum;}
bool BinaryExpr::isNodeSet(){return false;}
bool BinaryExpr::isString(){return false;}
bool BinaryExpr::isBoolean(){return isBool;}

bool BinaryExpr::requireContextSize(){
	return left->requireContextSize()
		|| right->requireContextSize();
}
void BinaryExpr::setContextSize(int size){
	left->setContextSize(size);
	right->setContextSize(size);
}

void BinaryExpr::setPosition(int pos){
	left->setPosition(pos);
	right->setPosition(pos);
}

int BinaryExpr::adjust(int n){
	int i = left->adjust(n);
	int j = right->adjust(n);
	return min(i,j);
}

bool BinaryExpr::compEmptyNodeSet(opType op, UCSChar *s){
		if (op == OP_NE ){
	        if (wcslen(s)==0) {
	            return false;
	        } else
	            return true;
	    }else{
	        if (wcslen(s)==0) {
	            return true;
	        } else
	            return false;
	    }
}
bool BinaryExpr::computeComp( opType op,VTDNav *vn){
	int btemp;
	UCSChar *st1=NULL, *st2=NULL;

	if (left->isNodeSet() && right->isNodeSet()) {
		return compNodeSetNodeSet( vn, op);
	} else {
		// first argument is always numerical, second a node set
		if (left->isNumerical() && right->isNodeSet()){
			return compNumericalNodeSet( vn,op);
		}
	    if (left->isNodeSet() && right->isNumerical()){
			return compNodeSetNumerical(vn,op);
		}
		// first argument is always String, second a node set
		if (left->isString() && right->isNodeSet()){
			return compStringNodeSet(vn,op);
		}
		if (left->isNodeSet() && right->isString()){
			return compNodeSetString(vn,op);
		}

	}
	if (op == OP_EQ || op == OP_NE){
	if (left->isBoolean() || right->isBoolean()){
		if (op == OP_EQ)
			return left->isBoolean() == right->isBoolean();
		else
			return left->isBoolean() != right->isBoolean();
	}

	if (left->isNumerical() || right->isNumerical()){
		if (op == OP_EQ)
			return left->evalNumber(vn) == right->evalNumber(vn);
		else
			return left->evalNumber(vn) != right->evalNumber(vn);
	}
	st1 = left->evalString(vn);
	st2 = right->evalString(vn);
	/*if (st1 == NULL || st2 == NULL){
		btemp = FALSE;
	}else{*/
	btemp = wcscmp(st1, st2);
	/*}*/
	delete (st1);
	delete(st2);
	if (op == OP_EQ){
		return btemp==0;
	}
	else
		return btemp!=0;
	}
	return compNumbers(left->evalNumber(vn),
		right->evalNumber(vn),op);
}
bool BinaryExpr::compNumericalNodeSet(  VTDNav *vn, opType op){
	int i,i1,stackSize;
	double d;
	try {
		   d = left->evalNumber(vn);
            vn->push2();
			stackSize = vn->contextBuf2->size;
            while ((i = right->evalNodeSet(vn)) != -1) {
                i1 = getStringVal(vn,i);
                if (i1!=-1 && compareVNumber1(i1,vn,d,op)){
                    right->reset(vn);
                    vn->contextBuf2->size = stackSize;
                    vn->pop2();
                    return true;
                }
            }
            vn->contextBuf2->size = stackSize;
            vn->pop2();
            right->reset(vn);
            return false;
	} catch (...) {
		throw OtherException("Undefined behavior in evalBoolean_be");
	}
	return false;
}
bool BinaryExpr::compNodeSetNumerical(  VTDNav *vn, opType op){
	int i,i1,stackSize;
	double d;
	try {
		   d = right->evalNumber(vn);
            vn->push2();
			stackSize = vn->contextBuf2->size;
            while ((i = left->evalNodeSet(vn)) != -1) {
                i1 = getStringVal(vn,i);
                if (i1!=-1 && compareVNumber2(i1,vn,d,op)){
                    left->reset(vn);
                    vn->contextBuf2->size = stackSize;
                    vn->pop2();
                    return true;
                }
            }
            vn->contextBuf2->size = stackSize;
            vn->pop2();
            left->reset(vn);
            return false;
	} catch (...) {
		//fib1.clear();
		//fib2.clear();
		throw OtherException("Undefined behavior in evalBoolean_be");
		//throw new RuntimeException("Undefined behavior");
	}
	return false;
}
bool BinaryExpr::compStringNodeSet( VTDNav *vn, opType op){
	int i,i1,stackSize;
	UCSChar *s=NULL;
	bool b;
	try {
		    s = left->evalString(vn);
            vn->push2();
            stackSize = vn->contextBuf2->size;
            while ((i = right->evalNodeSet(vn)) != -1) {
                i1 = getStringVal(vn,i);
                if (i1 != -1){
					b = compareVString2(i1,vn,s,op);
					if (b){
						right->reset(vn);
						vn->contextBuf2->size = stackSize;
						vn->pop2();
						delete(s);
						return b;
					}
                }
            }
            vn->contextBuf2->size = stackSize;
            vn->pop2();
            right->reset(vn);
			//b = compEmptyNodeSet(op,s);
			delete(s);
            return false; //b;
	} catch (...) {
		throw OtherException("undefined run time behavior in computerEQNE");
	}
	return false;
}
bool BinaryExpr::compNodeSetString(  VTDNav *vn, opType op){
	int i,i1 = 0,stackSize;
	UCSChar *s=NULL;
	bool b;
	try {
		    s = right->evalString(vn);
            vn->push2();
            stackSize = vn->contextBuf2->size;
			while ((i = left->evalNodeSet(vn)) != -1) {
				i1 = getStringVal(vn,i);
				if (i1 != -1){
					b = compareVString2(i1,vn,s,op);
					if (b){
						left->reset(vn);
						vn->contextBuf2->size = stackSize;
						vn->pop2();
						delete(s);
						return b;
					}
                }
            }
            vn->contextBuf2->size = stackSize;
            vn->pop2();
            left->reset(vn);
            //b = compEmptyNodeSet(op, s);
			delete(s);
			return false;//b;
	} catch (...) {
		throw OtherException("undefined run time behavior in computerEQNE");
	}
	return false;
}
bool BinaryExpr::compNodeSetNodeSet(  VTDNav *vn, opType op){
	int i,i1,k,s1,stackSize;
	try {
		if (fib1 == NULL)
			fib1 = new FastIntBuffer(BUF_SZ_EXP);
		vn->push2();
		stackSize = vn->contextBuf2->size;
		while ((i = left->evalNodeSet(vn)) != -1) {
			i1 = getStringVal(vn,i);
			if (i1 != -1)
				fib1->append(i1);
		}
		left->reset(vn);
		vn->contextBuf2->size = stackSize;
		vn->pop2();
		vn->push2();
		stackSize = vn->contextBuf2->size;
		while ((i = right->evalNodeSet(vn)) != -1) {
			i1 = getStringVal(vn,i);
			if (i1 != -1){
				s1 = fib1->size;
				for (k = 0; k < s1; k++) {
					bool b = compareVV(fib1->intAt(k),vn,i1,op);
					if (b){
						//delete(fib1);
						fib1->clear();
						vn->contextBuf2->size = stackSize;
						vn->pop2();
						right->reset(vn);
						return true;
					}
				}
			}
		}
		vn->contextBuf2->size = stackSize;
		vn->pop2();
		right->reset(vn);
		fib1->clear();
		return false;

	} catch (std::bad_alloc&) {
		throw;
	}catch (...){
		if(fib1!=NULL)
			delete(fib1);
		fib1=NULL;

		throw OtherException("undefined run time behavior in computerEQNE");
	}
	return false;
}
bool BinaryExpr::compNumbers( double d1, double d2, opType op){
	    switch (op) {
        case OP_LE:
            return d1 <= d2;
        case OP_GE:
            return d1 >= d2;
        case OP_LT:
            return d1 < d2;
        case OP_GT:
            return d1 > d2;
	default:
            return false;
        }
        //return false;
}
bool BinaryExpr::compareVNumber1( int i, VTDNav *vn, double d, opType op){
		double d1 = vn->parseDouble(i);
	    switch (op){
	    	case OP_EQ:
	    	    return d == d1;
	    	case OP_NE:
	    		return d != d1;
	    	case OP_GE:
	    	    return d >= d1;
	    	case OP_LE:
	    	    return d <= d1;
	    	case OP_GT:
	    	    return d > d1;
	    	default:
	    	    return d < d1;
	    }
}
bool BinaryExpr::compareVNumber2( int i, VTDNav *vn, double d, opType op){
	    double d1 = vn->parseDouble(i);
	    switch (op){
	    	case OP_EQ:
	    	    return d1 == d;
	    	case OP_NE:
	    		return d1 != d;
	    	case OP_GE:
	    	    return d1 >= d;
	    	case OP_LE:
	    	    return d1 <= d;
	    	case OP_GT:
	    	    return d1 > d;
	    	default:
	    	    return d1 < d;
	    }
}
int BinaryExpr::getStringVal(VTDNav *vn, int i){
	tokenType t = vn->getTokenType(i);
	if (t == TOKEN_STARTING_TAG){
		int i1 = vn->getText();
		return i1;
	}
	else if (t == TOKEN_ATTR_NAME
		|| t == TOKEN_ATTR_NS ||t==TOKEN_PI_NAME)
		return i+1;
	else
		 return i;
}
bool BinaryExpr::compareVString1(int k, VTDNav *vn, UCSChar *s, opType op){
	int i = vn->compareTokenString(k, s);
	switch (i) {
		case -1:
			if (op == OP_NE || op == OP_LT || op == OP_LE) {
				return true;
			}
			break;
		case 0:
			if (op == OP_EQ || op == OP_LE || op == OP_GE) {
				return true;
			}
			break;
		case 1:
			if (op == OP_NE || op == OP_GE || op == OP_GT) {
				return true;
			}
	}
	return false;
}
bool BinaryExpr::compareVString2(int k, VTDNav *vn, UCSChar *s, opType op){
	int i = vn->compareTokenString(k, s);
	switch(i){
			case -1:
				if (op== OP_NE || op == OP_GT || op == OP_GE){
					return true;
				}
				break;
			case 0:
				if (op==OP_EQ || op == OP_LE || op == OP_GE ){
					return true;
				}
				break;
			case 1:
				if (op == OP_NE || op==OP_LE  || op == OP_LT ){
					return true;
				}
	}
	return false;

}
bool BinaryExpr::compareVV(int k,  VTDNav *vn, int j,opType op){
	int i = vn->compareTokens(k, vn, j);
	switch(i){

			case 1:
				if (op == OP_NE || op==OP_GE  || op == OP_GT ){
					return true;
				}
				break;
			case 0:
				if (op==OP_EQ || op == OP_LE || op == OP_GE ){
					return true;
				}
				break;
			case -1:
				if (op== OP_NE || op == OP_LT || op == OP_LE){
					return true;
				}
	}
	return false;
}

bool BinaryExpr::isFinal(){return left->isFinal() && right->isFinal();}
		
void BinaryExpr::markCacheable(){
	left->markCacheable();
	right->markCacheable();		
}
void BinaryExpr::markCacheable2(){
	if (left->isFinal() && left->isNodeSet()){
		CachedExpr *ce = new CachedExpr(left);
		left = ce;
	} 
	left->markCacheable2();
	if (right->isFinal() && right->isNodeSet()){
		CachedExpr *ce = new CachedExpr(right);
		right = ce;
	} 
	right->markCacheable2();	
}
void BinaryExpr::clearCache(){
	left->clearCache();
	right->clearCache();
}
