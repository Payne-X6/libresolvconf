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

#include <getopt.h>
#include <stdio.h>

#include <libresolvconf/resolvconf.h>

#define RESOLV_CONF_PATH "/etc/resolv.conf"

static void print_help()
{
	printf("\n");	
}

int main(int argc, char **argv)
{
	static const struct option opts[] = {
		{ "help",    no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ NULL }
	};

	const char *input = RESOLV_CONF_PATH;

	int opt = 0;
	while ((opt = getopt_long(argc, argv, "hV", opts, NULL)) != -1) {
		switch (opt) {
		case 'h':
			print_help();
			return LRESCONF_EOK;
		case 'V':
			break;
		default:
			break;
		}
	}
	if (argc - optind) {
		input = argv[optind];
	}

	lresconf_conf_t conf = { 0 };
	int ret = lresconf_load_file(&conf, input);
	if (ret != 0) {
		lresconf_conf_deinit(&conf);
		fprintf(stderr, "Error: %s\n", lresconf_strerror(ret));
		return ret;
	}

	lresconf_conf_deinit(&conf);
	return 0;
}