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
#include "bookMark.h"
#include "vtdNav_L5.h"
using namespace com_ximpleware;

BookMark::BookMark():
ba(NULL),
ba_len(-1),
vn1(NULL)
{
}
BookMark::BookMark(VTDNav *vn):
ba(NULL),
ba_len(-1),
vn1(NULL)
{
	bind(vn);
	recordCursorPosition();
}

BookMark::~BookMark(){
	delete ba;
	ba = NULL;
}

void BookMark::unbind(){
	vn1 = NULL;
}

void BookMark::bind(VTDNav *vn){
		if (vn==NULL)
            throw InvalidArgumentException("vn can't be null");
        vn1 = vn;
		if (vn->shallowDepth){
			if (ba == NULL || vn->nestingLevel+8 != ba_len){
				if (vn->nestingLevel+8 != ba_len){
					delete ba;
				}
				try{
					ba = new int[vn->nestingLevel+8]; 
					//malloc(sizeof(int)*(vn->nestingLevel + 8));
				}
				catch(std::bad_alloc&){
					throw OutOfMemException("BookMark.ba allocation failed ");
				}
			}
		}else{
			if (ba == NULL || vn->nestingLevel+14 != ba_len){
				if (vn->nestingLevel+14 != ba_len){
					delete ba;
				}
				try{
					ba = new int[vn->nestingLevel+14]; 
					//malloc(sizeof(int)*(vn->nestingLevel + 8));
				}
				catch(std::bad_alloc&){
					throw OutOfMemException("BookMark.ba allocation failed ");
				}
			}
		}
        ba[0]= -2 ; // this would never happen in a VTDNav obj's context

}
VTDNav* BookMark::getNav(){
	return vn1;
}
bool BookMark::setCursorPosition( VTDNav *vn){
	int i=0;
	if (vn1 != vn || ba == NULL || ba[0] == -2)
		return false;
	for (i = 0; i < vn->nestingLevel; i++) {
		vn->context[i] = ba[i];
	}

	if (vn->shallowDepth){
		vn->l1index = ba[vn->nestingLevel];
		vn->l2index = ba[vn->nestingLevel + 1];
		vn->l3index = ba[vn->nestingLevel + 2];
		vn->l2lower = ba[vn->nestingLevel + 3];
		vn->l2upper = ba[vn->nestingLevel + 4];
		vn->l3lower = ba[vn->nestingLevel + 5];
		vn->l3upper = ba[vn->nestingLevel + 6];
		if (ba[vn->nestingLevel+7] < 0){
			vn->atTerminal = true;		    
		} else
			vn->atTerminal = false;

		vn->LN = ba[vn->nestingLevel+7] & 0x7fffffff;
	}else{
		VTDNav_L5 *vnl = (VTDNav_L5*)vn;
		vnl->l1index = ba[vn->nestingLevel];
		vnl->l2index = ba[vn->nestingLevel + 1];
		vnl->l3index = ba[vn->nestingLevel + 2];
		vnl->l4index = ba[vn->nestingLevel + 3];
		vnl->l5index = ba[vn->nestingLevel + 4];
		vnl->l2lower = ba[vn->nestingLevel + 5];
		vnl->l2upper = ba[vn->nestingLevel + 6];
		vnl->l3lower = ba[vn->nestingLevel + 7];
		vnl->l3upper = ba[vn->nestingLevel + 8];
		vnl->l4lower = ba[vn->nestingLevel + 9];
		vnl->l4upper = ba[vn->nestingLevel + 10] ;
		vnl->l5lower = ba[vn->nestingLevel + 11] ;
		vnl->l5upper = ba[vn->nestingLevel + 12];
		if (ba[vn->nestingLevel+13] < 0){
			vn->atTerminal = true;		    
		} else
			vn->atTerminal = false;

		vn->LN = ba[vn->nestingLevel+13] & 0x7fffffff;
	}
	return true;
}
bool BookMark::setCursorPosition(){
	return setCursorPosition(vn1);
}
/**
* Record the cursor position
* This method is implemented to be lenient on loading in
* that it can load nodes from any VTDNav object
* if vn is null, return false
*/
bool BookMark::recordCursorPosition( VTDNav *vn){
	int i;
	if (vn == NULL)
		return false;
	if (vn== vn1){
	}else {
		bind(vn);
	}
	for (i = 0; i < vn->nestingLevel; i++) {
		ba[i] = vn1->context[i];
	}

	if (vn->shallowDepth){
		ba[vn->nestingLevel]= vn->l1index ;
		ba[vn->nestingLevel + 1]= vn->l2index ;
		ba[vn->nestingLevel + 2]= vn->l3index ;
		ba[vn->nestingLevel + 3]= vn->l2lower ;
		ba[vn->nestingLevel + 4]= vn->l2upper ;
		ba[vn->nestingLevel + 5]= vn->l3lower ;
		ba[vn->nestingLevel + 6]= vn->l3upper ;
		//ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
		ba[vn->nestingLevel + 7]= 
			(vn->atTerminal == true)? 
			(vn->LN | 0x80000000) : vn->LN ;
	}else{
		VTDNav_L5* vnl = (VTDNav_L5 *)vn;
		ba[vn->nestingLevel]= vnl->l1index ;
		ba[vn->nestingLevel + 1]= vnl->l2index ;
		ba[vn->nestingLevel + 2]= vnl->l3index ;
		ba[vn->nestingLevel + 3]= vnl->l4index ;
		ba[vn->nestingLevel + 4]= vnl->l5index ;
		ba[vn->nestingLevel + 5]= vnl->l2lower ;
		ba[vn->nestingLevel + 6]= vnl->l2upper ;
		ba[vn->nestingLevel + 7]= vnl->l3lower ;
		ba[vn->nestingLevel + 8]= vnl->l3upper ;
		ba[vn->nestingLevel + 9]= vnl->l4lower ;
		ba[vn->nestingLevel + 10]= vnl->l4upper ;
		ba[vn->nestingLevel + 11]= vnl->l5lower ;
		ba[vn->nestingLevel + 12]= vnl->l5upper ;
		//ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
		ba[vn->nestingLevel + 13]= 
			(vn->atTerminal == true)? 
			(vn->LN | 0x80000000) : vn->LN ;
	}
	return true;
}
/**
* Record cursor position of the VTDNav object as embedded in the
* bookmark
*/
bool BookMark::recordCursorPosition(){
	return recordCursorPosition(vn1);
}
/**
* Compare the bookmarks to ensure they represent the same
* node in the same VTDnav instance
*/
bool BookMark::equal(BookMark *bm2){
	if (this == bm2)
		return true;
	return deepEqual(bm2);
}
/**
* Returns the hash code which is a unique integer for every node
*/
int BookMark::hashCode(){
	if (ba == NULL || vn1==NULL || ba[0]==-2)
		return -2;
	if (vn1->atTerminal)
		return vn1->LN;
	if (ba[0]==1)
		return vn1->rootIndex;
	return ba[ba[0]];
}
/**
* Compare the bookmarks to ensure they represent the same
* node in the same VTDnav instance
*/
bool BookMark::deepEqual(BookMark *bm2){
	if (bm2->vn1 == vn1){
		if (bm2->ba[bm2->ba[0]]==ba[ba[0]]){
			if (vn1->shallowDepth){
				if (ba[vn1->nestingLevel+7] < 0){
					if (ba[vn1->nestingLevel+7]
					!= bm2->ba[vn1->nestingLevel+7])
						return false;
				}				
			}else{
				if (ba[vn1->nestingLevel+13] < 0){
					if (ba[vn1->nestingLevel+13]
					!= bm2->ba[vn1->nestingLevel+13])
						return false;
				}				
			}			
			return true;
		}
	}
	return false;
}

bool BookMark::compare(BookMark *bm1){
	if (vn1->shallowDepth) {
		for (int i = 0; i < vn1->nestingLevel + 7; i++) {
			if (ba[i] != bm1->ba[i])
				return false;
		}
	}else {
		for (int i = 0; i < vn1->nestingLevel + 14; i++) {
			if (ba[i] != bm1->ba[i])
				return false;
		}
	}
	return true;
}