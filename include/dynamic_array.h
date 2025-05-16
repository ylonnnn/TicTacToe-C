#pragma once

#include <stdint.h> 

typedef struct array {
	void *data;

	int32_t size;
	int32_t cap;

	size_t e_size;
} array;

array array_create(int cap, size_t e_size);

void array_update_cap(array *arr, int cap);

void array_push_back(array *arr, const void *val);
void array_pop_back(array *arr);

void *array_get(array *arr, int index);
