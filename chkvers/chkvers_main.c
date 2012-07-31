#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chkvers.h"
#include "str_map.h"

static const char *accept[] = { "version", "revision", NULL };

int
main(int argc, char **argv)
{
	chkvers chk;

	chkvers_init(&chk);
	chk.env = str_map_create();

	if (chkvers_getopt(&chk, &argc, &argv) != 0) {
		chkvers_end(&chk);
		return (EXIT_FAILURE);
	}

	chkvers_process_dir(&chk,
		str_map_find(chk.env, "XBPS_SRCPKGDIR")->value, accept);

	chkvers_process_files(&chk, argv, (size_t)argc, accept);

	str_map_destroy(chk.env);
	chkvers_end(&chk);

	return (EXIT_SUCCESS);
}
