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
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "xgetopt.h"

xopt *
xopt_find(int optc, xopt *optv, const char *req)
{
	int i;
	xopt *opt;

	if (!req)
		return NULL;

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
	/* opt  : Pointer to the current option being processed,
	 * w    : ioctl structure for the terminal size,
	 * c    : Current character in description,
	 * err  : Error number (for errno),
	 * i    : Option index,
	 * j    : General-purpose for loop index,
	 * tlen : Total number of spaces until description starts,
	 * dlen : Description text string read index,
	 * line : Line position,
	 * tw   : Terminal width. */

	xopt *opt;
	struct winsize w;
	char c;
	int err, i, j, tlen, dlen, line, tw;

	/* Four tabs */
	tlen = 32;

	/* Get the input terminal width;  */
	tw = ioctl(fileno(stdin), TIOCGWINSZ, &w);

	/* Check for and print any error as a warning, but don't stop */
	if (tw == -1 && errno != 0) {
		err = errno;
		fprintf(stdout, "Warning: Can't get terminal width!: %s\n",
			strerror(err));
	}

	/* If we couldn't get the size for some reason, don't wrap text. */
	/* (If you have a terminal that is INT_MAX wide, file a bug report) */
	tw = (tw == -1) ? INT_MAX : w.ws_col;

	/* Avoid trying to print a null pointer */
	if (!x->usage_text)
		x->usage_text = "";

	/* Start printing! */
	printf("Usage: %s [OPTIONS] ", x->progname);

	/* Print out the usage text, wrapping it if necessary */
	for (j = 0, line = 0; j < (int)strlen(x->usage_text); j++) {
		/* Start with a couple of spaces if this is a wrapped line */
		if (line == 0 && j != 0) {
			printf("%s", "  ");
			line += 2;
		}
		/* If we're at the end of the line ... */
		if (line == tw - 1) {
			/* Next char is NOT a space? */
			if (x->usage_text[j] != ' ') {
				/* Backspace until a space */
				while (x->usage_text[--j] != ' ')
					printf("%s", "\b \b");
			}
			/* Then `continue' to next line */
			putchar('\n');
			line = 0;
			continue;
		}
		/* Print out a character of the usage text */
		if (line < tw - 1) {
			putchar(x->usage_text[j]);
			/* If we encounter a newline, act like
			 * it, otherwise, just incrememnt */
			if (x->usage_text[j] == '\n') {
				line = 0;
			} else {
				line++;
			}
		}
	}

	printf("\n\n %s\n", "Options:");

	/* For each option */
	for (i = 0; i < optc; i++) {
		/* Store it in a easy-to-type pointer */
		opt = &optv[i];

		/* Reset some things */
		dlen = line = 0;

		/* If this is just a normal argument, put a newline before it */
		if (!opt->sopt && !opt->lopt && opt->arg && opt->desc)
			putchar('\n');

		/* A little padding (two spaces) at the start */
		printf("%s", "  ");
		line += 2;

		/* Print out the short option */
		if (opt->sopt) {
			printf("%s", opt->sopt);
			line += (int)strlen(opt->sopt);
		}

		/* If there's *also* a long option, print a comma */
		if (opt->sopt && opt->lopt) {
			putchar(',');
			line++;
		}

		/* Print out the long option */
		if (opt->lopt) {
			printf("%s", opt->lopt);
			line += (int)strlen(opt->lopt);
		}

		/* If there's a short option, but not a long option, and the
		 * short option takes an argument, print a space */
		if (opt->sopt && !opt->lopt && opt->arg) {
			putchar(' ');
			line++;
		}

		/* If the long option takes an argument, print an equal sign */
		if (opt->lopt && opt->arg) {
			putchar('=');
			line++;
		}

		/* Print out required argument */
		if (opt->arg) {
			printf("%s", opt->arg);
			line += (int)strlen(opt->arg);
		}

		/* Print out description */
		if (opt->desc) {
			/* For each character in the description */
			while ((c = opt->desc[dlen]) != '\0') {
				/* Indent the beginning of the description, or
				 * at the beginning of a line up-to tlen */
				if (dlen == 0 || line == 0) {
					for (j = 0; j < tlen - line; j++)
						putchar(' ');
					line += j;
				}
				/* If we're at the end of the line ... */
				if (line == tw - 1) {
					/* Next char is NOT a space? */
					if (c != ' ') {
						/* Backspace until a space */
						while (opt->desc[--dlen] != ' ')
							printf("%s", "\b \b");
					}
					/* Then `continue' to next line,
					 * incrementing the description index
					 * to skip the space */
					putchar('\n');
					line = 0;
					dlen++;
					continue;
				}
				/* Print out a character of the description */
				if (line < tw - 1) {
					putchar(c);
					/* If we encounter a newline, act like
					 * it, otherwise, just incrememnt */
					line = (c != '\n') ? line + 1: 0;
				}
				/* Increment the description read index */
				dlen++;
			}
			/* If this is just a normal kind of argument and not an
			 * option flag, put a newline after it */
			if (!opt->sopt && !opt->lopt && opt->arg && opt->desc)
				putchar('\n');
		}
		/* On to the next option! */
		putchar('\n');
	}
	/* All done. */
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
