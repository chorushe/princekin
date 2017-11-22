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
#include "nodeRecorder.h"
#include "vtdNav_L5.h"

using namespace com_ximpleware;
//NodeRecorder::BUF_SZ_EXPO = 7;

NodeRecorder::NodeRecorder() try :
vn(NULL),
fib(new FastIntBuffer(BUF_SZ_EXPO)),
position(0),
size(0),
count(0)
{}
catch(std::bad_alloc&){
		throw OutOfMemException("NodeRecorder allocation in FastIntBuffer's constructor failed");
}

NodeRecorder::NodeRecorder(VTDNav *vn) try:
vn(vn),
fib(new FastIntBuffer(BUF_SZ_EXPO)),
position(0),
size(0),
count(0)
{}
catch(std::bad_alloc&){
		throw OutOfMemException("NodeRecorder allocation in FastIntBuffer's constructor failed");
}

NodeRecorder::~NodeRecorder(){
	delete fib;
}

void NodeRecorder::record(){
            //add the context and
            int i;
            switch (vn->context[0])
            {
                case -1:
                    fib->append((int)(0xff | 0x80000000));
                    size++;
                    position++;
                    count++;
                    break;
                case 0:
                    if (vn->atTerminal == false)
                    {
                        fib->append(0);
                        count++;
                    }
                    else
                    {
                       
                        fib->append((int)0x80000000);
                        
                        count += 2;
                    }
                    size++;
                    position++;
                    if (vn->atTerminal == true)
                        fib->append(vn->LN);
                    break;
                case 1:
                    if (vn->atTerminal == false)
                    {
                        fib->append(1);
                        fib->append(vn->context[1]);
                        fib->append(vn->l1index);
                        size++;
                        position++;
                        count += 3;
                    }
                    else
                    {                       
                        fib->append((int)(0x80000001));
                        fib->append(vn->context[1]);
                        fib->append(vn->l1index);
                        fib->append(vn->LN);
                        size++;
                        position++;
                        count += 4;
                    }
                    break;
                case 2:
                    if (vn->atTerminal == false)
                    {
                        fib->append(2);
                        count += 7;
                    }
                    else
                    {
                        fib->append((int)0x80000002);                        
                        count += 8;
                    }
                    fib->append(vn->context[1]);
                    fib->append(vn->context[2]);
                    fib->append(vn->l1index);
                    fib->append(vn->l2lower);
                    fib->append(vn->l2upper);
                    fib->append(vn->l2index);
                    size++;
                    position++;

                    if (vn->atTerminal == true)
                        fib->append(vn->LN);

                    break;
                case 3:
                    if (vn->atTerminal == false)
                    {
                        fib->append(3);
                        count += 11;
                    }
                    else
                    {
                        fib->append((int)(0x80000003));
                        count += 12;
                    }
                    fib->append(vn->context[1]);
                    fib->append(vn->context[2]);
                    fib->append(vn->context[3]);
                    fib->append(vn->l1index);
                    fib->append(vn->l2lower);
                    fib->append(vn->l2upper);
                    fib->append(vn->l2index);
                    fib->append(vn->l3lower);
                    fib->append(vn->l3upper);
                    fib->append(vn->l3index);
                    size++;
                    position++;

                    if (vn->atTerminal == true)
                        fib->append(vn->LN);

                    break;
                default:
			if (vn->shallowDepth) {
				if (vn->atTerminal == false) {
					i = vn->context[0];
					fib->append(i);
					count += i + 8;
				} else {
					i = vn->context[0];
					fib->append(i | 0x80000000);
					count += i + 9;
				}
				for (int k = 1; k <= i; k++) {
					fib->append(vn->context[k]);
				}
				fib->append(vn->l1index);
				fib->append(vn->l2lower);
				fib->append(vn->l2upper);
				fib->append(vn->l2index);
				fib->append(vn->l3lower);
				fib->append(vn->l3upper);
				fib->append(vn->l3index);
				size++;
				position++;

				if (vn->atTerminal)
					fib->append(vn->LN);
			}else{
				VTDNav_L5 *vnl = (VTDNav_L5 *)vn;
				switch (vn->context[0]) {
			       case 4:
			    	   
			        	if (vn->atTerminal == false) {
			                fib->append(4);
			                count += 15;
			            } else {
			                fib->append(0x80000004);
			                count += 16;
			            }
			            fib->append(vn->context[1]);
			            fib->append(vn->context[2]);
			            fib->append(vn->context[3]);
			            fib->append(vn->context[4]);
			            fib->append(vn->l1index);
			            fib->append(vn->l2lower);
			            fib->append(vn->l2upper);
			            fib->append(vn->l2index);
			            fib->append(vn->l3lower);
			            fib->append(vn->l3upper);
			            fib->append(vn->l3index);
			            fib->append(vnl->l4lower);
			            fib->append(vnl->l4upper);
			            fib->append(vnl->l4index);
			            size++;
			            position++;

			            if (vn->atTerminal == true)
			                fib->append(vn->LN);

			            break;
			        case 5:
			            if (vn->atTerminal == false) {
			                fib->append(5);
			                count += 19;
			            } else {
			                fib->append(0x80000005);
			                count += 20;
			            }
			            fib->append(vn->context[1]);
			            fib->append(vn->context[2]);
			            fib->append(vn->context[3]);
			            fib->append(vn->context[4]);
			            fib->append(vn->context[5]);
			            fib->append(vn->l1index);
			            fib->append(vn->l2lower);
			            fib->append(vn->l2upper);
			            fib->append(vn->l2index);
			            fib->append(vn->l3lower);
			            fib->append(vn->l3upper);
			            fib->append(vn->l3index);
			            fib->append(vnl->l4lower);
			            fib->append(vnl->l4upper);
			            fib->append(vnl->l4index);
			            fib->append(vnl->l5lower);
			            fib->append(vnl->l5upper);
			            fib->append(vnl->l5index);
			            size++;
			            position++;

			            if (vn->atTerminal == true)
			                fib->append(vn->LN);

			            break;   
			        default:
			            if (vn->atTerminal == false) {
			                i = vn->context[0];
			                fib->append(i);
			                count += i + 14;
			            } else {
			                i = vn->context[0];
			                fib->append(i | 0x80000000);
			                count += i + 15;
			            }
			            for (int k = 1; k <= i; k++) {
			                fib->append(vn->context[k]);
			            }
			            fib->append(vn->l1index);
			            fib->append(vn->l2lower);
			            fib->append(vn->l2upper);
			            fib->append(vn->l2index);
			            fib->append(vn->l3lower);
			            fib->append(vn->l3upper);
			            fib->append(vn->l3index);
			            fib->append(vnl->l4lower);
			            fib->append(vnl->l4upper);
			            fib->append(vnl->l4index);
			            fib->append(vnl->l5lower);
			            fib->append(vnl->l5upper);
			            fib->append(vnl->l5index);
			            size++;
			            position++;

			            if (vn->atTerminal)
			                fib->append(vn->LN);
			        }

                    break;
					}
            }
}

void NodeRecorder::resetPointer(){

	position = count = 0;
}

void NodeRecorder::clear(){
	count = size = position = 0;
	fib->clear();
}

int NodeRecorder::iterate(){
	int j, i;
	bool b;
	if (count < fib->size)
	{
		i = fib->intAt(count);
		b = (i >= 0);
		if (b == false)
		{
			i = i & 0x7fffffff;
		}
		switch (i)
		{
		case 0xff:
			vn->context[0] = -1;
			vn->atTerminal = false;
			count++;
			break;

		case 0:
			vn->context[0] = 0;
			if (b == false)
			{
				vn->atTerminal = true;
				vn->LN = fib->intAt(count + 1);
				count += 2;
			}
			else
			{
				vn->atTerminal = false;
				count++;
			}

			break;

		case 1:
			vn->context[0] = 1;
			vn->context[1] = fib->intAt(count + 1);
			vn->l1index = fib->intAt(count + 2);
			if (b == false)
			{
				vn->atTerminal = true;
				vn->LN = fib->intAt(count + 3);
				count += 4;
			}
			else
			{
				vn->atTerminal = false;
				count += 3;
			}

			break;

		case 2:
			vn->context[0] = 2;
			vn->context[1] = fib->intAt(count + 1);
			vn->context[2] = fib->intAt(count + 2);
			vn->l1index = fib->intAt(count + 3);
			vn->l2lower = fib->intAt(count + 4);
			vn->l2upper = fib->intAt(count + 5);
			vn->l2index = fib->intAt(count + 6);
			if (b == false)
			{
				vn->atTerminal = true;
				vn->LN = fib->intAt(count + 7);
				count += 8;
			}
			else
			{
				vn->atTerminal = false;
				count += 7;
			}

			break;

		case 3:
			vn->context[0] = 3;
			vn->context[1] = fib->intAt(count + 1);
			vn->context[2] = fib->intAt(count + 2);
			vn->context[3] = fib->intAt(count + 3);
			vn->l1index = fib->intAt(count + 4);
			vn->l2lower = fib->intAt(count + 5);
			vn->l2upper = fib->intAt(count + 6);
			vn->l2index = fib->intAt(count + 7);
			vn->l3lower = fib->intAt(count + 8);
			vn->l3upper = fib->intAt(count + 9);
			vn->l3index = fib->intAt(count + 10);
			if (b == false)
			{
				vn->atTerminal = true;
				vn->LN = fib->intAt(count + 11);
				count += 12;
			}
			else
			{
				vn->atTerminal = false;
				count += 11;
			}

			break;

		default:
				if (vn->shallowDepth) {
					vn->context[0] = i;
					for (j = 1; j < i; j++) {
						vn->context[j] = fib->intAt(count + j);
					}
					vn->l1index = fib->intAt(count + i);
					vn->l2lower = fib->intAt(count + i + 1);
					vn->l2upper = fib->intAt(count + i + 2);
					vn->l2index = fib->intAt(count + i + 3);
					vn->l3lower = fib->intAt(count + i + 4);
					vn->l3upper = fib->intAt(count + i + 5);
					vn->l3index = fib->intAt(count + i + 6);
					if (b == false) {
						vn->atTerminal = true;
						vn->LN = fib->intAt(count + 11);
						count += i + 8;
					} else {
						vn->atTerminal = false;
						count += i + 7;
					}
					break;
				} else {
					VTDNav_L5 *vnl = (VTDNav_L5 *) vn;
					switch (i) {
					case 4:
						vn->context[0] = 4;
						vn->context[1] = fib->intAt(count + 1);
						vn->context[2] = fib->intAt(count + 2);
						vn->context[3] = fib->intAt(count + 3);
						vn->context[4] = fib->intAt(count + 4);
						vn->l1index = fib->intAt(count + 5);
						vn->l2lower = fib->intAt(count + 6);
						vn->l2upper = fib->intAt(count + 7);
						vn->l2index = fib->intAt(count + 8);
						vn->l3lower = fib->intAt(count + 9);
						vn->l3upper = fib->intAt(count + 10);
						vn->l3index = fib->intAt(count + 11);
						vnl->l4lower = fib->intAt(count + 12);
						vnl->l4upper = fib->intAt(count + 13);
						vnl->l4index = fib->intAt(count + 14);
						if (b == false) {
							vn->atTerminal = true;
							vn->LN = fib->intAt(count + 15);
							count += 16;
						} else {
							vn->atTerminal = false;
							count += 15;
						}

						break;

					case 5:
						vn->context[0] = 5;
						vn->context[1] = fib->intAt(count + 1);
						vn->context[2] = fib->intAt(count + 2);
						vn->context[3] = fib->intAt(count + 3);
						vn->context[4] = fib->intAt(count + 4);
						vn->context[5] = fib->intAt(count + 5);
						vn->l1index = fib->intAt(count + 6);
						vn->l2lower = fib->intAt(count + 7);
						vn->l2upper = fib->intAt(count + 8);
						vn->l2index = fib->intAt(count + 9);
						vn->l3lower = fib->intAt(count + 10);
						vn->l3upper = fib->intAt(count + 11);
						vn->l3index = fib->intAt(count + 12);
						vnl->l4lower = fib->intAt(count + 13);
						vnl->l4upper = fib->intAt(count + 14);
						vnl->l4index = fib->intAt(count + 15);
						vnl->l5lower = fib->intAt(count + 16);
						vnl->l5upper = fib->intAt(count + 17);
						vnl->l5index = fib->intAt(count + 18);
						if (b == false) {
							vn->atTerminal = true;
							vn->LN = fib->intAt(count + 19);
							count += 20;
						} else {
							vn->atTerminal = false;
							count += 19;
						}

						break;

					default:
						vn->context[0] = i;
						for (j = 1; j < i; j++) {
							vn->context[j] = fib->intAt(count + j);
						}
						vn->l1index = fib->intAt(count + i);
						vn->l2lower = fib->intAt(count + i + 1);
						vn->l2upper = fib->intAt(count + i + 2);
						vn->l2index = fib->intAt(count + i + 3);
						vn->l3lower = fib->intAt(count + i + 4);
						vn->l3upper = fib->intAt(count + i + 5);
						vn->l3index = fib->intAt(count + i + 6);
						vnl->l4lower = fib->intAt(count + i + 7);
						vnl->l4upper = fib->intAt(count + i + 8);
						vnl->l4index = fib->intAt(count + i + 9);
						vnl->l5lower = fib->intAt(count + i + 10);
						vnl->l5upper = fib->intAt(count + i + 11);
						vnl->l5index = fib->intAt(count + i + 12);
						if (b == false) {
							vn->atTerminal = true;
							vn->LN = fib->intAt(count + i + 13);
							count += i + 14;
						} else {
							vn->atTerminal = false;
							count += i + 13;
						}
						break;
					}
				}
		}
		position++;
		return vn->getCurrentIndex();
		
	}
	return -1;
}

void NodeRecorder::bind(VTDNav *vn1){
	if (vn1==NULL){
		throw InvalidArgumentException("vn can't be null");	
	}
	vn = vn1;
}
