#pragma once

#include <stdbool.h>

typedef char* vector_it_t;

vector_it_t vector_it_next(vector_it_t it);

bool vector_it_end(vector_it_t it);