     #ifndef XPATH_DRIVER_HH
     # define XPATH_DRIVER_HH
     # include <string>
     #define YY_CHAR wchar_t
     //# include <map>
     # include "xpathParser.tab.hh"
     # include "FlexLexer.h"
     # include "expr.h"
     # include "location.hh"	

     using namespace com_ximpleware;
     // Tell Flex the lexer's prototype ...
     # define YY_DECL                                        \
       yy::xpathParser::token_type                      \
       yyFlexLexer::yylex (yy::xpathParser::semantic_type* yylval,      \
                           xpath_driver& driver)
     //YY_DECL;


     // Conducting the whole scanning and parsing of Calc++.
     class yyFlexLexer;
     class xpathParser;
     class xpath_driver
     {
     public:
       xpath_driver (NsList *nl1, ExprList *el);
       virtual ~xpath_driver ();
		
	   int num_of_chars;
       //std::map<std::string, int> variables;
     
       com_ximpleware::Expr *result;
	   int accept;
       yyFlexLexer *yfl;
       com_ximpleware::NsList *nl;
       com_ximpleware::ExprList *el;

       // Handling the scanner.
       void scan_begin ();
       void scan_end ();
       bool trace_scanning;

       // Run the parser.  Return 0 on success.
       int parse (wchar_t* inputString);
       //std::string file;
       bool trace_parsing;

       // Error handling.
       void error (const yy::location& l, const std::string& m);
       void error (const std::string& m);
     };
     #endif // ! XPATH_DRIVER_H




