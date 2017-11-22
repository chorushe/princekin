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

	#include "xpath_driver.h"
     //#include "xpathParser.tab.hh"
     #include <cstdio>
     #include <iostream>
     #include <sstream>
     #include "autoPilot.h"
     using namespace std;
     using namespace com_ximpleware;

     xpath_driver::xpath_driver (NsList *nl1, ExprList *el1)
       : num_of_chars(0),nl(nl1), el(el1), trace_scanning (false), trace_parsing (false)     {
       //variables["one"] = 1;
       //variables["two"] = 2;
       //printf("yyFlexLexer allocated \n");
       yfl = new yyFlexLexer(); 
       yfl->set_debug(0);
	   accept = 0;
	   result = NULL;
     }
     
     xpath_driver::~xpath_driver ()
     {
		delete yfl;
     }
     
     int
     xpath_driver::parse (wchar_t *input)
     {
       //file = f;
       scan_begin();
       yfl->isName = 1;
	   yfl->colonPosition = -1;
	   yfl->set_debug(0);
       wstringstream wss(input); 

       yfl->switch_streams(&wss,&wcout);
       //printf("scan began \n");
       yy::xpathParser parser (*this);
       //parser.set_debug_level (trace_parsing);
       //cout << "parse begins "<<endl;
       
       int res = parser.parse ();
	   //result->toString(NULL);
       scan_end();
       //printf("scan ended \n");
       return res;
     }
     
     void
     xpath_driver::error (const yy::location& l, const std::string& m)
     {
       std::cerr << l << ": " << m << std::endl;
     }
     
     void
     xpath_driver::error (const std::string& m)
     {
       std::cerr << m << std::endl;
     }

