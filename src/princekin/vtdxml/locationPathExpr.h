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
#ifndef LOCATION_PATH_EXPR_H
#define LOCATION_PATH_EXPR_H
#include "expr.h"
#include "intHash.h"
#include "textIter.h"

namespace com_ximpleware{

	class LocationPathExpr: public Expr{
	friend class xpathParser;
	public:
		LocationPathExpr();
		virtual ~LocationPathExpr();
		bool evalBoolean(VTDNav *vn);
		double evalNumber(VTDNav *vn);
		int evalNodeSet(VTDNav *vn);
		UCSChar* evalString(VTDNav *vn);
		void setStep(Step* st){
			s = st;
		}
		void reset(VTDNav *vn);
		void toString(UCSChar *s);

		bool isNumerical();
		bool isNodeSet();
		bool isString();
		bool isBoolean();

		bool requireContextSize();
		void setContextSize(int size);

		void setPosition(int pos);
		int adjust(int n);
		void clearCache();
		void markCacheable();
		void markCacheable2();
		bool isFinal(){return (pathType ==  ABSOLUTE_PATH);}
		void optimize();
		//void transition_child(VTDNav *vn);
		//void transition_DDFP(VTDNav *vn);

		pt pathType;

	private:
		bool isUnique_lpe(int i);
		int computeContextSize(Predicate *p, VTDNav *vn);
		int computeContextSize4Ancestor(Predicate *p, VTDNav *vn);
		int computeContextSize4Ancestor2(Predicate *p, VTDNav *vn);
		int computeContextSize4AncestorOrSelf(Predicate *p, VTDNav *vn);
		int computeContextSize4AncestorOrSelf2(Predicate *p, VTDNav *vn);
		int computeContextSize4Child(Predicate *p, VTDNav *vn);
		int computeContextSize4Child2(Predicate *p, VTDNav *vn);
		int computeContextSize4DDFP(Predicate *p, VTDNav *vn);
		int computeContextSize4DDFP2(Predicate *p, VTDNav *vn);
		int computeContextSize4FollowingSibling(Predicate *p, VTDNav *vn);
		int computeContextSize4FollowingSibling2(Predicate *p, VTDNav *vn);
		int computeContextSize4Parent(Predicate *p, VTDNav *vn);
		int computeContextSize4Parent2(Predicate *p, VTDNav *vn);
		int computeContextSize4PrecedingSibling(Predicate *p, VTDNav *vn);
		int computeContextSize4PrecedingSibling2(Predicate *p, VTDNav *vn);
		int computeContextSize4Self(Predicate *p, VTDNav *vn);
		int computeContextSize4Self2(Predicate *p, VTDNav *vn);
		int process_ancestor_or_self(VTDNav *vn);
		int process_ancestor_or_self2(VTDNav *vn);
		int process_ancestor(VTDNav *vn);
		int process_ancestor2(VTDNav *vn);
		int process_attribute(VTDNav *vn);
		//int process_attribute2(VTDNav *vn);
		int process_child(VTDNav *vn);
		int process_child2(VTDNav *vn);
		int process_DDFP(VTDNav *vn);
		int process_DDFP2(VTDNav *vn);
		int process_following_sibling(VTDNav *vn);
		int process_following_sibling2(VTDNav *vn);
		int process_parent(VTDNav *vn);
		int process_parent2(VTDNav *vn);
		int process_preceding_sibling(VTDNav *vn);
		int process_preceding_sibling2(VTDNav *vn);
		int process_self(VTDNav *vn);
		int process_self2(VTDNav *vn);
		int process_namespace(VTDNav *vn);
		//int process_namespace2(VTDNav *vn);
		void selectNodeType(TextIter *ti);
		bool isUnique(int i){
			return ih->isUnique(i);
		}
		void transition_child(VTDNav *vn);
		void transition_DDFP(VTDNav *vn);

		
		Step* s;
		Step* currentStep;
		LPstate state;
		/*FastIntBuffer* fib; // for uniqueness checking */
		IntHash *ih; /* for uniqueness checking*/

	};

	UCSChar *axisName(axisType i);

}

#endif
