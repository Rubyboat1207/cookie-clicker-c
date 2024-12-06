#include "utils.h"
#include<stdlib.h>
#include<memory.h>
#include<stdio.h>

void dyn_add_element(DynArr *dyn_array, void *element)
{
    printf("dynamically adding an element\n");
    if(dyn_array->length + 1 > dyn_array->real_length) {
        int new_size = (dyn_array->real_length == 0 ? 1 : dyn_array->real_length) * 2;
        void** new_array = malloc(new_size * sizeof(void*));

        if(new_array == NULL) {
            printf("memory allocation failure in dyn_add_element.\n");
            exit(2);
        }
        memcpy(new_array, dyn_array->arr, dyn_array->real_length * sizeof(void*));
        dyn_array->real_length = new_size;
        free(dyn_array->arr);
        dyn_array->arr = new_array;
    }
    
    dyn_array->arr[dyn_array->length++] = element;
    dyn_array->end = dyn_array->arr[dyn_array->length - 1];
}

void dyn_clear(DynArr *arr)
{
    arr->end = NULL;
    arr->length = 0;
    free(arr->arr);
    arr->arr = NULL;
    arr->real_length = 0;
}

DynArr *new_dynamic_array()
{
    DynArr* arr = calloc(1, sizeof(DynArr));

    return arr;
}