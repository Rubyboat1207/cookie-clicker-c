#pragma once
#define rand_range(min, max) rand() % (max + 1 - min)

typedef struct DynArr {
    void** arr;
    int length;
    int real_length;
    void* end;
} DynArr;

void dyn_add_element(DynArr* arr, void* element);
void dyn_clear(DynArr* arr);
DynArr* new_dynamic_array();