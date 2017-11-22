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
#include "customTypes.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include "expr.h"
#include "funcExpr.h"
#include "literalExpr.h"
#include "filterExpr.h"
#include "binaryExpr.h"
#include "numberExpr.h"
#include "locationPathExpr.h"
#include "unionExpr.h"
#include "pathExpr.h"
#include "variableExpr.h"
#include "xpath_driver.h"
#include "pathExpr.h"
#include "unaryExpr.h"

using namespace std;
using namespace com_ximpleware;

//#include "helper.h"

//Step *tmpStep;
//Step *tmpStep2;
//NodeTest *tmpNt;
//locationPathExpr *tmpLPExpr ;
//AList *tmpAl;
//Predicate *tmpP;

//static Expr *x;
//static NsList *xpathNSList = NULL;
//static ExprList *xpathExprList = NULL;
//extern int isName;
//extern int colonPosition;
//expr* ex;
%}
%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4.1"
%defines
%define parser_class_name "xpathParser"
%error-verbose
%debug
%parse-param { xpath_driver& driver }
%lex-param   { xpath_driver& driver }

%union {
	com_ximpleware::UCSChar*   literal;
	com_ximpleware::axisType at;
	int	 integer;
	double	 number;
	struct nametype {
		com_ximpleware::UCSChar* prefix;
		com_ximpleware::UCSChar* qname;
		com_ximpleware::UCSChar* localname;
	} name;
	struct {
       		com_ximpleware::nodeTestType nt;
       		com_ximpleware::UCSChar*   arg;
	} ntest;
	com_ximpleware::FuncName fname;
	com_ximpleware::Expr *expression;
 	com_ximpleware::LocationPathExpr *lpe;
 	com_ximpleware::UnionExpr *une;
	com_ximpleware::PathExpr *pe;
	com_ximpleware::Step *s;
	com_ximpleware::AList *a;
	com_ximpleware::NodeTest *nodetest;
	com_ximpleware::Predicate* p;
}

%token <at> AXISNAME 
%token <literal> LITERAL
%token <number>  NUMBER
%token <name> NAME
%token <fname> FNAME
%token <ntest> NTEST
%type <expression> Expr OrExpr AndExpr EqualityExpr RelationalExpr AdditiveExpr
%type <expression> MultiplicativeExpr UnaryExpr FilterExpr
%type <expression> FunctionCall Argument PrimaryExpr VariableReference
%type <une> UnionExpr
%type <lpe> LocationPath
%type <pe> PathExpr
%type <s> Step AbbreviatedStep RelativeLocationPath  AbsoluteLocationPath
%type <s> AbbreviatedAbsoluteLocationPath  AbbreviatedRelativeLocationPath
%type <p> Predicate PredicateList
%type <a> ArgumentList
%type <at> AxisSpecifier AbbreviatedAxisSpecifier
%type <fname> FunctionName
%type <nodetest> NodeTest
%token OR AND EQ NE GT LT GE LE ADD SUB MULT DIV MOD
%token LP RP DOLLAR  UNION SLASH DSLASH COMMA ERROR 
%token AT DOT DDOT LB RB
%left  OR
%left  AND
%left  EQ NE
%left  GE LE GT LT
%nonassoc UMINUS


%destructor {delete $$ ;} <literal>
%destructor {delete $$ ;} <expression>
%destructor {delete $$ ;} <s>
%destructor {delete $$ ;} <p>
%destructor {delete $$ ;} <a>
%destructor {delete $$ ;} <nodetest>
%destructor {delete $$ ;} <une>
%destructor {delete $$ ;} <lpe>
%destructor {delete $$ ;} <pe>
%destructor {delete $$.prefix; delete $$.qname; delete $$.localname ;} <name>
%destructor {delete $$.arg ;} <ntest>



%%
Expr 		:    OrExpr  { driver.result=$$ = $1; }
       		;

OrExpr 		:    AndExpr  {$$ = $1;}
		|    OrExpr OR AndExpr {
								try{
										$$ = (Expr *)new BinaryExpr($1, OP_OR,$3);
										//addObj($$);
									}
								catch(...){
									//freeAllObj();
									 YYABORT;
								} 
		
								}
		;

AndExpr		:    EqualityExpr { $$ = $1;}
	 	|    AndExpr AND EqualityExpr {
	 							try {
	 									$$ =(Expr *) new BinaryExpr($1,OP_AND,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
								}
		;

EqualityExpr    :    RelationalExpr  { $$ = $1;}
		|    EqualityExpr EQ RelationalExpr  { 
								try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_EQ,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    EqualityExpr NE RelationalExpr {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_NE,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		;

RelationalExpr  :    AdditiveExpr  { $$ = $1; }
  		|    RelationalExpr LT AdditiveExpr  {
			 					try {
	 									$$ =(Expr *)new BinaryExpr($1,OP_LT,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    RelationalExpr GT AdditiveExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_GT,$3);
	 									//addObj($$);	 									
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    RelationalExpr LE AdditiveExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_LE,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    RelationalExpr GE AdditiveExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_GE,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		;

AdditiveExpr    :    MultiplicativeExpr  {$$  = $1; }
		|    AdditiveExpr ADD MultiplicativeExpr  {
			 					try {
	 									$$ =(Expr *)ew BinaryExpr($1,OP_ADD,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    AdditiveExpr SUB MultiplicativeExpr {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_SUB,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		;

MultiplicativeExpr    :  UnaryExpr  {$$ = $1; }
   		|    MultiplicativeExpr MULT UnaryExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_MULT,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    MultiplicativeExpr DIV UnaryExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_DIV,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		|    MultiplicativeExpr MOD UnaryExpr  {
			 					try {
	 									$$ = (Expr *)new BinaryExpr($1,OP_MOD,$3);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
		;

UnaryExpr    	:    UnionExpr  { if ($$->next==NULL) 
									  $$ = $1->fe;
								   else 
									  $$=$1;
					 }
  		|    SUB UnaryExpr  %prec UMINUS  {
			 					try {
	 									$$ = (Expr *)new UnaryExpr(OP_NE,$2);
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
								}
		;


UnionExpr    	:    PathExpr  	{ 
									try {
										$$ = new UnionExpr($1);
										//addObj($$);
									}
									catch(...){
										YYABORT;
									 }
								
								}		
   		|    PathExpr UNION UnionExpr {
   										try {
   										   $$ = new UnionExpr($1);
   										   //addObj($$);
   										   $$->next = $3;
   										}
   										catch(...){
   											YYABORT;   											
   										}
   										
   		                         /*freeAllObj();*/
   		                         //YYABORT;
   		                         
   		                         }
		;


PathExpr     	:    LocationPath  { $$ = (PathExpr *) $1;  }
   		|    FilterExpr  { $$ = (PathExpr *)$1;}
  		|    FilterExpr SLASH RelativeLocationPath  {  														
  														try{
  															LocationPathExpr *tmpLPExpr = new LocationPathExpr();
  															//addObj(tmpLPExpr);
  															//addObj(tmpLPExpr->ih);
  															//addObj(tmpLPExpr->ih->storage);
  															tmpLPExpr->setStep($3);
  															$$ = new PathExpr($1, tmpLPExpr);
  															//addObj($$);
  															//addObj($$->ih);
  															//addObj($$->ih->storage);
  															tmpLPExpr = NULL;
  														}catch(...){  														
  															//freeAllObj();
  															YYABORT;
  														}  		
  													}
  													
		|    FilterExpr DSLASH RelativeLocationPath {
														try {
															Step *tmpStep = new Step();
															//addObj(tmpStep);
															tmpStep->setAxisType(AXIS_DESCENDANT_OR_SELF);
															NodeTest *tmpNt = new NodeTest();
															//addObj(tmpNt);
															tmpNt->setTestType(NT_NODE);
															tmpStep->setNodeTest( tmpNt);
															tmpStep->nextS = $3;
															$3->prevS = tmpStep;
															LocationPathExpr *tmpLPExpr = new LocationPathExpr();
															//addObj(tmpLPExpr);
															//addObj(tmpLPExpr->ih);
															//addObj(tmpLPExpr->ih->storage);
															tmpLPExpr->setStep(tmpStep);
															$$ = new PathExpr($1, tmpLPExpr);
															//addObj($$);
															//addObj($$->ih);	
															//addObj($$->ih->storage);															
														} catch(...){
															//freeAllObj();		
															YYABORT;																											
														}								
													}
		;


FilterExpr   	:    PrimaryExpr  { $$ = $1;}
  		|    FilterExpr Predicate  {try {
  									$$ =(Expr *) new FilterExpr($1,$2);
  										//addObj($$);
  										}
  									catch(...){
  										//freeAllObj();
  										YYABORT;
  									}
  								}  									
  								;



PrimaryExpr     :    VariableReference {$$ = $1;}
		|    LP Expr RP  { $$ = $2;} 
		|    LITERAL  { try {
							$$ =(Expr *) new LiteralExpr($1);
							//addObj($$);
							//addObj($1);
						} catch(...){
							//freeAllObj();
							YYABORT;	
						}						
					  } 
		|    NUMBER  { try {
						    $$ = new(Expr *) NumberExpr($1);
						    //addObj($$);
					   } catch(...){
							//freeAllObj();
							YYABORT;
					   }
					 } 
		|    FunctionCall { $$ = $1;} 
		|    ERROR {
						//freeAllObj();
						//printf(" Invalid char encoutered \n");
						YYABORT;
					}
		;


FunctionCall    :    FunctionName LP ArgumentList RP { try {
													$$ =(Expr *) new FuncExpr($1, $3);
													if  (!$$->checkArgumentCount())
														throw new XPathParseException("Invalid argument for functional expression");
															//addObj($$);
													   }
													   catch(...){
															//freeAllObj();
															YYABORT;
													   }

													 } 
		;

ArgumentList	: 	{ $$ = NULL;}   
	     	|    Argument  {	try {
	     							$$ = new AList();
	     							//addObj($$);
	     							$$->e = $1;  								     	
	     						}catch(...){
	     							//freeAllObj();
	     							YYABORT;
	     						}	
	     					} 
	     	|    Argument COMMA ArgumentList  {
	     											try {	     											
	     												$$ = new AList();
	     												//addObj($$);
	     												$$->e = $1;
	     												$$->next = $3;	     												
	     											} catch(...){
	     												//freeAllObj();
	     												YYABORT;	     													     												
	     											}	     	
	     									  } 
		;

Argument   	:    Expr  {$$ = $1;} 
	   	; 


LocationPath    :    RelativeLocationPath	{ try {
													$$ = new LocationPathExpr();
													//addObj($$);
													//addObj($$->ih);
													//addObj($$->ih->storage);
													$$->setStep($1);
													$$->optimize();
												  }
											 catch(...){
													//freeAllObj();
													YYABORT;
											  }
											}
		|    AbsoluteLocationPath	{		try {  /*printf("absolute locationpath \n");*/
													$$ = new LocationPathExpr();
													//addObj($$);
													//addObj($$->ih);
													$$->pathType = ABSOLUTE_PATH;
													$$->setStep($1);
													$$->optimize();
												  }
											  catch(...) {
													//freeLocationPathExpr($$);
													//freeAllObj();
													YYABORT;
											  }
									}
		;

AbsoluteLocationPath :  SLASH 				{$$ = NULL;}
		|    SLASH RelativeLocationPath		{/*printf("slash detected \n");*/ $$ = $2;}
		|    AbbreviatedAbsoluteLocationPath	{$$ = $1;}
		;

RelativeLocationPath :  Step {$$ = $1;}
		|    Step SLASH RelativeLocationPath  { $1->nextS = $3;
												$3->prevS = $1;
												$$ = $1;		
												}
		|    AbbreviatedRelativeLocationPath {$$ = $1;}
		;


Step		:    AxisSpecifier NodeTest PredicateList {
														try {
															$$ = new Step();
															//addObj($$);
															$$->setAxisType($1);
															if ( ($1== AXIS_ATTRIBUTE
      														|| $1 == AXIS_NAMESPACE) && 
      														($2->testType>1)){
      																printf("%s axis can't operate on comment(), pi(), or text()\n", com_ximpleware::getAxisString($1));
      																throw XPathParseException(" attr|namespace axis can't operate on comment(), pi(), or text()");
      	         											}
															$$->setNodeTest($2);
															$$->setPredicate($3);
															}
														catch(...){
															//freeAllObj();
															YYABORT;
														}
														}
      		|    AbbreviatedStep			{$$ = $1;}
		;


NodeTest	:    NAME 				{ try {
										$$ = new NodeTest();
										//addObj($$);
										$$->setTestType(NT_NAMETEST);
										//wprintf(L"$1.qname %ls\n",$1.qname);
										$$->setNodeName($1.qname);
										//addObj($1.qname);
										if ($1.localname!=NULL){
											$$->setNodeNameNS($1.prefix,$1.localname);
											//addObj($1.prefix);
											//addObj($1.localname);
											// the part for URL mapping goes here
											$$->URL = driver.nl->lookup($1.prefix);
											if ($$->URL == NULL){
												YYABORT;
											}
										}
										$1.prefix = $1.localname = $1.qname = NULL;
										
									  } catch(...){
										//freeAllObj();
										YYABORT;
									  }
									} 
	 	|    NTEST				{ try{
	 								$$ = new NodeTest();
	 								//addObj($$);
	 								if ($1.nt !=3)
	 									$$->setTestType($1.nt);
	 								else {
	 									if ($1.arg == NULL)
	 										$$->setTestType($1.nt);
	 									else{
	 										$$->setTestType(NT_PI1);
	 										$$->setNodeName($1.arg);
	 									}
	 								}
	 								//setTestType($$,$1.nt);
	 								}
	 								catch(...){
	 									//freeAllObj();
	 									YYABORT;
	 								}	 								
	 							}
		;

PredicateList	: { $$ = NULL;} 
	     	|    Predicate	PredicateList { $1->nextP = $2;
	     									$$ = $1;	
	     								  }  
		;

AxisSpecifier	:    AXISNAME				{$$  = $1;}
	      	|    AbbreviatedAxisSpecifier		{$$  = $1;}
	      	;

AbbreviatedAxisSpecifier  : { /*printf("abbreviated child axis \n");*/$$  = AXIS_CHILD;}
			  | AT	{$$ = AXIS_ATTRIBUTE;}
			;

AbbreviatedStep	:    DOT {try{
								$$ = new Step();
								//addObj($$);
								NodeTest *tmpNt = new NodeTest();
								//addObj(tmpNt);
								tmpNt->setTestType(NT_NODE);
								$$->setAxisType(AXIS_SELF);
								$$->setNodeTest(tmpNt);
							}
							catch(...){
								//freeAllObj();
								YYABORT;
							}
						}
		|    DDOT {
					try{
						$$ = new Step();
						//addObj($$);
						NodeTest *tmpNt = new NodeTest();
						//addObj(tmpNt);
						tmpNt->setTestType(NT_NODE);
						$$->setAxisType(AXIS_PARENT);
						$$->setNodeTest(tmpNt);
					}
					catch(...){
						//freeAllObj();
						YYABORT;
					}
		
					}
		;

AbbreviatedAbsoluteLocationPath : DSLASH RelativeLocationPath {
																try{
																	$$ = new Step();
																	//addObj($$);
																	$$->setAxisType(AXIS_DESCENDANT_OR_SELF);
																	NodeTest *tmpNt = new NodeTest();
																	//addObj(tmpNt);
																	tmpNt->setTestType(NT_NODE);
																	$$->setNodeTest(tmpNt);
																	$$->nextS = $2;
																	$2->prevS = $$;																
																}catch(...){
																	//freeAllObj();
																	YYABORT;																	
																}
																}
		;

AbbreviatedRelativeLocationPath : Step DSLASH RelativeLocationPath  {
																	try{
																		$$= new Step();
																		//addObj($$);
																		$$->setAxisType(AXIS_DESCENDANT_OR_SELF);
																		NodeTest *tmpNt = new NodeTest();
																		//addObj(tmpNt);
																		tmpNt->setTestType(NT_NODE);
																		$$->setNodeTest(tmpNt);
																		$1->nextS = $$;
																		$$->prevS = $1;
																		$$->nextS = $3;
																		$3->prevS = $$;
																		$$ = $1;
																	}catch(...){
																		//freeAllObj();
																		YYABORT;	
																	}
																	}
		;


Predicate 	:    LB Expr RB {
							   try {
									$$ = new Predicate();
									//addObj($$);
									$$->e = $2;
									if ($2->isFinal() && $2->isNumerical()){
										if ($$->d<1){
											throw new XPathParseExcpetion("Invalid index number <1");
										}
										$$->type = SIMPLE_P;
									}
									$$->requireContext=$2->requireContextSize();
								} catch(...){
									//freeAllObj();
									YYABORT;
								}
							}
	   	;

VariableReference : DOLLAR NAME {
								try {
								    //addObj($2);
								    Expr *ex = driver.el->getExprFromList($2.qname);
								    if (ex==NULL) {
										YYABORT;
								    }
								    $$ = (Expr *)new VariableExpr($2.qname, ex);
									//$$ = createPredicate();
									//addObj($$);
									//$$->e = $2;
								} catch(...){
									//freeAllObj();
									YYABORT;
								}

							}
		  ;

FunctionName 	:  FNAME  {$$  = $1;}
	      ;
	      
%%

 void
     yy::xpathParser::error (const yy::xpathParser::location_type& l,
                               const std::string& m)
     {
       driver.error (l, m);
     }


//extern unsigned short *xpathInput;
//extern unsigned short *xpathInputPtr;
//extern unsigned short *xpathInputLimit;
//extern int num_chars;
//extern int isName;
//expr *xpathParse(UCSChar *input, NsList *nl, ExprList *el){
//	int l = wcslen(input);
//	int i = 0;
//	num_chars = 0;	
//	xpathNSList = nl;
//	xpathExprList = el;
//	XMLChar_init();
//	xpathInputPtr = xpathInput = (unsigned short *)malloc((l+1)<<1);
//	// copy the string from wchar_t to unsigned short
//	for (i=0;i<l;i++){
//		xpathInput[i] = (unsigned short)input[i];
//	}
//	xpathInputLimit = (unsigned short *) (xpathInput + ((l)<<1));
//	isName = 1;
//	colonPosition = -1;
//	if (yyparse() == 0){
		//isName = 1;
		//wprintf(L"yyparse return 0\n");
//		free(xpathInput);
//		xpathInput = xpathInputPtr = xpathInputLimit = NULL;
//		resetObj();
//		return x;
//	} else {
		//wprintf(L"yyparse YYABORT\n");
//		wprintf(L"xpath syntax error after or around the end of \n     %.*s\n", num_chars, input);
//		free(xpathInput);
//		xpathInput = xpathInputPtr = xpathInputLimit = NULL;
		//isName = 1;
//		freeAllObj();
//		return NULL;
//	}
//}

