#ifndef REPO_CHECKVERS_H
#define REPO_CHECKVERS H

#define DIE(fmt, ...)						\
do {								\
	fprintf(stderr, "ERROR: [%s] ", strerror(errno));	\
	fprintf(stderr, fmt, __VA_ARGS__);			\
	fputs("\n", stderr);					\
	exit(EXIT_FAILURE);					\
} while (0)

#endif /* REPO_CHECKVERS_H */
