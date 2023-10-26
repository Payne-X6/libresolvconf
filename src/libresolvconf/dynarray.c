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

#include "dynarray.h"

#include <stdlib.h>
#include <string.h>

#include <error.h>

#include "defines.h"

static size_t bit_ceil(size_t n) {
	--n;
	for (uint8_t shift = 1; shift < (8 * sizeof(n)); shift <<= 1) {
		n |= n >> shift;
	}
	return ++n;
}

int vector_init(dynarray_t *vector, size_t init_size)
{
	size_t size = bit_ceil(init_size);
	vector->data = malloc(size);
	if (unlikely(vector->data == NULL)) {
		return ENOMEM;
	}
	vector->idx = 0;
	vector->size = size;
	vector->data[0] = '\0';
	return LRESCONF_EOK;
}

int vector_reserve(dynarray_t *vector, size_t size)
{
	size += 2;
	if (size <= vector->size) {
		return LRESCONF_EOK;
	}
	size_t new_size = bit_ceil(size);
	lresconf_arr_str_t data_tmp = realloc(vector->data, new_size);
	if (unlikely(data_tmp == NULL)) {
		return ENOMEM;
	}
	vector->data = data_tmp;
	vector->size = new_size;

	return LRESCONF_EOK;
}

int vector_push_back(dynarray_t *vector, char *src, size_t size)
{
	int ret = vector_reserve(vector, vector->idx + size);
	if (unlikely(ret != LRESCONF_EOK)) {
		return ret;
	}
	memcpy(vector->data + vector->idx, src, size);
	vector->idx += size;
	vector->data[vector->idx++] = '\0';
	vector->data[vector->idx] = '\0';
	return LRESCONF_EOK;
}

bool vector_is_empty(dynarray_t *vector)
{
	return vector->idx == 0;
}

lresconf_arr_cstr_it_t vector_begin(dynarray_t *vector)
{
	return vector->data;
}

void vector_clear(dynarray_t *vector)
{
	vector->idx = 0;
}

void vector_deinit(dynarray_t *vector)
{
	free(vector->data);
	vector->data = NULL;
	vector->idx = 0;
	vector->size = 0;
}