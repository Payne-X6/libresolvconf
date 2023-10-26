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

#include <stdio.h>

#include <libresolvconf/resolvconf.h>

#include <sys/socket.h>

#define RESOLV_CONF_PATH "/etc/resolv.conf"

#define bool_to_str(x) (x ? "true" : "false")

int main(int argc, char **argv)
{
	lresconf_conf_t conf = { 0 };
	int ret = lresconf_load_defaults(&conf);
	if (ret != 0) {
		lresconf_conf_deinit(&conf);
		fprintf(stderr, "Error: %s\n", lresconf_strerror(ret));
		return ret;
	}
	ret = lresconf_load_file(&conf, RESOLV_CONF_PATH);
	if (ret != 0) {
		lresconf_conf_deinit(&conf);
		fprintf(stderr, "Error: %s\n", lresconf_strerror(ret));
		return ret;
	}
	ret = lresconf_load_env(&conf);
	if (ret != 0) {
		lresconf_conf_deinit(&conf);
		fprintf(stderr, "Error: %s\n", lresconf_strerror(ret));
		return ret;
	}

	// Print
	printf("nameservers:");
	for (lresconf_arr_cstr_it_t it = conf.nameservers;
	     !lresconf_arr_cstr_it_end(it);
	     lresconf_arr_cstr_it_next(&it)
	) {
		printf(" %s", it);
	}
	printf("\n");

	printf("domains:");
	for (lresconf_arr_cstr_it_t it = conf.domains;
	     !lresconf_arr_cstr_it_end(it);
	     lresconf_arr_cstr_it_next(&it)
	) {
		printf(" %s", it);
	}
	printf("\n");

	printf("sortlist:");
	for (const char *it = conf.sortlist;
	     !lresconf_arr_cstr_it_end(it);
	     lresconf_arr_cstr_it_next(&it)
	) {
		printf(" %s", it);
	}
	printf("\n");

	printf("family:");
	for (int *it = conf.family;
	     it != (conf.family + LRESCONF_FAMILY_LEN);
	     ++it
	) {
		switch (*it) {
		case AF_INET:
			printf(" inet");
			break;
		case AF_INET6:
			printf(" inet6");
			break;
		case AF_UNSPEC:
			goto break_family;
		default:
			fprintf(stderr,
			        "Error: %s\n",
			        lresconf_strerror(ENOTSUP));
			lresconf_conf_deinit(&conf);
			return ENOTSUP;
		}
	}
	break_family: printf("\n");

	printf("lookup:");
	for (lresconf_lookup_t *it = conf.lookup;
	     it != (conf.lookup + LRESCONF_LOOKUP_LEN);
	     ++it
	) {
		switch (*it) {
		case IRESCONF_LOOKUP_BIND:
			printf(" bind");
			break;
		case IRESCONF_LOOKUP_FILE:
			printf(" file");
			break;
		case IRESCONF_LOOKUP_UNSPEC:
			goto break_lookup;
		default:
			fprintf(stderr,
			        "Error: %s\n",
			        lresconf_strerror(ENOTSUP));
			lresconf_conf_deinit(&conf);
			return ENOTSUP;
		}
	}
	break_lookup: printf("\n");

	printf("options:\n");
	printf("\tattempts: %d\n", conf.options.attempts);
	printf("\tdebug: %s\n", bool_to_str(conf.options.debug));
	printf("\tedns0: %s\n", bool_to_str(conf.options.edns0));
	printf("\tinet6: %s\n", bool_to_str(conf.options.inet6));
	printf("\tinsecure1: %s\n", bool_to_str(conf.options.insecure1));
	printf("\tinsecure2: %s\n", bool_to_str(conf.options.insecure2));
	printf("\tip6-bytestring: %s\n", bool_to_str(conf.options.ip6_bytestring));
	printf("\tip6-dotint: %s\n", bool_to_str(conf.options.ip6_dotint));
	printf("\tndots: %d\n", conf.options.ndots);
	printf("\tno-check-names: %s\n", bool_to_str(conf.options.no_check_names));
	printf("\tno-reload: %s\n", bool_to_str(conf.options.no_reload));
	printf("\tno-tld-query: %s\n", bool_to_str(conf.options.no_tld_query));
	printf("\trotate: %s\n", bool_to_str(conf.options.rotate));
	printf("\tsingle-request: %s\n", bool_to_str(conf.options.single_request));
	printf("\tsingle-request-reopen: %s\n", bool_to_str(conf.options.single_request_reopen));
	printf("\ttcp: %s\n", bool_to_str(conf.options.tcp));
	printf("\ttimeout: %d\n", conf.options.timeout);
	printf("\ttrust-ad: %s\n", bool_to_str(conf.options.trust_ad));
	printf("\tuse-vc: %s\n", bool_to_str(conf.options.use_vc));

	return 0;
}