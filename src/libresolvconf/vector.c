#include "vector.h"

#include <stdlib.h>
#include <string.h>

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
		return -1;
	}
	vector->idx = 0;
	vector->size = size;
	return 0;
}

void vector_clean(vector_t *vector)
{
	vector->idx = 0;
}

int vector_reserve(vector_t *vector, size_t size)
{
	if (size <= vector->size) {
		return 0;
	}
	size_t new_size = bit_ceil(size);
	char *data_tmp = realloc(vector->data, new_size);
	if (unlikely(data_tmp == NULL)) {
		return -1;
	}
	vector->data = data_tmp;
	vector->size = new_size;

	return 0;
}

int vector_push_back(vector_t *vector, char *src, size_t size)
{
	int ret = vector_reserve(vector, vector->idx + size + 2);
	if (unlikely(ret != 0)) {
		return ret;
	}
	memcpy(vector->data + vector->idx, src, size);
	vector->idx += size;
	vector->data[vector->idx++] = '\0';
	vector->data[vector->idx] = '\0';
	return 0;
}

vector_it_t vector_begin(vector_t *vector)
{
	return vector->data;
}

void vector_deinit(vector_t *vector)
{
	free(vector->data);
	vector->data = NULL;
	vector->idx = 0;
	vector->size = 0;
}