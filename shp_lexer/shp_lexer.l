%{
/*-
 * Copyright (c) 2012 Dave Elusive <davehome@redthumb.info.tm>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
VALUE		[A-z0-9\$\(\)\-_\{\}\|\:\\/\.,~\%\+= '"\[\]\*\@\<\>]*
FUNC		[A-z_][A-z0-9_]+"() {".[^}]*"}"$
COMMENT		[#].*$
WS		[\t\n ]+

%x ASSIGN

%%

{INCLUDE}		{ __LVALUE(yytext, strlen(yytext));
				return INCLUDE; }
{ID}			{ BEGIN(ASSIGN);
				if((strcmp("distfiles=", yytext) == 0))
					return END;
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
