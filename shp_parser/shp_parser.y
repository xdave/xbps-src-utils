%{
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
		str_map_add(s->env, $1, $2);
		shp_xtrace(s, "+ %s=%s\n", $1, $2);
		_VARCHK($1);
	}
	;

id	: ID { strncpy($$, $1, strlen($1) + 1); }
   	;

value	: VALUE  { _REF($$, $1); }
	| DQUOTE { _REF($$, $1); }
	| SQUOTE { _REF($$, $1); }
	;

%%
