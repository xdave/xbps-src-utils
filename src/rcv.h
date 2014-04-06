/*
 * Copyright (c) 2012-2013 Dave Elusive <davehome@redthumb.info.tm>
 * All rights reserved
 *
 * You may redistribute this file and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation. For the terms of this license, see 
 * <http://www.gnu.org/licenses/>.
 *
 * You are free to use this file under the terms of the GNU General Public
 * License, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifndef _RCV_H_
#define _RCV_H_ 1

#ifdef _RCV_DEBUG
# define _dprintf(...)							\
do {									\
	fprintf(stderr, "DEBUG => %s:%d in %s(): ",			\
		__FILE__, __LINE__, __PRETTY_FUNCTION__);		\
	fprintf(stderr, __VA_ARGS__);					\
} while (0)
#else
#define _dprintf(...)
#endif

#include <stddef.h>
#include <stdbool.h>
#include <xbps.h>

typedef struct str_ptr_t {
	char *s;
	size_t len;
	int vmalloc;
} string;

typedef struct _map_item_t {
	string k, v;
	size_t i;
} map_item_t;

typedef struct _map_t {
	size_t size, len;
	map_item_t *items;
} map_t;

typedef struct _rcv_t {
	const char *prog, *fname;
	char *input, *ptr, *xsrc_conf, *xbps_conf, *distdir, *pkgdir;
	size_t len, have_vars;
	map_t *env;
	struct xbps_handle xhp;
	xbps_dictionary_t pkgd;
	bool show_missing;
	bool manual;
} rcv_t;

typedef int (*rcv_check_func)(rcv_t *);
typedef int (*rcv_proc_func)(rcv_t *, const char *, rcv_check_func);

#endif /* _RCV_H_ */
