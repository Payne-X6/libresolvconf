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

#include "dynarray_cit.h"

typedef struct {
	char   *data;
	size_t  size;
	size_t  idx;
} dynarray_t;

int vector_init(dynarray_t *vector, size_t init_size);

int vector_reserve(dynarray_t *vector, size_t size);

int vector_push_back(dynarray_t *vector, char *src, size_t size);

bool vector_is_empty(dynarray_t *vector);

lresconf_dynarray_cit_t vector_begin(dynarray_t *vector);

void vector_clear(dynarray_t *vector);
void vector_deinit(dynarray_t *vector);