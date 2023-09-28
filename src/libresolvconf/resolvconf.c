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

#include "resolvconf.h"

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
#include "dynarray.h"
#include "parser.h"

void lresconf_conf_deinit(lresconf_conf_t *conf)
{
	lresconf_dynarray_cit_destroy(&conf->domains);
	lresconf_dynarray_cit_destroy(&conf->nameservers);
	lresconf_dynarray_cit_destroy(&conf->sortlist);
}

int lresconf_load_defaults(lresconf_conf_t *conf)
{
	dynarray_t nameservers, domains, sortlist;

	int ret = vector_init(&nameservers, INET6_ADDRSTRLEN * 3);
	if (ret) {
		return ret;
	}
	ret = vector_push_back(&nameservers, "127.0.0.1", sizeof("127.0.0.1") - 1);
	if (ret) {
		vector_deinit(&nameservers);
		return ret;
	}
	ret = vector_push_back(&nameservers, "::1", sizeof("::1") - 1);
	if (ret) {
		vector_deinit(&nameservers);
		return ret;
	}

	ret = vector_init(&domains, DOMAIN_NAME_LEN * 4);
	if (ret) {
		vector_deinit(&nameservers);
		return ret;
	}
	char hostname[MAXHOSTNAMELEN];
	ret = gethostname(hostname, MAXHOSTNAMELEN);
	char *domainname = strchr(hostname, '.');
	if (domainname != NULL) {
		domainname++;
		vector_push_back(&domains, domainname, strlen(domainname));
	}

	ret = vector_init(&sortlist, SORTLIST_LEN * 10);
	if (ret) {
		vector_deinit(&nameservers);
		vector_deinit(&domains);
		return ret;
	}

	conf->nameservers = vector_begin(&nameservers);
	conf->domains = vector_begin(&domains);
	conf->sortlist = vector_begin(&sortlist);
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
		// dynarray_t domains;
		// int ret = vector_init(&domains, MAXHOSTNAMELEN * 4);
		// if (ret) {
		// 	return ret;
		// }

		// while (localdomain) {
		// 	char *localdomain_end = strchr(localdomain, ' ');
		// 	if (localdomain_end == NULL) {
		// 		vector_push_back(&domains, localdomain, strlen(localdomain));
		// 		localdomain = localdomain_end;
		// 	} else {
		// 		vector_push_back(&domains, localdomain, localdomain_end - localdomain);
		// 		localdomain = localdomain_end + 1;
		// 	}
		// }

		// lresconf_dynarray_cit_destroy(&conf->domains);
		// conf->domains = vector_begin(&domains);
	}

	char *options = getenv(RES_OPTIONS_ENV);
	if (options) {
		parse_env_opts(conf, options);
	}

	return 0;
}