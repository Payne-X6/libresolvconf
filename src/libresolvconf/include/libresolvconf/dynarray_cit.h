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

#include <stdbool.h>

typedef const char *lresconf_dynarray_cit_t;

/**
 * @brief Get a string representation of the function name.
 *
 * Again, a somewhat longer description with fancy notes etc.
 *
 * @param[out]  str  char* where string representation will be stored.
 *
 * @return 0 on success, or -1 if an error occurred.
 *
 * @pre Same as with @foo. Why?!
 * @post Yeah, really, why?!
 */
void lresconf_dynarray_cit_next(lresconf_dynarray_cit_t *it);

bool lresconf_dynarray_cit_end(lresconf_dynarray_cit_t it);

void lresconf_dynarray_cit_destroy(lresconf_dynarray_cit_t *it);