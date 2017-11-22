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
#ifndef VTDNAV_L5_H
#define VTDNAV_l5_H

//#include "customTypes.h"
//#include "fastIntBuffer.h"
//#include "fastLongBuffer.h"
//#include "contextBuffer.h"
//#include "elementFragmentNs.h"
//#include "XMLChar.h"
#include "indexHandler.h"
#include "fastIntBuffer.h"
#include "fastLongBuffer.h"
#include "contextBuffer.h"
#include "UTF8Char.h"
#include "XMLChar.h"
// Get declaration for f(int i, char c, float x)
#include "decoder.h"
#include <math.h>
#include "vtdNav.h"
//#include <new>
//#include <iostream>

namespace com_ximpleware {
	class FastLongBuffer;
	class ElementFragmentNs;
	//class VTDNav;
	class VTDNav_L5: public VTDNav{
		friend class ElementFragmentNs;
		friend class FastIntBuffer;
		friend class FastLongBuffer;
		friend class Expr;
		friend class FilterExpr;
		friend class UnionExpr;
		friend class PathExpr;
		friend class LocationPathExpr;
		friend class BinaryExpr;
		friend class VTDGen;
		friend class TextIter;
		friend struct NodeTest;
		friend class BookMark;
		friend class AutoPilot;
		friend class FuncExpr;
		friend class NodeRecorder;
		friend class XMLModifier;
	private:
		void resolveLC_l3();
		void resolveLC_l4();
		void resolveLC_l5();
		void recoverNode_l4(int i);
		void recoverNode_l5(int i);
		
	protected: 
		int l4index;
		int l5index;
		
		int l4lower;
		int l4upper;
		int l5lower;
		int l5upper;
		FastLongBuffer *l3Buffer;
		FastLongBuffer *l4Buffer;
		FastIntBuffer *l5Buffer;
		VTDNav_L5(int r, 
			encoding_t enc, 
			bool ns, 
			int depth,
			UByte *x, 
			int xLen, 
			FastLongBuffer *vtd, 
			FastLongBuffer *l1,
			FastLongBuffer *l2, 
			FastLongBuffer *l3, 
			FastLongBuffer *l4, 
			FastIntBuffer *l5, 
			int so, 
			int len,
			bool br);
		~VTDNav_L5();

		//This method is similar to getElementByName in DOM except it doesn't
		//return the nodeset, instead it iterates over those nodes.
		//bool iterate(int dp, UCSChar *en, bool special);

		//This method is similar to getElementByName in DOM except it doesn't
		//return the nodeset, instead it iterates over those nodes .
		//When URL is "*" it will match any namespace
		//if ns is false, return false immediately
		//bool iterateNS(int dp, UCSChar *URL, UCSChar *ln);

		// This function is called by selectElement_P in autoPilot
		//bool iterate_preceding(UCSChar *en, int* a, bool special);

		// This function is called by selectElementNS_P in autoPilot
		//bool iterate_precedingNS(UCSChar *URL, UCSChar *ln, int* a);

		// This function is called by selectElement_F in autoPilot
		//bool iterate_following(UCSChar *en, bool special);


		// This function is called by selectElementNS_F in autoPilot
		//bool iterate_followingNS( UCSChar *URL, UCSChar *ln);
		virtual void resolveLC();
		//void resolveLC_l3();
		//void resolveLC_l4();
		//void resolveLC_l5();
		void recoverNode_l3(int i);

		bool nodeToElement(int direction);
		void sync(int depth, int index);
		//virtual void sync(int depth, int index);
		virtual bool toNode_LastChild();
		virtual bool toNode_PrevSibling();
	public:
		//Load the context info from ContextBuffer.
		//Info saved including LC and current state of the context 
		bool pop();
		bool pop2();
		//Store the context info into the ContextBuffer.
		//Info saved including LC and current state of the context 
		bool push();
		bool push2();
		void sampleState( FastIntBuffer *fib);

		// A generic navigation method.
		// Move the current to the element according to the direction constants
		// If no such element, no position change and return false (0).
		/* Legal direction constants are 	<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		*/
		bool toElement( navDir direction);

		/**
		* A generic navigation method.
		* Move the current to the element according to the direction 
		* constants and the element name
		* If no such element, no position change and return false (0).
		* "*" matches any element
		* Legal direction constants are 	<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		* for ROOT and PARENT, element name will be ignored.
		*/
		bool toElement( navDir direction, UCSChar *en);
		/*	
		* A generic navigation function with namespace support.
		* Move the current to the element according to the direction constants and the prefix and local names
		* If no such element, no position change and return false(0).
		* URL * matches any namespace, including undefined namespaces
		* a null URL means hte namespace prefix is undefined for the element
		* ln *  matches any localname
		* Legal direction constants are<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		* for ROOT and PARENT, element name will be ignored.
		* If not ns enabled, return false immediately with no position change.
		*/
		bool toElementNS( navDir direction, UCSChar *URL, UCSChar *ln);

		/* Write VTD+XML into a FILE pointer */
		bool writeIndex(FILE *f);

		/* Write VTD+XML into a file of given name */
		bool writeIndex(char *fileName);

		/* Write the VTDs and LCs into an file*/
		bool writeSeparateIndex( char *vtdIndexFileName);
		bool writeSeparateIndex( FILE *f);
		void recoverNode(int i);
		void dumpState();
		bool toNode(int direction);
		virtual bool verifyNodeCorrectness();
	};
};

#endif