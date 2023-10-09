#include "vector.h"

#include <stdlib.h>
#include <string.h>

#include <error.h>

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

static size_t bit_ceil(size_t n) {
	--n;
	for (uint8_t shift = 1; shift < (8 * sizeof(n)); shift <<= 1) {
		n |= n >> shift;
	}
	return ++n;
}

int vector_init(vector_t *vector, size_t init_size)
{
	size_t size = bit_ceil(init_size);
	vector->data = malloc(size);
	if (unlikely(vector->data == NULL)) {
		return ENOMEM;
	}
	vector->idx = 0;
	vector->size = size;
	vector->data[0] = '\0';
	return E_OK;
}

int vector_reserve(vector_t *vector, size_t size)
{
	if (size <= vector->size) {
		return E_OK;
	}
	size_t new_size = bit_ceil(size);
	char *data_tmp = realloc(vector->data, new_size);
	if (unlikely(data_tmp == NULL)) {
		return ENOMEM;
	}
	vector->data = data_tmp;
	vector->size = new_size;

	return E_OK;
}

int vector_push_back(vector_t *vector, char *src, size_t size)
{
	int ret = vector_reserve(vector, vector->idx + size + 2);
	if (unlikely(ret != E_OK)) {
		return ret;
	}
	memcpy(vector->data + vector->idx, src, size);
	vector->idx += size;
	vector->data[vector->idx++] = '\0';
	vector->data[vector->idx] = '\0';
	return E_OK;
}

bool vector_is_empty(vector_t *vector)
{
	return vector->idx == 0;
}

vector_it_t vector_begin(vector_t *vector)
{
	return vector->data;
}

void vector_clear(vector_t *vector)
{
	vector->idx = 0;
}

void vector_deinit(vector_t *vector)
{
	free(vector->data);
	vector->data = NULL;
	vector->idx = 0;
	vector->size = 0;
}