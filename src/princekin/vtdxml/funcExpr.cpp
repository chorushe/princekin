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
#include "funcExpr.h"
#include "cachedExpr.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include "vtdGen.h"
using namespace com_ximpleware;

static double myround(double v);

FuncExpr::FuncExpr(funcName oc, AList *a):
opCode(oc),
al(a),
isNum (false),
isBool(false),
isStr(false),
isNode(false),
argCount1(argCount()),
a(0),
newVN(NULL),
xslVN(NULL),
vg(NULL)
{

	 switch(oc){
			case FN_LAST: 			isNum = true;break;
			case FN_POSITION: 		isNum = true;break;
			case FN_COUNT: 			isNum = true;break;
			case FN_LOCAL_NAME: 		isStr = true; break;
			case FN_NAMESPACE_URI: 	isStr = true; break;
			case FN_NAME: 			isStr = true; break;
			case FN_STRING: 			isStr = true; break;
			case FN_CONCAT: 			isStr = true; break;
			case FN_STARTS_WITH:		isBool= true;break;
			case FN_CONTAINS: 		isBool= true;break;
			case FN_SUBSTRING_BEFORE: isStr = true; break;
			case FN_SUBSTRING_AFTER: 	isStr = true; break;
			case FN_SUBSTRING: 		isStr = true; break;
			case FN_STRING_LENGTH: 	isNum = true;break;
			case FN_NORMALIZE_SPACE: 	isStr = true; break;
			case FN_TRANSLATE:	 	isStr= true;break;
			case FN_BOOLEAN: 			isBool =true;break;
			case FN_NOT: 			    isBool =true;break;
			case FN_TRUE: 			isBool = true;break;
			case FN_FALSE: 			isBool = true;break;
			case FN_LANG: 			isBool = true;break;
			case FN_NUMBER:			isNum = true;break;
			case FN_SUM: 			    isNum = true;break;
			case FN_FLOOR: 			isNum = true;break;
			case FN_CEILING: 			isNum = true;break;
			case FN_ROUND:		   isNum = true;break;
			case FN_ABS:		   isNum = true;break;
			case FN_ROUND_HALF_TO_EVEN:		   isNum = true;break;
			case FN_ROUND_HALF_TO_ODD:		   isNum = true;break;
			case FN_CODE_POINTS_TO_STRING: 	   isStr = true;break;
			case FN_COMPARE:		   isBool = true; break;
			case FN_UPPER_CASE:		   isStr = true; break;
			case FN_LOWER_CASE:		   isStr = true; break;
			case FN_ENDS_WITH:		   isBool = true; break;
			case FN_QNAME:		   isStr = true; break;
			case FN_LOCAL_NAME_FROM_QNAME:		   isStr = true; break;
			case FN_NAMESPACE_URI_FROM_QNAME:	   isStr = true; break;
			case FN_NAMESPACE_URI_FOR_PREFIX:	   isStr = true; break;
			case FN_RESOLVE_QNAME:		   isStr = true; break;
			case FN_IRI_TO_URI:			   isStr = true; break;
			case FN_ESCAPE_HTML_URI:	   isStr = true; break;
			case FN_ENCODE_FOR_URI:	isStr  = true; break;
			case FN_MATCH_NAME:		isBool  =true; break;
			case FN_MATCH_LOCAL_NAME: isBool =true;break;
			case FN_NOT_MATCH_NAME:		isBool  =true; break;
			case FN_NOT_MATCH_LOCAL_NAME: isBool =true;break;
			case FN_GENERATE_ID : 	isStr  = true; break;
			case FN_FORMAT_NUMBER:  	isStr  = true;break;
			case FN_KEY:				isNode  = true; state = XPATH_EVAL_START; vg = new VTDGen();break;
			case FN_DOCUMENT:			isNode  = true; state = XPATH_EVAL_START; vg = new VTDGen();break;
			case FN_CURRENT:			isNode  = true; state = XPATH_EVAL_START; vg = new VTDGen();break;
			case FN_SYSTEM_PROPERTY: 	isStr  = true; break;
			case FN_ELEMENT_AVAILABLE: isBool  = true; break;
			//case FN_FUNCTION_AVAILABLE: isBool  = true; break;
			default:		isBool = true; break;	
	  }

}
FuncExpr::~FuncExpr(){
	if (al!=NULL)
		delete(al);
	if (this->xslVN!=NULL){
		delete xslVN->XMLDoc;
		delete xslVN;
	}
	if (this->newVN!=NULL){
	
	}

	al = NULL;
}
bool FuncExpr::evalBoolean(VTDNav *vn){
	switch(opCode){
			case FN_STARTS_WITH:
				/*if (argCount()!=2){
					throw InvalidArgumentException("starts-with()'s <funcExpr> argument count is invalid");
				}*/
				return startsWith(vn);

			case FN_CONTAINS:
				/*if (argCount()!=2){
					throw InvalidArgumentException("contains()'s <funcExpr> argument count is invalid");
				}*/
				return contains(vn);

			case FN_TRUE:
				/*if (argCount()!=0){
					throw InvalidArgumentException("true()'s <funcExpr> argument count is invalid");
				}*/
				return true;
			case FN_FALSE:
				/*if (argCount()!=0){
					throw InvalidArgumentException("false()'s <funcExpr> argument count is invalid");
				}*/
				return false;
			case FN_BOOLEAN:
				/*if (argCount()!=1){
					throw InvalidArgumentException("boolean()'s <funcExpr> argument count is invalid");
				}*/
				return al->e->evalBoolean( vn);
			case FN_NOT:
				/*if (argCount()!=1){
					throw InvalidArgumentException("not()'s <funcExpr> argument count is invalid");
				}*/
				return !al->e->evalBoolean(vn);
			case FN_LANG:
				/*if (argCount()!=1){
					throw InvalidArgumentException("boolean()'s <funcExpr> argument count is invalid");
				}*/
				return lang( vn, al->e->evalString(vn));

			case FN_COMPARE:
				throw OtherException("functions not yet supported");
			
			case FN_ENDS_WITH:
				/*if (argCount()!=2){
					throw InvalidArgumentException("ends-with()'s <funcExpr> argument count is invalid");
				}*/
				return endsWith(vn);
			case FN_MATCH_NAME:return matchName(vn);
		    case FN_MATCH_LOCAL_NAME: return matchLocalName(vn);
		    case FN_NOT_MATCH_NAME:return !matchName(vn);
		    case FN_NOT_MATCH_LOCAL_NAME: return !matchLocalName(vn);
		    case FN_ELEMENT_AVAILABLE: return isElementAvailable(vn);
		    case FN_FUNCTION_AVAILABLE: return isElementAvailable(vn);
				
			default:
				if (isNumerical()){
					double d = evalNumber(vn);
					if (d==0 || d!=d)
						return false;
					return true;
				}else{
					UCSChar *tmp = evalString(vn);
					size_t len = wcslen(tmp);
					delete(tmp);
					return len!=0;
				}
	}
}
double FuncExpr::evalNumber(VTDNav *vn){
	int ac;
	size_t len;
	UCSChar *tmpString = NULL;
	switch(opCode){
			case FN_LAST:  /*if (argCount()!=0 ){
								throw InvalidArgumentException("last()'s  <funcExpr> argument count is invalid");
							}*/
						   return contextSize;
			case FN_POSITION:   /*if (argCount()!=0 ){
									throw InvalidArgumentException("position()'s  <funcExpr> argument count is invalid");
								}*/

								return position;

			case FN_COUNT: 		return count(vn);

			case FN_NUMBER:		/*if (argCount()!=1){
									throw InvalidArgumentException("number()'s  <funcExpr> argument count is invalid");
								}*/
								return al->e->evalNumber(vn);

			case FN_SUM:	    return sum(vn);
			case FN_FLOOR: 		/*if (argCount()!=1 ){
									throw InvalidArgumentException("floor()'s  <funcExpr> argument count is invalid");
								}*/
								return floor(al->e->evalNumber(vn));

			case FN_CEILING:	/*if (argCount()!=1 ){
									throw InvalidArgumentException("ceiling()'s  <funcExpr> argument count is invalid");
								}*/
								return ceil(al->e->evalNumber(vn));

			case FN_STRING_LENGTH:
								ac = argCount1;
			    				if (ac == 0){
									
			    				    try{
			    				        if (vn->atTerminal == true){
			    				            tokenType type = vn->getTokenType(vn->LN);

											if (type == TOKEN_ATTR_NAME
			    				                || type == TOKEN_ATTR_NS){
													len = vn->getStringLength(vn->LN+1);
			    				                //return vn.toString(vn.LN+1).length();
			    				            } else {
												    len = vn->getStringLength(vn->LN);
			    				                //return vn.toString(vn.LN).length();
			    				            }
											return len;

			    				        }else {
			    				            int i = vn->getText();
			    				            if (i==-1)
			    				                return 0;
											else {
												return vn->getStringLength(i);
											}
			    				        }
			    				    }catch (...){
			    				        return 0;
			    				    }
			    				} else if (ac == 1){
									tmpString = al->e->evalString(vn);
									len = wcslen(tmpString);
									delete(tmpString);
			    				    return len;
			    				} else {
									throw InvalidArgumentException("string-length()'s  <funcExpr> argument count is invalid");
			    				}

			case FN_ROUND: 	/*if (argCount()==1 )*/
								return myround(al->e->evalNumber(vn));
							/*else {
								throw InvalidArgumentException("round()'s  <funcExpr> argument count is invalid");
							}*/
			case FN_ABS:    //if (argCount()==1 )
								return fabs(al->e->evalNumber(vn));
							/*else {
								throw InvalidArgumentException("abs()'s  <funcExpr> argument count is invalid");
							}*/
			case FN_ROUND_HALF_TO_EVEN:
							return roundHalfToEven( vn);
			case FN_ROUND_HALF_TO_ODD:
				    throw OtherException("functions not yet supported");
			
			default: if (isBool){
						if (evalBoolean(vn))
							return 1;
						else
							return 0;
					 }else {
                         	double d  = 0, result;
							UCSChar *string = evalString(vn);
							UCSChar *temp;
							if (wcslen(string)==0){
								delete(string);
								return d/d;
							}

							result = wcstod(string,&temp);
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
							delete(string);
							return result;

					 }

					 }
}
int FuncExpr::evalNodeSet(VTDNav *vn){
	switch (opCode) {
		case FN_CURRENT:
			if (state == XPATH_EVAL_START) {
				vn->loadCurrentNode();
				state = XPATH_EVAL_END;
				return vn->getCurrentIndex2();
			} else {
				return -1;
			}
			// break;
		case FN_DOCUMENT:
			if (argCount1 == 1) {
				if (!al->e->isNodeSet()) {
					if (state == XPATH_EVAL_START) {
						UCSChar *s = al->e->evalString(vn);
						Long l= getBytes_UTF8(s);
						//delete s;
						int len=(int)(l>>32);
						char *s1 = (char *)l;
						if (wcslen(s) == 0) {
							newVN = xslVN;
							newVN->context[0] = -1;
						} else if (vg->parseFile(true, s1)) {
							newVN = vg->getNav();
							
							newVN->context[0] = -1;
							newVN->URIName = s;
						} else {
							state = XPATH_EVAL_END;
							delete s;
							delete s1;
							return -1;
						}
						state = XPATH_EVAL_END;
						delete s;
						delete s1;
						return 0;
					} else {
						return -1;
					}
				} else {
					try {
						if (state != XPATH_EVAL_END) {
							a = al->e->evalNodeSet(vn);
							if (a != -1) {
								UCSChar *s = vn->toString(getStringVal(vn,a));
								Long l= getBytes_UTF8(s);
								//delete s;
								int len=(int)(l>>32);
								char *s1 = (char *)l;
								if (wcslen(s) == 0) {
									newVN = xslVN;
									newVN->context[0] = -1;
								} else if (vg->parseFile(true,s1)) {
									newVN = vg->getNav();
									newVN->context[0] = -1;
									newVN->URIName = s;
								} else {
									state = XPATH_EVAL_END;
									delete s;
									delete s1;
									return -1;
								}
								state = XPATH_EVAL_END;
								delete s;
								delete s1;
								return 0;
							} else {
								state = XPATH_EVAL_END;
								return -1;
							}
						} else
							return -1;
					} catch (NavException e) {

					}
				}
			}

			break;
		case FN_KEY:
			throw new XPathEvalException(" key() not yet implemented ");
			// break;
		}
		throw new XPathEvalException(" Function Expr can't eval to node set ");
	return -1;
}
UCSChar* FuncExpr::evalString(VTDNav *vn){
	UCSChar *tmp;

	switch(opCode){
			case FN_LOCAL_NAME:
				return getLocalName(vn);
			case FN_NAMESPACE_URI:
				return getNameSpaceURI(vn);
			case FN_NAME:
				return getName(vn);
			case FN_STRING:
				return getString(vn);

			case FN_CONCAT:
				return concat(vn);
			case FN_SUBSTRING_BEFORE: return subStringBefore(vn);
			case FN_SUBSTRING_AFTER: return subStringAfter(vn);

			case FN_SUBSTRING: 	return subString(vn);
			case FN_TRANSLATE:  return translate(vn);
			
			case FN_NORMALIZE_SPACE: return normalizeString(vn);

			case FN_CODE_POINTS_TO_STRING:throw OtherException("functions not yet supported");
  			case FN_UPPER_CASE: return upperCase(vn);
  			case FN_LOWER_CASE: return lowerCase(vn);
  			case FN_QNAME:
  			case FN_LOCAL_NAME_FROM_QNAME:
   			case FN_NAMESPACE_URI_FROM_QNAME:
  			case FN_NAMESPACE_URI_FOR_PREFIX:
  			case FN_RESOLVE_QNAME:
  			case FN_IRI_TO_URI:
  			case FN_ESCAPE_HTML_URI:
  			case FN_ENCODE_FOR_URI:
				throw OtherException("functions not yet supported");
			case FN_GENERATE_ID: return generateID(vn);
			case FN_FORMAT_NUMBER: return formatNumber(vn);
			case FN_SYSTEM_PROPERTY: return getSystemProperty(vn);
			default: if (isBoolean()){
			    		if (evalBoolean(vn)== true)
			    		    tmp = wcsdup(L"true");
			    		else
			    		    tmp = wcsdup(L"false");
						if (tmp == NULL){
							throw OutOfMemException(
								"allocate string failed in funcExpr's evalString()");
						}
						return tmp;
					 } else {
						 double d1 = 0;
						 double d = evalNumber(vn);
						 bool b = false;
							if (d != d){
								tmp = wcsdup(L"NaN");
								b = true;
							}
							else if ( d == 1/d1){
								tmp = wcsdup(L"Infinity");
								b = true;
							}
							else if (d == -1/d1){
								tmp = wcsdup(L"-Infinity");
								b = true;
							}	else
								tmp = new UCSChar[1<<8];

							if (tmp == NULL) {
								throw OutOfMemException("allocate string failed in funcExpr's evalString()");
							}
							if(b)
								return tmp;

						    if (d == (Long) d){
								swprintf(tmp,64,L"%d",(Long)d);
							} else {
								swprintf(tmp,64,L"%f", d);
							}
							return tmp;
					 }
	  }
}

void FuncExpr::reset(VTDNav *vn){
	a = 0;
	state= XPATH_EVAL_START;
	//contextSize = 0;
	if (al!=NULL)
		al->reset_al(vn);
}
void FuncExpr::toString(UCSChar* string){
	if (al == NULL){
		wprintf(L"%ls()",fname());
	}else {
		wprintf(L"%ls(",fname());
		al->toString_al(string);
		wprintf(L")");
	}
}

bool FuncExpr::isNumerical(){return isNum;}
bool FuncExpr::isNodeSet(){return isNode;}
bool FuncExpr::isString(){return isStr;}
bool FuncExpr::isBoolean(){return isBool;}

bool FuncExpr::requireContextSize(){
	    if (opCode == FN_LAST)
	        return true;
	    else {
			AList *temp = al;
	        while(temp!=NULL){
	            if (temp->e->requireContextSize()){
	                return true;
	            }
	            temp = temp->next;
	        }
	    }
	    return false;
}
void FuncExpr::setContextSize(int s){
	    if (opCode == FN_LAST){
	        contextSize = s;
	        //System.out.println("contextSize: "+size);
	    } else {
	        AList *temp = al;
	        //boolean b = false;
	        while(temp!=NULL){
	            temp->e->setContextSize(s);
	            temp = temp->next;
	        }
	    }
}

void FuncExpr::setPosition(int pos){
	    if (opCode == FN_POSITION){
	        position = pos;
	        //System.out.println("PO: "+size);
	    } else {
	        AList *temp = al;
	        while(temp!=NULL){
	            temp->e->setPosition(pos);
	            temp = temp->next;
	        }
	    }
}
int FuncExpr::adjust(int n){
	switch(opCode){
		case FN_COUNT:
		case FN_SUM:
			return al->e->adjust(n);

		default:
			return 0;
	}
}

double FuncExpr::sum( VTDNav *vn){
	double d = 0;
	double n=0;
	int i1;
	//if (argCount() != 1
	//	|| al->e->isNodeSet() == false){
	//		throw InvalidArgumentException("sum() <funcExpr> 's argument has to be a node set ");
	//	}

	vn->push2();
	try {
		a = 0;
		while ((a = al->e->evalNodeSet(vn)) != -1) {
			tokenType t = vn->getTokenType(this->a);
			if (t == TOKEN_STARTING_TAG){
				i1 = vn->getText();
				if (i1!=-1)
					d += vn->parseDouble(i1);
				if (d!=d)
					break;
			}
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS){
					d += vn->parseDouble(a+1);
					if (d!=d)
						break;
				}
			else if (t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL){
					d += vn->parseDouble(a);
					if (d != d)
						break;
				}
				//    fib1.append(i);
		}
		al->e->reset(vn);
		vn->pop2();
		return d;
	} catch (...) {
		al->e->reset(vn);
		vn->pop2();
	}
	return n/n;
}
int FuncExpr::argCount(){
	AList *temp = al;
	int count = 0;
	while(temp!=NULL){
		count++;
		temp = temp->next;
	}
	return count;
}
int FuncExpr::count( VTDNav *vn){
	int a = -1;

	/*if (argCount()!=1
		||(al->e->isNodeSet)()==false){
			throw InvalidArgumentException("count <funcExpr> 's argument has to be a node set ");
		}*/

	vn->push2();
	try{
		a = 0;
		al->e->adjust(vn->vtdSize);
		while(al->e->evalNodeSet(vn)!=-1){
			a ++;
		}
	}catch(...){
		printf("something wrong in count");
	}
	al->e->reset(vn);
	vn->pop2();
	return a;
}
UCSChar* FuncExpr::getLocalName( VTDNav *vn){
	int index;
	tokenType type;
	if (argCount1== 0){
		try{
			index = vn->getCurrentIndex();
			type = vn->getTokenType(index);
			if (vn->ns && (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME)) {
				int offset = vn->getTokenOffset(index);
				int length = vn->getTokenLength(index);
				if (length < 0x10000){
					if (vn->localNameIndex!=index){
						vn->localNameIndex = index;
						if (vn->localName !=NULL)
							delete vn->localName;
						vn->localName = vn->toRawString(index);
					}
					return vn->localName;
				}
				else {
					int preLen = length >> 16;
					int QLen = length & 0xffff;
					if (preLen != 0){
						     
						if (vn->localNameIndex != index){
                        		vn->localNameIndex = index;
								if (vn->localName!=NULL)
									delete vn->localName;
                        		vn->localName = vn->toRawString(offset + preLen+1, QLen
                                        - preLen - 1);
                        
						}
                        
						return vn->localName;
						//return vn->toRawString(offset + preLen+1, QLen
						//- preLen - 1);
					}else {
						if (vn->localNameIndex != index){
							vn->localNameIndex = index;
							if (vn->localName!=NULL)
								delete vn->localName;
							vn->localName = vn->toRawString(offset, QLen);
						}
						return vn->localName;
					}
				}
			} else if (type ==TOKEN_PI_NAME){
				if (vn->localNameIndex != index){
                		vn->localNameIndex = index;
						if (vn->localName!=NULL)
							delete vn->localName;
                		vn->localName = vn->toRawString(index);
                	}
                    return vn->localName;
			}
		}
		catch(...){

		}
	}
	else if (argCount1 == 1){
		int a = -1;
		tokenType type;
		vn->push2();
		try{
			a = al->e->evalNodeSet(vn);
			al->e->reset(vn);
			vn->pop2();
		}catch(...){
			al->e->reset(vn);
			vn->pop2();
		}
		if (a == -1 || vn->ns == false)
			return createEmptyString();
		type = vn->getTokenType(a);

		if (type != TOKEN_STARTING_TAG && type != TOKEN_ATTR_NAME)
			return createEmptyString();
		try {
			int offset = vn->getTokenOffset(a);
			int length = vn->getTokenLength(a);
			if (length < 0x10000)
				return vn->toRawString(a);
			else {
				int preLen = length >> 16;
				int QLen = length & 0xffff;
				if (preLen != 0)
					return vn->toRawString(offset + preLen+1,
					QLen - preLen - 1);
			}
		} catch (...) {
			 // this will almost never occur
		}
	} else {
		throw InvalidArgumentException("local-name()'s  <funcExpr> argument count is invalid");
	}
	return createEmptyString();
}
UCSChar* FuncExpr::getName( VTDNav *vn){
	int a;
	int type;
	if (argCount1== 0){
		a = vn->getCurrentIndex();
		type = vn->getTokenType(a);
		if ( type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_PI_NAME){
			try{
				if (vn->nameIndex!=a){
					if (vn->name !=NULL)
						delete vn->name;
	            	vn->name = vn->toRawString(a);
	            	vn->nameIndex = a;	
	            }
            	return vn->name;	  
				//return vn->toString(a);
			}catch(...){
				return createEmptyString();
			}
		}
		else
			return createEmptyString();
	}
	else if (argCount1 == 1){
		a = -1;
		vn->push2();
		try{
			a = al->e->evalNodeSet(vn);
			al->e->reset(vn);
			vn->pop2();
		}catch(...){
			al->e->reset(vn);
			vn->pop2();
		}
		try{
			if (a == -1 || vn->ns == false){
				return createEmptyString();
			}
			else{
				int type = vn->getTokenType(a);
				if ( type== TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME || type == TOKEN_PI_NAME)
					return vn->toRawString(a);
				return createEmptyString();
			}
		}catch(...){
		}

		//return al->e->evalString(al->e, vn);
	} else { 
		throw InvalidArgumentException("name()'s  <funcExpr> argument count is invalid");
	}
	return createEmptyString();
}
UCSChar* FuncExpr::getNameSpaceURI( VTDNav *vn){
	if (argCount1== 0){
		try{
			int i = vn->getCurrentIndex();
			int type = vn->getTokenType(i);
			if (vn->ns && (type == TOKEN_STARTING_TAG
				|| type == TOKEN_ATTR_NAME)) {
				int a = vn->lookupNS();
				if (a == 0)
					return createEmptyString();
				else
					return vn->toString(a);
			}
		}catch (...){
		}
	}
	else if (argCount1 == 1 &&  al->e->isNodeSet()){
		int a = -1, size;
		vn->push2();
		size = vn->contextBuf2->size;
		try{
			a = al->e->evalNodeSet(vn);
			if (a==-1 || vn->ns == false){
				return createEmptyString();
			}
			else {
				int type = vn->getTokenType(a);
				if (type == TOKEN_STARTING_TAG
					|| type == TOKEN_ATTR_NAME){
						UCSChar *tmp = vn->toString(vn->lookupNS());
						al->e->reset(vn);
						vn->contextBuf2->size = size;
						vn->pop2();
						return tmp;
				}
			}
		}catch(...){
			
		}
		al->e->reset(vn);
		vn->contextBuf2->size = size;
		vn->pop2();
	} else {
		throw InvalidArgumentException("namespace-uri()'s  <funcExpr> argument count is invalid");
	}
	return createEmptyString();
}
UCSChar* FuncExpr::getString( VTDNav *vn){
	if (argCount1== 0){
		try{
			if (vn->atTerminal){
				if (vn->getTokenType(vn->LN) == TOKEN_CDATA_VAL )
					return vn->toRawString(vn->LN);
				return vn->toString(vn->LN);
			}
			return vn->getXPathStringVal();
		}
		catch(...){
			 // this will almost never occur
		}
	}
	else if (argCount1 == 1){
		return al->e->evalString(vn);
	} else {
		throw InvalidArgumentException(
			"string()'s  <funcExpr> argument count is invalid");
	}
	return createEmptyString();
}
bool FuncExpr::contains( VTDNav *vn){
	bool b=false;
	UCSChar* s2 = al->next->e->evalString(vn);
	//UCSChar* s3 = NULL;
	if(al->e->isNodeSet()){
		//UCSChar* s1 = al->e->evalString( vn);
		int a = evalFirstArgumentListNodeSet(vn);
		if (a==-1)
			return false;
		try {
			int t=vn->getTokenType(a);
			if (t!=TOKEN_STARTING_TAG && t!=TOKEN_DOCUMENT)
				b= vn->contains(a, s2);
			else
				b= vn->XPathStringVal_Contains(a,s2);
			delete s2;
			return b;
		}catch (...){
			delete s2;
			return false;
		}		
	}
	
	UCSChar* s1 = al->e->evalString( vn);
	if (wcsstr(s1,s2)!=NULL)
		b = true;
	delete(s1);
	delete(s2);
    return b;
}
UCSChar* FuncExpr::concat( VTDNav *vn){
	size_t totalLen = 0,capacity = 16;
	size_t len = 0;
	UCSChar *result = NULL, *s = NULL, *tempBuf = NULL;
	result = new UCSChar[1<<4];
	if (result==NULL){
		throw OutOfMemException("String allocation failed in concat");
	}
	result[0]=0;/*end of string set*/

	//if (argCount()>=2){
		AList* temp = al;
		while(temp!=NULL){
			/*perform concatenation here*/
			s = temp->e->evalString(vn);
			len = wcslen(s);
			totalLen += len;
			if (totalLen+1> capacity){
				while(totalLen+1> capacity)
					capacity = (capacity<<1);
				tempBuf = result;
				//delete result;
				result = new UCSChar[capacity];
				memcpy(result,tempBuf,capacity>>1);
				delete tempBuf;
				//	= realloc(result, capacity*sizeof(UCSChar));
				if (result == NULL) {
					delete(tempBuf);
					delete(s);
					throw OutOfMemException("String allocation failed in concat");
				}
			}
			wcscat(result,s);
			temp = temp->next;
		}
		return result;
	//}
	//throw InvalidArgumentException("concat()'s <funcExpr> argument count is invalid");
	//return NULL;
}
bool FuncExpr::startsWith( VTDNav *vn){
	bool b=false;
	UCSChar* s2 = al->next->e->evalString(vn);
	if (al->e->isNodeSet()){
			//boolean b = false;
			int a = evalFirstArgumentListNodeSet(vn);
			
	        if (a==-1)
	        	return wcslen(s2)==0;
	        else{
	        	try{
	        		int t = vn->getTokenType(a);
	        		if (t!=TOKEN_STARTING_TAG&& t!=TOKEN_DOCUMENT)
	        			b= vn->startsWith(a, s2);
	        		else 
	        			b= vn->XPathStringVal_StartsWith(a,s2);
					delete s2;
					return b;
	        	}catch(...){
					delete s2;
					return false;
	        	}
	        	
	        }								
		} 
	//UCSChar* s3 = NULL;
	UCSChar* s1 = al->e->evalString( vn);
	
	if (wcsstr(s1,s2)==s1)
		b = true;
	delete(s1);
	delete(s2);
    return b;
}
bool FuncExpr::endsWith( VTDNav *vn){
	bool b=false;
	UCSChar* s2 = al->next->e->evalString(vn);
	if (al->e->isNodeSet()){
		int a = evalFirstArgumentListNodeSet(vn);
		if (a==-1)
			return wcslen(s2)==0;
		else{
			try{
				int t=vn->getTokenType(a);
				if (t!=TOKEN_STARTING_TAG && t!=TOKEN_DOCUMENT)
					b= vn->endsWith(a, s2);
				else
					b= vn->XPathStringVal_EndsWith(a, s2);
				delete s2;
				return b;
			}catch(...){
				delete s2;
				return false;
			}
			
			
		}								
	}	
    UCSChar* s1 = al->e->evalString(vn);
	
	size_t l1 = wcslen(s1);
	size_t l2 = wcslen(s2);
	//UCSChar* s3 = NULL;
	
	if (wcsstr(s1+(l1-l2),s2)== (s1+l1-l2)){
		//
		b = true;

	}
	delete(s1);
	delete(s2);
    return b;
}
UCSChar* FuncExpr::subString( VTDNav *vn){
	UCSChar *str;
	size_t len;
	if (argCount1==2){
		double d1;
		int temp;
		
		str = al->e->evalString(vn);
		len = wcslen(str);
		d1 = al->next->e->evalNumber(vn);
		
		if (d1!=d1 || d1>len)
			return createEmptyString();
		
		d1--; //change it to zero based index
		
		temp = max((int)d1,0);

		len -= temp; //calculate the length to copy
		wcsncpy(str,str+temp,len);
		*(str+len) = 0;
		
		return str;

	} else if (argCount1==3){
		double d1, d2;
		int temp1, endIdx;
		size_t cpLen;

		str = al->e->evalString(vn);
		len = wcslen(str);

		d1 = floor(al->next->e->evalNumber(vn)+0.5);
		d2 = floor(al->next->next->e->evalNumber(vn)+0.5);

		if ((d1+d2)!=(d1+d2) || d1>len)
			return createEmptyString();

		d1--;//change it to zero based index
		temp1 = max((int)d1,0);

		endIdx = (int)d1 + (int)d2;
		cpLen = min((int)len, endIdx);

		cpLen -= temp1; //calculate copy length

		wcsncpy(str,str+temp1,cpLen);
		*(str+cpLen)=0;

		return str;
	}
	throw InvalidArgumentException(
		"substring()'s <funcExpr> argument count is invalid");
	return NULL;
}
UCSChar* FuncExpr::normalizeString( VTDNav *vn){
	if (argCount1 == 0){
		UCSChar *s = NULL;
		try{
			if (vn->atTerminal)
			{
				int ttype = vn->getTokenType(vn->LN);
				if (ttype == TOKEN_CDATA_VAL)
					s =vn->toRawString(vn->LN);
				else if (ttype == TOKEN_ATTR_NAME
					|| ttype == TOKEN_ATTR_NS){
						s = vn->toNormalizedString(vn->LN+1);
				}else{
					s= vn->toNormalizedString(vn->LN);
				}
			}else{
				int i = vn->getCurrentIndex();
	            	int t = vn->getTokenType(i);
	            	if (t==TOKEN_STARTING_TAG || t==TOKEN_DOCUMENT){
	            		s = vn->toNormalizedXPathString(i);
	            	}else
	                s= vn->toNormalizedString(i);
			}
			return s;
		}
		catch(...){
			return createEmptyString();
		}
	} else if (argCount1 ==1){
		UCSChar *s=NULL;
	    if (al->e->isNodeSet()){
			//boolean b = false;
			int a = evalFirstArgumentListNodeSet(vn);
		    if (a==-1)
		       	return wcsdup(L""); 
		    else {		        	
		       	try{
		        		int t = vn->getTokenType(a);
		        		if (t==TOKEN_STARTING_TAG || t==TOKEN_DOCUMENT){
		        			s =  vn->toNormalizedXPathString(a);
		        		}else
		        			s = vn->toNormalizedString(a); 
		        	} catch (...){
		        	}
		        	return s;	
		        }	    	
	    	}
	    	else {
				UCSChar *s = al->e->evalString(vn);
				return normalize(s);
	    	}
		
	}
	{
		throw InvalidArgumentException("normalize-space()'s <funcExpr> argument count is invalid");
		return NULL;
	}
}
UCSChar* FuncExpr::subStringBefore( VTDNav *vn){
if (argCount1 == 2){
		UCSChar* s1 = al->e->evalString(vn);
		UCSChar* s2 = al->next->e->evalString(vn);
		UCSChar* temp = NULL;
		if ((temp=wcsstr(s1,s2))!=NULL){
			*(temp) = 0;
			delete(s2);
			return s1;
		}
		else{
			*s2 = 0;
			delete(s1);
			return s2;
		}
	}
	throw InvalidArgumentException("substring-before()'s <funcExpr> argument count is invalid");
	return NULL;
}
UCSChar* FuncExpr::subStringAfter( VTDNav *vn){
	if (argCount1 == 2){
		UCSChar* s1 = al->e->evalString(vn);
		UCSChar* s2 = al->next->e->evalString(vn);
		size_t /*len1=wcslen(s1),*/len2=wcslen(s2);
		UCSChar* temp = NULL;
		if ((temp=wcsstr(s1,s2))!=NULL){
			size_t len = wcslen(temp);
			wcsncpy(s1,temp+len2,len-len2);
			*(s1+len-len2) = 0;
			delete(s2);
			return s1;
		}
		else{
			*s2 = 0;
			delete(s1);
			return s2;
		}
	}
	throw InvalidArgumentException("substring-after()'s <funcExpr> argument count is invalid");
	return NULL;
}
UCSChar* FuncExpr::translate( VTDNav *vn){
	if (argCount() == 0){
		UCSChar *s = NULL;
		
		try{
			if (vn->atTerminal)
			{
				int ttype = vn->getTokenType(vn->LN);
				if (ttype == TOKEN_CDATA_VAL)
					s =vn->toRawString(vn->LN);
				else if (ttype == TOKEN_ATTR_NAME
					|| ttype == TOKEN_ATTR_NS){
						s = vn->toString(vn->LN+1);
				}else
					s = vn->toString(vn->LN);
			}else
				s = vn->toString(vn->getCurrentIndex());
			return normalize(s);
		}
		catch(...){
			return createEmptyString();
		}
	} else if (argCount() ==1){
		UCSChar *s = al->e->evalString(vn);
		return normalize(s);
	}
	{
		throw InvalidArgumentException("normalize-space()'s <funcExpr> argument count is invalid");
		return NULL;
	}

}
bool FuncExpr::isWS(UCSChar c){
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
		return true;
	return false;
}
bool FuncExpr::lang( VTDNav *vn, UCSChar* s){
	bool b = false;
	vn->push2();
	try {
		while (vn->getCurrentDepth() >= 0) {
			int i = vn->getAttrVal((UCSChar *)L"xml:lang");
			if (i!=-1){
				b = vn->matchTokenString(i,s);
				break;
			}
			vn->toElement(PARENT);
		}
	} catch (...) {

	}
	vn->pop2();
	return b;
}
UCSChar* FuncExpr::normalize(UCSChar *s){
	 int os=0; size_t i,len;
	 len = wcslen(s);
	 // strip off leading ws
	 for (i = 0; i < len; i++)
	 {
		 if (isWS(s[i]))
		 {
		 }
		 else
		 {
			 break;
		 }
	 }
	 while (i < len)
	 {
		 UCSChar c = s[i];
		 if (!isWS(c))
		 {
			 s[os++]=c;
			 i++;
		 }
		 else
		 {
			 while (i < len)
			 {
				 c = s[i];
				 if (isWS(c))
					 i++;
				 else
					 break;
			 }
			 if (i < len){
				 s[os++]=' ';
			 }
		 }
	 }
	 s[os]=0;
	 return s;
}

// int evalFirstArgumentListNodeSet( VTDNav *vn);
// int evalFirstArgumentListNodeSet2( VTDNav *vn);
UCSChar* FuncExpr::upperCase( VTDNav *vn){
	if (argCount1 == 1){
		if (al->e->isNodeSet()){
			int a = evalFirstArgumentListNodeSet(vn);
		        if (a==-1)
		        	return createEmptyString();
		        else{
		        	try{
		        		int t = vn->getTokenType(a);
		        		if (t!= TOKEN_STARTING_TAG && t!=TOKEN_DOCUMENT)
		        			return vn->toStringUpperCase(a);
		        		return vn->getXPathStringVal(a, (short)1);
		        	}catch(...){
		        	}
		        	return createEmptyString();
		        }		
		}
		else{
		UCSChar *s = al->e->evalString(vn);
		UCSChar *s1= s;
		int i=0;
		while( s1[i] != 0){
			if (s1[i]>96 && s1[i] <123){
				s1[i] -=32;
			}
			i++;
		}
		return s;
		}
	}else{
		throw InvalidArgumentException("upper-case()'s  <funcExpr> argument count is invalid");
		return createEmptyString();
	}
}
UCSChar* FuncExpr::lowerCase( VTDNav *vn){
	if (argCount1 == 1){
		if (al->e->isNodeSet()){
		int a = evalFirstArgumentListNodeSet(vn);
		if (a==-1)
		    return createEmptyString(); 
		else{
        	try{
        		int t = vn->getTokenType(a);
        		if (t!= TOKEN_STARTING_TAG && t!=TOKEN_DOCUMENT)
        			return vn->toStringLowerCase(a);
        		return vn->getXPathStringVal(a,(short)2);
        	}catch(NavException&){
	       	}
		   	return createEmptyString();
        }	
		}else {
			UCSChar *s1 = al->e->evalString(vn);
			int i=0;
			while( s1[i] != 0){
				if (s1[i]>64 && s1[i]<91){
					s1[i] +=32;
				}
				i++;
			}
			return s1;
		}
		
		
	}else{
		throw InvalidArgumentException("lower-case()'s  <funcExpr> argument count is invalid");
		return createEmptyString();
	}
}

static double myround(double v){
			return (v>0.0) ? floor(v+0.5) : ceil(v-0.5);
		}
static double roundHalfToEvenPositive(double value, Long precision){
		int i;
		double result = 0;
		const double ROUNDING_EPSILON  = 0.00000001;
	    Long dec = 1;
		Long intPart = (Long)value;
	    
	    //shif the decimal point by precision
	    Long absPre = abs(precision);
	    
	    for(i = 0; i < absPre; i++){
	    	dec *= 10;
	    }
	    
	    if(precision > 0) value *= dec;
	    else if (precision < 0)value /= dec;
	    	    
	    //'value' is exctly halfway between two integers
	    if(fabs(value -((double)intPart +(double)0.5)) < ROUNDING_EPSILON){
	    	// 'ipart' is even 
	    	if(intPart%2 == 0){
	    		result = intPart;
	    	}else{// nearest even integer
	    		result = (Long)ceil( intPart + (double)0.5 );
	    	}
	    }else{
	    	//use the usual round to closest	    
			result = myround(value);
	    }
	    
	    //shif the decimal point back to where it was
	    if(precision > 0) result /= dec;
	    else if(precision < 0) result *= dec;
	    
		return result;		
	}

double FuncExpr::roundHalfToEven( VTDNav *vn){
		double value;
		Long precision;
		int numArg = argCount();

	    if (numArg < 1 || numArg > 2){
	    	throw InvalidArgumentException("Argument count for roundHalfToEven() is invalid.");	    	
	    }

		value = (double)al->e->evalNumber( vn);	    
	    precision = (numArg == 2)? (Long)floor((double)al->next->e->evalNumber(vn)+0.5) : 0;
	    
	    if(value < 0) return -roundHalfToEvenPositive(-value, precision);	    
	    else return roundHalfToEvenPositive(value, precision);
}

UCSChar* FuncExpr::fname(){
	switch(opCode){
			case FN_LAST: 			return (UCSChar *)L"last";
			case FN_POSITION: 		return (UCSChar *)L"position";
			case FN_COUNT: 			return (UCSChar *)L"count";
			case FN_LOCAL_NAME: 		return (UCSChar *)L"local-name";
			case FN_NAMESPACE_URI: 		return (UCSChar *)L"namespace-uri";
			case FN_NAME: 			return (UCSChar *)L"name";
			case FN_STRING: 			return (UCSChar *)L"string";
			case FN_CONCAT: 			return (UCSChar *)L"concat";
			case FN_STARTS_WITH:		return (UCSChar *)L"starts-with";
			case FN_CONTAINS: 		return (UCSChar *)L"contains";
			case FN_SUBSTRING_BEFORE: 	return (UCSChar *)L"substring_before";
			case FN_SUBSTRING_AFTER: 		return (UCSChar *)L"substring_after";
			case FN_SUBSTRING: 		return (UCSChar *)L"substring";
			case FN_STRING_LENGTH: 		return (UCSChar *)L"string-length";
			case FN_NORMALIZE_SPACE: 		return (UCSChar *)L"normalize-space";
			case FN_TRANSLATE:	 	return (UCSChar *)L"translate";
			case FN_BOOLEAN: 			return (UCSChar *)L"boolean";
			case FN_NOT: 			return (UCSChar *)L"not";
			case FN_TRUE: 			return (UCSChar *)L"true";
			case FN_FALSE: 			return (UCSChar *)L"false";
			case FN_LANG: 			return (UCSChar *)L"lang";
			case FN_NUMBER:			return (UCSChar *)L"number";
			case FN_SUM: 			return (UCSChar *)L"sum";
			case FN_FLOOR: 			return (UCSChar *)L"floor";
			case FN_CEILING: 		return (UCSChar *)L"ceiling";
			case FN_ROUND:		    return (UCSChar *)L"round";
			case FN_ABS:		    return (UCSChar *)L"abs";
			case FN_ROUND_HALF_TO_EVEN:		return (UCSChar *)L"round-half-to-even";
			case FN_ROUND_HALF_TO_ODD:		return (UCSChar *)L"round-half-to-odd";
			case FN_CODE_POINTS_TO_STRING: 	  return (UCSChar *)L"code-points-to-string";
			case FN_COMPARE:		   return (UCSChar *)L"compare";
			case FN_UPPER_CASE:		   return (UCSChar *)L"upper-case";
			case FN_LOWER_CASE:		   return (UCSChar *)L"lower-case";
			case FN_ENDS_WITH:		   return (UCSChar *)L"ends-with";
			case FN_QNAME:			   return (UCSChar *)L"QName";
			case FN_LOCAL_NAME_FROM_QNAME:		  return (UCSChar *)L"local-name-from-QName";
			case FN_NAMESPACE_URI_FROM_QNAME:	  return (UCSChar *)L"namespace-uri-from-QName";
			case FN_NAMESPACE_URI_FOR_PREFIX:	  return (UCSChar *)L"namespace-uri-for-prefix";
			case FN_RESOLVE_QNAME:		  return (UCSChar *)L"resolve-QName";
			case FN_IRI_TO_URI:			  return (UCSChar *)L"iri-to-uri";
			case FN_ESCAPE_HTML_URI:	  return (UCSChar *)L"escape-html-uri";
			case FN_ENCODE_FOR_URI:	return (UCSChar *)L"encode-for-uri";
			case FN_MATCH_NAME:		return (UCSChar *)L"match-name";
			case FN_MATCH_LOCAL_NAME:	return (UCSChar *)L"match-local-name";
			case FN_CURRENT:			return (UCSChar *)L"current";
			case FN_GENERATE_ID:		return (UCSChar *)L"generate-id";
			case FN_FORMAT_NUMBER:	return (UCSChar *)L"format-number";
			case FN_KEY:		return (UCSChar *)L"key";
			default:			return (UCSChar *)L"encode-for-uri";
	}
}


bool FuncExpr::isFinal(){
	AList *temp = al;
	if (temp ==NULL)
		return false;
	if (temp->e==NULL)
		return false;
	bool s=true;
	while(temp!=NULL){
		s= s && temp->e->isFinal();
		if (!s)
			return false;
		temp = temp->next;
	}
	return s;	
}
		
void FuncExpr::markCacheable2(){
	AList *temp = al;
	while(temp!=NULL ){
		if (temp->e!=NULL){
			if (temp->e->isFinal() && temp->e->isNodeSet()){
				CachedExpr *ce = new CachedExpr(temp->e);
				temp->e = ce;
			}
			temp->e->markCacheable2();
		}
		temp = temp->next; 
	}
}

void FuncExpr::markCacheable(){
	AList *temp = al;
	while(temp!=NULL){
		if (temp->e!=NULL)
			temp->e->markCacheable();
		temp = temp->next; 
	}
}

void FuncExpr::clearCache(){
	AList *temp =al;
	while(temp!=NULL ){
		if (temp->e!=NULL){				
			temp->e->clearCache();
		}
		temp = temp->next; 
	}
}

bool FuncExpr::checkArgumentCount(){
	switch(opCode){
		case FN_LAST: 			return argCount1==0;
		case FN_POSITION: 		return argCount1==0;
		case FN_COUNT: 			return (argCount1==1 && al->e->isNodeSet());
		
		case FN_LOCAL_NAME: 		return (argCount1==0 ||(argCount1==1 && al->e->isNodeSet()));
		case FN_NAMESPACE_URI: 	return (argCount1==0 ||(argCount1==1 && al->e->isNodeSet()));
		case FN_NAME: 			return (argCount1==0 ||(argCount1==1 && al->e->isNodeSet()));
		case FN_STRING: 			return argCount1 < 2;
		case FN_CONCAT: 			return argCount1 > 1;
		case FN_STARTS_WITH:		return argCount1 ==2;
		case FN_CONTAINS: 		return argCount1 ==2;
		case FN_SUBSTRING_BEFORE: return argCount1==2;
		case FN_SUBSTRING_AFTER: 	return argCount1==2;
		case FN_SUBSTRING: 		return argCount1==2 || argCount1==3;
		case FN_STRING_LENGTH: 	return argCount1<2;
		case FN_NORMALIZE_SPACE: 	return argCount1 <2;
		case FN_TRANSLATE:	 	return argCount1 ==3;
		case FN_BOOLEAN: 			return argCount1 ==1;
		case FN_NOT: 			    return argCount1 ==1;
		case FN_TRUE: 			return argCount1 ==0;
		case FN_FALSE: 			return argCount1 ==0;
		case FN_LANG: 			return (argCount1==1);
		case FN_NUMBER:			return argCount1==1;
		case FN_SUM: 			    return (argCount1==1 && al->e->isNodeSet());
		case FN_FLOOR: 			return argCount1==1;
		case FN_CEILING: 			return argCount1==1;
		case FN_ROUND:			return argCount1==1;
		case FN_ABS:				return argCount1==1;
		case FN_ROUND_HALF_TO_EVEN :
										return argCount1==1 || argCount1 == 2;
		case FN_ROUND_HALF_TO_ODD:
										return argCount1==1 || argCount1 == 2;
		case FN_CODE_POINTS_TO_STRING:
		    							break;
		case FN_COMPARE:			break;
		case FN_UPPER_CASE:		return argCount1==1;
		case FN_LOWER_CASE:		return argCount1==1;
		case FN_ENDS_WITH:		return argCount1==2;
		case FN_QNAME:			break;
		case FN_LOCAL_NAME_FROM_QNAME:
		    							break;
		case FN_NAMESPACE_URI_FROM_QNAME:
		    							break;
		case FN_NAMESPACE_URI_FOR_PREFIX:
		    							break;
		case FN_RESOLVE_QNAME:	break;
		case FN_IRI_TO_URI:    	break;
		case FN_ESCAPE_HTML_URI:	break;
		case FN_ENCODE_FOR_URI:	break;
		case FN_MATCH_NAME:		return argCount1==1 || argCount1 == 2;
		case FN_MATCH_LOCAL_NAME: return argCount1==1 || argCount1 == 2;
		case FN_NOT_MATCH_NAME:		return argCount1==1 || argCount1 == 2;
		case FN_NOT_MATCH_LOCAL_NAME: return argCount1==1 || argCount1 == 2;
		case FN_CURRENT:			return argCount1==0;
		case FN_GENERATE_ID : 	return argCount1==0 || (argCount1 ==1 && al->e->isNodeSet());
		case FN_FORMAT_NUMBER:  	return argCount1==2 || argCount1== 3;
		case FN_KEY:				return argCount1==2;
		case FN_DOCUMENT:			return argCount1==1 || (argCount1==2 && al->next->e->isNodeSet());	
		case FN_SYSTEM_PROPERTY: 	return argCount1==1 && al->e->isString() ;
		case FN_ELEMENT_AVAILABLE: return argCount1==1 && al->e->isString();
		case FN_FUNCTION_AVAILABLE: return argCount1==1 && al->e->isString();
		}
		return false;	
}



//bool FuncExpr::isElementAvailable(VTDNav *vn){return false;}
//bool FuncExpr::isFunctionAvailable(VTDNav *vn){return false;}
UCSChar *FuncExpr::formatNumber(VTDNav *vn){return createEmptyString();}
UCSChar *FuncExpr::generateID(VTDNav *vn){
	if (argCount1== 0){
		UCSChar *str = new UCSChar[14];
		swprintf(str,14, L"v%d", vn->getCurrentIndex2());
		return str;
		//return "v"+vn->getCurrentIndex2();
	}else if (argCount1== 1) {
		UCSChar *str = new UCSChar[14];
		int i=evalFirstArgumentListNodeSet2(vn);
		swprintf(str,14, L"v%d",i);
		return str;
	} else 
	    throw new InvalidArgumentException
			("generate-id()'s argument count is invalid");
}


int FuncExpr::evalFirstArgumentListNodeSet( VTDNav *vn){
	vn->push2();
        int size = vn->contextBuf2->size;
        int a = -1;
        try {
            a = al->e->evalNodeSet(vn);
            if (a != -1) {
            	int t = vn->getTokenType(a);
                if (t == TOKEN_ATTR_NAME) {
                    a++;
                }else if (t == TOKEN_PI_NAME){
                	//if (a+1 < vn.vtdSize || vn.getTokenType(a+1)==VTDNav.TOKEN_PI_VAL)
                	a++;
                	//else 
                	//	a=-1;
                }
                 
                //else if (t== VTDNav.T)
            }	            
        } catch (NavException&) {
        }
        vn->contextBuf2->size = size;
        al->e->reset(vn);
        vn->pop2();
        return a;
}

int FuncExpr::evalFirstArgumentListNodeSet2( VTDNav *vn){
	vn->push2();
        int size = vn->contextBuf2->size;
        int a = -1;
        try {
            a = al->e->evalNodeSet(vn);	            
        } catch (NavException&) {
        }
        vn->contextBuf2->size = size;
        al->e->reset(vn);
        vn->pop2();
        return a;
}

bool FuncExpr::matchName(VTDNav *vn){
	int a;
	bool b=false;
	if (argCount1 == 1) {
		a = vn->getCurrentIndex();
		int type = vn->getTokenType(a);
		UCSChar *s1 = al->e->evalString(vn);
		if (type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_PI_NAME) {
				try {
					b= vn->matchRawTokenString(a, s1);
				} catch (NavException&) {
					b =false;
				}
				delete s1;
				return b;
		} else
			return false;
	} else if (argCount1 == 2) {
		a = evalFirstArgumentListNodeSet2(vn);
		UCSChar *s1 = al->next->e->evalString(vn);
		try {
			if (a == -1 || vn->ns == false){}
			//return false;
			else {
				int type = vn->getTokenType(a);
				if (type ==  TOKEN_STARTING_TAG
					|| type ==  TOKEN_ATTR_NAME
					|| type ==  TOKEN_PI_NAME)
					b =vn->matchRawTokenString(a, s1);

			}
			delete s1;
			return b;
		} catch (NavException &) {
		}
		return false;
	} else
		throw new InvalidArgumentException(
		"name()'s argument count is invalid");
}

bool FuncExpr::matchLocalName(VTDNav *vn){
	bool b=false;
	 if (argCount1== 1){
	        try{
	            int index = vn->getCurrentIndex();
	            int type = vn->getTokenType(index);
	            UCSChar *s1 = al->e->evalString(vn);
	            if (vn->ns && (type == TOKEN_STARTING_TAG 
	                    || type == TOKEN_ATTR_NAME)) {
                    int offset = vn->getTokenOffset(index);
                    int length = vn->getTokenLength(index);
                    if (length < 0x10000 || (length>>16)==0){
                    	b= (vn->compareRawTokenString(index, s1)==0);//vn.toRawString(index);
                    }
                    else {
                        int preLen = length >> 16;
                        int QLen = length & 0xffff;
                        if (preLen != 0){
                        	 b= (vn->compareRawTokenString2(offset + preLen+1, QLen
                                        - preLen - 1,s1)==0);                        	
                        }
                    }
                } else if (type == TOKEN_PI_NAME){
                	b= (vn->compareRawTokenString(index, s1)==0);
                } else 
                    b= (wcscmp(s1,L"")==0);
				delete s1;
				return b;
	        }catch(NavException&){
	        	 return false; // this will never occur
	        }
	        
	    } else if (argCount1 == 2){
	        int a=evalFirstArgumentListNodeSet2(vn);
	        UCSChar *s1 = al->next->e->evalString(vn);
			if (a == -1 || vn->ns == false)
			    b= (wcscmp(L"",s1)==0);
			int type = vn->getTokenType(a);
			if (type==TOKEN_STARTING_TAG || type== TOKEN_ATTR_NAME){
			    //return "".equals(s1);
			try {			    
			    int offset = vn->getTokenOffset(a);
			    int length = vn->getTokenLength(a);
			    if (length < 0x10000 || (length>> 16)==0)
			       b= vn->compareRawTokenString(a,s1)==0;
			    else {
			        int preLen = length >> 16;
			        int QLen = length & 0xffff;
			        if (preLen != 0)
			            b= vn->compareRawTokenString2(offset + preLen+1, 
			                    QLen - preLen - 1,s1)==0;
			        /*else {
			            return vn.toRawString(offset, QLen);
			        }*/
			    }
			} catch (NavException&) {
				 b= (wcscmp(L"",s1)==0); // this will almost never occur
			}		
			delete s1;
			return b;
			}else if (type == TOKEN_PI_NAME){
				try{
					b= (vn->compareRawTokenString(a,s1)==0);
				}catch(NavException&){
					b= (wcscmp(L"",s1)==0);
				}
				delete s1;
				return b;
			}
			 b= (wcscmp(L"",s1)==0);
			 delete s1;
			 return b;
	    } else 
	        throw new InvalidArgumentException
			("local-name()'s argument count is invalid");
		return false;
}

UByte* FuncExpr::doubleCapacity(UByte *b, size_t cap){
	UByte *t = new UByte[cap];
	memcpy(t,b,cap>>1);
	delete b;
	return t;
}
Long FuncExpr::getBytes_UTF8(UCSChar *s){
	/* < 0x7f  1 byte*/
	/* < 0x7ff  2 bytes */
	/* < 0xffff  3 bytes */
	/* < 0x 10ffff  4 bytes*/
	size_t len = wcslen(s),i=0,k=0,capacity = max<size_t>(len,8);/*minimum size is 8 */
	UByte *ba = new UByte[capacity]; 
	if (ba == NULL)
		throw OutOfMemException("Byte allocation failed in getBytes_UTF_8");
	for (i=0;i<len;i++){
		if (s[i]<= 0x7f){
			if (capacity -k<1){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]=(UByte)s[i];
			k++;
		}else if (s[i] <=0x7ff){
			if (capacity -k<2){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0x7c0) >> 6) | 0xc0;
			ba[k+1] = (s[i] & 0x3f) | 0x80;
			k += 2;
		}else if (s[i] <=0xffff){
			if (capacity -k<3){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0xf000) >> 12) | 0xe0;
			ba[k+1] = ((s[i] & 0xfc) >> 6) | 0x80;
			ba[k+2] = (s[i] & 0x3f) | 0x80;
			k += 3;
		}else if (s[i] <=0x10ffff){
			if (capacity -k<4){
				capacity = capacity<<1;
				ba = doubleCapacity(ba,capacity);
			}
			ba[k]= ((s[i] & 0x1c0000) >> 18) | 0xf0;
			ba[k+1] = ((s[i] & 0x3f0) >> 12) | 0x80;
			ba[k+2] = ((s[i] & 0xfc) >> 6) | 0x80;
			ba[k+3] = (s[i] & 0x3f) | 0x80;
			k += 4;
		}else
			throw ModifyException("Invalid XML char for getBytes_UTF_8");
	}
	return ((Long)k<<32)|(Long)ba;
}

int FuncExpr::getStringVal(VTDNav *vn, int i){
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

AList::AList(){
	next  = NULL;	
}

AList::~AList(){
	AList *temp = this;
	while(temp!=NULL){
		if (temp->e!= NULL )
			delete(temp->e);
		temp = temp->next;
	}
}

void  AList::reset_al(VTDNav *vn){
	AList *temp = this;
	while(temp!=NULL){
		if (temp->e!= NULL )
			(temp->e->reset)(vn);
		temp = temp->next;
	}
}

void  AList::toString_al(UCSChar* string){
	AList *temp = this;

	while(temp!=NULL){
		(temp->e->toString)(string);
		temp = temp->next;
		if (temp!=NULL)
			wprintf(L",");
	}
}

