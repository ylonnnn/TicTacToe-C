#include <stdlib.h>
#include <string.h>

#include "../include/dynamic_array.h"

array array_create(int cap, size_t e_size) {
	array arr = (array) { .data = malloc(cap * e_size), .size = 0, .cap = cap, .e_size = e_size };
	if (arr.data == NULL) {
		perror("Failed to allocate memory to dynamic array!");
		return arr;
	}	

	return arr;
}

void array_update_cap(array *arr, int cap) {
	if (cap < arr->size) 
		arr->size = cap;

	arr->cap = cap;	
	
	void *temp = realloc(arr->data, arr->cap * arr->e_size);
	if (temp == NULL) {
		perror("Failed to update the capacity of the dynamic array!");
		return;
	}

	arr->data = temp;
}

void array_push_back(array *arr, const void *val) {
	// If the size of the array is greater than or equal to the capacity
	// Update the capacity of the array
	if (arr->size >= arr->cap) 
		array_update_cap(arr, arr->cap * 2);

	// Create the target where the value will be stored
	void *target = (char *)arr->data + (arr->size * arr->e_size);
	memcpy(target, val, arr->e_size);

	arr->size++;
}

void array_pop_back(array *arr) {
	if (arr->size == 0)
		return;

	arr->size--;
}

void *array_get(array *arr, int index) {
	return index >= 0 && index < arr->size ? (char *)arr->data + (index * arr->e_size) : NULL;
}
