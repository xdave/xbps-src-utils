#ifndef __XGETOPT_H__
#define __XGETOPT_H__

#include <stdio.h>

#define XGO_UNKN_OP "Unknown option: `%s'\n"
#define XGO_REQ_ARG "Option `%s' requires argument `%s'\n"

typedef struct xopt_t xopt;
typedef struct xgetopt_t xgetopt;

struct xopt_t {
	const char *sopt, *lopt, *arg, *desc;
};

struct xgetopt_t {
	int i, optind;
	const char *progname;
	const char *usage_text;
	char optarg[BUFSIZ];
	xopt *opt;
};

xopt *xopt_find(int optc, xopt *optv, const char *req) __attribute__ ((pure));
void xopt_usage(xgetopt *x, int optc, xopt *optv) __attribute__ ((noreturn));
int xgetopts(xgetopt *x, int optc, xopt *optv, int argc, char **argv);


#endif /* __XGETOPT_H__ */
