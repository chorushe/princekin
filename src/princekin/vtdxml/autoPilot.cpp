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
#include "autoPilot.h"
//#include "xpathParser.tab.hh"
#include "xpath_driver.h"
using namespace com_ximpleware;


NsList *AutoPilot::nl = NULL;
ExprList *AutoPilot::el = NULL;



bool AutoPilot::checkNsUniqueness( int index){
	int j;
	for (j=0;j<fib->size;j++){
		if (vn->compareTokens(fib->intAt(j),vn, index)==0)
			return false;
	}		
	fib->append(index);
	return true;
}

AutoPilot::AutoPilot():
URL(NULL),
localName(NULL),
elementName(NULL),
elementName2(NULL),
vn(NULL),
ft (true),
special( false ),
it(UNDEFINED), /* initial state: not defined */
//startIndex = -1;
xpe(NULL),
//nl(NULL),
contextCopy(NULL),
fib(NULL),
cachingOption(true),
stackSize(0)
{
}

AutoPilot::AutoPilot(VTDNav *v):
URL(NULL),
localName(NULL),
elementName(NULL),
elementName2(NULL),
vn(v),
ft (true),
special( false ),
it(UNDEFINED), /* initial state: not defined */
//startIndex = -1;
xpe(NULL),
//nl(NULL),
contextCopy(NULL),
fib(NULL),
cachingOption(true),
stackSize(0)
{
}

AutoPilot::~AutoPilot(){
	if (contextCopy!=NULL)
		delete(contextCopy);
	if (xpe!= NULL)
		delete(xpe);
	if (fib!=NULL)
		delete(fib);
	if (elementName2!=NULL)
		delete(elementName2);
	xpe=NULL;
	fib=NULL;
	elementName2=NULL;
	contextCopy = NULL;
}

void AutoPilot::printExprString(){
	if (xpe!=NULL)
		xpe->toString(NULL);
}

/* bind */
void AutoPilot::bind(VTDNav *v){
	elementName = NULL;
	elementName2 = NULL;
    vn = v;
	it = UNDEFINED; 
    ft = true;
    size = 0;
    special = false;
}

/* Select an attribute name for iteration, * choose all attributes of an element*/
void AutoPilot::selectAttr(UCSChar *an){
	if (an == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElement, elementName can't be NULL");
	}
	it= ATTR;
    ft = true;
    size = vn->getTokenCount();
	elementName = an;
}

/* Select an attribute name, both local part and namespace URL part*/
void AutoPilot::selectAttrNS(UCSChar *URL, UCSChar *ln){
	if (ln == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElement, localName can't be NULL");
	}
	it = ATTR_NS;
    ft = true;
    localName = ln;
    this->URL = URL;
}


/*Select the element name before iterating*/
void AutoPilot::selectElement(UCSChar *en){
   if (en == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElement, elementName can't be NULL");
	 }
    it = SIMPLE;
    depth = vn->getCurrentDepth();
    elementName = en;
    ft = true;
}

/*Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname*/
void AutoPilot::selectElementNS(UCSChar *URL, UCSChar *ln){
    if (ln == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElementNS, localName can't be NULL");
	}
    it = SIMPLE_NS;
    depth = vn->getCurrentDepth();
    localName = ln;
	URL = URL;
    ft = true;
}


/**
* Select all descendent elements along the descendent axis, without ns awareness
* This function is not intended to be called directly
* @param en
*/
void AutoPilot::selectElement_D(UCSChar *en){
    if (en == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElement_D, elementName can't be NULL");
	 }
	it = DESCENDANT;
	depth = vn->getCurrentDepth();
	elementName = en;
	ft = true;
}

/**
* Select all descendent elements along the Descendent axis, with ns awareness
* This function is not intended to be called directly
*/
void AutoPilot::selectElementNS_D(UCSChar *URL, UCSChar *ln){
 if (ln == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElementNS_D, localName can't be NULL");
	}
	it = DESCENDANT_NS;
    depth = vn->getCurrentDepth();
    localName = ln;
	URL = URL;
    ft = true;
}
/**
* Select all descendent elements along the following axis, without ns awareness
* This function is not intended to be called directly
* @param en
*/
void AutoPilot::selectElement_F(UCSChar *en){
	if (en == NULL){
		throw InvalidArgumentException(" invalid argument for selectElement_F, elementName can't be NULL");
	 }
	it = FOLLOWING;
	ft = true;
	elementName = en;
}

/**
* Select all descendent elements along the following axis, with ns awareness
* This function is not intended to be called directly
*/
void AutoPilot::selectElementNS_F(UCSChar *URL, UCSChar *ln){
	if (ln == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElementNS_F, localName can't be NULL");
	}
	it = FOLLOWING_NS;
    ft= true;
    localName = ln;
    URL = URL;
}

/**
* Select all descendent elements along the preceding axis, without ns awareness
* This function is not intended to be called directly
* @param en
*/
void AutoPilot::selectElement_P(UCSChar *en){
	int i;
	int a = sizeof(int)* vn->nestingLevel;
    if (en == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElement_P, elementName can't be NULL");
	 }
	depth = vn->getCurrentDepth();
	it = PRECEDING;
    ft = true;	
	elementName = en;
    contextCopy = new int[vn->nestingLevel]; //(int[])vn.context.clone();
	memcpy(contextCopy,vn->context,a);
	endIndex = vn->getCurrentIndex2();
	for(i = vn->context[0]+1 ; i<vn->nestingLevel ; i++){
        contextCopy[i]=-1;
    }
    contextCopy[0]=vn->rootIndex;
}

/**
* This function is not intended to be called directly
* Select all descendent elements along the preceding axis, with ns awareness
*/
void AutoPilot::selectElementNS_P(UCSChar *URL, UCSChar *ln){
	int i;
	int a = sizeof(int)* vn->nestingLevel;
    if (ln == NULL){
		 throw InvalidArgumentException(" invalid argument for selectElementNS_P, localName can't be NULL");
	}
	depth = vn->getCurrentDepth();
	it = PRECEDING_NS;
    ft = true;	
	URL = URL;
	localName = ln;
    contextCopy = new int[vn->nestingLevel]; //(int[])vn.context.clone();
	memcpy(contextCopy,vn->context,a);
	endIndex = vn->getCurrentIndex2();
	for(i = vn->context[0]+1 ; i<vn->nestingLevel ; i++){
        contextCopy[i]=-1;
    }
    contextCopy[0]=vn->rootIndex;
}



//Iterate over all the selected element nodes.

bool AutoPilot::iterate(){
	switch (it) {
		case SIMPLE :
			if (vn->atTerminal)
				return false;
			if (ft == false)
				return vn->iterate( depth, elementName, special);
			else {
				ft = false;
				if (special || vn->matchElement( elementName)) {
					return true;
				} else
					return vn->iterate(depth, elementName,special);
			}
		case SIMPLE_NS :
			if (vn->atTerminal)
				return false;
			if (ft == false)
				return vn->iterateNS( depth, URL, localName);
			else {
				ft = false;
				if (vn->matchElementNS( URL, localName)) {
					
					return true;
				} else
					return vn->iterateNS( depth, URL, localName);
			}
		case DESCENDANT:
			if (vn->atTerminal)
				return false;
			return vn->iterate( depth, elementName, special);
		case DESCENDANT_NS:
         	if (vn->atTerminal)
         	    return false;         	
         	return vn->iterateNS(depth, URL, localName);
		case FOLLOWING:
		   	if (vn->atTerminal)
         	    return false;
            if (ft == false)
                return vn->iterate_following( elementName, special);
            else {
            	ft = false;
            	while(true){
            		while (vn->toElement( NEXT_SIBLING)){
            			 if (special || vn->matchElement(elementName)) {                	
                            return true;
            			 }
            			 return vn->iterate_following(elementName, special);
            		}
                    if (vn->toElement( PARENT)==false){
                        return false;
                    } 
            	}
            }
		case FOLLOWING_NS:
        	if (vn->atTerminal)
         	    return false;
         	if (ft == false)
                return vn->iterate_followingNS(URL,localName);
            else {
            	ft = false;
            	while(true){
            		while (vn->toElement( NEXT_SIBLING)){
            			 if (vn->matchElementNS( URL,localName)) {                	
                            return true;
            			 }
						 return vn->iterate_followingNS( URL,localName);
            		}
                    if (vn->toElement(PARENT)==false){
						return false;
                    } 
            	}
            }
		case PRECEDING:
			if (vn->atTerminal)
         	    return false;
			if (ft){
				ft = false;
				vn->toElement(ROOT);
			}
         	return vn->iterate_preceding( elementName, contextCopy, endIndex);

		case PRECEDING_NS:
			if (vn->atTerminal)
         	    return false;
			if (ft){
				ft = false;
				vn->toElement(ROOT);
			}
         	return vn->iterate_precedingNS( URL,localName,contextCopy,endIndex);
		default :
			throw PilotException("unknow iteration type for iterateAP");
			return false;
	}
}

// This method implements the attribute axis for XPath
int AutoPilot::iterateAttr2(){
		int i;
	switch(it){
			case ATTR:
				if (wcscmp(elementName,L"*")==0){
					if (ft != false){
						ft = false;
						index = vn->getCurrentIndex2()+1;
					} else
						index +=2;
					if (vn->ns == false){
						while(index<size){
							tokenType type = vn->getTokenType(index);
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									vn->LN = index;
									return index;
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}else {						
						while(index<size){
							tokenType type = vn->getTokenType(index);
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									if (type == TOKEN_ATTR_NAME){
										vn->LN = index;
										return index;
									}
									else 
										index += 2;	    						
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}
				}else{
					if (ft == false){
						return -1;
					} else {
						ft = false;
						i = vn->getAttrVal(elementName);
						if(i!=-1){
							vn->LN = i-1;
							return i-1;
						}
						else 
							return -1;
					}   	    			
				}
			case ATTR_NS:
				if (ft == false){
					return -1;
				} else {
					ft = false;
				    i = vn->getAttrValNS(URL,localName);
					if(i!=-1){
						vn->LN = i-1;
						return i-1;
					}
					else 
						return -1;
				} 
			default:
				throw PilotException("unknow iteration type for iterateAP");
	}
}

int AutoPilot::iterateAttr(){
		int i;
	switch(it){
			case ATTR:
				if (wcscmp(elementName,L"*")==0){
					if (ft != false){
						ft = false;
						index = vn->getCurrentIndex2()+1;
					} else
						index +=2;
					if (vn->ns == false){
						while(index<size){
							tokenType type = vn->getTokenType(index);
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									//vn->LN = index;
									return index;
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}else {						
						while(index<size){
							tokenType type = vn->getTokenType(index);
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									if (type == TOKEN_ATTR_NAME){
										//vn->LN = index;
										return index;
									}
									else 
										index += 2;	    						
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}
				}else{
					if (ft == false){
						return -1;
					} else {
						ft = false;
						i = vn->getAttrVal(elementName);
						if(i!=-1){
							//vn->LN = i-1;
							return i-1;
						}
						else 
							return -1;
					}   	    			
				}
			case ATTR_NS:
				if (ft == false){
					return -1;
				} else {
					ft = false;
				    i = vn->getAttrValNS(URL,localName);
					if(i!=-1){
						//vn->LN = i-1;
						return i-1;
					}
					else 
						return -1;
				} 
			default:
				throw PilotException("unknow iteration type for iterateAP");
	}
}

/*
* This function selects the string representing XPath expression
* Usually evalXPath is called afterwards
* return true is the XPath is valid
*/
bool AutoPilot::selectXPath(UCSChar *s){
	XMLChar_init();
	if (s==NULL){
		throw XPathParseException(" xpath input string can't be NULL ");
		return false;
	}
	if(xpe!=NULL)
		delete(xpe);
	xpath_driver xd(nl,el);
	//xd.trace_parsing = true;
      //else if (*argv == std::string ("-s"))
    //xd.trace_scanning = true;
	xd.parse(s);
	xpe = xd.result;
	//xpe = xpathParse(s, nl,el);
	ft = true;
	if (xpe == NULL){
		throw XPathParseException("Invalid XPath expression");
		return false;
	}
	if (cachingOption)
		xpe->markCacheable();
	return true;
}
/*
* Evaluate XPath to a bool
*/
bool AutoPilot::evalXPathToBool(){
	return	xpe->evalBoolean(vn);
}

/*
* Evaluate XPath to a String
*/
UCSChar* AutoPilot::evalXPathToString(){
	return xpe->evalString(vn);
}

/*
* Evaluate XPath to a number
*/
double AutoPilot::evalXPathToNumber(){
	return xpe->evalNumber(vn);
}
/*
* Evaluate XPath
*/
int AutoPilot::evalXPath(){
	if (xpe != NULL){
		if (ft == true){
			if (vn != NULL){
	            stackSize = vn->contextBuf2->size;
			}
			ft = false;
			xpe->adjust(vn->vtdSize);
		}
		return xpe->evalNodeSet(vn);
	}
	throw OtherException("xpe is NULL in autoPilot");
	return -1;
}

/*
* Reset XPath
*/
void AutoPilot::resetXPath(){
	if (xpe != NULL && vn!=NULL){
		xpe->reset(vn);
		vn->contextBuf2->size = stackSize;
		ft = true;
		if (cachingOption)
			xpe->clearCache();
	}
}

/*
* Declare prefix/URL binding
*/

void AutoPilot::declareXPathNameSpace( UCSChar *prefix, UCSChar *URL){

	insertItem(prefix,URL);
}

/* Clear the namespace prefix URL bindings in the global list */
void AutoPilot::clearXPathNameSpaces(){
		/*nl has been made a package scoped static variable*/
		NsList *tmp = nl;
		NsList *tmp2 = NULL;
		while(tmp!=NULL){
			tmp2 = tmp->next;
			delete(tmp);
			tmp  = tmp2;			
		}

}
/* Clear the variable name and exprs in the global list  */
void AutoPilot::clearVariableExprs(){
		ExprList *tmp = el;
		ExprList *tmp2 = NULL;
		while(tmp!=NULL){
			delete(tmp->ve);
			tmp2 = tmp->next;			
			delete(tmp);
			tmp  = tmp2;			
		}
}

/* Declare the variable name and expression binding*/
void AutoPilot::declareVariableExpr(UCSChar* varName, UCSChar* varExpr){
	Expr* tmp = NULL;
	if (varExpr==NULL || varName==NULL){
		throw XPathParseException("xpath input string or variable name can't be NULL ");
		//return FALSE;
	}
	tmp = el->getExprFromList(varName);
	if(tmp!=NULL)
		delete(tmp);
	try{
		xpath_driver xd(nl,el);
		xd.trace_parsing = true;
		//else if (*argv == std::string ("-s"))
		xd.trace_scanning = true;
		xd.parse(varExpr);
		tmp = xd.result;
		//tmp = xpathParse(varExpr, nl,el);
		ft = true;
		if (tmp == NULL){
			throw XPathParseException("variable expr declaration failed ");
		}
		insertExpr(varName,tmp);
	}catch(...){
		//freeAllObj();
		throw;
	}
}

void AutoPilot::selectNameSpace(UCSChar *en){
	if (en == NULL)
		throw InvalidArgumentException("namespace name can't be null");
	it = NAMESPACE;
    ft = true;
    size = vn->getTokenCount();
	elementName = en;
	if (wcscmp(en,L"*")!=0){
		UCSChar *tmp = new UCSChar[(wcslen(en)+7)];
		if (tmp == NULL)
			throw OutOfMemException("allocating string failed in selectNameSpace()");
		wcscpy(tmp,L"xmlns:");
		wcscpy(tmp+6,en);
		*(tmp+(wcslen(en)+6))= 0;
		elementName2=tmp;
	}
    if (fib== NULL)
    	fib = new FastIntBuffer(4);
    else 
    	delete(fib);
}

int AutoPilot::iterateNameSpace(){
		if (vn->ns == false)
			return -1;
		if (ft != false) {
			ft = false;
			index = vn->getCurrentIndex2() + 1;
		} else
			index += 2;

		while (index < size) {
			tokenType type = vn->getTokenType(index);
			if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS) {
				if (type == TOKEN_ATTR_NS){ 
					if  (wcscmp(elementName,L"*") == 0   
						||vn->matchRawTokenString(index, elementName2)
				    ){
				    	// check to see if the namespace has appeared before
				    	if (checkNsUniqueness(index)){
				    		vn->LN = index;
				    		vn->atTerminal = true;
				    		return index;
				    	}
				    }
				} 
				index += 2;
			} else {
				vn->atTerminal = false;
				if (vn->toElement( PARENT) == false) {
					return -1;
				} else {
					index = vn->getCurrentIndex2() + 1;
				}
			}
		}

		return -1;
}


Expr* ExprList::getExprFromList(UCSChar *varName){
	ExprList *tmp = this;
	if (tmp==NULL)
		return NULL;
	while(tmp!= NULL){
		if (wcscmp(tmp->variableName,varName)==0){
			return tmp->ve;
		}
		tmp = tmp->next;
	}
	return NULL;
}

UCSChar* NsList::lookup(UCSChar *prefix){
	NsList *tmp = this;
	if (tmp==NULL)
		return NULL;
	while(tmp!= NULL){
		if (wcscmp(tmp->prefix,prefix)==0){
			return tmp->URL;
		}
		tmp = tmp->next;
	}
	return NULL;
}

void AutoPilot::insertItem(UCSChar *prefix, UCSChar *URL){
	NsList *tmp = nl;
	/* search first */
	if (tmp == NULL){
		tmp = new NsList();
		if (tmp == NULL){
			throw OutOfMemException("out of memory in insertItem");
		}
		tmp->next = NULL; 
		tmp->prefix = prefix;
		tmp->URL = URL;
		nl = tmp;
		return;
	}else {
		if (wcscmp(tmp->prefix,prefix) == 0){
			tmp->URL = URL; /* overwritten */
			return;
		}
		while(tmp->next!=NULL){
			if (wcscmp(tmp->prefix,prefix) == 0){
				tmp->URL = URL; /* overwritten */
				return;
			}
			tmp = tmp->next;
		}
		
		tmp->next = new NsList();
		if (tmp->next == NULL){
			throw OutOfMemException("out of memory in insertItem");
		}
		tmp->next->next = NULL; 
		tmp->next->prefix = prefix;
		tmp->next->URL = URL;
		return;
	}
}


void AutoPilot::insertExpr(UCSChar *varName, Expr *e){
	ExprList *tmp = el;
	
	/* search first */
	if (tmp == NULL){
		tmp = new ExprList();
		if (tmp == NULL){
			throw OutOfMemException("out of memory in insertExpr");
		}
		tmp->next = NULL; 
		tmp->variableName = varName;
		tmp->ve = e;
		el = tmp;
		return;
	}else {
		if (wcscmp(tmp->variableName, varName) == 0){
			delete(tmp->ve);
			tmp->ve = e; /* overwritten */
			return;
		}
		while(tmp->next!=NULL){
			if (wcscmp(tmp->variableName,varName) == 0){
				delete(tmp->ve);
				tmp->ve = e; /* overwritten */
				return;
			}
			tmp = tmp->next;
		}
		
		tmp->next = new ExprList();
		if (tmp->next == NULL){
			throw OutOfMemException("out of memory in insertExpr");
		}
		tmp->next->next = NULL; 
		tmp->next->variableName = varName;
		tmp->next->ve = e;
		return;
	}
}


void AutoPilot::selectDescendantNode(){
	ft = true;  
	depth = vn->getCurrentDepth();
	it = DESCENDANT_NODE;
}

void AutoPilot::selectNode(){
	ft = true;
	depth = vn->getCurrentDepth();
	it = SIMPLE_NODE;
}

void AutoPilot::selectPrecedingNode(){
	int a = sizeof(int)* vn->nestingLevel;
	ft = true;
	   depth = vn->getCurrentDepth();
	   contextCopy = new int[vn->nestingLevel]; //(int[])vn.context.clone();
		memcpy(contextCopy,vn->context,a);
	   
	   if (contextCopy[0]!=-1){
		   for (int i=contextCopy[0]+1;i<vn->nestingLevel;i++){
			contextCopy[i]=0;
	   }
	   }//else{
	   //   for (int i=1;i<contextCopy.length;i++){
	   //	   contextCopy[i]=0;
	   //	   }
	   //}
	   it = PRECEDING_NODE;
	   endIndex = vn->getCurrentIndex();
}
void AutoPilot::selectFollowingNode(){
	ft = true;
	   depth = vn->getCurrentDepth();
	   it = FOLLOWING_NODE;
}
bool AutoPilot::iterate2(){
	switch (it) {
		case SIMPLE_NODE:
			if (ft && vn->atTerminal)
				return false;
			if (ft){
				ft =false;
				return true;
			}
			return vn->iterateNode(depth);
			
		case DESCENDANT_NODE:
			if (ft&&vn->atTerminal)
				return false;
			else{
				ft=false;
				return vn->iterateNode(depth);
			}
         	
		case FOLLOWING_NODE:
			if (ft){
				bool b= false;
				do{
					b = vn->toNode(NEXT_SIBLING);
					if (b){
						ft = false;
						return true;
					}else{
						b = vn->toNode(PARENT);
					}
				}while(b);
				return false;
			}			
			return vn->iterate_following_node();
			
		case PRECEDING_NODE:
			if(ft){
				ft = false;
				vn->toNode(ROOT);
				vn->toNode(PARENT);	
			}
			return vn->iterate_preceding_node(contextCopy,endIndex);
		//case 
		default :
			throw new PilotException(" iteration action type undefined");
	}
}