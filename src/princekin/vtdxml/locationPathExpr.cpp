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
#include "locationPathExpr.h"
#include "filterExpr.h"
#include "autoPilot.h"
#include "textIter.h"
#include "cachedExpr.h"

using namespace com_ximpleware;

LocationPathExpr::LocationPathExpr() try : 
pathType(RELATIVE_PATH),
s(NULL),
currentStep(NULL),
state(XPATH_EVAL_START),
ih(new IntHash())
{
}
catch (std::bad_alloc&){
	throw OutOfMemException("Allocation failed for LocationPathExpr");
}

LocationPathExpr::~LocationPathExpr(){
	Step *tmp, *tmp2;
	if (s != NULL){
		tmp = s;
		tmp2 = tmp->nextS;
		while(tmp2!=NULL){
			delete(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextS;
		}
		delete(tmp);
	}
	delete(ih);
}

bool LocationPathExpr::evalBoolean(VTDNav *vn){
	bool a = false;
	int size;
	vn->push2();
	// record stack size
	size = vn->contextBuf2->size;
	try{
		a = (evalNodeSet(vn) != -1);
	}catch (...){
	}
	//rewind stack
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return a;
}
double LocationPathExpr::evalNumber(VTDNav *vn){
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
int LocationPathExpr::evalNodeSet(VTDNav *vn){
    int result;
	if (currentStep == NULL) {
		if ( pathType ==  ABSOLUTE_PATH){
			vn->context[0]=-1;
			vn->atTerminal = false;
		}
		currentStep =  s;
		if (currentStep == NULL){
			if (state ==  XPATH_EVAL_START){
				state = XPATH_EVAL_END;
				return 0;
			}
			else{
				return -1;
			}
		}
	}

	while (true) {
		switch (currentStep->axis_type) {
			case AXIS_CHILD0:
			    if ( (result = process_child(vn))!=-2)
				   return result;
			    break;
			case AXIS_CHILD:
			    if ( (result = process_child2(vn))!=-2)
				   return result;
			    break;
			case AXIS_DESCENDANT_OR_SELF0:
			case AXIS_DESCENDANT0:
			case AXIS_PRECEDING0:
			case AXIS_FOLLOWING0:
			    if ((result = process_DDFP(vn))!= -2)
			        return result;
			    break;
			case AXIS_DESCENDANT_OR_SELF:
			case AXIS_DESCENDANT:
			case AXIS_PRECEDING:
			case AXIS_FOLLOWING:
			    if ((result = process_DDFP2(vn))!= -2)
			        return result;
			    break;
			case AXIS_PARENT:
			    if ((result = process_parent(vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR:
			    if ((result = process_ancestor2(vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR_OR_SELF:
			    if ((result = process_ancestor_or_self(vn))!= -2)
			        return result;
			    break;
			case AXIS_SELF:
			    if ((result = process_self(vn))!= -2)
			        return result;
			    break;
			case AXIS_FOLLOWING_SIBLING:
			    if ((result = process_following_sibling2(vn))!= -2)
			        return result;
			    break;
			case AXIS_FOLLOWING_SIBLING0:
			    if ((result = process_following_sibling(vn))!= -2)
			        return result;
			    break;
			case AXIS_PRECEDING_SIBLING:
			    if ((result = process_preceding_sibling2(vn))!= -2)
			        return result;
			    break;
			case AXIS_PRECEDING_SIBLING0:
			    if ((result = process_preceding_sibling(vn))!= -2)
			        return result;
			    break;
			case AXIS_ATTRIBUTE:
			    if ((result = process_attribute(vn))!= -2)
			        return result;
			    break;
			case AXIS_NAMESPACE:
			    if ((result = process_namespace(vn))!= -2)
			        return result;
				break;
			default:
				throw XPathEvalException("axis not supported");
			}
		}
}
UCSChar* LocationPathExpr::evalString(VTDNav *vn){
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

void LocationPathExpr::reset(VTDNav *vn){
	Step *temp = s;
	state = XPATH_EVAL_START;
	ih->reset();
	currentStep = NULL;
	while(temp!=NULL){
		temp->reset_s(vn);
		temp = temp->nextS;
	}
}
void LocationPathExpr::toString(UCSChar *string){
	Step *ts = s;
	if (pathType == ABSOLUTE_PATH){
		wprintf(L"/");
	}
	if (ts == NULL)
		return;
	else
		ts->toString_s(string);
}

bool LocationPathExpr::isNumerical(){return false;}
bool LocationPathExpr::isNodeSet(){return true;}
bool LocationPathExpr::isString(){return false;}
bool LocationPathExpr::isBoolean(){return false;}

bool LocationPathExpr::requireContextSize(){return false;}
void LocationPathExpr::setContextSize(int size){}

void LocationPathExpr::setPosition(int pos){}
int LocationPathExpr::adjust(int n){
	//printf("adjusting \n");
	int i;
	if (pathType == RELATIVE_PATH){
		i= min(6,IntHash::determineHashWidth(n));//hashwidth 64
	} else {
		i=IntHash::determineHashWidth(n);
	}
	if (ih!=NULL && i<=ih->e)
	{}else {
		delete (ih);
		ih =  new IntHash(i);
	}
	Step *temp = s;
	while(temp!=NULL){
		temp->adjust(n);
		temp = temp->nextS;
	}	
	return i;
}


int LocationPathExpr::computeContextSize(Predicate *p, VTDNav *vn){
	bool b = false;
	//Predicate *tp = NULL;
    int i = 0;
    AutoPilot *ap = (AutoPilot *)currentStep->o;
	UCSChar *helper = NULL;
	switch(currentStep->axis_type){
    	case AXIS_CHILD0:
			return computeContextSize4Child(p,vn);
		case AXIS_CHILD:
			return computeContextSize4Child2(p,vn);
		case AXIS_DESCENDANT_OR_SELF0:
		case AXIS_DESCENDANT0:
		case AXIS_PRECEDING0:
		case AXIS_FOLLOWING0:
			return computeContextSize4DDFP(p,vn);
		case AXIS_DESCENDANT_OR_SELF:
		case AXIS_DESCENDANT:
		case AXIS_PRECEDING:
		case AXIS_FOLLOWING:
			return computeContextSize4DDFP2(p,vn);

		case AXIS_PARENT:
			return computeContextSize4Parent2(p,vn);

		case AXIS_ANCESTOR:
			return computeContextSize4Ancestor2(p,vn);		

		case AXIS_ANCESTOR_OR_SELF:
			return computeContextSize4AncestorOrSelf2(p,vn);	

		case AXIS_SELF:
			return computeContextSize4Self2(p,vn);	

		case AXIS_FOLLOWING_SIBLING:
			return computeContextSize4FollowingSibling2(p,vn);
		case AXIS_FOLLOWING_SIBLING0:
			return computeContextSize4FollowingSibling(p,vn);
		case AXIS_PRECEDING_SIBLING:
			return computeContextSize4PrecedingSibling2(p,vn);
		case AXIS_PRECEDING_SIBLING0:
			return computeContextSize4PrecedingSibling(p,vn);

		case AXIS_ATTRIBUTE:
			if (ap==NULL)
					ap =  new AutoPilot(vn);
				else
					ap->bind(vn);
			//ap = createAutoPilot(vn);
			if (currentStep->nt->testType == NT_NODE)
				ap->selectAttr((UCSChar *)L"*");
			else if (currentStep->nt->localName!=NULL)
				ap->selectAttrNS(currentStep->nt->URL,
				currentStep->nt->localName);
			else
				ap->selectAttr(currentStep->nt->nodeName);
			i = 0;
			while(ap->iterateAttr2()!=-1){
				if (currentStep->evalPredicates2( vn, p)){
					i++;
				}
			}
			currentStep->resetP2_s(vn,p);
			currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		case AXIS_NAMESPACE:
			if (ap==NULL)
					ap = new AutoPilot(vn);
				else
					ap->bind(vn);
			if (currentStep->nt->testType == NT_NODE)
				ap->selectNameSpace((UCSChar *)L"*");
			else
				ap->selectNameSpace(currentStep->nt->nodeName);
			i = 0;
			while(ap->iterateNameSpace()!=-1){
				if (currentStep->evalPredicates2( vn, p)){
					i++;
				}
			}
			currentStep->resetP2_s(vn,p);
			currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		default:
			throw XPathEvalException("unknown state");
			return 0;
	}
}
int LocationPathExpr::process_ancestor_or_self(VTDNav *vn){
	bool b = false, b1= false;
	int result;
	//int contextSize;
	Predicate *t= NULL;

	switch ( state) {
		case  XPATH_EVAL_START:
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				state = XPATH_EVAL_END;
				break;
			}

			state =  XPATH_EVAL_END;
			vn->push2();

			if (currentStep->ft){
				currentStep->ft=(false);
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			while (vn->toElement(PARENT)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			if ( state ==  XPATH_EVAL_END) {
				if (currentStep->hasPredicate)
					currentStep->resetP_s( vn);
				vn->pop2();
			}

			break;

		case  XPATH_EVAL_FORWARD:
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				currentStep = currentStep->prevS;
				state = XPATH_EVAL_BACKWARD;
				break;
			}

			state =  XPATH_EVAL_BACKWARD;
			vn->push2();
			if (currentStep->ft) {
				currentStep->ft= false;
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			while (vn->toElement(PARENT)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			if ( state ==  XPATH_EVAL_BACKWARD) {
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				currentStep->ft = true;
				vn->pop2();
				currentStep = currentStep->prevS;
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
	    	return -1;


		case  XPATH_EVAL_BACKWARD:
			//b = false;
			vn->push2();

			while (vn->toElement(PARENT)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						b = true;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			if (b == false) {
				vn->pop2();
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				if (currentStep->prevS != NULL) {
					currentStep->ft = true;
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else {
					state =  XPATH_EVAL_END;
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			while (vn->toElement(PARENT)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			vn->pop2();
			if (currentStep->hasPredicate)
				currentStep->resetP_s(vn);
			if (currentStep->prevS != NULL) {
				currentStep->ft = true;
				state =  XPATH_EVAL_BACKWARD;
				currentStep = currentStep->prevS;
			}
			else {
				 state =  XPATH_EVAL_END;
			}
			break;


		default:
			throw XPathEvalException("unknown state");
		}
		return -2;
}
int LocationPathExpr::process_ancestor(VTDNav *vn){
	int result;
	bool b = false, b1 = false;
	//int contextSize;
	Predicate *t= NULL;

	switch(state){
			case XPATH_EVAL_START:
				t = currentStep->p;
				while (t != NULL) {
					if (t->requireContext) {
						int i = computeContextSize(t, vn);
						if (i == 0) {
							b1 = true;
							break;
						} else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1) {
					state = XPATH_EVAL_END;
					break;
				}

				state = XPATH_EVAL_END;
				if (vn->getCurrentDepth() != -1) {
					vn->push2();

					while (vn->toElement(PARENT)) {
						if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
	    	            		&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
							if (currentStep->nextS != NULL) {
								state = XPATH_EVAL_FORWARD;
								currentStep = currentStep->nextS;
								break;
							} else {
								//vn.pop();
								state = XPATH_EVAL_TERMINAL;
								result = vn->getCurrentIndex();
								if (isUnique(result))
									return result;
							}
						}
					}
					if (state == XPATH_EVAL_END) {
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						vn->pop2();
					}
				}
				break;

			case XPATH_EVAL_END:
				currentStep =NULL;
				// reset();
				return -1;

			case XPATH_EVAL_FORWARD:
				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContext){
						int i = computeContextSize(t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					currentStep = currentStep->prevS;
					state = XPATH_EVAL_BACKWARD;
					break;
				}
				state =  XPATH_EVAL_BACKWARD;
				vn->push2();

				while(vn->toElement(PARENT)){
					if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
			   				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
						if (currentStep->nextS != NULL){
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							break;
						}
						else {
							//vn.pop();
							state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if ( state== XPATH_EVAL_BACKWARD){
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					vn->pop2();
					currentStep=currentStep->prevS;
				}
				break;

			case XPATH_EVAL_BACKWARD:
				b = false;
				vn->push2();

				while (vn->toElement(PARENT)) {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS!= NULL) {
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							b = true;
							break;
						} else {
							//vn.pop();
							state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if (b==false){
					vn->pop2();
					if (currentStep->prevS!=NULL) {
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
					else {
						state =  XPATH_EVAL_END;
					}
				}
				break;

			case XPATH_EVAL_TERMINAL:
				while (vn->toElement(PARENT)) {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				vn->pop2();

				if (currentStep->prevS!=NULL) {
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}
				else {
					state =  XPATH_EVAL_END;
				}
				break;

			default:
				throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_attribute(VTDNav *vn){
	AutoPilot *ap = NULL;
	bool b1 = false;
	Predicate *t= NULL;
	int temp;
	switch(state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = currentStep->p;
			while(t!=NULL){
				if (t->requireContext){
					int i = computeContextSize(t,vn);
					if (i==0){
						b1 = true;
						break;
					}else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1){
				if (state == XPATH_EVAL_FORWARD){
					state= XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}else
					state= XPATH_EVAL_END;
				break;
			}

			if (vn->atTerminal){
				if (state ==XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else {
					state = XPATH_EVAL_BACKWARD;
					currentStep  = currentStep->prevS;
				}
			} else {
				if (currentStep->ft == true) {
					if (currentStep->o == NULL)
						currentStep->o = ap = new AutoPilot(vn);
					else {
						ap = currentStep->o;
						ap->bind(vn);
					}
					if (currentStep->nt->testType== NT_NODE)
						ap->selectAttr((UCSChar *)L"*");
					else if (currentStep->nt->localName != NULL)
						ap->selectAttrNS(currentStep->nt->URL,
                                currentStep->nt->localName);
					else 
						ap->selectAttr(currentStep->nt->nodeName);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;
				vn->atTerminal=(true);
				while( (temp = ap->iterateAttr2()) != -1){
					if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
						break;
					}
				}
				if (temp == -1){
					currentStep->ft = true;
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					vn->atTerminal=false;
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				}else {

					if (currentStep->nextS != NULL){
						vn->LN = temp;
						state=  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state=  XPATH_EVAL_TERMINAL;
						if ( isUnique(temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = currentStep->o;
			//vn.push();
			while( (temp = ap->iterateAttr2()) != -1){
				if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp == -1) {
				currentStep->ft = true;
				//freeAutoPilot(currentStep->o);
				//currentStep->o = NULL;
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				vn->atTerminal=(false);
				if (currentStep->prevS != NULL) {
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else
					state =  XPATH_EVAL_END;
			} else {
				if (currentStep->nextS != NULL) {
					state =  XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state =  XPATH_EVAL_TERMINAL;
					if ( isUnique(temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = currentStep->o;
			while( (temp = ap->iterateAttr2()) != -1){
				if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique(temp)){
					vn->LN = temp;
					return temp;
				}
				vn->atTerminal=(false);
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				currentStep->ft = true;
				if (currentStep->prevS == NULL) {
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					state=  XPATH_EVAL_END;
				} else {
					state=  XPATH_EVAL_BACKWARD;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					currentStep = currentStep->prevS;
				}

				break;

		default:
			throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_child(VTDNav *vn){
	int result;
	bool b = false, b1 = false;
	Predicate *t= NULL;

	switch(state){
				case XPATH_EVAL_START:
					//if (currentStep->nt->testType < NT_TEXT){
						/* first search for any predicate that
						// requires contextSize
						// if so, compute its context size
						// if size > 0
						// set context
						// if size ==0
						// immediately set the state to backward or end*/
						t = currentStep->p;
						while(t!=NULL){
							if (t->requireContext){
								int i = computeContextSize(t,vn);
								if (i==0){
									b1 = true;
									break;
								}else
									t->setContextSize_p(i);
							}
							t = t->nextP;
						}
						if (b1){
							state= XPATH_EVAL_END;
							break;
						}

						b=vn->toElement(FIRST_CHILD);
						state=  XPATH_EVAL_END;
						if (b == true){
						 do {
							 if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
									&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
								 if (currentStep->nextS != NULL){
									 //currentStep.position++;
									 state=  XPATH_EVAL_FORWARD;
									 currentStep = currentStep->nextS;
								 }
								 else {
									 state=  XPATH_EVAL_TERMINAL;
									 result = vn->getCurrentIndex();
									 if ( isUnique(result)){
										 return result;
									 }
								 }
								 break;
							 }
						 } while (vn->toElement(NEXT_SIBLING));
						 if (state == XPATH_EVAL_END)
							 vn->toElement(PARENT);
					 }
					
					break;
				case XPATH_EVAL_END:
					currentStep =NULL;
					// reset();
					return -1;

				case XPATH_EVAL_FORWARD:
						t = currentStep->p;
						while(t!=NULL){
							if (t->requireContext){
								int i = computeContextSize( t,vn);
								if (i==0){
									b1 = true;
									break;
								}else
									t->setContextSize_p(i);
							}
							t = t->nextP;
						}
						if (b1){
							currentStep = currentStep->prevS;
							state= XPATH_EVAL_BACKWARD;
							break;
						}

						state =  XPATH_EVAL_BACKWARD;
						if (vn->toElement(FIRST_CHILD)) {
							do {
								if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
									&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
									if (currentStep->nextS != NULL) {
										state=  XPATH_EVAL_FORWARD;
										currentStep = currentStep->nextS;
									} else {
										state=  XPATH_EVAL_TERMINAL;
										result = vn->getCurrentIndex();
										if ( isUnique(result))
											return result;
									}
									goto forward;
								}
							} while (vn->toElement(NEXT_SIBLING));
							vn->toElement(PARENT);
							if (currentStep->hasPredicate)
								currentStep->resetP_s(vn);
							currentStep = currentStep->prevS;
						} else {
							//vn.toElement(VTDNav.P);
							currentStep = currentStep->prevS;
						}
				forward:;

					break;

				case XPATH_EVAL_BACKWARD:
						if(currentStep->out_of_range){
							currentStep->out_of_range = false;
							if (currentStep->hasPredicate)
								currentStep->resetP_s(vn);
							transition_child(vn);
							break;
						}
						b = false;
						while (vn->toElement(NEXT_SIBLING)) {
							if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
									&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
								b = true;
								break;
							}
						}
						if (b ) {
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
						} else {
							if (currentStep->hasPredicate)
		    					currentStep->resetP_s(vn);
							transition_child(vn);
						}
					
					break;

				case XPATH_EVAL_TERMINAL:
						if(currentStep->out_of_range){
							currentStep->out_of_range = false;
							if (currentStep->hasPredicate)
								currentStep->resetP_s(vn);
							transition_child(vn);
							break;
						}
						while (vn->toElement(NEXT_SIBLING)) {
							if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
									&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
								result = vn->getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						transition_child(vn);
				
					break;

				default:
					throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_DDFP(VTDNav *vn){
	AutoPilot *ap;
	bool b = false, b1 = false;
	Predicate *t= NULL;
	int result;
	UCSChar *helper;
	switch(state){
			case XPATH_EVAL_START:
			case XPATH_EVAL_FORWARD:
				if (vn->atTerminal){
					if (state == XPATH_EVAL_START)
						state= XPATH_EVAL_END;
					else {
						// no need to set_ft to true
						// no need to resetP
						state= XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
					break;
				}

				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContext){
						int i = computeContextSize( t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					if (state ==XPATH_EVAL_START)
						state= XPATH_EVAL_END;
					else {
						currentStep = currentStep->prevS;
						state= XPATH_EVAL_BACKWARD;
					}
					break;
				}


				helper = NULL;
				if (currentStep->nt->testType == NT_NAMETEST){
					helper = currentStep->nt->nodeName;
				}else if (currentStep->nt->testType == NT_NODE){
					helper = (UCSChar *) L"*";
				}else
    				throw XPathEvalException(
					"can't run descendant following, or following-sibling axis over comment(), pi(), and text()"); 
				if (currentStep->o == NULL)
					currentStep->o = ap = new AutoPilot(vn);
				else{
					ap = currentStep->o;
					ap->bind(vn);
				}
				if (currentStep->ft == true) {

					if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF0 ){
						if (currentStep->nt->testType == NT_NODE)
							ap->setSpecial(true);
						else
							ap->setSpecial(false);
					}
					//currentStep.o = ap = createAutoPilot(vn);
					if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF0)
						ap->selectElement(helper);
					else if (currentStep->axis_type == AXIS_DESCENDANT0)
						ap->selectElement_D(helper);
					else if (currentStep->axis_type == AXIS_PRECEDING0)
						ap->selectElement_P(helper);
					else
						ap->selectElement_F(helper);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;

				vn->push2(); // not the most efficient. good for now
				//System.out.println("  --++ push in //");
				b = false;
				while(ap->iterate()){
					if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
						b = true;
						break;
					}
				}
				if (b == false) {
					vn->pop2();
					//System.out.println("  --++ pop in //");
					currentStep->ft = true;
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				} else {
					if (currentStep->nextS != NULL){
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				break;

			case XPATH_EVAL_END:
				currentStep = NULL;
				// reset();
				return -1;

			case XPATH_EVAL_BACKWARD:
				//currentStep = currentStep->prevS;
				if (currentStep->out_of_range){
					currentStep->out_of_range = false;
					transition_DDFP(vn);
					break;
				}
				ap = currentStep->o;
				//vn.push();
				b = false;
				while(ap->iterate()){
					if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
						b = true;
						break;
					}
				}
				if (b ) {
					if (currentStep->nextS != NULL) {
						//vn.push();
						//System.out.println("  --++ push in //");
						 state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					} else {
						 state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}									
				} else {
					transition_DDFP(vn);
				}
				break;

			case XPATH_EVAL_TERMINAL:
				if (currentStep->out_of_range){
					currentStep->out_of_range = false;
					transition_DDFP(vn);
					break;
				}
				ap = currentStep->o;
				b = false;
				while (ap->iterate()) {
					if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)) {
						b = true;
						break;
					}
				}
				 if (b ) {
			        //if (currentStep.evalPredicates(vn)) {
			        result = vn->getCurrentIndex();
			        if (isUnique(result))
			            return result;
			        //}
			    } else {
			    	transition_DDFP(vn);
			    }
				break;

			default:
				throw XPathEvalException(
					"unknown state");
	}
	return -2;
}
int LocationPathExpr::process_following_sibling(VTDNav *vn){

	bool b = false, b1 = false;
	//int contextSize;
	Predicate *t= NULL;
	int result;
	switch( state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:

			  t = currentStep->p;
			  while(t!=NULL){
				  if (t->requireContext){
					  int i = computeContextSize( t,vn);
					  if (i==0){
						  b1 = true;
						  break;
					  }else
						  t->setContextSize_p(i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (state == XPATH_EVAL_FORWARD){
					  state= XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }else
					  state= XPATH_EVAL_END;
				  break;
			  }
			  if ( state==  XPATH_EVAL_START)
				  state=  XPATH_EVAL_END;
			  else
				  state=  XPATH_EVAL_BACKWARD;
			  vn->push2();
			  while (vn->toElement(NEXT_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (state == XPATH_EVAL_END){
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				vn->pop2();
			  }else if ( state==  XPATH_EVAL_BACKWARD){
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				vn->pop2();
				  currentStep = currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (vn->toElement(NEXT_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  b = true;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (b==false){
				  vn->pop2();
				  if (currentStep->hasPredicate)
					  currentStep->resetP_s(vn);
				  if (currentStep->prevS==NULL){
					  state=  XPATH_EVAL_END;
				  }else{
					  state=  XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (vn->toElement(NEXT_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  // state=  XPATH_EVAL_TERMINAL;
					  result = vn->getCurrentIndex();
					  if ( isUnique(result))
						  return result;
				  }
			  }
			  vn->pop2();
			  if (currentStep->hasPredicate)
				  currentStep->resetP_s(vn);
			  if(currentStep->prevS!=NULL){
				  currentStep = currentStep->prevS;
				  state=  XPATH_EVAL_BACKWARD;
			  }else{
				  state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_parent(VTDNav *vn){
	bool /*b = false,*/ b1 = false;
	Predicate *t= NULL;
	int result;
	switch ( state) {
			case  XPATH_EVAL_START:
			case  XPATH_EVAL_FORWARD:
				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContext){
						int i = computeContextSize(t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					if (state == XPATH_EVAL_FORWARD){
						state= XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}else
						state= XPATH_EVAL_END;
					break;
				}

				if (vn->getCurrentDepth() == -1) {
					if ( state==  XPATH_EVAL_START)
						state=  XPATH_EVAL_END;
					else {
						//vn.pop();
						state=  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				} else {
					vn->push2();
					vn->toElement(PARENT); // must return true
					if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
    						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
						if (currentStep->nextS != NULL) {
							state=  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
						} else {
							state=  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}else{
						vn->pop2();
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						if ( state==  XPATH_EVAL_START)
							state=  XPATH_EVAL_END;
						else {
							state=  XPATH_EVAL_BACKWARD;
							currentStep = currentStep->prevS;
						}
					}
				}

				break;

			case  XPATH_EVAL_END:
				currentStep = NULL;
				// reset();
				return -1;

			case  XPATH_EVAL_BACKWARD:
			case  XPATH_EVAL_TERMINAL:
				if (currentStep->prevS == NULL) {
					vn->pop2();
					state=  XPATH_EVAL_END;
					break;
				}else {
					vn->pop2();
					state=  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
					break;
				}

			default:
				throw XPathEvalException("unknown state");

	}
	return -2;
}
int LocationPathExpr::process_preceding_sibling(VTDNav *vn){

	bool b = false, b1 = false;
	Predicate *t= NULL;
	int result;
	switch(state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
			  t = currentStep->p;
			  while(t!=NULL){
				  if (t->requireContext){
					  int i = computeContextSize(t,vn);
					  if (i==0){
						  b1 = true;
						  break;
					  }else
						  t->setContextSize_p(i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (state == XPATH_EVAL_FORWARD){
					  state= XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }else
					  state= XPATH_EVAL_END;
				  break;
			  }
			  if ( state==  XPATH_EVAL_START)
				  state=  XPATH_EVAL_END;
			  else
				  state=  XPATH_EVAL_BACKWARD;
			  vn->push2();
			  while (vn->toElement(PREV_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (state == XPATH_EVAL_END){
				if (currentStep->hasPredicate)
					  currentStep->resetP_s(vn);
				vn->pop2();
			  }else if ( state==  XPATH_EVAL_BACKWARD){
				  if (currentStep->hasPredicate)
					  currentStep->resetP_s(vn);
				  vn->pop2();
				  currentStep = currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (vn->toElement(PREV_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  b = true;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (b==false){
				  vn->pop2();
				  if (currentStep->hasPredicate)
					  currentStep->resetP_s(vn);
				  if (currentStep->prevS==NULL){
					  state=  XPATH_EVAL_END;
				  }else{
					  state=  XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (vn->toElement(PREV_SIBLING)){
				  if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
					  // state =  XPATH_EVAL_TERMINAL;
					  result = vn->getCurrentIndex();
					  if ( isUnique(result))
						  return result;
				  }
			  }
			  if (currentStep->hasPredicate)
		  		currentStep->resetP_s(vn);
			  vn->pop2();
			  if(currentStep->prevS!=NULL){
				  currentStep = currentStep->prevS;
				  state=  XPATH_EVAL_BACKWARD;
			  }else{
				  state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_self(VTDNav *vn){		
	
	bool /*b = false,*/ b1 = false;
	    //int contextSize;
	    Predicate *t= NULL;
	    int result;
		switch( state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
  	        t = currentStep->p;
	        while(t!=NULL){
	            if (t->requireContext){
	                int i = computeContextSize(t,vn);
	                if (i==0){
	                    b1 = true;
	                    break;
	                }else
	                    t->setContextSize_p(i);
	            }
	            t = t->nextP;
	        }
	        if (b1){
	            if (state == XPATH_EVAL_FORWARD){
	                state= XPATH_EVAL_BACKWARD;
	                currentStep = currentStep->prevS;
	            }else
	                state= XPATH_EVAL_END;
	            break;
	        }
		  	if ((currentStep->nt_eval || currentStep->nt->eval_nt(vn)) 
		  			&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  		if (currentStep->nextS!=NULL){
		  			 state=  XPATH_EVAL_FORWARD;
		  			currentStep = currentStep->nextS;
		  		}
		  		else{
		  			 state=  XPATH_EVAL_TERMINAL;
		  			 if (vn->atTerminal)
		  			     result = vn->LN;
		  			 else
		  			     result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}else {
				if (currentStep->hasPredicate)
			  		currentStep->resetP_s(vn);
		  		if ( state==  XPATH_EVAL_START)
		  			 state=  XPATH_EVAL_END;
		  		else
		  			 state=  XPATH_EVAL_BACKWARD;
		  	}
		    break;

		  case  XPATH_EVAL_END:
		  	currentStep = NULL;
		  	// reset();
		  	return -1;

		  case  XPATH_EVAL_BACKWARD:
		  case  XPATH_EVAL_TERMINAL:
		  	if (currentStep->prevS!=NULL){
	  			 state=  XPATH_EVAL_BACKWARD;
	  			currentStep= currentStep->prevS;
	  		}else{
	  			 state=  XPATH_EVAL_END;
	  		}
		  	break;

		  default:
			throw XPathEvalException("unknown state");
		}
	    return -2;
}
int LocationPathExpr::process_namespace(VTDNav *vn){
	AutoPilot *ap = NULL;
	bool b1 = false;
	Predicate *t= NULL;
	int temp;
	switch(state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = currentStep->p;
			while(t!=NULL){
				if (t->requireContext){
					int i = computeContextSize(t,vn);
					if (i==0){
						b1 = true;
						break;
					}else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1){
				if (state == XPATH_EVAL_FORWARD){
					state= XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}else
					state= XPATH_EVAL_END;
				break;
			}

			if (vn->atTerminal){
				if (state ==XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else {
					state = XPATH_EVAL_BACKWARD;
					currentStep  = currentStep->prevS;
				}
			} else {
				if (currentStep->ft == true) {
					if (currentStep->o == NULL)
						currentStep->o = ap = new AutoPilot(vn);
					else {
						ap = currentStep->o;
						ap->bind(vn);
					}
					if (currentStep->nt->testType== NT_NODE)
						ap->selectNameSpace((UCSChar *)L"*");
					else 
						ap->selectNameSpace(currentStep->nt->nodeName);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;
				vn->push2();
				//vn->setAtTerminal(true);
				while( (temp = ap->iterateNameSpace()) != -1){
					if ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn)){
						break;
					}
				}
				if (temp == -1){
					currentStep->ft = true;
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					vn->atTerminal=(false);
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				}else {
					vn->atTerminal=true;
					if (currentStep->nextS != NULL){
						vn->LN = temp;
						state=  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state=  XPATH_EVAL_TERMINAL;
						if ( isUnique(temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = currentStep->o;
			//vn.push();
			while( (temp = ap->iterateNameSpace()) != -1){
				if ((!currentStep->hasPredicate)|| currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp == -1) {
				vn->pop2();
				currentStep->ft = true;
				//freeAutoPilot(currentStep->o);
				//currentStep->o = NULL;
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				vn->atTerminal=(false);
				if (currentStep->prevS != NULL) {
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else
					state =  XPATH_EVAL_END;
			} else {
				if (currentStep->nextS != NULL) {
					state =  XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state =  XPATH_EVAL_TERMINAL;
					if ( isUnique(temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = currentStep->o;
			while( (temp = ap->iterateNameSpace()) != -1){
				if (!currentStep->hasPredicate || currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique(temp)){
					vn->LN = temp;
					return temp;
				}
				vn->atTerminal=(false);
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				if (currentStep->prevS == NULL) {
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					vn->pop2();
					state=  XPATH_EVAL_END;
				} else {
					state=  XPATH_EVAL_BACKWARD;
					vn->pop2();
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					currentStep = currentStep->prevS;
				}

				break;

		default:
			throw XPathEvalException(
				"unknown state");
	}
	return -2;
}
void LocationPathExpr::selectNodeType(TextIter *ti){
		if (currentStep->nt->testType == NT_TEXT )
			ti->selectText();
		else if (currentStep->nt->testType == NT_COMMENT )
			ti->selectComment();
		else if (currentStep->nt->testType == NT_PI0 )
			ti->selectPI0();
		else {
			ti->selectPI1(currentStep->nt->nodeName);
		}
}



// predicate 
Predicate::Predicate():
d(0),
count(0),
nextP(NULL),
e(NULL),
requireContext(false),
type(COMPLEX_P),
s(NULL),
fe(NULL)
{}

Predicate::~Predicate(){
	if (e!=NULL)
		delete e;
}
bool Predicate::eval_p(VTDNav *vn){
		count++;
		switch (type){
		case SIMPLE_P:
			if (d<count)
				return false;
			else if(d==count){
				if (s!=NULL){
					s->out_of_range=true;
				}else
					fe->out_of_range=true;
				
				return true;	
			}
	    default:
			bool b;
			e->setPosition(count);
			if (e->isNumerical()){		    
				b = (e->evalNumber(vn)== count);
			}
			else{ 
				b = e->evalBoolean(vn);
			}
			if (b) 
				return true;
			else 
				return false;
		}		
}

bool Predicate::eval2_p(VTDNav *vn){

	bool b;		
	count++; // increment the position
	e->setPosition(count);
	if (e->isNumerical()){		    
		b = (e->evalNumber(vn)== count);
	}
	else{ 
		b = e->evalBoolean(vn);
	}
	return b;
}

void Predicate::setIndex_p(int i){
	if (i<=0){
		throw XPathEvalException("Invalid index number");
	}
	d = (double) i;
}

void Predicate::toString_p( UCSChar *string){
	//String s = "["+expr+"]";
		if (nextP==NULL){
			wprintf(L"[");
			e->toString(string);
			wprintf(L"]");
		} else {
			wprintf(L"[");
			e->toString(string);
			wprintf(L"]");
			nextP->toString_p(string);
		}
}

void Predicate::adjust(int n){e->adjust(n);};

void Predicate::setContextSize_p(int size){
			e->setContextSize(size);
		
}
bool Predicate::requireContextSize_p(){
	return e->requireContextSize();
}

void Predicate::reset_p(VTDNav *vn){
			count = 0;
			e->reset(vn); // is this really needed?
}





Step::Step():
nt(NULL),
p(NULL),
pt(NULL),
nextS(NULL),
position(1),
prevS(NULL),
o(NULL),
ft(true),
hasPredicate(false),
nt_eval(false),
out_of_range(false)
{}


Step::~Step(){
	Predicate *tmp, *tmp2;
	
	if (p != NULL){
		tmp = p;
		tmp2 = tmp->nextP;
		while(tmp2!=NULL){
			delete(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextP;
		}
		delete(tmp);
	}
	if (nt->testType == NT_TEXT){
		if (o!=NULL)
			delete((TextIter *)o);
	}else
		delete((AutoPilot *)o);
	delete(nt);
}

void Step::reset_s(VTDNav *vn){
	ft = true;
	if (hasPredicate) resetP_s(vn);
	out_of_range=false;
	//position =1;
}

void Step::resetP_s(VTDNav *vn){
	Predicate *temp = p;
	while(temp!=NULL){
		temp->reset_p(vn);
		temp = temp->nextP;
	}
}
void Step::resetP2_s(VTDNav *vn, Predicate *p1){
		Predicate *temp = p;
		while(temp!=p1){
			temp->reset_p(vn);
			temp = temp->nextP;
		}
}

inline NodeTest *Step::getNodeTest(){
	return nt;
}

inline Step *Step::getNextStep(){
	return nextS;
}

inline bool Step::get_ft(){
	return ft;
}

inline void Step::set_ft(bool b){
	ft = b;
}

inline Step *Step::getPrevStep(){
	return prevS;
}

void Step::setNodeTest(NodeTest *n){
	nt = n;
	if (axis_type == AXIS_CHILD && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_CHILD0;
		}else if (axis_type == AXIS_DESCENDANT && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_DESCENDANT0;
		}else if (axis_type == AXIS_DESCENDANT_OR_SELF && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_DESCENDANT_OR_SELF0;
		}else if (axis_type == AXIS_FOLLOWING && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_FOLLOWING0;
		}else if (axis_type == AXIS_PRECEDING && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_PRECEDING0;
		}else if (axis_type == AXIS_FOLLOWING_SIBLING && n->testType ==NT_NAMETEST ){
			axis_type = AXIS_FOLLOWING_SIBLING0;
		}else if (axis_type == AXIS_PRECEDING_SIBLING&& n->testType ==NT_NAMETEST ){
			axis_type = AXIS_PRECEDING_SIBLING0;
		}
		if (n->testType== NT_NODE 
				|| (n->testType==NT_NAMETEST && wcscmp(n->nodeName, L"*")==0)){
			nt_eval= true;
		}
		
}

void Step::setPredicate(Predicate *p1){
	if (p == NULL){
		p = pt = p1;
	} else {
		pt->nextP = p1;
		pt = pt->nextP;			
	}
	setStep4Predicates();
	if (p1!=NULL) hasPredicate = true;
}

/*inline bool Step::eval_s(VTDNav *vn){
	return nt->eval_nt(vn) && ((!hasPredicate) || evalPredicates(vn));
}

inline bool Step::eval_s2(VTDNav *vn, Predicate *p){
	return nt->eval_nt(vn) && evalPredicates2(vn,p);
}

inline bool Step::eval2_s(VTDNav *vn){
	return nt->eval_nt2(vn) && ((!hasPredicate) || evalPredicates(vn));
}

inline bool Step::eval2_s2(VTDNav *vn, Predicate *p){
	return nt->eval_nt2(vn) && evalPredicates2(vn,p);
}*/

bool Step::evalPredicates(VTDNav *vn){
	Predicate *temp = p;
	while(temp!=NULL) {
		if (temp->eval_p(vn)== false)
			return false;
		temp = temp->nextP;
	}
	return true;
}

bool Step::evalPredicates2(VTDNav *vn, Predicate *p){
	Predicate *temp = p;
	while(temp!=p) {
		if (temp->eval_p(vn)== false)
			return false;
		temp = temp->nextP;
	}
	return true;
}



void Step::toString_s(UCSChar *string){
	//String s;
	if (p == NULL){
		wprintf(axisName(axis_type));
	    nt->toString_nt(string);
	}
	else {
		wprintf(axisName(axis_type));
		nt->toString_nt(string);
		wprintf(L" ");
		p->toString_p(string);
	}
	if (nextS == NULL)
		return;
	else {
		wprintf(L"/");
		nextS->toString_s(string);
	}
}

void Step::adjust(int n){
			Predicate* temp = p;
			while(temp!=NULL){
				temp->adjust(n);
				temp = temp->nextP;
			}
		}

void Step::setStep4Predicates(){
	Predicate *temp = p;
		while(temp!=NULL){
			temp->s=this;
			temp = temp->nextP;
		}
}




NodeTest::NodeTest():
nodeName(NULL),
prefix(NULL),
localName(NULL),
URL(NULL),
nsEnabled(false)
{
}

NodeTest::~NodeTest(){
	delete localName;
	delete nodeName;
	delete prefix;
}
bool NodeTest::eval_nt(VTDNav *vn){
	if (testType == NT_NODE)
		return true;
	switch(type){
			case 0: return true;
			case 1: return vn->matchElement(nodeName);
			case 2: return vn->matchElementNS(URL,localName);
		}
	return true;
}
bool NodeTest::eval_nt2(VTDNav *vn){
	int t;
	switch(testType){
		case NT_NAMETEST:
			if (vn->atTerminal)
		        return false;
			switch(type){
			case 0: return true;
			case 1: return vn->matchElement(nodeName);
			case 2: return vn->matchElementNS(URL,localName);
			}
		case NT_NODE:
			return true;
		case NT_TEXT:
			if (!vn->atTerminal)
		        return false;
			t = vn->getTokenType(vn->LN);
			if (t== TOKEN_CHARACTER_DATA
					|| t == TOKEN_CDATA_VAL){
				return true;
			}
			return false;
			
		case NT_PI0:
			if (!vn->atTerminal)
				return false;
			if (vn->getTokenType(vn->LN)==TOKEN_PI_NAME){
				return true;
			}
			return false;
		case NT_PI1:
			if (!vn->atTerminal)
				return false;
			if (vn->getTokenType(vn->LN)==TOKEN_PI_NAME){
				return vn->matchTokenString(vn->LN, nodeName);
			}
			return false;
			
		default: // comment
			if (!vn->atTerminal)
				return false;
			if (vn->getTokenType(vn->LN)== TOKEN_COMMENT){
				return true;
			}
			return false;
		}
}
void NodeTest::setNodeName(UCSChar *name){
	nodeName = name;
	if (wcscmp(name,L"*")==0){
		type=0;
	}else{
		type=1;
	}
}
void NodeTest::setNodeNameNS(UCSChar *p, UCSChar *ln){
	localName = ln;
	prefix = p;
	type=2;
}
/*inline void NodeTest::setTestType(nodeTestType ntt){
	testType = ntt;
}*/
void NodeTest::toString_nt(UCSChar *string){
	switch (testType){
		case NT_NAMETEST :
			if (localName == NULL)
				wprintf(L"%ls",nodeName);
			else
				wprintf(L"%ls:%ls", prefix,localName);
			break;
		case NT_NODE: wprintf(L"node()");break;
		case NT_TEXT: wprintf(L"text()");break;
		case NT_PI0:  wprintf(L"processing-instruction()");break;
		case NT_PI1: wprintf(L"processing-instruction(");
			if (wcschr(nodeName,'"')!=NULL){
				wprintf(L"'");
				wprintf(nodeName);
				wprintf(L"'");
			}else{
				wprintf(L"\"");
				wprintf(nodeName);
				wprintf(L"\"");
			}
			wprintf(L")");
			break;
		default:  wprintf(L"comment()");
	}
}









UCSChar *com_ximpleware::axisName(axisType i){
	switch(i){
			case AXIS_CHILD0: return (UCSChar *)L"child::";
			case AXIS_CHILD: return (UCSChar *)L"child::";
			case AXIS_DESCENDANT_OR_SELF0: return (UCSChar *)L"descendant-or-self::";
			case AXIS_DESCENDANT0: return (UCSChar *)L"descendant::";
			case AXIS_PRECEDING0: return (UCSChar *)L"preceding::";
			case AXIS_FOLLOWING0: return (UCSChar *)L"following::";
			case AXIS_DESCENDANT_OR_SELF: return (UCSChar *)L"descendant-or-self::";
			case AXIS_DESCENDANT: return (UCSChar *)L"descendant::";
			case AXIS_PRECEDING: return (UCSChar *)L"preceding::";
			case AXIS_FOLLOWING: return (UCSChar *)L"following::";
			case AXIS_PARENT: return (UCSChar *)L"parent::";
			case AXIS_ANCESTOR: return (UCSChar *)L"ancestor::";
			case AXIS_ANCESTOR_OR_SELF: return (UCSChar *)L"ancestor-or-self::";
			case AXIS_SELF: return (UCSChar *)L"self::";
			case AXIS_FOLLOWING_SIBLING: return (UCSChar *)L"following-sibling::";
			case AXIS_FOLLOWING_SIBLING0: return (UCSChar *)L"following-sibling::";
			case AXIS_PRECEDING_SIBLING: return (UCSChar *)L"preceding-sibling::";
			case AXIS_PRECEDING_SIBLING0: return (UCSChar *)L"preceding-sibling::";			
			case AXIS_ATTRIBUTE: return (UCSChar *)L"attribute::";
			//case AXIS_NAMESPACE: return (UCSChar *)L"namespace::";			
			default: return (UCSChar *)L"namespace::";
	}
}


void LocationPathExpr::clearCache(){
	Step *temp = s;
	while(temp!=NULL){
		if (temp->p!=NULL){
			temp->p->e->clearCache();
		}
		temp = temp->nextS;
	}
}

void LocationPathExpr::markCacheable(){
	Step *temp = s;
	while(temp!=NULL){
		if (temp->p!=NULL){
			if (temp->p->e!=NULL ){
				if (temp->p->e->isFinal() && temp->p->e->isNodeSet()){
					CachedExpr *ce = new CachedExpr(temp->p->e);
					temp->p->e = ce;
				}
				temp->p->e->markCacheable2();
			}
		}
		temp = temp->nextS;
	}
}

void LocationPathExpr::markCacheable2(){
	markCacheable();
}

int LocationPathExpr::computeContextSize4Ancestor(Predicate *p, VTDNav *vn){
		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		i = 0;
		while (vn->toElement(PARENT)) {
			if (currentStep->eval_s2(vn, p)) {
            	i++;
		    }	
		}				
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;	
}
int LocationPathExpr::computeContextSize4Ancestor2(Predicate *p, VTDNav *vn){

		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		i = 0;
		while (vn->toNode(PARENT)) {
			if (currentStep->eval2_s2(vn, p)) {
           		i++;
			}
		}				
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}
int LocationPathExpr::computeContextSize4AncestorOrSelf(Predicate *p, VTDNav *vn){
		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		i = 0;
		do {
			if (currentStep->eval_s2(vn, p)) {
            	i++;
		       }
		}while(vn->toElement(PARENT));
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}
int LocationPathExpr:: computeContextSize4AncestorOrSelf2(Predicate *p, VTDNav *vn){
		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		i = 0;
		do {
			if (currentStep->eval2_s2(vn, p)) {
            	i++;
		       }
		}while(vn->toNode(PARENT));
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}
int LocationPathExpr:: computeContextSize4Child(Predicate *p, VTDNav *vn){
		int i=0;
    	bool b = vn->toElement(FIRST_CHILD);
    	if (b) {
    	    do {
				if (currentStep->eval_s2(vn, p)) {
                   	i++;
    	        }
    	    } while (vn->toElement(NEXT_SIBLING));	    		    
    	    vn->toElement(PARENT);
			currentStep->resetP2_s(vn,p);
    	    currentStep->out_of_range=false;
    	    return i;
    	} else
    	    return 0;
}
int LocationPathExpr::computeContextSize4Child2(Predicate *p, VTDNav *vn){
		int i=0;
    	bool b = vn->toElement(FIRST_CHILD);
    	if (b) {
    	    do {
				if (currentStep->eval2_s2(vn, p)) {
                   	i++;
    	        }
    	    } while (vn->toNode(NEXT_SIBLING));	    		    
    	    vn->toNode(PARENT);
			currentStep->resetP2_s(vn,p);
    	    currentStep->out_of_range=false;
    	    return i;
    	} else
    	    return 0;
}
	
int LocationPathExpr::computeContextSize4DDFP(Predicate *p, VTDNav *vn){
		UCSChar *helper = NULL;
	    int i=0;
	    AutoPilot *ap = (AutoPilot *)currentStep->o;
		if (currentStep->nt->testType == NT_NODE){
		    helper = L"*";
		}else if (currentStep->nt->testType == NT_NAMETEST){
			helper = currentStep->nt->nodeName;
		}else
			throw new XPathEvalException("can't run descendant "\
					"following, or following-sibling axis over comment(), pi(), and text()");
		if (ap==NULL)
			ap = new AutoPilot(vn);
		else
			ap->bind(vn);
		if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF0 )
			if (currentStep->nt->testType == NT_NODE)
				ap->setSpecial(true);
			else
				ap->setSpecial(false);
		//currentStep.o = ap = new AutoPilot(vn);
	    if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF0)
	        if (currentStep->nt->localName!=NULL)
	            ap->selectElementNS(currentStep->nt->URL,currentStep->nt->localName);
	        else 
	            ap->selectElement(helper);
		else if (currentStep->axis_type == AXIS_DESCENDANT0)
		    if (currentStep->nt->localName!=NULL)
		        ap->selectElementNS_D(currentStep->nt->URL,currentStep->nt->localName);
		    else 
		        ap->selectElement_D(helper);
		else if (currentStep->axis_type == AXIS_PRECEDING0)
		    if (currentStep->nt->localName!=NULL)
		        ap->selectElementNS_P(currentStep->nt->URL,currentStep->nt->localName);
		    else 
		        ap->selectElement_P(helper);
		else 
		    if (currentStep->nt->localName!=NULL)
		        ap->selectElementNS_F(currentStep->nt->URL,currentStep->nt->localName);
		    else 
		        ap->selectElement_F(helper);
	    vn->push2();
		while(ap->iterate()){
			if (currentStep->evalPredicates2(vn,p)){
				i++;
			}
		}
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}
int LocationPathExpr::computeContextSize4DDFP2(Predicate *p, VTDNav *vn){
		int i=0;
	    AutoPilot *ap = (AutoPilot *)currentStep->o;
		
		if (ap==NULL)
			ap = new AutoPilot(vn);
		else
			ap->bind(vn);
		
		//currentStep.o = ap = new AutoPilot(vn);
	    if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
	       ap->selectNode();
		else if (currentStep->axis_type == AXIS_DESCENDANT)
		   ap->selectDescendantNode();
		else if (currentStep->axis_type == AXIS_PRECEDING)
		   ap->selectPrecedingNode();
		else 
		   ap->selectFollowingNode();
	    vn->push2();
		while(ap->iterate2()){
			if (currentStep->eval2_s2(vn,p)){
				i++;
			}
		}
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4FollowingSibling(Predicate *p, VTDNav *vn){
		int i=0;
		//AutoPilot ap = (AutoPilot)currentStep.o;
		vn->push2();
		while(vn->toElement(NEXT_SIBLING)){
			if (currentStep->eval_s2(vn,p)){
		        i++;
		    }
		}			    
	    vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		//currentStep.o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4FollowingSibling2(Predicate *p, VTDNav *vn){
		int i=0;
		//AutoPilot ap = (AutoPilot)currentStep.o;
		vn->push2();
		while(vn->toNode(NEXT_SIBLING)){
			if (currentStep->eval2_s2(vn,p)){
		        i++;
		    }
		}			    
	    vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		//currentStep.o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4Parent(Predicate *p, VTDNav *vn){
		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		//i = 0;
		if (vn->toElement(PARENT)){
			if (currentStep->eval_s2(vn,p)){
		        i++;
		    }
		}			    
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4Parent2(Predicate *p, VTDNav *vn){
		int i=0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		vn->push2();
		//i = 0;
		if (vn->toNode(PARENT)){
		    if (currentStep->eval2_s2(vn,p)){
		        i++;
		    }
		}			    
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4PrecedingSibling(Predicate *p, VTDNav *vn){
		int i=0;
		vn->push2();
		while(vn->toElement(PREV_SIBLING)){
		    if (currentStep->eval_s2(vn,p)){
		        i++;
		    }
		}			    
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		//currentStep.o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4PrecedingSibling2(Predicate *p, VTDNav *vn){
		int i=0;
		vn->push2();
		while(vn->toNode(PREV_SIBLING)){
		    if (currentStep->eval2_s2(vn,p)){
		        i++;
		    }
		}			    
		vn->pop2();
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		//currentStep.o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4Self(Predicate *p, VTDNav *vn){
		int i = 0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		//if (vn->toElement(PARENT)){
		if (currentStep->eval_s2(vn,p)){
		   i++;
		}
		//}			    
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}

int LocationPathExpr::computeContextSize4Self2(Predicate *p, VTDNav *vn){
		int i = 0;
		AutoPilot *ap = (AutoPilot *)currentStep->o;
		//if (vn->toNode(PARENT)){
		if (currentStep->eval2_s2(vn,p)){
		   i++;
		}
		//}			    
		currentStep->resetP2_s(vn,p);
		currentStep->out_of_range=false;
		currentStep->o = ap;
		return i;
}

void LocationPathExpr::optimize(){
				// get to last step
			Step *ts = s;
			if (ts==NULL)
				return;
			while(ts->nextS!=NULL){
				ts = ts->nextS;
			}
			
			while(ts->prevS !=NULL){
				// logic of optmize here
				if (ts->axis_type == AXIS_CHILD0
						|| ts->axis_type == AXIS_CHILD
						|| ts->axis_type == AXIS_ATTRIBUTE){
					switch(ts->prevS->axis_type){
					 case AXIS_CHILD:
						 if (ts->prevS->nt->testType == NT_NODE){
							 ts->prevS->axis_type = AXIS_CHILD0;
							 ts->prevS->nt->testType = NT_NAMETEST;
							 ts->prevS->nt->type= 0;
							 ts->prevS->nt->nodeName = L"*";
						 }
						 break;
					 case AXIS_DESCENDANT: 
						 ts->prevS->axis_type = AXIS_DESCENDANT0;
						 break;
					 case AXIS_DESCENDANT_OR_SELF:						 
						 ts->prevS->axis_type = AXIS_DESCENDANT_OR_SELF0;
						 break;
					 case AXIS_PRECEDING:						 
						 ts->prevS->axis_type = AXIS_PRECEDING0;
						 break;
					 case AXIS_FOLLOWING:
						 ts->prevS->axis_type = AXIS_FOLLOWING0;
						 break;
					 case AXIS_FOLLOWING_SIBLING:
						 ts->prevS->axis_type = AXIS_FOLLOWING_SIBLING0;
						 ts->prevS->nt->testType = NT_NAMETEST;
						 ts->prevS->nt->type= 0;
						 ts->prevS->nt->nodeName = L"*";
						 break;
					 case AXIS_PRECEDING_SIBLING:
					 	 ts->prevS->axis_type = AXIS_PRECEDING_SIBLING0;
					 	 ts->prevS->nt->testType = NT_NAMETEST;
						 ts->prevS->nt->type= 0;
						 ts->prevS->nt->nodeName = L"*";
						 break;
					}
				}
				
				ts= ts->prevS;
			}
			// rewrite steps
}

int LocationPathExpr::process_ancestor_or_self2(VTDNav *vn){
	    bool b = false, b1 = false;
	    Predicate *t= NULL;
	    int result;
		switch ( state) {
			case  XPATH_EVAL_START:
	    	    t = currentStep->p;
	    	    while (t != NULL) {
	    	        if (t->requireContext) {
	    	            int i = computeContextSize( t, vn);
	    	            if (i == 0) {
	    	                b1 = true;
	    	                break;
	    	            } else
	    	                t->setContextSize_p(i);
	    	        }
	    	        t = t->nextP;
	    	    }
	    	    if (b1) {
	    	        state = XPATH_EVAL_END;
	    	        break;
	    	    }
				state =  XPATH_EVAL_END;
				vn->push2();
				
				if (currentStep->ft){						
					currentStep->ft = false;
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS != NULL) {
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							break;
						} else {
							//vn.pop();
							state =  XPATH_EVAL_TERMINAL;
							if (vn->atTerminal)
							    result = vn->LN;
							else 
							    result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				
					while (vn->toNode(PARENT)) {
						if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
							if (currentStep->nextS != NULL) {
								 state =  XPATH_EVAL_FORWARD;
								currentStep = currentStep->nextS;
								break;
							} else {
								//vn.pop();
								 state =  XPATH_EVAL_TERMINAL;
								result = vn->getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					}
				
				if ( state ==  XPATH_EVAL_END) {
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					vn->pop2();
				}

				break;
				
			case  XPATH_EVAL_FORWARD:
	    	     t = currentStep->p;
	    	     while(t!=NULL){
	    	        if (t->requireContext){
	    	             int i = computeContextSize(t,vn);
	    	             if (i==0){
	    	                 b1 = true;
	    	                 break;
	    	             }else
	    	                 t->setContextSize_p(i);
	    	        }
	    	        t = t->nextP;
	    	    }
	    	    if (b1){
	    	        currentStep = currentStep->prevS;
	    	        state = XPATH_EVAL_BACKWARD;
	    	        break;
	    	    }
				 state =  XPATH_EVAL_BACKWARD;
					vn->push2();
					if (currentStep->ft ) {
						currentStep->ft = false;
						
						if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
								&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
							if (currentStep->nextS != NULL) {
								 state =  XPATH_EVAL_FORWARD;
								currentStep = currentStep->nextS;
								break;
							} else {
								//vn.pop();
								 state =  XPATH_EVAL_TERMINAL;
								 if (vn->atTerminal)
								     result = vn->LN;
								 else 
								     result = vn->getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					} 
						while (vn->toNode(PARENT)) {
							if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
									&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
								if (currentStep->nextS != NULL) {
									 state =  XPATH_EVAL_FORWARD;
									currentStep = currentStep->nextS;
									break;
								} else {
									//vn.pop();
									 state =  XPATH_EVAL_TERMINAL;
									result = vn->getCurrentIndex();
									if ( isUnique(result))
										return result;
								}
							}
						}
					
					if ( state ==  XPATH_EVAL_BACKWARD) {
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						currentStep->ft = true;
						vn->pop2();
						currentStep = currentStep->prevS;
					}
					break;
					
			case  XPATH_EVAL_END:
				currentStep = NULL;
				// reset();
		    	return -1;
				
			
			case  XPATH_EVAL_BACKWARD:
				//b = false;
				vn->push2();

				while (vn->toNode(PARENT)) {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS != NULL) {
							 state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							b = true;
							break;
						} else {
							//vn.pop();
							 state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if (b == false) {
					vn->pop2();
					if (currentStep->hasPredicate)
						currentStep->resetP_s(vn);
					if (currentStep->prevS != NULL) {
						currentStep->ft = true;
						 state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					} else {
						 state =  XPATH_EVAL_END;
					}
				}
				break;
			
			case  XPATH_EVAL_TERMINAL:
				while (vn->toNode(PARENT)) {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				vn->pop2();
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				if (currentStep->prevS!=NULL) {
					currentStep->ft = true;
					 state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}
				else {
					 state =  XPATH_EVAL_END;
				}
				break;
				
			
			default:
				throw new  XPathEvalException("unknown state");
		}
	    return -2;

}


int LocationPathExpr::process_ancestor2(VTDNav *vn){
	    int result;
	    bool b= false, b1 = false;
	    //int contextSize;
	    Predicate *t= NULL;
	    
	    switch(state){
	    	case XPATH_EVAL_START:
	    		
	    	    t = currentStep->p;
	    	    while (t != NULL) {
	    	        if (t->requireContext) {
	    	            int i = computeContextSize( t, vn);
	    	            if (i == 0) {
	    	                b1 = true;
	    	                break;
	    	            } else
	    	                t->setContextSize_p(i);
	    	        }
	    	        t = t->nextP;
	    	    }
	    	    if (b1) {
	    	        state = XPATH_EVAL_END;
	    	        break;
	    	    }

	    	    state = XPATH_EVAL_END;
	    	    //if (vn.getCurrentDepth() != -1) {
	    	        vn->push2();

	    	        while (vn->toNode(PARENT)) {
						if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
	    	            		&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
	    	                if (currentStep->nextS != NULL) {
	    	                    state = XPATH_EVAL_FORWARD;
	    	                    currentStep = currentStep->nextS;
	    	                    break;
	    	                } else {
	    	                    //vn.pop();
	    	                    state = XPATH_EVAL_TERMINAL;
	    	                    result = vn->getCurrentIndex();
	    	                    if (isUnique(result))
	    	                        return result;
	    	                }
	    	            }
	    	        }
	    	        if (state == XPATH_EVAL_END) {
	    	        	if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
	    	            vn->pop2();
	    	        }
	    	   // }
	    	    break;
    	        
	    	case XPATH_EVAL_END:   
				currentStep =NULL;
				// reset();
			    return -1;
			    
	    	case XPATH_EVAL_FORWARD:	    	    
	    	     t = currentStep->p;
	    	     while(t!=NULL){
	    	        if (t->requireContext){
	    	             int i = computeContextSize(t,vn);
	    	             if (i==0){
	    	                 b1 = true;
	    	                 break;
	    	             }else
	    	                 t->setContextSize_p(i);
	    	        }
	    	        t = t->nextP;
	    	    }
	    	    if (b1){
	    	        currentStep = currentStep->prevS;
	    	        state = XPATH_EVAL_BACKWARD;
	    	        break;
	    	    }
			    state =  XPATH_EVAL_BACKWARD;
			   	vn->push2();
					
			   	while(vn->toNode(PARENT)){
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
			   				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
			   			if (currentStep->nextS != NULL){
			   				 state =  XPATH_EVAL_FORWARD;
			   				currentStep = currentStep->nextS;
			   				break;
			   			}
			   			else {
			   				//vn.pop();
			   				 state =  XPATH_EVAL_TERMINAL;
			   				result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
			   			}
			   		}							
			   	}
			   	if ( state == XPATH_EVAL_BACKWARD){
					if (currentStep->hasPredicate)
				   		currentStep->resetP_s(vn);
					vn->pop2();
			   		currentStep=currentStep->prevS;
			   	}			    
			  	break;
	    	    
	    	case XPATH_EVAL_BACKWARD:
				b = false;
				vn->push2();

				while (vn->toNode(PARENT)) {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS!= NULL) {
							 state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							b = true;
							break;
						} else {
							//vn.pop();
							 state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if (b==false){
					vn->pop2();
					if (currentStep->prevS!=NULL) {
						if (currentStep->hasPredicate)
							currentStep->resetP_s(vn);
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
					else {
						 state =  XPATH_EVAL_END;
					}
				}
				break;
				
	    	case XPATH_EVAL_TERMINAL:			
	    	    while (vn->toNode(PARENT)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			vn->pop2();
			
			if (currentStep->prevS!=NULL) {
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				state =  XPATH_EVAL_BACKWARD;
				currentStep = currentStep->prevS;
			}
			else {
				 state =  XPATH_EVAL_END;
			}
			break;
		
		default:
			throw new  XPathEvalException("unknown state");
	    }
	    return -2;
}


//int LocationPathExpr::process_attribute2(VTDNav *vn){}


int LocationPathExpr::process_child2(VTDNav *vn){
		int result;
	    bool b=false;
		bool b1 = false;
	    Predicate *t= NULL;
	    
	    switch(state){
		case XPATH_EVAL_START:
			// first search for any predicate that
			// requires contextSize
			// if so, compute its context size
			// if size > 0
			// set context
			// if size ==0
			// immediately set the state to backward or end
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				state = XPATH_EVAL_END;
				break;
			}

			b = vn->toNode(FIRST_CHILD);
			state = XPATH_EVAL_END;
			if (b) {
				do {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS != NULL) {
							// currentStep.position++;
							state = XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;							
						} else {
							state = XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if (isUnique(result)) {
								return result;
							}
						}
						break;
					}
				} while (vn->toNode(NEXT_SIBLING));
				if (state == XPATH_EVAL_END)
					vn->toNode(PARENT);
			}
			break;

		case XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case XPATH_EVAL_FORWARD:

			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				currentStep = currentStep->prevS;
				state = XPATH_EVAL_BACKWARD;
				break;
			}

			state = XPATH_EVAL_BACKWARD;
			 if (vn->toNode(FIRST_CHILD)) {
				do {
					if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
							&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
						if (currentStep->nextS != NULL) {
							state = XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
						} else {
							state = XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if (isUnique(result))
								return result;
						}
						goto forward;
					}
				} while (vn->toNode(NEXT_SIBLING));
			
				vn->toNode(PARENT);
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				currentStep = currentStep->prevS;
			} else {
				// vn.toElement(VTDNav.P);
				currentStep = currentStep->prevS;
			}
forward:
			break;

		case XPATH_EVAL_BACKWARD:
			if (currentStep->out_of_range){
				currentStep->out_of_range = false;
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				transition_child(vn);
				break;
			}

			// currentStep = currentStep.prevS;
			//b = false;
			while (vn->toNode(NEXT_SIBLING)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					b = true;
					break;
				}
			}
			if (b) {
				state = XPATH_EVAL_FORWARD;
				currentStep = currentStep->nextS;
			} else{
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				transition_child(vn);
			}

			break;

		case XPATH_EVAL_TERMINAL:
			if (currentStep->out_of_range){
				currentStep->out_of_range = false;
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				transition_child(vn);
				break;
			}
			while (vn->toNode(NEXT_SIBLING)) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					// state = TERMINAL;
					result = vn->getCurrentIndex();
					if (isUnique(result))
						return result;
				}
			}
			if (currentStep->hasPredicate)
				currentStep->resetP_s(vn);
			transition_child(vn);

			break;

		default:
			throw new XPathEvalException("unknown state");
		}
	    return -2;
}


int LocationPathExpr::process_DDFP2(VTDNav *vn){
		AutoPilot *ap;
		bool b = false, b1 = false;
		Predicate *t = NULL;
		int result;
		//UCSChar *helper = NULL;
		switch (state) {
		case XPATH_EVAL_START:
		case XPATH_EVAL_FORWARD:
			/*if (vn.atTerminal) {
				if (state == START)
					state = END;
				else {
					// no need to set_ft to true
					// no need to resetP
					state = BACKWARD;
					currentStep = currentStep.prevS;
				}
				break;
			}*/

			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				if (state == XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else {
					currentStep = currentStep->prevS;
					state = XPATH_EVAL_BACKWARD;
				}
				break;
			}

			//UCSChar *helper = NULL;
			/*if (currentStep.nt.testType == NodeTest.NAMETEST) {
				helper = currentStep.nt.nodeName;
			} else if (currentStep.nt.testType == NodeTest.NODE) {
				helper = "*";
			} else
				throw new XPathEvalException(
						"can't run descendant "
								+ "following, or following-sibling axis over comment(), pi(), and text()");*/
			if (currentStep->o == NULL) {
				ap = new AutoPilot(vn);
				currentStep->o = ap;
			}
			else {
				ap = (AutoPilot *) currentStep->o;
				ap->bind(vn);
			}
			if (currentStep->ft) {

				/*if (currentStep.axis_type == AxisType.DESCENDANT_OR_SELF)
					if (currentStep.nt.testType == NodeTest.NODE)
						ap.setSpecial(true);
					else
						ap.setSpecial(false);*/
				// currentStep.o = ap = new AutoPilot(vn);
				if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
					ap->selectNode();
				else if (currentStep->axis_type == AXIS_DESCENDANT)
					ap->selectDescendantNode();
				else if (currentStep->axis_type == AXIS_PRECEDING)
					ap->selectPrecedingNode();
				else
					ap->selectFollowingNode();
				currentStep->ft = false;
			}
			if (state == XPATH_EVAL_START)
				state = XPATH_EVAL_END;

			vn->push2(); // not the most efficient. good for now
			// System.out.println("  --++ push in //");
			b = false;
			while (ap->iterate2()) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					b = true;
					break;
				}
			}
			if (b == false) {
				vn->pop2();
				// System.out.println("  --++ pop in //");
				currentStep->ft = true;
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				if (state == XPATH_EVAL_FORWARD) {
					state = XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}
			} else {
				if (currentStep->nextS != NULL) {
					state = XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					// vn.pop();
					state = XPATH_EVAL_TERMINAL;
					result = vn->getCurrentIndex();
					if (isUnique(result))
						return result;
				}
			}
			break;

		case XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case XPATH_EVAL_BACKWARD:
			if(currentStep->out_of_range){
				currentStep->out_of_range = false;
				transition_DDFP(vn);
				break;
			}
			// currentStep = currentStep.prevS;
			ap = (AutoPilot *) currentStep->o;
			// vn.push();
			//b = false;
			while (ap->iterate2()) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					b = true;
					break;
				}
			}
			if (b ) {
				if (currentStep->nextS != NULL) {
					// vn.push();
					// System.out.println("  --++ push in //");
					state = XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state = XPATH_EVAL_TERMINAL;
					result = vn->getCurrentIndex();
					if (isUnique(result))
						return result;
				}
			} else 
				transition_DDFP(vn);
			break;

		case XPATH_EVAL_TERMINAL:
			if(currentStep->out_of_range){
				currentStep->out_of_range = false;
				transition_DDFP(vn);
				break;
			}
			ap = (AutoPilot *) currentStep->o;
			b = false;
			while (ap->iterate2()) {
				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
					b = true;
					break;
				}
			}
			if (b) {
				//if (currentStep.evalPredicates(vn)) {
				result = vn->getCurrentIndex();
				if (isUnique(result))
					return result;
				//}
			} else{ 
				transition_DDFP(vn);
			}
			break;

		default:
			throw new XPathEvalException("unknown state");
		}
		return -2;
}


int LocationPathExpr::process_following_sibling2(VTDNav *vn){
	    bool b = false, b1 = false;
	    Predicate *t= NULL;
	    int result;
		switch( state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:

  	        t = currentStep->p;
	        while(t!=NULL){
	            if (t->requireContext){
	                int i = computeContextSize(t,vn);
	                if (i==0){
	                    b1 = true;
	                    break;
	                }else
	                    t->setContextSize_p(i);
	            }
	            t = t->nextP;
	        }
	        if (b1){
	            if (state == XPATH_EVAL_FORWARD){
	                state = XPATH_EVAL_BACKWARD;
	                currentStep = currentStep->prevS;
	            }else 
	                state = XPATH_EVAL_END;
	            break;
	        }
		  	if ( state ==  XPATH_EVAL_START)
		  		 state =  XPATH_EVAL_END;
		  	else
		  		 state =  XPATH_EVAL_BACKWARD;
		  	vn->push2();
		  	while (vn->toNode(NEXT_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			if (currentStep->nextS!=NULL){
		  				 state =  XPATH_EVAL_FORWARD;
		  				currentStep = currentStep->nextS;
		  				break;
		  			} else {
		  				 state =  XPATH_EVAL_TERMINAL;
		  				result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
			if (state == XPATH_EVAL_END){
		  		if (currentStep->hasPredicate)
		  			currentStep->resetP_s(vn);	
		  		vn->pop2();
			}
		  	else if ( state ==  XPATH_EVAL_BACKWARD){
				if (currentStep->hasPredicate)
		  			currentStep->resetP_s(vn);	
		  		vn->pop2();
		  		currentStep = currentStep->prevS;				  		
		  	}
		    break;
		  	 
		  case  XPATH_EVAL_END:
		  	currentStep = NULL;
		  	// reset();
		  	return -1;
		  	
		  case  XPATH_EVAL_BACKWARD:
		  	while (vn->toNode(NEXT_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			if (currentStep->nextS!=NULL){
		  				 state =  XPATH_EVAL_FORWARD;
		  				currentStep = currentStep->nextS;
		  				b = true;
		  				break;
		  			} else {
		  				 state =  XPATH_EVAL_TERMINAL;
		  				result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		    if (b==false){
		    	vn->pop2();
				if (currentStep->hasPredicate)
			    	currentStep->resetP_s(vn);
		    	if (currentStep->prevS==NULL){
		    		 state =  XPATH_EVAL_END;
		    	}else{
		    		 state =  XPATH_EVAL_BACKWARD;
		    		currentStep = currentStep->prevS;
		    	}
		    }
		  	break;
		  
		  case  XPATH_EVAL_TERMINAL:
		  	while (vn->toNode(NEXT_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			// state =  TERMINAL;
		  			result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}
		  	vn->pop2();
		  	if (currentStep->hasPredicate)
		  		currentStep->resetP_s(vn);
		  	if(currentStep->prevS!=NULL){
		  		currentStep = currentStep->prevS;
		  		state =  XPATH_EVAL_BACKWARD;
		  	}else{
		  		state =  XPATH_EVAL_END;
		  	}
		  	break;

		  default:
			throw new  XPathEvalException("unknown state");
		}
	    return -2;
}


int LocationPathExpr::process_parent2(VTDNav *vn){
		bool b1 = false;
	    Predicate *t= NULL;
	    int result;
		switch ( state) {
			case  XPATH_EVAL_START:
			case  XPATH_EVAL_FORWARD:
    	        t = currentStep->p;
    	        while(t!=NULL){
    	            if (t->requireContext){
    	                int i = computeContextSize(t,vn);
    	                if (i==0){
    	                    b1 = true;
    	                    break;
    	                }else
    	                    t->setContextSize_p(i);
    	            }
    	            t = t->nextP;
    	        }
    	        if (b1){
    	            if (state == XPATH_EVAL_FORWARD){
    	                state = XPATH_EVAL_BACKWARD;
    	                currentStep = currentStep->prevS;
    	            }else 
    	                state = XPATH_EVAL_END;
    	            break;
    	        }
    	        
    			if (vn->getCurrentDepth() == -1) {
    				if ( state ==  XPATH_EVAL_START)
    					 state =  XPATH_EVAL_END;
    				else {
    					//vn.pop();
    					 state =  XPATH_EVAL_BACKWARD;
    					currentStep = currentStep->prevS;
    				}
    			} else {
    				vn->push2();
    				vn->toNode(PARENT); // must return true
    				if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
    						&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
    				    if (currentStep->nextS != NULL) {
    					    state =  XPATH_EVAL_FORWARD;
    					   currentStep = currentStep->nextS;
    				    } else {
    					    state =  XPATH_EVAL_TERMINAL;
    					   result = vn->getCurrentIndex();
    						if ( isUnique(result))
    							return result;
    				    }
    				}else{
    					vn->pop2();
    					if (currentStep->hasPredicate)
    						currentStep->resetP_s(vn);
    					if ( state ==  XPATH_EVAL_START)
    						 state =  XPATH_EVAL_END;
    					else {								
    						 state =  XPATH_EVAL_BACKWARD;
    						currentStep = currentStep->prevS;
    					}
    				}
    			}

    			break;				
    			
    		case  XPATH_EVAL_END:
    			currentStep = NULL;
    			// reset();
    		    return -1;
    			
    		case  XPATH_EVAL_BACKWARD:
    		case  XPATH_EVAL_TERMINAL:
    			if (currentStep->prevS == NULL) {
    			    vn->pop2();
    				 state =  XPATH_EVAL_END;
    				break;
    			}else {
    				vn->pop2();
    				 state =  XPATH_EVAL_BACKWARD;
    				currentStep = currentStep->prevS;
    				break;
    			}
    			
    		default:
    			throw new  XPathEvalException("unknown state");
		
		}
	    return -2;
}


int LocationPathExpr::process_preceding_sibling2(VTDNav *vn){
		bool b = false, b1 = false;
	    Predicate *t= NULL;
	    int result;
	    switch(state){
		  case   XPATH_EVAL_START:
		  case   XPATH_EVAL_FORWARD:
  	        t = currentStep->p;
	        while(t!=NULL){
	            if (t->requireContext){
	                int i = computeContextSize(t,vn);
	                if (i==0){
	                    b1 = true;
	                    break;
	                }else
	                    t->setContextSize_p(i);
	            }
	            t = t->nextP;
	        }
	        if (b1){
	            if (state ==  XPATH_EVAL_FORWARD){
	                state =  XPATH_EVAL_BACKWARD;
	                currentStep = currentStep->prevS;
	            }else 
	                state =  XPATH_EVAL_END;
	            break;
	        }  
		  	if ( state ==   XPATH_EVAL_START)
		  		 state =   XPATH_EVAL_END;
		  	else
		  		 state =   XPATH_EVAL_BACKWARD;
		  	vn->push2();
		  	while (vn->toNode(PREV_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			if (currentStep->nextS!=NULL){
		  				 state =   XPATH_EVAL_FORWARD;
		  				currentStep = currentStep->nextS;
		  				break;
		  			} else {
		  				 state =   XPATH_EVAL_TERMINAL;
		  				result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
			if (state == XPATH_EVAL_END){
		  		if (currentStep->hasPredicate)
		  			currentStep->resetP_s(vn);
		  		vn->pop2();
			}else if ( state ==   XPATH_EVAL_BACKWARD){
				if (currentStep->hasPredicate)
		  			currentStep->resetP_s(vn);
		  		vn->pop2();
		  		currentStep = currentStep->prevS;				  		
		  	}
		  	break;
		  	 
		  case   XPATH_EVAL_END:
		  	currentStep = NULL;
		  	// reset();
		  	return -1;
		  
		  case   XPATH_EVAL_BACKWARD:
		  	while (vn->toNode(PREV_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			if (currentStep->nextS!=NULL){
		  				 state =   XPATH_EVAL_FORWARD;
		  				currentStep = currentStep->nextS;
		  				b = true;
		  				break;
		  			} else {
		  				 state =   XPATH_EVAL_TERMINAL;
		  				result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		    if (b==false){
		    	vn->pop2();
				if (currentStep->hasPredicate)
			    	currentStep->resetP_s(vn);
		    	if (currentStep->prevS==NULL){
		    		 state =   XPATH_EVAL_END;
		    	}else{
		    		 state =   XPATH_EVAL_BACKWARD;
		    		currentStep = currentStep->prevS;
		    	}
		    }
		  	break;
		  
		  case   XPATH_EVAL_TERMINAL:
		  	while (vn->toNode(PREV_SIBLING)){
		  		if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
		  				&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))){
		  			// state =  TERMINAL;
		  			result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}
		  	vn->pop2();
		  	if (currentStep->hasPredicate)
		  		currentStep->resetP_s(vn);
		  	if(currentStep->prevS!=NULL){
		  		currentStep = currentStep->prevS;
		  		 state =   XPATH_EVAL_BACKWARD;
		  	}else{
		  		 state =   XPATH_EVAL_END;
		  	}
		  	break;
		  
		  default:
			throw new  XPathEvalException("unknown state");
		}
	    return -2;
}


int LocationPathExpr::process_self2(VTDNav *vn){
		bool b1 = false;
		Predicate *t = NULL;
		int result;
		switch (state) {
		case XPATH_EVAL_START:
		case XPATH_EVAL_FORWARD:
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContext) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				if (state == XPATH_EVAL_FORWARD) {
					state = XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else
					state = XPATH_EVAL_END;
				break;
			}
			if ((currentStep->nt_eval || currentStep->nt->eval_nt2(vn)) 
					&& ((!currentStep->hasPredicate) || currentStep->evalPredicates(vn))) {
				if (currentStep->nextS != NULL) {
					state = XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state = XPATH_EVAL_TERMINAL;
					if (vn->atTerminal)
						result = vn->LN;
					else
						result = vn->getCurrentIndex();
					if (isUnique(result))
						return result;
				}
			} else {
				if (currentStep->hasPredicate)
					currentStep->resetP_s(vn);
				if (state == XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else
					state = XPATH_EVAL_BACKWARD;
			}
			break;

		case XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case XPATH_EVAL_BACKWARD:
		case XPATH_EVAL_TERMINAL:
			if (currentStep->prevS != NULL) {
				state = XPATH_EVAL_BACKWARD;
				currentStep = currentStep->prevS;
			} else {
				state = XPATH_EVAL_END;
			}
			break;

		default:
			throw new XPathEvalException("unknown state");
		}
		return -2;
}


//int LocationPathExpr::process_namespace2(VTDNav *vn){}

void LocationPathExpr::transition_child(VTDNav *vn){
	vn->toElement(PARENT);
	if (currentStep->prevS != NULL){
		currentStep = currentStep->prevS;
		state = XPATH_EVAL_BACKWARD;
	}else{
		state = XPATH_EVAL_END;								
	}
}

void LocationPathExpr::transition_DDFP(VTDNav *vn){
	vn->pop2();
	currentStep->ft = true;
	if(currentStep->hasPredicate)
		currentStep->resetP_s(vn);
		//System.out.println("  --++ pop in //");
	if (currentStep->prevS != NULL) {
		state =  XPATH_EVAL_BACKWARD;
		currentStep = currentStep->prevS;
	} else
		state =  XPATH_EVAL_END;
}

