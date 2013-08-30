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

#include "slre.h"

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
} rcv_t;

typedef int (*rcv_check_func)(rcv_t *);
typedef int (*rcv_proc_func)(rcv_t *, const char *, rcv_check_func);

void		rcv_find_conf(rcv_t *rcv);
int		rcv_show_usage(const char *prog);
void		rcv_set_distdir(rcv_t *rcv, const char *distdir);

void		rcv_init(rcv_t *rcv, const char *prog);
bool		rcv_load_file(rcv_t *rcv, const char *fname);
int		rcv_process_file(rcv_t *rcv, const char *fname, rcv_check_func);
int		rcv_process_dir(rcv_t *rcv, const char *path, rcv_proc_func);
int		rcv_check_version(rcv_t *rcv);
int		rcv_parse_config(rcv_t *rcv);
int		rcv_get_pkgver(rcv_t *rcv);
char *		rcv_refs(rcv_t *rcv, const char *s, size_t len);
char *		rcv_cmd(rcv_t *rcv, const char *s, size_t len);
void		rcv_end(rcv_t *rcv);

map_t *		map_create(void);
map_item_t	map_new_item(void) __attribute__ ((const));
map_item_t	map_add_n(map_t *map, const char *k, size_t kn, const char *v, size_t vn);
map_item_t	map_add(map_t *map, const char *k, const char *v);
map_item_t	map_find(map_t *map, const char *k) __attribute__ ((pure));
void		map_destroy(map_t *map);

#endif /* _RCV_H_ */
