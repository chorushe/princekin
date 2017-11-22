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
#ifndef AUTO_PILOT_H
#define AUTO_PILOT_H
#include "customTypes.h"
#include "vtdNav.h"
#include "expr.h"
namespace com_ximpleware{
	typedef enum iter_type { UNDEFINED,
		SIMPLE,
		SIMPLE_NS,
		DESCENDANT,
		DESCENDANT_NS,
		FOLLOWING,
		FOLLOWING_NS,
		PRECEDING,
		PRECEDING_NS,
		ATTR,
		ATTR_NS,
		NAMESPACE,
		SIMPLE_NODE,
		DESCENDANT_NODE,
		FOLLOWING_NODE,
		PRECEDING_NODE} iterType;


	class Expr;
	struct NsList;
	struct ExprList;
	class AutoPilot{
		friend class LocationPathExpr;
	private:
		UCSChar *URL;
		UCSChar *localName;
		UCSChar *elementName;
		UCSChar *elementName2;
		int depth;
		VTDNav *vn;
		int index; /* for iterAttr*/
		int endIndex;
		bool ft;
		bool special;
		iterType it;
		int size; /* iterAttr*/
		Expr *xpe; /* xpath Expr*/
		int *contextCopy; /* for preceding axis */
		int stackSize; /* record stack size for xpath evaluation */
		FastIntBuffer *fib;
		static NsList *nl;
		bool cachingOption;
		static ExprList *el;
		void insertItem(UCSChar *prefix, UCSChar *URL);
		void insertExpr(UCSChar *varName, Expr *e);
		bool checkNsUniqueness(int index);
	public:
		AutoPilot();
		AutoPilot(VTDNav *v);
		virtual ~AutoPilot();

		void printExprString();

		/* bind */
		void bind(VTDNav *vn);

		/* Select an attribute name for iteration, * choose all attributes of an element*/
		void selectAttr(UCSChar *an);

		/* Select an attribute name, both local part and namespace URL part*/
		void selectAttrNS(UCSChar *URL, UCSChar *ln);


		/*Select the element name before iterating*/
		void selectElement(UCSChar *en);

		/*Select the element name (name space version) before iterating.
		// * URL, if set to *, matches every namespace
		// * URL, if set to null, indicates the namespace is undefined.
		// * localname, if set to *, matches any localname*/
		void selectElementNS(UCSChar *URL, UCSChar *ln);


		/**
		* Select all descendent elements along the descendent axis, without ns awareness
		* This function is not intended to be called directly
		* @param en
		*/
		void selectElement_D(UCSChar *en);

		/**
		* Select all descendent elements along the Descendent axis, with ns awareness
		* This function is not intended to be called directly
		*/
		void selectElementNS_D(UCSChar *URL, UCSChar *ln);
		/**
		* Select all descendent elements along the following axis, without ns awareness
		* This function is not intended to be called directly
		* @param en
		*/
		void selectElement_F(UCSChar *en);

		/**
		* Select all descendent elements along the following axis, with ns awareness
		* This function is not intended to be called directly
		*/
		void selectElementNS_F(UCSChar *URL, UCSChar *ln);

		/**
		* Select all descendent elements along the preceding axis, without ns awareness
		* This function is not intended to be called directly
		* @param en
		*/
		void selectElement_P(UCSChar *en);

		/**
		* This function is not intended to be called directly
		* Select all descendent elements along the preceding axis, with ns awareness
		*/
		void selectElementNS_P(UCSChar *URL, UCSChar *ln);


		


		//Iterate over all the selected element nodes.

		bool iterate();

		bool iterate2();

		// This method implements the attribute axis for XPath
		int iterateAttr();

		/*
		* This function selects the string representing XPath expression
		* Usually evalXPath is called afterwards
		* return true is the XPath is valid
		*/
		bool selectXPath(UCSChar *s);
		/*
		* Evaluate XPath to a bool
		*/
		bool evalXPathToBool();

		/*
		* Evaluate XPath to a String
		*/
		UCSChar* evalXPathToString();

		/*
		* Evaluate XPath to a number
		*/
		double evalXPathToNumber();
		/*
		* Evaluate XPath
		*/
		int evalXPath();

		/*
		* Reset XPath
		*/
		void resetXPath();

		/*
		* Declare prefix/URL binding
		*/

		void declareXPathNameSpace( UCSChar *prefix, UCSChar *URL);

		/* Clear the namespace prefix URL bindings in the global list */
		void clearXPathNameSpaces();
		/* Clear the variable name and exprs in the global list  */
		void clearVariableExprs();

		/* Declare the variable name and expression binding*/
		void declareVariableExpr(UCSChar* varName, UCSChar* varExpr);

		void enableCaching(bool state){cachingOption = state;}
	
	protected:

		int iterateAttr2();
		/**
		* Setspecial is used by XPath evaluator to distinguish between
		* node() and *
		* node() corresponding to b= true;
		* This function is not intended to be called directly
		*/
		void setSpecial(bool b){special = b;}
		void selectNameSpace(UCSChar *name);

		int iterateNameSpace();
		void selectDescendantNode();
		void selectNode();
		void selectPrecedingNode();
		void selectFollowingNode();
			
	};


	struct ExprList {
		UCSChar *variableName;
		Expr *ve;
		ExprList *next;
		Expr *getExprFromList(UCSChar *varName);
	};

	struct NsList {
		UCSChar *URL;
		UCSChar *prefix;
		NsList *next;
		UCSChar *lookup(UCSChar *prefix);
		//void insertItem(UCSChar *prefix, UCSChar *URL);
	};
}
#endif
