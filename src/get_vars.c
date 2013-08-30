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

#include <string.h>

#include "rcv.h"

int
rcv_get_pkgver(rcv_t *rcv)
{
	struct slre_cap caps[2];
	const char *regex = "^([_A-Za-z][_A-Za-z0-9]*)=([^\n]*)$";
	const char *k, *v, *w;
	size_t klen, vlen, tlen = 0, plen = 0, wlen = 0;
	map_item_t _item;
	map_item_t *item;
	
	plen = strlen(rcv->ptr);
	w = strchr(rcv->ptr, '\n');
	wlen = (w == NULL) ? 0 : strlen(w);

	tlen = slre_match(regex, rcv->ptr, plen - wlen, caps, 2, NULL);
	if (tlen > 0) {
		k = caps[0].ptr;
		v = caps[1].ptr;
		klen = caps[0].len;
		vlen = caps[1].len;
		if (v[0] == '"') { v++; vlen--; }
		if (v[vlen-1] == '"') { vlen--; }
		if (v[0] == '\n') { goto end; } /* Skips multiline string vars*/
		_item = map_add_n(rcv->env, k, klen, v, vlen);
		item = &rcv->env->items[_item.i];
		if (strchr(v, '$')) {
			item->v.s = rcv_refs(rcv, item->v.s, item->v.len);
			item->v.len = strlen(item->v.s);
			item->v.vmalloc = 1;
		} else {
			item->v.vmalloc = 0;
		}
		if (strchr(item->v.s, '$') && item->v.vmalloc == 1) {
			item->v.s = rcv_cmd(rcv, item->v.s, item->v.len);
			item->v.len = strlen(item->v.s);
		}
		if ((strncmp("pkgname",  k, klen) == 0) ||
		    (strncmp("version",  k, klen) == 0) ||
		    (strncmp("revision", k, klen) == 0)) {
			rcv->have_vars += 1;
		}
	}
	end:
	rcv->ptr += plen - wlen;
	if (*rcv->ptr == '\n')
		rcv->ptr++;

	if (*(rcv->ptr) == '\0' || rcv->have_vars > 2) {
		return 0;
	}

	return 1;
}
