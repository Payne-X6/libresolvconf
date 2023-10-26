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
#include <stddef.h>
#include <stdint.h>

#include "arr_str.h"

typedef struct {
	lresconf_arr_str_t data;
	size_t  size;
	size_t  idx;
} vec_str_t;

int vec_str_init(vec_str_t *vector, const size_t init_size);
void vec_str_move_arr_cstr(vec_str_t * restrict vector,  lresconf_arr_cstr_t * restrict arr);

int vec_str_reserve(vec_str_t *vector, size_t size);

int vec_str_push_back(vec_str_t * restrict vector, char * restrict src, size_t size);
bool vec_str_is_empty(vec_str_t *vector);

void vec_str_clear(vec_str_t *vector);
void vec_str_deinit(vec_str_t *vector);