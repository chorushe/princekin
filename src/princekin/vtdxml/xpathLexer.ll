%{
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
# include <cstdlib>
# include <cerrno>
# include <climits>
# include <string>
# include "xpath_driver.h"
# include "xpathParser.tab.hh"
# include <iostream>
# include <sstream> 

using namespace com_ximpleware;
using namespace std;

//#define yyFlexLexer
#define yyterminate() return token::END

typedef yy::xpathParser::token token;
typedef com_ximpleware::FuncName fn;
//int my_yyinput(unsigned short *buf, int s);
//#define YY_USER_INIT  buf_state = yy_scan_string( name );

#define YY_USER_ACTION  driver.num_of_chars += yyleng;
//static YY_BUFFER_STATE buf_state;
//wchar_t *mydup(YY_CHAR *text, int len);
//static int i;
//int isName;
//int colonPosition;
//int num_chars;
//int tmp1,tmp2;
//UCSChar *c1, *c2;
//#define yyterminate() return token::END

%}
%option noyywrap
%option 16bit
%option batch
%option c++
%option debug

ws 	[ \t\r\n]
digits	[0-9]+
nc    ([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")
nc2   ([^!-/:-@[-^ \n\r\t|0-9]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|".")

%%

[ \t\r\n]      ; //do nothing

"+" 	{isName = 1 ; return token::ADD;}
"-" 	{isName = 1 ; return token::SUB;}
"."	{isName = 0 ; return token::DOT;}
".." 	{isName = 0 ; return token::DDOT;}
"@"	{isName = 1 ; return token::AT;}

","	{isName = 1 ; return token::COMMA;}
"("	{isName = 1 ; return token::LP;}
")"	{isName = 0 ; return token::RP;}
"["	{isName = 1 ; return token::LB;}
"]"	{isName = 0 ; return token::RB;}
">"	{isName = 1 ; return token::GT;}
"<"	{isName = 1 ; return token::LT;}
">="	{isName = 1 ; return token::GE;}
"<="	{isName = 1 ; return token::LE;}
"="	{isName = 1 ; return token::EQ;}
"!="	{isName = 1 ; return token::NE;}
"$"      {isName = 1; return token::DOLLAR;}
"*"	{if (isName ==0 ){
		isName = 1;
		return token::MULT;
	}
	 else {
		isName = 0;
		yylval->name.qname = NULL;
		yylval->name.localname = NULL;
		yylval->name.prefix = NULL;
		yylval->name.qname = wcsdup(L"*");
		//addObj(yylval->name.qname);
		//yylval->name.localname = NULL;
		//yylval->name.prefix = NULL;
		//addObj(yylval->name.qname);
		return token::NAME;
	 }	 
	}

"/"	{isName = 1 ; return token::SLASH;}
"//"	{isName = 1 ; return token::DSLASH;}

"div"	{if (isName == 0 ) {
		isName = 1;
		return token::DIV;
	}
	 else {
		isName = 0;
		yylval->name.qname = NULL;
		yylval->name.localname = NULL;
		yylval->name.prefix = NULL;
		yylval->name.qname = wcsdup(L"div");
		//addObj(yylval->name.qname);
		return token::NAME;
	 } 
	}
"mod"	{if (isName == 0 ) {
		isName = 1;
		return token::MOD;
	}
	 else {
		yylval->name.qname = NULL;
		yylval->name.localname = NULL;
		yylval->name.prefix = NULL;
		isName = 0;
		yylval->name.qname = wcsdup(L"mod");
		//addObj(yylval->name.qname);
		return token::NAME;
	 } 
	}

"and"	{if (isName == 0 ) {
		isName = 1;
		return token::AND;
	}
	 else {
		yylval->name.qname = NULL;
		yylval->name.localname = NULL;
		yylval->name.prefix = NULL;
		isName = 0;
		yylval->name.qname = wcsdup(L"and");
		//addObj(yylval->name.qname);
		return token::NAME;
	 } 
	}

"or" {if (isName == 0 ){
		isName = 1; 
		return token::OR;
	}
	 else {
		yylval->name.qname = NULL;
		yylval->name.localname = NULL;
		yylval->name.prefix = NULL;
		isName = 0;
		yylval->name.qname = wcsdup(L"or");
		//addObj(yylval->name.qname);
		return token::NAME;
	 }
	}

"|"	{isName = 1 ; return token::UNION ; }

last([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_LAST;
				return token::FNAME;				 
			}

position([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_POSITION;
				return token::FNAME;	
			}

count([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_COUNT;
				return token::FNAME;	
			}

local-name([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_LOCAL_NAME;
				return token::FNAME;	
			}

namespace-uri([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NAMESPACE_URI;
				return token::FNAME;	
			}

name([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NAME;
				return token::FNAME;	
			}

string([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_STRING;
				return token::FNAME;	
			}


concat([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_CONCAT;
				return token::FNAME;	
			}

starts-with([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_STARTS_WITH;
				return token::FNAME;	
			}

contains([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_CONTAINS;
				return token::FNAME;	
			}

substring-before([ \t\r\n])*"("  {  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_SUBSTRING_BEFORE;
				return token::FNAME;	
			}

substring-after([ \t\r\n])*"(" {  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_SUBSTRING_AFTER;
				return token::FNAME;	
			} 	

substring([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_SUBSTRING;
				return token::FNAME;	
			}

string-length([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_STRING_LENGTH;
				return token::FNAME;	
			}

normalize-space([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NORMALIZE_SPACE;
				return token::FNAME;	
			}

translate([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_TRANSLATE;
				return token::FNAME;	
			}
			
abs([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ABS;
				return token::FNAME;	
			}

round-half-to-even([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ROUND_HALF_TO_EVEN;
				return token::FNAME;	
			}

round-half-to-odd([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ROUND_HALF_TO_ODD;
				return token::FNAME;	
			}

code-points-to-string([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_CODE_POINTS_TO_STRING;
				return token::FNAME;	
			}

compare([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_COMPARE;
				return token::FNAME;	
			}

upper-case([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_UPPER_CASE;
				return token::FNAME;	
			}

lower-case([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_LOWER_CASE;
				return token::FNAME;	
			}

ends-with([ \t\r\n])*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ENDS_WITH;
				return token::FNAME;	
			}

QName([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_QNAME;
				return token::FNAME;	
			}

local-name-from-QName([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_LOCAL_NAME_FROM_QNAME;
				return token::FNAME;	
			}

namespace-uri-from-QName([ \t\r\n])*"(" 	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NAMESPACE_URI_FROM_QNAME;
				return token::FNAME;	
			}

namespace-uri-for-prefix{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NAMESPACE_URI_FOR_PREFIX;
				return token::FNAME;	
			}

resolve-QName([ \t\r\n])*"(" 	{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_RESOLVE_QNAME;
				return token::FNAME;	
			}

iri-to-uri([ \t\r\n])*"("	{	isName = 1; 
			 	unput('(');
				yylval->fname = FN_IRI_TO_URI;
				return token::FNAME;	
			}

escape-html-uri([ \t\r\n])*"(" {	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ESCAPE_HTML_URI;
				return token::FNAME;	
			}


encode-for-uri{ws}*"(" {	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ENCODE_FOR_URI;
				return token::FNAME;	
			}

boolean([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_BOOLEAN;
				return token::FNAME;	
			}

not([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NOT;
				return token::FNAME;	
			}

true([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_TRUE;
				return token::FNAME;	
			}

false([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_FALSE;
				return token::FNAME;	
			}

lang([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_LANG;
				return token::FNAME;	
			}

number([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_NUMBER;
				return token::FNAME;	
			}

sum([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_SUM;
				return token::FNAME;	
			}

floor([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_FLOOR;
				return token::FNAME;	
			}

ceiling([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_CEILING;
				return token::FNAME;	
			}

round([ \t\r\n])*"("		{  	isName = 1; 
			 	unput('(');
				yylval->fname = FN_ROUND;
				return token::FNAME;	
			}


\"[^\"]*\" |
'[^']*'			{
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				yylval->literal =  new wchar_t[yyleng-1]; //malloc(sizeof(wchar_t)*(yyleng-1));
				if (yylval->literal!=NULL){
					wcsncpy(yylval->literal, (wchar_t *)(tmpstring+1), yyleng-2);
					//addObj(yylval->literal);
				} 	
				*(yylval->literal+yyleng-2) = L'\0';
				delete(tmpstring);
				tmpstring = NULL;
				return token::LITERAL;
			}

([0-9]+)("."([0-9]+)?)? |
"."[0-9]+		{
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				yylval->number = wcstod(tmpstring,NULL);
				delete(tmpstring);
				tmpstring = NULL;
				return token::NUMBER;
			}


text([ \t\r\n])*"("([ \t\r\n])*")"	{	
				isName = 0;
				yylval->ntest.nt = NT_TEXT;
				yylval->ntest.arg = NULL;
				return token::NTEST;
			}

comment([ \t\r\n])*"("([ \t\r\n])*")"	{
				isName = 0;
				yylval->ntest.nt = NT_COMMENT;
				yylval->ntest.arg = NULL;
				return token::NTEST;
			}

node([ \t\r\n])*"("([ \t\r\n])*")"	{
				isName = 0;
				yylval->ntest.nt = NT_NODE;
				yylval->ntest.arg = NULL;
				return token::NTEST;
			}

			
processing-instruction([ \t\r\n])*"("([ \t\r\n])*'[^']*'([ \t\r\n])*")"	{
						isName = 0;
						//ntest = new Ntest();
						//ntest.i = Ntest.PROCESSING_INSTRUCTION;
						//ntest.arg = getArgument('\'', yytext());
						//System.out.println(" !!!!!!!!!!!argument ==>" +ntest.arg);
						//return sym(NTEST,ntest);
						yylval->ntest.nt = NT_PI0;
						c1 = wcsrchr(yytext,'\'');
						c2 = wcschr(yytext,'\'');
						yylval->ntest.arg =  new wchar_t[c1-c2];  //(UCSChar *)malloc (sizeof(UCSChar)*(c1-c2));
						if (yylval->ntest.arg == NULL)
							return token::ERROR;
						//addObj(yylval->ntest.arg);
						wcsncpy(yylval->ntest.arg, c2+1, c1-c2-1);
						yylval->ntest.arg[c1-c2-1]=0;
						//yylval->ntest.arg = NULL;
						return token::NTEST;
					}
					
processing-instruction([ \t\r\n])*"("([ \t\r\n])*\"[^\"]*\"([ \t\r\n])*")"	{
						isName = 0;
						//ntest = new Ntest();
						//ntest.i = Ntest.PROCESSING_INSTRUCTION;
						//ntest.arg = getArgument('"',yytext());
						//System.out.println(" !!!!!!!!!!!argument ==>" +ntest.arg);
						//return sym(NTEST,ntest);
						yylval->ntest.nt = NT_PI0;
						c1 = wcsrchr(yytext,'"');
						c2 = wcschr(yytext,'"');
						yylval->ntest.arg =  new wchar_t[c1-c2]; //(UCSChar *)malloc (sizeof(UCSChar)*(c1-c2));
						if (yylval->ntest.arg == NULL)
							return token::ERROR;
						//addObj(yylval->ntest.arg);
						wcsncpy(yylval->ntest.arg, c2+1, c1-c2-1);
						yylval->ntest.arg[c1-c2-1]=0;
						//yylval->ntest.arg = 
						//yylval->ntest.arg = NULL;
						return token::NTEST;
					}

processing-instruction([ \t\r\n])*"("([ \t\r\n])*")"	{
						isName = 0;
						yylval->ntest.nt = NT_PI0;
						yylval->ntest.arg = NULL;
						return token::NTEST;
					}


ancestor([ \t\r\n])*::		{	isName = 1;
				yylval->at = AXIS_ANCESTOR;
				return token::AXISNAME;
			}

ancestor-or-self([ \t\r\n])*::	{	isName = 1;
				yylval->at = AXIS_ANCESTOR_OR_SELF;
				return token::AXISNAME;
			}


attribute([ \t\r\n])*::	{	isName =1;
				yylval->at = AXIS_ATTRIBUTE;
				return token::AXISNAME;
			}

child([ \t\r\n])*::		{	isName = 1;
				yylval->at = AXIS_CHILD;
				return token::AXISNAME;
			}

descendant([ \t\r\n])*::	{	isName = 1;
				yylval->at = AXIS_DESCENDANT;
				return token::AXISNAME;
			}

descendant-or-self([ \t\r\n])*:: {	isName = 1;
				yylval->at = AXIS_DESCENDANT_OR_SELF;
				return token::AXISNAME;
			}

following([ \t\r\n])*::	{	isName = 1;
				yylval->at = AXIS_FOLLOWING;
				return token::AXISNAME;
			}

following-sibling([ \t\r\n])*::  {	isName = 1;
				yylval->at = AXIS_FOLLOWING_SIBLING;
				return token::AXISNAME;
			}

namespace([ \t\r\n])*::	{	isName = 1;
				yylval->at = AXIS_NAMESPACE;
				return token::AXISNAME;
			}

parent([ \t\r\n])*::		{	isName = 1;
				yylval->at = AXIS_PARENT;
				return token::AXISNAME;
			}

preceding([ \t\r\n])*::	{	isName = 1;
				yylval->at = AXIS_PRECEDING;
				return token::AXISNAME;
			}

preceding-sibling([ \t\r\n])*:: {	isName = 1;
				yylval->at = AXIS_PRECEDING_SIBLING;
				return token::AXISNAME;
			}

self([ \t\r\n])*::		{	isName = 1;
				yylval->at = AXIS_SELF;
				return token::AXISNAME;
			}

([^!-/:-@[-^ \n\r\t|0-9]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|".")([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")*:"*"		{
				isName = 0;
				//len = wcslen(yytext);
				yylval->name.prefix = NULL;
				yylval->name.localname = NULL;
				yylval->name.qname = NULL;
				if (!XMLChar_isNCNameStart(yytext[0]))
					return token::ERROR;
				for(int i=1;i<yyleng-2;i++){
					if (!XMLChar_isNCName(yytext[i]))
						return token::ERROR;
				}
				yylval->name.prefix = new wchar_t[yyleng-1]; //(UCSChar *)malloc (sizeof(UCSChar)*(yyleng-1));
				if (yylval->name.prefix == NULL)
						return token::ERROR; 
				//addObj(yylval->name.prefix);
				tmpstring = mydup(yytext,yyleng);
				if (tmpstring!=NULL){
					wcsncpy(yylval->name.prefix, tmpstring, yyleng-2);
					yylval->name.prefix[yyleng-2] = 0;
					//addObj(yylval->name.prefix);
				}
				//delete(tmpstring);
				//tmpstring = NULL;
				yylval->name.localname = wcsdup(L"*");
				yylval->name.qname = tmpstring;
				//addObj(yylval->name.localname);
				return token::NAME;
			}

([^!-/:-@[-^ \n\r\t|0-9]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|".")([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")*:([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")+ |
([^!-/:-@[-^ \n\r\t|0-9]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|".")([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")*		{
				yylval->name.prefix = NULL;
				yylval->name.localname = NULL;
				yylval->name.qname = NULL;
				if (!XMLChar_isNCNameStart(yytext[0]))
					return token::ERROR;
				for(int i=1;i<yyleng;i++){
					if (!XMLChar_isNCName(yytext[i]) && yytext[i]!= ':')
						return token::ERROR;
					if (yytext[i]==':')
						colonPosition = i;						
				}
				if (colonPosition != -1){
					yylval->name.prefix = new wchar_t[colonPosition-1];  //(wchar_t *)malloc(sizeof(wchar_t)*(colonPosition+1));
					if (yylval->name.prefix == NULL)
						return token::ERROR; 
					//addObj(yylval->name.prefix);
					yylval->name.localname = new wchar_t[yyleng - colonPosition]; //(wchar_t *)malloc(sizeof(wchar_t)*(yyleng - colonPosition));
					if (yylval->name.localname == NULL){
						return token::ERROR;
					}					
					//addObj(yylval->name.localname);
					// copy content of prefix and localname
					int i;
					for (i=0;i<colonPosition;i++){
						yylval->name.prefix[i] = yytext[i];
					}
					yylval->name.prefix[i] = 0;
					for(i=0;i<yyleng - colonPosition -1;i++){
						yylval->name.localname[i] = yytext[i + colonPosition +1];
					}
					yylval->name.localname[i] = 0;
				}
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				if (tmpstring ==NULL){
					printf("tmpstring can't be null");
					return token::ERROR;
				}
				yylval->name.qname = tmpstring;
				//addObj(tmpstring);
				tmpstring = NULL;
				//printf("returned a NAME \n");

				colonPosition = -1;

				return token::NAME;
			}

.			{	isName =1;
				wprintf(L"error here!!!!!!!!!!!!!  %s \n",yytext); 
				return token::ERROR;
			}

%%
YY_CHAR* yyFlexLexer::mydup(YY_CHAR *text,int len){
	int i=0;
	YY_CHAR *string = new YY_CHAR[len+1];
	if (string!=NULL){
		while(text[i]!=0){
			string[i] = (YY_CHAR) text[i];
			i++;
		}
		string[i]=0;
	}
	return string;
}

