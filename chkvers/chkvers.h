#ifndef __CHKVERS_H__
#define __CHKVERS_H__

#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <xbps_api.h>

#define find_pkg(c,n) xbps_rpool_find_pkg(c,n,false,true)
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

int chkvers_show_usage(const char *progname);
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
