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

#include "arr_str.h"

#include <stdlib.h>

lresconf_arr_cstr_it_t lresconf_arr_cstr_begin(lresconf_arr_cstr_t arr)
{
	return (lresconf_arr_cstr_it_t)arr;
}

void lresconf_arr_cstr_destroy(lresconf_arr_cstr_t *arr)
{
	free((void *)*arr);
	*arr = NULL;
}

void lresconf_arr_cstr_it_next(lresconf_arr_cstr_it_t *it)
{
	for (; (**it) != '\0'; ++(*it)) {}
	++(*it);
}

bool lresconf_arr_cstr_it_end(lresconf_arr_cstr_it_t it)
{
	return *it == '\0';
}