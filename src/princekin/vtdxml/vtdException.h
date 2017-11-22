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
#ifndef VTDEXCEPTION_H
#define VTDEXCEPTION_H
#include <exception>
#include <stdexcept>
namespace com_ximpleware{
	class VTDException : public std::exception {
	public:

		VTDException(const char* s):std::exception(){
			message = s;
		}
		VTDException(std::exception& e):std::exception(){}
		virtual const char* what() const throw()
		{
			return "VTD exception happened";
		}
		const char* getMessage() {return message;}
	private:
		const char *message;
	};

	class IOException:public VTDException{
	public:
		IOException(const char* s):VTDException(s){}
	};

	class TranscodeException:public VTDException{
	public:
		TranscodeException(const char* s):VTDException(s){}
	};


	class InvalidArgumentException: public std::runtime_error{
	public:
		InvalidArgumentException(const char* string):std::runtime_error(string){
			s = string;
		}
		virtual const char* what() const throw()
		{
			return s;
		}
	private:
		const char* s;
	};

	class XPathParseException: public VTDException{
	public:
		XPathParseException(const char* s):VTDException(s){}
	};

	class NumberFormatException: public VTDException{	
	public:
		NumberFormatException(const char* s):VTDException(s){}
	};

	class XPathEvalException: public VTDException{
	public:
		XPathEvalException(const char* s):VTDException(s){}
	};

	class ParseException: public VTDException{
	public:
		ParseException(const char* s):VTDException(s){}
	};

	class NavException: public VTDException{
	public:
		NavException(const char* s):VTDException(s){}
	};

	class PilotException: public NavException{
	public:
		PilotException(const char* s):NavException(s){}
	};

	class EntityException:public ParseException{
	public:
		EntityException(const char* s):ParseException(s){}
	};

	class EncodingException:public ParseException{
	public:
		EncodingException(const char* s):ParseException(s){}
	};

	class EOFException:public ParseException{
	public:
		EOFException(const char* s):ParseException(s){}
	};

	class IndexReadException: public VTDException{
	public:
		IndexReadException(const char* s):VTDException(s){}
	};

	class IndexWriteException:public VTDException{
	public:
		IndexWriteException(const char* s):VTDException(s){}
	};

	class ModifyException: public VTDException{
	public:
		ModifyException(const char* s):VTDException(s){}
	};

	class OutOfMemException: public std::bad_alloc{
	public:
		OutOfMemException(const char* s1):
		  std::bad_alloc(),
		  s(s1) 
		{
		}
	private:
		const char *s;
	};


	class OtherException: public VTDException {
	public:
		OtherException(const char* s):VTDException(s){}
	};
};

#endif