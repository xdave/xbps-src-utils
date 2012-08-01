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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shp_api.h"
#include "shp_parser.h"
#include "shp_lexer.h"

void
shp_init(shp *s, const char *fname, const char **accept)
{
	char errbuf[BUFSIZ] = {'\0'};

	s = memset(s, 0, sizeof(shp));

	s->accept = accept;
	s->fname = fname;
	s->print_xtrace = 0;
	s->file = stdin;

	if (strcmp(fname, "__STDIN__") != 0) {
		if ((s->file = fopen(fname, "r")) == NULL) {
			strerror_r(errno, errbuf, BUFSIZ);
			fprintf(stderr, "Can't open %s: %s\n", fname, errbuf);
			exit(EXIT_FAILURE);
		}
	}
	shp_scanner_init(s);
	s->state = shp_create_buffer(s->file, YY_BUF_SIZE, s->scanner);
	shpset_in(s->file, s->scanner);
}
