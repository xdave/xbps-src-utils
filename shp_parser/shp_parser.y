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

#include <assert.h>
#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"
#include "str_map.h"

int done = 0;

%}

%debug
%output  "shp_parser.c"
%defines "shp_parser.h"

%define api.prefix shp
%define api.pure
%locations
%error-verbose
%lex-param   { void *scanner }
%parse-param { shp *s }
%parse-param { void *scanner }

%union { char str[BUFSIZ]; }

%token <str> ID
%token <str> DQUOTE	"Double-quoted String"
%token <str> SQUOTE	"Single-quoted String"
%token <str> VALUE
%token <str> INCLUDE
%token <str> UNPARSED

%type <str> expr	"Expression"
%type <str> id		"Identifier"
%type <str> assign	"Variable Assignment"
%type <str> value	"Value"
%type <str> include	"External Include"

%token END	0	"End of File"

%start script

%%

script	: exprsec
	;

exprsec	: /* empty */
	| exprsec expr
	;

expr	: assign
     	| include
	| UNPARSED
	;

include : INCLUDE {{
		memset($$, '\0', BUFSIZ);
		shp inc;
		_REF($$, $1);
		shp_xtrace(s, "+ %s\n", $$);
		shp_init(&inc, ($$)+2, s->accept);
		inc.env = s->env;
		shp_process(&inc, s);
		shp_end(&inc);
	}}
	;

assign	: id value {
		str_map_del(s->env, $1);
		assert(str_map_find(s->env, $1) == NULL);
		str_map_add(s->env, $1, $2);
		shp_xtrace(s, "+ %s=%s\n", $1, $2);
		_VARCHK($1);
	}
	;

id	: ID {
		memset($$, '\0', BUFSIZ);
		strncpy($$, $1, strlen($1) + 1);
	}
   	;

value	: VALUE  { memset($$, '\0', BUFSIZ); _REF($$, $1); }
	| DQUOTE { memset($$, '\0', BUFSIZ); _REF($$, $1); }
	| SQUOTE { memset($$, '\0', BUFSIZ); _REF($$, $1); }
	;

%%
