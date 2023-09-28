#include "vector_it.h"

void vector_it_next(vector_it_t *it)
{
	for (; (**it) != '\0'; ++(*it)) {}
	++(*it);
}

bool vector_it_end(vector_it_t it)
{
	return *it == '\0';
}