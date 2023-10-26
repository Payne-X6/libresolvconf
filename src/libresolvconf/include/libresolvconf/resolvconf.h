/*
* 'libresolvconf' is a shared library for parsing resolv.conf files,
* alongside associated utilities.
*
* Copyright (C) 2023 libresolvconf
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "arr_str.h"

#define LRESCONF_FAMILY_LEN 2
#define LRESCONF_LOOKUP_LEN 2

typedef enum {
	IRESCONF_LOOKUP_UNSPEC = 0,
	IRESCONF_LOOKUP_BIND = 1,
	IRESCONF_LOOKUP_FILE = 2
} __attribute__ ((__packed__)) lresconf_lookup_t;

typedef struct {
	lresconf_arr_cstr_t nameservers;
	lresconf_arr_cstr_t domains;
	lresconf_arr_cstr_t sortlist;
	int family[LRESCONF_FAMILY_LEN];
	lresconf_lookup_t lookup[LRESCONF_LOOKUP_LEN];
	struct {
		uint8_t attempts;
		uint8_t timeout;
		uint8_t ndots;
		bool    debug                 : 1;
		bool    edns0                 : 1;
		bool    inet6                 : 1;
		bool    insecure1             : 1;
		bool    insecure2             : 1;
		bool    ip6_bytestring        : 1;
		bool    ip6_dotint            : 1;
		bool    no_check_names        : 1;
		bool    no_reload             : 1;
		bool    no_tld_query          : 1;
		bool    rotate                : 1;
		bool    single_request        : 1;
		bool    single_request_reopen : 1;
		bool    tcp                   : 1;
		bool    trust_ad              : 1;
		bool    use_vc                : 1;
	} options;
	struct {
		size_t line;
		size_t col;
	} error;
} lresconf_conf_t;

void lresconf_conf_deinit(lresconf_conf_t *conf);

int lresconf_load_defaults(lresconf_conf_t *conf);
int lresconf_load_file(lresconf_conf_t *conf, const char *path);
int lresconf_load_env(lresconf_conf_t *conf);