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
#ifndef NR_H
#define NR_H
#include "vtdNav.h"
namespace com_ximpleware{


class NodeRecorder{
public:
	NodeRecorder();
	NodeRecorder(VTDNav *vn);
	virtual ~NodeRecorder();
	void record();
	void resetPointer();
	void clear();
	int iterate();
	void bind(VTDNav *vn);
    static const int BUF_SZ_EXPO  = 7;
private:
	VTDNav *vn;
	FastIntBuffer *fib;
	int position;
	int size;
	int count;	
};

}
#endif