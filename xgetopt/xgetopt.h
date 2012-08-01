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
