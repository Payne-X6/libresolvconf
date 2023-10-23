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

#define RESOLV_CONF_PATH "/etc/resolv.conf"

int main(int argc, const char **argv)
{
    const char *input = RESOLV_CONF_PATH;
    if (argc > 1) {
        input = argv[1];
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