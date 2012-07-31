#ifndef __SHP_API_H__
#define __SHP_API_H__

#include <stdio.h>

struct yyguts_t;
typedef struct SHPLTYPE SHPLTYPE;
typedef struct yy_buffer_state *YY_BUFFER_STATE;

typedef struct str_map_t str_map;

typedef struct _shp shp;

struct _shp {
	void *scanner;
	YY_BUFFER_STATE state;
	FILE *file;
	str_map *env;
	const char **accept;
	const char *fname;
	char *input;
	int print_xtrace;
};

#define YYSTYPE SHPSTYPE
#define YYLTYPE SHPLTYPE
#define shperror shp_error

/* Used in the Lexer */
#define __LVALUE(text, len) do {					\
	strncpy(yylval->str, text, len);				\
	yylval->str[len] = '\0';					\
} while(0)

/* Used in the Parser */
#define _REF(dest, src) do {{						\
	char __tmp[BUFSIZ] = {'\0'};					\
	shp_get_ref(s, __tmp, src);					\
	shp_get_cmd(s, dest, __tmp);					\
}} while (0)

/* Used in the Parser */
#define _VARCHK(str) do {						\
	if (s->accept != NULL) {{					\
		int i = 0;						\
		for (; s->accept[i] != NULL; i++) {			\
			if (strcmp(str, s->accept[i]) == 0)		\
				done++;					\
		}							\
		if (done == i){						\
			done = 0;					\
			YYABORT;					\
		}							\
	}}								\
} while (0)

shp *shp_new(const char *fname, const char **acceptp);
void shp_init(shp *s, const char *fname, const char **acceptp);
void shp_end(shp *s);
void shp_free(shp *s);
void shp_process(shp *s, shp *parent);
void shp_scanner_init(shp *s);
void shp_error(SHPLTYPE *locp, shp *s, struct yyguts_t *scnr, const char *msg);
void shp_get_ref(shp *s, char *dest, char *src);
int shp_get_cmd(shp *s, char *dest, const char *src);
void shp_xtrace(shp *s, const char *fmt, ...)
	__attribute__ ((format(printf,2,3)));

#endif /* __SHP_API_H__ */
