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
#pragma once
#ifndef EXPR_H
#define EXPR_H
#include "vtdNav.h"
#include "autoPilot.h"

namespace com_ximpleware{
	class AutoPilot;
        class Expr;
        class FilterExpr;
	typedef enum{
		ABSOLUTE_PATH,
		RELATIVE_PATH
	} pt;
	typedef enum OpType{		
		OP_ADD,
		OP_SUB,
		OP_MULT,
		OP_DIV,
		OP_MOD,
		OP_OR ,
		OP_AND,
		OP_EQ,
		OP_NE,
		OP_LE,
		OP_GE,
		OP_LT,
		OP_GT
	} opType;

	typedef enum AxisType {  AXIS_CHILD0,
						 AXIS_CHILD,
						 AXIS_DESCENDANT_OR_SELF0,
						 AXIS_DESCENDANT0,
						 AXIS_PRECEDING0,
						 AXIS_FOLLOWING0,
						 AXIS_DESCENDANT_OR_SELF,
						 AXIS_DESCENDANT,
						 AXIS_PRECEDING,
						 AXIS_FOLLOWING,
						 AXIS_PARENT,
						 AXIS_ANCESTOR,
						 AXIS_ANCESTOR_OR_SELF,
						 AXIS_SELF,
						 AXIS_FOLLOWING_SIBLING,
						 AXIS_FOLLOWING_SIBLING0,
						 AXIS_PRECEDING_SIBLING,
						 AXIS_PRECEDING_SIBLING0,					 
						 AXIS_ATTRIBUTE,
						 AXIS_NAMESPACE	
	} axisType;

	typedef enum {	NT_NAMETEST ,
		NT_NODE,
		NT_TEXT,
		NT_PI0,
		NT_PI1,
		NT_COMMENT
	} nodeTestType;

	typedef enum{
		XPATH_EVAL_START, 
		XPATH_EVAL_END,
		XPATH_EVAL_TERMINAL,
		XPATH_EVAL_FORWARD, 
		XPATH_EVAL_BACKWARD			
	} LPstate;

	typedef enum{
		SIMPLE_P,
		COMPLEX_P
	} PredicateEvalOption;

	struct AList {
		Expr *e;
		AList *next;
		AList();
		virtual ~AList();
		void  reset_al(VTDNav *vn);
		void  toString_al(UCSChar* string);
	};
	//class Expr;
	

	struct NodeTest{
		UCSChar* nodeName;
		UCSChar* prefix;
		UCSChar* localName;
		UCSChar* URL;
		bool nsEnabled;
		nodeTestType testType;
		int type;
		NodeTest();
		virtual ~NodeTest();
		bool eval_nt(VTDNav *vn);
		bool eval_nt2(VTDNav *vn);
		void setNodeName(UCSChar *name);
		void setNodeNameNS(UCSChar *p, UCSChar *ln);
		void setTestType(nodeTestType ntt){
			testType = ntt;
		}
		void toString_nt(UCSChar *string);
	} ;

	
	struct Step;

	struct Predicate{
		double d; /* only supports a[1] style of location path for now*/
		int count;
		Predicate *nextP;
		Expr *e;
		int type;
		Step *s;
		FilterExpr *fe;
		bool requireContext;
		Predicate();
		virtual ~Predicate();
		bool eval_p(VTDNav *vn);
		bool eval2_p(VTDNav *vn);
		void setIndex_p(int i);
		void setContextSize_p(int size);/*{
					e->setContextSize(size);
				}*/
		bool requireContextSize_p();/*{
					return e->requireContextSize();
				}*/
		void reset_p(VTDNav *vn);/*{
					count = 0;
					e->reset(vn); // is this really needed?
				}*/
		void toString_p( UCSChar *string);
		void adjust(int n);//{e->adjust(n);};
	};


	struct Step{
		axisType axis_type;
		NodeTest *nt;  
		Predicate *p,*pt;/* linked list */
		Step *nextS; /* points to next step */
		int position; /* position */
		Step *prevS; /* points to the prev step */
		AutoPilot *o; /*AutoPilot goes here*/
		bool ft; /* first time*/
		bool hasPredicate;
		bool nt_eval;
		bool out_of_range;
		Step();
		virtual ~Step();
		void reset_s(VTDNav *vn);
		void resetP_s(VTDNav *vn);
		void resetP2_s(VTDNav *vn, Predicate *p1);
		NodeTest *getNodeTest();
		Step *getNextStep();
		bool get_ft();
		void set_ft(bool b);
		Step *getPrevStep();
		void setNodeTest(NodeTest *n);
		void setPredicate(Predicate *p1);
		bool eval_s(VTDNav *vn){
			return nt->eval_nt(vn) && ((!hasPredicate) || evalPredicates(vn));
		}
		bool eval_s2(VTDNav *vn, Predicate *p){
			return nt->eval_nt(vn) && evalPredicates2(vn,p);
		}
		bool eval2_s(VTDNav *vn){
			return nt->eval_nt2(vn) && ((!hasPredicate) || evalPredicates(vn));
		}
		bool eval2_s2(VTDNav *vn, Predicate *p){
			return nt->eval_nt2(vn) && evalPredicates2(vn,p);
		}
		bool evalPredicates(VTDNav *vn);
		bool evalPredicates2(VTDNav *vn, Predicate *p);
		void setAxisType(axisType st){
			axis_type = st;
		}
		void toString_s(UCSChar *string);
		void setStep4Predicates();
		void adjust(int n);/*{
			Predicate* temp = p;
			while(temp!=NULL){
				temp->adjust(n);
				temp = temp->nextP;
			}
		};*/
	};

	UCSChar *createEmptyString();/*{
		return wcsdup(L""):
	}*/
	

	//base class
	class Expr{
	public:
		virtual bool evalBoolean(VTDNav *vn) = 0;

		virtual double evalNumber(VTDNav *vn) =0;

		virtual int evalNodeSet(VTDNav *vn) = 0;

		virtual UCSChar* evalString(VTDNav *vn) = 0;

		virtual void reset(VTDNav *vn)=0;
		virtual void toString(UCSChar* string)=0;

		virtual bool isNumerical()=0;
		virtual bool isNodeSet()=0;
		virtual bool isString()=0;
		virtual bool isBoolean()=0;

		virtual bool requireContextSize()=0;
		virtual void setContextSize(int size)=0;

		virtual void setPosition(int pos)=0;
		virtual int adjust(int n)=0;
		virtual ~Expr(){}
		virtual bool isFinal()=0;
		virtual void markCacheable(){}
		virtual void markCacheable2(){}
	//public boolean isConstant(){return false;}
		virtual void clearCache(){}
		// to support computer context size 
		// needs to add 
		//virtual public boolean needContextSize();
		//virtual public boolean SetContextSize(int contextSize);

	protected:
		int getStringIndex(VTDNav* vn){
			int a = -1;
			vn->push2();
			int size = vn->contextBuf2->size;
			try {
				a = evalNodeSet(vn);
				if (a != -1) {
					tokenType t = vn->getTokenType(a);
					if (t == com_ximpleware::TOKEN_ATTR_NAME) {
						a++;
					} else if (vn->getTokenType(a) == com_ximpleware::TOKEN_STARTING_TAG) {
						a = vn->getText();
					}else if (t == com_ximpleware::TOKEN_PI_NAME) {
						a++;                
					}
				}
			} catch (XPathEvalException&) {

			}
			vn->contextBuf2->size = size;
			reset(vn);
			vn->pop2();
			return a;
		}
	};

	
	inline UCSChar *createEmptyString(){
		return wcsdup(L"");
	}
}

#endif
