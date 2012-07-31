%{
#include "shp_api.h"
#include "shp_parser.h"
%}

%option outfile="shp_lexer.c" header-file="shp_lexer.h"
%option warn never-interactive
%option noyywrap noinput nounput nounistd nodefault
%option yylineno
%option bison-bridge bison-locations
%option reentrant prefix="shp"

INCLUDE		^\.[ ][^\n]*$
ID		^[\t ]+?[A-z_][A-z0-9_]*[=]
DQUOTE		["][^"]*["]
SQUOTE		['][^\n']*[']
VALUE		[A-z0-9\$\(\)\-_\{\}\|\:\\/\.,~\%\+= '"\[\]\*]*
FUNC		[A-z_][A-z0-9_]+"() {".[^}]*"}"$
COMMENT		[#].*$
WS		[\t\n ]+

%x ASSIGN

%%

{INCLUDE}		{ __LVALUE(yytext, strlen(yytext));
				return INCLUDE; }
{ID}			{ BEGIN(ASSIGN);
				__LVALUE(yytext, strlen(yytext) - 1);
				return ID; }
<ASSIGN>[\n]		{ BEGIN(INITIAL); }
<ASSIGN>{DQUOTE}$	{ __LVALUE(yytext + 1, strlen(yytext) - 2);
				return DQUOTE; }
<ASSIGN>{SQUOTE}$	{ __LVALUE(yytext + 1, strlen(yytext) - 2);
				return SQUOTE; }
<ASSIGN>{VALUE}		{ __LVALUE(yytext, strlen(yytext));
				return VALUE; }
<*>{COMMENT}
{FUNC}			{ return UNPARSED; }
{WS}			{ return UNPARSED; }
.			{ return UNPARSED; }

%%
