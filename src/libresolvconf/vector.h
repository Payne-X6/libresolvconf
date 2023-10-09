#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "vector_it.h"

typedef struct {
	char *data;
	size_t idx;
	size_t size;
} vector_t;

int vector_init(vector_t *vector, size_t init_size);

int vector_reserve(vector_t *vector, size_t size);

int vector_push_back(vector_t *vector, char *src, size_t size);

bool vector_is_empty(vector_t *vector);

vector_it_t vector_begin(vector_t *vector);

void vector_clear(vector_t *vector);
void vector_deinit(vector_t *vector);