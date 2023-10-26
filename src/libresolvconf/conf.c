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

#include "conf.h"

#include <fcntl.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "defines.h"
#include "vec_str.h"
#include "parser.h"

void lresconf_conf_deinit(lresconf_conf_t *conf)
{
	lresconf_arr_cstr_destroy(&conf->domains);
	lresconf_arr_cstr_destroy(&conf->nameservers);
	lresconf_arr_cstr_destroy(&conf->sortlist);
}

int lresconf_load_defaults(lresconf_conf_t *conf)
{
	vec_str_t nameservers, domains, sortlist;

	int ret = vec_str_init(&nameservers, INET6_ADDRSTRLEN * 3);
	if (ret) {
		return ret;
	}
	ret = vec_str_push_back(&nameservers, "127.0.0.1", sizeof("127.0.0.1") - 1);
	if (ret) {
		vec_str_deinit(&nameservers);
		return ret;
	}
	ret = vec_str_push_back(&nameservers, "::1", sizeof("::1") - 1);
	if (ret) {
		vec_str_deinit(&nameservers);
		return ret;
	}

	ret = vec_str_init(&domains, DOMAIN_NAME_LEN * 4);
	if (ret) {
		vec_str_deinit(&nameservers);
		return ret;
	}
	char hostname[MAXHOSTNAMELEN];
	ret = gethostname(hostname, MAXHOSTNAMELEN);
	char *domainname = strchr(hostname, '.');
	if (domainname != NULL) {
		domainname++;
		vec_str_push_back(&domains, domainname, strlen(domainname));
	}

	ret = vec_str_init(&sortlist, SORTLIST_LEN * 10);
	if (ret) {
		vec_str_deinit(&nameservers);
		vec_str_deinit(&domains);
		return ret;
	}

	vec_str_move_arr_cstr(&nameservers, &conf->nameservers);
	vec_str_move_arr_cstr(&domains, &conf->domains);
	vec_str_move_arr_cstr(&sortlist, &conf->sortlist);
	conf->family[0] = AF_INET;
	conf->family[1] = AF_INET6;
	conf->lookup[0] = IRESCONF_LOOKUP_BIND;
	conf->lookup[1] = IRESCONF_LOOKUP_FILE;
	conf->options = (typeof(conf->options)){
		.attempts = RES_DFLRETRY,
		.debug = false,
		.edns0 = false,
		.inet6 = false,
		.insecure1 = false,
		.insecure2 = false,
		.ip6_bytestring = false,
		.ip6_dotint = false,
		.ndots = 1,
		.no_check_names = false,
		.no_reload = false,
		.no_tld_query = false,
		.rotate = false,
		.single_request = false,
		.single_request_reopen = false,
		.tcp = false,
		.timeout = RES_TIMEOUT,
		.trust_ad = false,
		.use_vc = false
	};
	conf->error = (typeof(conf->error)){
		.col = 0,
		.line = 0
	};

	return 0;
}

int lresconf_load_file(lresconf_conf_t *conf, const char *path)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		return errno;
	}

	struct stat statbuf;
	int ret = fstat(fd, &statbuf);
	if (ret < 0) {
		close(fd);
		return errno;
	}
	if (statbuf.st_size == 0) {
		close(fd);
		return LRESCONF_EOK;
	}

	char *ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		return errno;
	}

	ret = parse(conf, ptr, statbuf.st_size);

	ret = munmap(ptr, statbuf.st_size);
	if(ret != 0){
		return errno;
	}

	return 0;
}

int lresconf_load_env(lresconf_conf_t *conf)
{
	static const char *LOCALDOMAIN_ENV = "LOCALDOMAIN";
	static const char *RES_OPTIONS_ENV = "RES_OPTIONS";

	char *localdomain = getenv(LOCALDOMAIN_ENV);
	if (localdomain) {
		parse_env_domains(conf, localdomain);
	}

	char *options = getenv(RES_OPTIONS_ENV);
	if (options) {
		parse_env_opts(conf, options);
	}

	return 0;
}