
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */

/* Line 311 of lalr1.cc  */
#line 1 "xpathParser.yy"

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


/* Line 311 of lalr1.cc  */
#line 98 "xpathParser.tab.cc"


#include "xpathParser.tab.hh"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 107 "xpathParser.tab.cc"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif
//#define YYDEBUG 1
/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))
//#define YYDEBUG 1
/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace yy {

/* Line 380 of lalr1.cc  */
#line 176 "xpathParser.tab.cc"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  xpathParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  xpathParser::xpathParser (xpath_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (true),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  xpathParser::~xpathParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  xpathParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  xpathParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  xpathParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 4: /* "LITERAL" */

/* Line 480 of lalr1.cc  */
#line 119 "xpathParser.yy"
	{delete (yyvaluep->literal) ;};

/* Line 480 of lalr1.cc  */
#line 283 "xpathParser.tab.cc"
	break;
      case 6: /* "NAME" */

/* Line 480 of lalr1.cc  */
#line 128 "xpathParser.yy"
	{delete (yyvaluep->name).prefix; delete (yyvaluep->name).qname; delete (yyvaluep->name).localname ;};

/* Line 480 of lalr1.cc  */
#line 292 "xpathParser.tab.cc"
	break;
      case 8: /* "NTEST" */

/* Line 480 of lalr1.cc  */
#line 129 "xpathParser.yy"
	{delete (yyvaluep->ntest).arg ;};

/* Line 480 of lalr1.cc  */
#line 301 "xpathParser.tab.cc"
	break;
      case 37: /* "Expr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 310 "xpathParser.tab.cc"
	break;
      case 38: /* "OrExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 319 "xpathParser.tab.cc"
	break;
      case 39: /* "AndExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 328 "xpathParser.tab.cc"
	break;
      case 40: /* "EqualityExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 337 "xpathParser.tab.cc"
	break;
      case 41: /* "RelationalExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 346 "xpathParser.tab.cc"
	break;
      case 42: /* "AdditiveExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 355 "xpathParser.tab.cc"
	break;
      case 43: /* "MultiplicativeExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 364 "xpathParser.tab.cc"
	break;
      case 44: /* "UnaryExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 373 "xpathParser.tab.cc"
	break;
      case 45: /* "UnionExpr" */

/* Line 480 of lalr1.cc  */
#line 125 "xpathParser.yy"
	{delete (yyvaluep->une) ;};

/* Line 480 of lalr1.cc  */
#line 382 "xpathParser.tab.cc"
	break;
      case 46: /* "PathExpr" */

/* Line 480 of lalr1.cc  */
#line 127 "xpathParser.yy"
	{delete (yyvaluep->pe) ;};

/* Line 480 of lalr1.cc  */
#line 391 "xpathParser.tab.cc"
	break;
      case 47: /* "FilterExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 400 "xpathParser.tab.cc"
	break;
      case 48: /* "PrimaryExpr" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 409 "xpathParser.tab.cc"
	break;
      case 49: /* "FunctionCall" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 418 "xpathParser.tab.cc"
	break;
      case 50: /* "ArgumentList" */

/* Line 480 of lalr1.cc  */
#line 123 "xpathParser.yy"
	{delete (yyvaluep->a) ;};

/* Line 480 of lalr1.cc  */
#line 427 "xpathParser.tab.cc"
	break;
      case 51: /* "Argument" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 436 "xpathParser.tab.cc"
	break;
      case 52: /* "LocationPath" */

/* Line 480 of lalr1.cc  */
#line 126 "xpathParser.yy"
	{delete (yyvaluep->lpe) ;};

/* Line 480 of lalr1.cc  */
#line 445 "xpathParser.tab.cc"
	break;
      case 53: /* "AbsoluteLocationPath" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 454 "xpathParser.tab.cc"
	break;
      case 54: /* "RelativeLocationPath" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 463 "xpathParser.tab.cc"
	break;
      case 55: /* "Step" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 472 "xpathParser.tab.cc"
	break;
      case 56: /* "NodeTest" */

/* Line 480 of lalr1.cc  */
#line 124 "xpathParser.yy"
	{delete (yyvaluep->nodetest) ;};

/* Line 480 of lalr1.cc  */
#line 481 "xpathParser.tab.cc"
	break;
      case 57: /* "PredicateList" */

/* Line 480 of lalr1.cc  */
#line 122 "xpathParser.yy"
	{delete (yyvaluep->p) ;};

/* Line 480 of lalr1.cc  */
#line 490 "xpathParser.tab.cc"
	break;
      case 60: /* "AbbreviatedStep" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 499 "xpathParser.tab.cc"
	break;
      case 61: /* "AbbreviatedAbsoluteLocationPath" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 508 "xpathParser.tab.cc"
	break;
      case 62: /* "AbbreviatedRelativeLocationPath" */

/* Line 480 of lalr1.cc  */
#line 121 "xpathParser.yy"
	{delete (yyvaluep->s) ;};

/* Line 480 of lalr1.cc  */
#line 517 "xpathParser.tab.cc"
	break;
      case 63: /* "Predicate" */

/* Line 480 of lalr1.cc  */
#line 122 "xpathParser.yy"
	{delete (yyvaluep->p) ;};

/* Line 480 of lalr1.cc  */
#line 526 "xpathParser.tab.cc"
	break;
      case 64: /* "VariableReference" */

/* Line 480 of lalr1.cc  */
#line 120 "xpathParser.yy"
	{delete (yyvaluep->expression) ;};

/* Line 480 of lalr1.cc  */
#line 535 "xpathParser.tab.cc"
	break;

	default:
	  break;
      }
  }

  void
  xpathParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  xpathParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  xpathParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  xpathParser::debug_level_type
  xpathParser::debug_level () const
  {
    return yydebug_;
  }

  void
  xpathParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  xpathParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = driver.yfl->yylex (&yylval, driver);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 678 of lalr1.cc  */
#line 134 "xpathParser.yy"
    { driver.result=(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	  //yyval.expression->toString(NULL);
	}
    break;

  case 3:

/* Line 678 of lalr1.cc  */
#line 137 "xpathParser.yy"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 4:

/* Line 678 of lalr1.cc  */
#line 138 "xpathParser.yy"
    {
								try{
										(yyval.expression) =(Expr *) new BinaryExpr((yysemantic_stack_[(3) - (1)].expression), OP_OR,(yysemantic_stack_[(3) - (3)].expression));
										//addObj($$);
									}
								catch(...){
									//freeAllObj();
									 YYABORT;
								} 
		
								}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 151 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 152 "xpathParser.yy"
    {
	 							try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_AND,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
								}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 164 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 165 "xpathParser.yy"
    { 
								try {
	 									(yyval.expression) =(Expr *) new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_EQ,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 175 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_NE,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 187 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 188 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) =(Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_LT,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 198 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_GT,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);	 									
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 208 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_LE,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 218 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_GE,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 230 "xpathParser.yy"
    {(yyval.expression)  = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 231 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) =(Expr *) new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_ADD,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 241 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_SUB,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 253 "xpathParser.yy"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 254 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_MULT,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 264 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_DIV,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 274 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new BinaryExpr((yysemantic_stack_[(3) - (1)].expression),OP_MOD,(yysemantic_stack_[(3) - (3)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 286 "xpathParser.yy"
	  { if (yysemantic_stack_[(1) - (1)].une->next == NULL){
		   yyval.expression = yysemantic_stack_[(1) - (1)].une->fe;
		}else {
			(yyval.expression) = (Expr *)(yysemantic_stack_[(1) - (1)].une);
		}
	}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 287 "xpathParser.yy"
    {
			 					try {
	 									(yyval.expression) = (Expr *)new UnaryExpr(OP_NE,(yysemantic_stack_[(2) - (2)].expression));
	 									//addObj($$);
	 								}
	 							catch(...){
	 								//freeAllObj();
									YYABORT;
								}
								}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 300 "xpathParser.yy"
    { 
									try {
										(yyval.une) = new UnionExpr((Expr *)(yysemantic_stack_[(1) - (1)].pe));
										//addObj($$);
									}
									catch(...){
										YYABORT;
									 }
								
								}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 310 "xpathParser.yy"
    {
   										try {
   										   (yyval.une) = new UnionExpr((Expr *)(yysemantic_stack_[(3) - (1)].pe));
   										   //addObj($$);
   										   (yyval.une)->next = (yysemantic_stack_[(3) - (3)].une);
   										}
   										catch(...){
   											YYABORT;   											
   										}
   										
   		                         /*freeAllObj();*/
   		                         //YYABORT;
   		                         
   		                         }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 327 "xpathParser.yy"
    { (yyval.pe) = (PathExpr *) (yysemantic_stack_[(1) - (1)].lpe);  }
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 328 "xpathParser.yy"
    { (yyval.pe) = (PathExpr *)(yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 329 "xpathParser.yy"
    {  														
  														try{
  															LocationPathExpr *tmpLPExpr = new LocationPathExpr();
  															//addObj(tmpLPExpr);
  															//addObj(tmpLPExpr->ih);
  															//addObj(tmpLPExpr->ih->storage);
  															tmpLPExpr->setStep((yysemantic_stack_[(3) - (3)].s));
  															(yyval.pe) = new PathExpr((yysemantic_stack_[(3) - (1)].expression), tmpLPExpr);
  															//addObj($$);
  															//addObj($$->ih);
  															//addObj($$->ih->storage);
  															tmpLPExpr = NULL;
  														}catch(...){  														
  															//freeAllObj();
  															YYABORT;
  														}  		
  													}
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 347 "xpathParser.yy"
    {
														try {
															Step *tmpStep = new Step();
															//addObj(tmpStep);
															tmpStep->setAxisType(AXIS_DESCENDANT_OR_SELF);
															NodeTest *tmpNt = new NodeTest();
															//addObj(tmpNt);
															tmpNt->setTestType(NT_NODE);
															tmpStep->setNodeTest( tmpNt);
															tmpStep->nextS = (yysemantic_stack_[(3) - (3)].s);
															(yysemantic_stack_[(3) - (3)].s)->prevS = tmpStep;
															LocationPathExpr *tmpLPExpr = new LocationPathExpr();
															//addObj(tmpLPExpr);
															//addObj(tmpLPExpr->ih);
															//addObj(tmpLPExpr->ih->storage);
															tmpLPExpr->setStep(tmpStep);
															(yyval.pe) = new PathExpr((yysemantic_stack_[(3) - (1)].expression), tmpLPExpr);
															//addObj($$);
															//addObj($$->ih);	
															//addObj($$->ih->storage);															
														} catch(...){
															//freeAllObj();		
															YYABORT;																											
														}								
													}
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 375 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 376 "xpathParser.yy"
    {try {
  									(yyval.expression) = (Expr *)new FilterExpr((yysemantic_stack_[(2) - (1)].expression),(yysemantic_stack_[(2) - (2)].p));
  										//addObj($$);
  										}
  									catch(...){
  										//freeAllObj();
  										YYABORT;
  									}
  								}
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 389 "xpathParser.yy"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 390 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(3) - (2)].expression);}
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 391 "xpathParser.yy"
    { try {
							(yyval.expression) =(Expr *) new LiteralExpr((yysemantic_stack_[(1) - (1)].literal));
							//addObj($$);
							//addObj($1);
						} catch(...){
							//freeAllObj();
							YYABORT;	
						}						
					  }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 400 "xpathParser.yy"
    { try {
						    (yyval.expression) = (Expr *)new NumberExpr((yysemantic_stack_[(1) - (1)].number));
						    //addObj($$);
					   } catch(...){
							//freeAllObj();
							YYABORT;
					   }
					 }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 408 "xpathParser.yy"
    { (yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 409 "xpathParser.yy"
    {
						//freeAllObj();
						//printf(" Invalid char encoutered \n");
						YYABORT;
					}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 417 "xpathParser.yy"
    { try {
													(yyval.expression) = (Expr *)new FuncExpr((yysemantic_stack_[(4) - (1)].fname), (yysemantic_stack_[(4) - (3)].a));
													if  (!((FuncExpr *)yyval.expression)->checkArgumentCount())
														throw new XPathParseException("Invalid argument for functional expression");
															//addObj($$);
													   }
													   catch(...){
															//freeAllObj();
															YYABORT;
													   }

													 }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 429 "xpathParser.yy"
    { (yyval.a) = NULL;}
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 430 "xpathParser.yy"
    {	try {
	     							(yyval.a) = new AList();
	     							//addObj($$);
	     							(yyval.a)->e = (yysemantic_stack_[(1) - (1)].expression);  								     	
	     						}catch(...){
	     							//freeAllObj();
	     							YYABORT;
	     						}	
	     					}
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 439 "xpathParser.yy"
    {
	     											try {	     											
	     												(yyval.a) = new AList();
	     												//addObj($$);
	     												(yyval.a)->e = (yysemantic_stack_[(3) - (1)].expression);
	     												(yyval.a)->next = (yysemantic_stack_[(3) - (3)].a);	     												
	     											} catch(...){
	     												//freeAllObj();
	     												YYABORT;	     													     												
	     											}	     	
	     									  }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 452 "xpathParser.yy"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 456 "xpathParser.yy"
    { try {
													(yyval.lpe) = new LocationPathExpr();
													//addObj($$);
													//addObj($$->ih);
													//addObj($$->ih->storage);
													(yyval.lpe)->setStep((yysemantic_stack_[(1) - (1)].s));
													(yyval.lpe)->optimize();
												  }
											 catch(...){
													//freeAllObj();
													YYABORT;
											  }
											}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 468 "xpathParser.yy"
    {		try {  /*printf("absolute locationpath \n");*/
													(yyval.lpe) = new LocationPathExpr();
													//addObj($$);
													//addObj($$->ih);
													(yyval.lpe)->pathType = ABSOLUTE_PATH;
													(yyval.lpe)->setStep((yysemantic_stack_[(1) - (1)].s));
													(yyval.lpe)->optimize();
												  }
											  catch(...) {
													//freeLocationPathExpr($$);
													//freeAllObj();
													YYABORT;
											  }
									}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 483 "xpathParser.yy"
    {(yyval.s) = NULL;}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 484 "xpathParser.yy"
    {/*printf("slash detected \n");*/ (yyval.s) = (yysemantic_stack_[(2) - (2)].s);}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 485 "xpathParser.yy"
    {(yyval.s) = (yysemantic_stack_[(1) - (1)].s);}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 488 "xpathParser.yy"
    {(yyval.s) = (yysemantic_stack_[(1) - (1)].s);}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 489 "xpathParser.yy"
    { (yysemantic_stack_[(3) - (1)].s)->nextS = (yysemantic_stack_[(3) - (3)].s);
												(yysemantic_stack_[(3) - (3)].s)->prevS = (yysemantic_stack_[(3) - (1)].s);
												(yyval.s) = (yysemantic_stack_[(3) - (1)].s);		
												}
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 493 "xpathParser.yy"
    {(yyval.s) = (yysemantic_stack_[(1) - (1)].s);}
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 497 "xpathParser.yy"
    {
														try {
															(yyval.s) = new Step();
															//addObj($$);
															(yyval.s)->setAxisType((yysemantic_stack_[(3) - (1)].at));
															if ( ((yysemantic_stack_[(3) - (1)].at)== AXIS_ATTRIBUTE
      														|| (yysemantic_stack_[(3) - (1)].at) == AXIS_NAMESPACE) && 
      														((yysemantic_stack_[(3) - (2)].nodetest)->testType>1)){
      																printf("%s axis can't operate on comment(), pi(), or text()\n", com_ximpleware::getAxisString((yysemantic_stack_[(3) - (1)].at)));
      																throw XPathParseException(" attr|namespace axis can't operate on comment(), pi(), or text()");
      	         											}
															(yyval.s)->setNodeTest((yysemantic_stack_[(3) - (2)].nodetest));
															(yyval.s)->setPredicate((yysemantic_stack_[(3) - (3)].p));
															}
														catch(...){
															//freeAllObj();
															YYABORT;
														}
														}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 521 "xpathParser.yy"
    {(yyval.s) = (yysemantic_stack_[(1) - (1)].s);}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 525 "xpathParser.yy"
    { try {
										(yyval.nodetest) = new NodeTest();
										//addObj($$);
										(yyval.nodetest)->setTestType(NT_NAMETEST);
										//wprintf(L"$1.qname %ls\n",$1.qname);
										(yyval.nodetest)->setNodeName((yysemantic_stack_[(1) - (1)].name).qname);
										//addObj($1.qname);
										if ((yysemantic_stack_[(1) - (1)].name).localname!=NULL){
											(yyval.nodetest)->setNodeNameNS((yysemantic_stack_[(1) - (1)].name).prefix,(yysemantic_stack_[(1) - (1)].name).localname);
											//addObj($1.prefix);
											//addObj($1.localname);
											// the part for URL mapping goes here
											(yyval.nodetest)->URL = driver.nl->lookup((yysemantic_stack_[(1) - (1)].name).prefix);
											if ((yyval.nodetest)->URL == NULL){
												YYABORT;
											}
										}
										(yysemantic_stack_[(1) - (1)].name).prefix = (yysemantic_stack_[(1) - (1)].name).localname = (yysemantic_stack_[(1) - (1)].name).qname = NULL;
										
									  } catch(...){
										//freeAllObj();
										YYABORT;
									  }
									}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 549 "xpathParser.yy"
    { try{
	 								(yyval.nodetest) = new NodeTest();
	 								//addObj($$);
	 								if ((yysemantic_stack_[(1) - (1)].ntest).nt !=3)
	 									(yyval.nodetest)->setTestType((yysemantic_stack_[(1) - (1)].ntest).nt);
	 								else {
	 									if ((yysemantic_stack_[(1) - (1)].ntest).arg == NULL)
	 										(yyval.nodetest)->setTestType((yysemantic_stack_[(1) - (1)].ntest).nt);
	 									else{
	 										(yyval.nodetest)->setTestType(NT_PI1);
	 										(yyval.nodetest)->setNodeName((yysemantic_stack_[(1) - (1)].ntest).arg);
	 									}
	 								}
	 								//setTestType($$,$1.nt);
	 								}
	 								catch(...){
	 									//freeAllObj();
	 									YYABORT;
	 								}	 								
	 							}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 571 "xpathParser.yy"
    { (yyval.p) = NULL;}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 572 "xpathParser.yy"
    { (yysemantic_stack_[(2) - (1)].p)->nextP = (yysemantic_stack_[(2) - (2)].p);
	     									(yyval.p) = (yysemantic_stack_[(2) - (1)].p);	
	     								  }
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 577 "xpathParser.yy"
    {(yyval.at)  = (yysemantic_stack_[(1) - (1)].at);}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 578 "xpathParser.yy"
    {(yyval.at)  = (yysemantic_stack_[(1) - (1)].at);}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 581 "xpathParser.yy"
    { /*printf("abbreviated child axis \n");*/(yyval.at)  = AXIS_CHILD;}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 582 "xpathParser.yy"
    {(yyval.at) = AXIS_ATTRIBUTE;}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 585 "xpathParser.yy"
    {try{
								(yyval.s) = new Step();
								//addObj($$);
								NodeTest *tmpNt = new NodeTest();
								//addObj(tmpNt);
								tmpNt->setTestType(NT_NODE);
								(yyval.s)->setAxisType(AXIS_SELF);
								(yyval.s)->setNodeTest(tmpNt);
							}
							catch(...){
								//freeAllObj();
								YYABORT;
							}
						}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 599 "xpathParser.yy"
    {
					try{
						(yyval.s) = new Step();
						//addObj($$);
						NodeTest *tmpNt = new NodeTest();
						//addObj(tmpNt);
						tmpNt->setTestType(NT_NODE);
						(yyval.s)->setAxisType(AXIS_PARENT);
						(yyval.s)->setNodeTest(tmpNt);
					}
					catch(...){
						//freeAllObj();
						YYABORT;
					}
		
					}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 617 "xpathParser.yy"
    {
																try{
																	(yyval.s) = new Step();
																	//addObj($$);
																	(yyval.s)->setAxisType(AXIS_DESCENDANT_OR_SELF);
																	NodeTest *tmpNt = new NodeTest();
																	//addObj(tmpNt);
																	tmpNt->setTestType(NT_NODE);
																	(yyval.s)->setNodeTest(tmpNt);
																	(yyval.s)->nextS = (yysemantic_stack_[(2) - (2)].s);
																	(yysemantic_stack_[(2) - (2)].s)->prevS = (yyval.s);																
																}catch(...){
																	//freeAllObj();
																	YYABORT;																	
																}
																}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 635 "xpathParser.yy"
    {
																	try{
																		(yyval.s)= new Step();
																		//addObj($$);
																		(yyval.s)->setAxisType(AXIS_DESCENDANT_OR_SELF);
																		NodeTest *tmpNt = new NodeTest();
																		//addObj(tmpNt);
																		tmpNt->setTestType(NT_NODE);
																		(yyval.s)->setNodeTest(tmpNt);
																		(yysemantic_stack_[(3) - (1)].s)->nextS = (yyval.s);
																		(yyval.s)->prevS = (yysemantic_stack_[(3) - (1)].s);
																		(yyval.s)->nextS = (yysemantic_stack_[(3) - (3)].s);
																		(yysemantic_stack_[(3) - (3)].s)->prevS = (yyval.s);
																		(yyval.s) = (yysemantic_stack_[(3) - (1)].s);
																	}catch(...){
																		//freeAllObj();
																		YYABORT;	
																	}
																	}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 657 "xpathParser.yy"
    {
							   try {
									(yyval.p) = new Predicate();
									//addObj($$);
									(yyval.p)->e = (yysemantic_stack_[(3) - (2)].expression);
									if ((yyval.p)->e->isFinal() && (yyval.p)->e->isNumerical()){
										yyval.p->d = yyval.p->e->evalNumber(NULL);
										if (yyval.p->d <1){
											throw new XPathParseException("Invalid index number <1");
										}
										yyval.p->type = SIMPLE_P;
									}
								} catch(...){
									//freeAllObj();
									YYABORT;
								}
							}
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 669 "xpathParser.yy"
    {
								try {
								    //addObj($2);
								    Expr *ex = driver.el->getExprFromList((yysemantic_stack_[(2) - (2)].name).qname);
								    if (ex==NULL) {
										YYABORT;
								    }
								    (yyval.expression) = (Expr *)new VariableExpr((yysemantic_stack_[(2) - (2)].name).qname, ex);
									//$$ = createPredicate();
									//addObj($$);
									//$$->e = $2;
								} catch(...){
									//freeAllObj();
									YYABORT;
								}

							}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 688 "xpathParser.yy"
    {(yyval.fname)  = (yysemantic_stack_[(1) - (1)].fname);}
    break;



/* Line 678 of lalr1.cc  */
#line 1612 "xpathParser.tab.cc"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
	if (yyresult !=0){
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }
	}
    return yyresult;
  }

  // Generate an error message.
  std::string
  xpathParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char xpathParser::yypact_ninf_ = -17;
  const signed char
  xpathParser::yypact_[] =
  {
        43,   -17,   -17,   -17,   -17,    43,    43,     1,     9,    13,
     -17,   -17,   -17,   -17,    14,    22,    24,    15,     7,    36,
     -16,   -17,   -17,    17,    -8,   -17,   -17,   -17,   -17,   -17,
      66,    74,   -17,   -17,   -17,   -17,   -17,    27,   -17,    45,
     -17,   -17,   -17,   -17,    43,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    43,    43,    59,    13,    13,
      43,   -17,    13,    13,   -17,   -17,    38,     6,   -17,    24,
      15,     7,     7,    36,    36,    36,    36,   -16,   -16,   -17,
     -17,   -17,   -17,   -17,   -17,    50,   -17,   -17,   -17,    38,
     -17,    64,    70,   -17,   -17,   -17,     6,   -17
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  xpathParser::yydefact_[] =
  {
        59,    57,    34,    35,    67,    59,    59,     0,    45,    59,
      37,    60,    61,    62,     0,     2,     3,     5,     7,    10,
      15,    18,    22,    24,    27,    30,    36,    26,    44,    43,
      48,     0,    58,    52,    47,    50,    32,     0,    23,     0,
      66,    46,    63,     1,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    31,    59,    59,    53,    54,    55,    59,    33,     4,
       6,     8,     9,    12,    11,    14,    13,    16,    17,    19,
      20,    21,    25,    28,    29,     0,    49,    64,    51,    55,
      42,     0,    40,    65,    56,    38,    59,    41
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  xpathParser::yypgoto_[] =
  {
       -17,     0,   -17,    55,    56,    48,    28,    44,     3,    46,
     -17,   -17,   -17,   -17,     4,   -17,   -17,   -17,    -7,   -17,
     -17,    16,   -17,   -17,   -17,   -17,   -17,    78,   -17,   -17
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  xpathParser::yydefgoto_[] =
  {
        -1,    90,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    91,    92,    27,    28,    29,    30,
      66,    88,    31,    32,    33,    34,    35,    89,    36,    37
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char xpathParser::yytable_ninf_ = -60;
  const signed char
  xpathParser::yytable_[] =
  {
        14,    41,    42,    54,    55,    56,    39,    40,    38,     1,
       2,     3,     1,     4,    43,   -59,     1,   -59,    58,    59,
      48,    49,    50,    51,     5,    60,    46,    47,     6,   -39,
       7,    44,     8,     9,    45,    10,    11,    12,    13,    11,
      12,    13,    57,    11,    12,    13,     1,     2,     3,    67,
       4,    83,    84,    52,    53,    86,    87,    79,    80,    81,
      85,     5,     1,     2,     3,     6,     4,     7,    68,     8,
       9,    60,    10,    11,    12,    13,    73,    74,    75,    76,
      64,     6,    65,     7,    93,     8,     9,    95,    10,    11,
      12,    13,    62,    63,    71,    72,    77,    78,    96,    69,
      97,    70,    61,    82,     0,    94
  };

  /* YYCHECK.  */
  const signed char
  xpathParser::yycheck_[] =
  {
         0,     8,     9,    19,    20,    21,     6,     6,     5,     3,
       4,     5,     3,     7,     0,     6,     3,     8,    26,    27,
      13,    14,    15,    16,    18,    33,    11,    12,    22,    23,
      24,     9,    26,    27,    10,    29,    30,    31,    32,    30,
      31,    32,    25,    30,    31,    32,     3,     4,     5,    22,
       7,    58,    59,    17,    18,    62,    63,    54,    55,    56,
      60,    18,     3,     4,     5,    22,     7,    24,    23,    26,
      27,    33,    29,    30,    31,    32,    48,    49,    50,    51,
       6,    22,     8,    24,    34,    26,    27,    23,    29,    30,
      31,    32,    26,    27,    46,    47,    52,    53,    28,    44,
      96,    45,    24,    57,    -1,    89
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  xpathParser::yystos_[] =
  {
         0,     3,     4,     5,     7,    18,    22,    24,    26,    27,
      29,    30,    31,    32,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    52,    53,    54,
      55,    58,    59,    60,    61,    62,    64,    65,    44,    37,
       6,    54,    54,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    25,    26,    27,
      33,    63,    26,    27,     6,     8,    56,    22,    23,    39,
      40,    41,    41,    42,    42,    42,    42,    43,    43,    44,
      44,    44,    45,    54,    54,    37,    54,    54,    57,    63,
      37,    50,    51,    34,    57,    23,    28,    50
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  xpathParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  xpathParser::yyr1_[] =
  {
         0,    36,    37,    38,    38,    39,    39,    40,    40,    40,
      41,    41,    41,    41,    41,    42,    42,    42,    43,    43,
      43,    43,    44,    44,    45,    45,    46,    46,    46,    46,
      47,    47,    48,    48,    48,    48,    48,    48,    49,    50,
      50,    50,    51,    52,    52,    53,    53,    53,    54,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    59,
      59,    60,    60,    61,    62,    63,    64,    65
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  xpathParser::yyr2_[] =
  {
         0,     2,     1,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     1,     3,     1,     1,     3,     3,
       1,     2,     1,     3,     1,     1,     1,     1,     4,     0,
       1,     3,     1,     1,     1,     1,     2,     1,     1,     3,
       1,     3,     1,     1,     1,     0,     2,     1,     1,     0,
       1,     1,     1,     2,     3,     3,     2,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const xpathParser::yytname_[] =
  {
    "$end", "error", "$undefined", "AXISNAME", "LITERAL", "NUMBER", "NAME",
  "FNAME", "NTEST", "OR", "AND", "EQ", "NE", "GT", "LT", "GE", "LE", "ADD",
  "SUB", "MULT", "DIV", "MOD", "LP", "RP", "DOLLAR", "UNION", "SLASH",
  "DSLASH", "COMMA", "ERROR", "AT", "DOT", "DDOT", "LB", "RB", "UMINUS",
  "$accept", "Expr", "OrExpr", "AndExpr", "EqualityExpr", "RelationalExpr",
  "AdditiveExpr", "MultiplicativeExpr", "UnaryExpr", "UnionExpr",
  "PathExpr", "FilterExpr", "PrimaryExpr", "FunctionCall", "ArgumentList",
  "Argument", "LocationPath", "AbsoluteLocationPath",
  "RelativeLocationPath", "Step", "NodeTest", "PredicateList",
  "AxisSpecifier", "AbbreviatedAxisSpecifier", "AbbreviatedStep",
  "AbbreviatedAbsoluteLocationPath", "AbbreviatedRelativeLocationPath",
  "Predicate", "VariableReference", "FunctionName", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const xpathParser::rhs_number_type
  xpathParser::yyrhs_[] =
  {
        37,     0,    -1,    38,    -1,    39,    -1,    38,     9,    39,
      -1,    40,    -1,    39,    10,    40,    -1,    41,    -1,    40,
      11,    41,    -1,    40,    12,    41,    -1,    42,    -1,    41,
      14,    42,    -1,    41,    13,    42,    -1,    41,    16,    42,
      -1,    41,    15,    42,    -1,    43,    -1,    42,    17,    43,
      -1,    42,    18,    43,    -1,    44,    -1,    43,    19,    44,
      -1,    43,    20,    44,    -1,    43,    21,    44,    -1,    45,
      -1,    18,    44,    -1,    46,    -1,    46,    25,    45,    -1,
      52,    -1,    47,    -1,    47,    26,    54,    -1,    47,    27,
      54,    -1,    48,    -1,    47,    63,    -1,    64,    -1,    22,
      37,    23,    -1,     4,    -1,     5,    -1,    49,    -1,    29,
      -1,    65,    22,    50,    23,    -1,    -1,    51,    -1,    51,
      28,    50,    -1,    37,    -1,    54,    -1,    53,    -1,    26,
      -1,    26,    54,    -1,    61,    -1,    55,    -1,    55,    26,
      54,    -1,    62,    -1,    58,    56,    57,    -1,    60,    -1,
       6,    -1,     8,    -1,    -1,    63,    57,    -1,     3,    -1,
      59,    -1,    -1,    30,    -1,    31,    -1,    32,    -1,    27,
      54,    -1,    55,    27,    54,    -1,    33,    37,    34,    -1,
      24,     6,    -1,     7,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  xpathParser::yyprhs_[] =
  {
         0,     0,     3,     5,     7,    11,    13,    17,    19,    23,
      27,    29,    33,    37,    41,    45,    47,    51,    55,    57,
      61,    65,    69,    71,    74,    76,    80,    82,    84,    88,
      92,    94,    97,    99,   103,   105,   107,   109,   111,   116,
     117,   119,   123,   125,   127,   129,   131,   134,   136,   138,
     142,   144,   148,   150,   152,   154,   155,   158,   160,   162,
     163,   165,   167,   169,   172,   176,   180,   183
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  xpathParser::yyrline_[] =
  {
         0,   134,   134,   137,   138,   151,   152,   164,   165,   175,
     187,   188,   198,   208,   218,   230,   231,   241,   253,   254,
     264,   274,   286,   287,   300,   310,   327,   328,   329,   347,
     375,   376,   389,   390,   391,   400,   408,   409,   417,   429,
     430,   439,   452,   456,   468,   483,   484,   485,   488,   489,
     493,   497,   521,   525,   549,   571,   572,   577,   578,   581,
     582,   585,   599,   617,   635,   657,   669,   688
  };

  // Print the state stack on the debug stream.
  void
  xpathParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  xpathParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  xpathParser::token_number_type
  xpathParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int xpathParser::yyeof_ = 0;
  const int xpathParser::yylast_ = 105;
  const int xpathParser::yynnts_ = 30;
  const int xpathParser::yyempty_ = -2;
  const int xpathParser::yyfinal_ = 43;
  const int xpathParser::yyterror_ = 1;
  const int xpathParser::yyerrcode_ = 256;
  const int xpathParser::yyntokens_ = 36;

  const unsigned int xpathParser::yyuser_token_number_max_ = 290;
  const xpathParser::token_number_type xpathParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // yy

/* Line 1054 of lalr1.cc  */
#line 2129 "xpathParser.tab.cc"


/* Line 1056 of lalr1.cc  */
#line 691 "xpathParser.yy"


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


