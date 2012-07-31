#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xgetopt.h"

xopt *
xopt_find(int optc, xopt *optv, const char *req)
{
	int i;
	xopt *opt;
	if (!req) return NULL;
	for (i = 0; i < optc; i++) {
		opt = &optv[i];
		if (opt->sopt)
			if (strncmp(opt->sopt, req, strlen(opt->sopt)) == 0)
				return opt;
		if (opt->lopt)
			if (strncmp(opt->lopt, req, strlen(opt->lopt)) == 0)
				return opt;
	}
	return NULL;
}

void
xopt_usage(xgetopt *x, int optc, xopt *optv)
{
	xopt *opt;
	int i, j, tlen, slen;
	char optline[BUFSIZ] = {'\0'};

	tlen = 32;
	printf("Usage: %s [OPTIONS] %s\n", x->progname,
		(x->usage_text) ? x->usage_text : "");
	printf(" %s\n", "Options:");

	for (i = 0; i < optc; i++) {
		opt = &optv[i];
		strcat(optline, "  ");
		if (opt->sopt) strcat(optline, opt->sopt);
		if (opt->sopt && opt->lopt) strcat(optline, ",");
		if (opt->lopt) strcat(optline, opt->lopt);
		if (opt->sopt && !opt->lopt && opt->arg) {
			strcat(optline, " ");
			strcat(optline, opt->arg);
		}
		if (opt->lopt && opt->arg) {
			strcat(optline, "=");
			strcat(optline, opt->arg);
		}
		if (opt->desc) {
			slen = (int)strlen(optline);
			for (j = 0; j < tlen-slen; j++)
				strcat(optline, " ");
			strcat(optline, opt->desc);
		}
		printf("%s\n", optline);
		optline[0] = '\0';
	}
	exit(1);
}

int
xgetopts(xgetopt *x, int optc, xopt *optv, int argc, char **argv)
{
	const char *tmp;

	if (!x->progname) x->progname = argv[0];

	if (x->i < argc) {
		x->opt = NULL;
		x->optarg[0] = '\0';

		if (argv[x->i][0] == '-') {
			x->opt = xopt_find(optc, optv, argv[x->i]);
			if (!x->opt) {
				printf(XGO_UNKN_OP, argv[x->i]);
				xopt_usage(x, optc, optv);
			}
			tmp = (x->opt->lopt) ? x->opt->lopt : x->opt->sopt;
			if (x->opt->arg && !strstr(argv[x->i], "=")
					&& !argv[x->i+1]) {
				printf(XGO_REQ_ARG, tmp, x->opt->arg);
				xopt_usage(x, optc, optv);
			}
			if (x->opt->arg && strstr(argv[x->i], "=")) {
				if (strlen(strstr(argv[x->i], "=") + 1) == 0) {
					printf(XGO_REQ_ARG, tmp, x->opt->arg);
					xopt_usage(x, optc, optv);
				}
				strcpy(x->optarg, strstr(argv[x->i], "=") + 1);
			} else if (x->opt->arg && argv[x->i+1]) {
				if (argv[x->i+1][0] == '-') {
					printf(XGO_REQ_ARG, tmp, x->opt->arg);
					xopt_usage(x, optc, optv);
				}
				strcpy(x->optarg, argv[x->i+1]);
				x->i++;
				x->optind++;
			}
			x->optind++;
		}
		x->i++;
		return 0;
	}
	x->optind++;
	return -1;
}
