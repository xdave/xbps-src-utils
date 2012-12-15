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

#ifndef __CHKVERS_H__
#define __CHKVERS_H__

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <xbps_api.h>

#define find_pkg(c,n) xbps_rpool_get_pkg(c,n)
#define dict_get(d,k,r) prop_dictionary_get_cstring_nocopy(d,k,r)

typedef struct str_map_t str_map;

typedef struct _chkvers chkvers;

struct _chkvers {
	DIR *dir;
	struct dirent entry, *result;
	struct stat st;
	struct xbps_handle xhp;
	prop_dictionary_t pkgd;
	str_map *env;
	const char *dirpath, *repover;
	char xbps_src_cfg[BUFSIZ],
	     pkgname[BUFSIZ],
	     srcpkgver[BUFSIZ];
	int xtrace, debug, show_missing;
};

int chkvers_init(chkvers *chk);
chkvers *chkvers_new(void);
void chkvers_end(chkvers *chk);
void chkvers_free(chkvers *chk);
void chkvers_find_xbps_src_cfg(chkvers *chk);
int chkvers_getopt(chkvers *chk, int *argcp, char ***argvp);
void chkvers_process_dir(chkvers *chk, const char *dir, const char **acceptp);
void chkvers_process_files(chkvers *chk, char **files, size_t fc,
		const char **acceptp);

#endif /* __CHKVERS_H__ */
