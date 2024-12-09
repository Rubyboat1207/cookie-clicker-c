#pragma once
#include<stdbool.h>
#include<time.h>
#include"raylib.h"
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

typedef struct EventTimeBuffer {
    long long* times;
    int length;
    long long event_timeout;
} EventTimeBuffer;

void etb_add(EventTimeBuffer* rtb);
int etb_events_count(EventTimeBuffer* rtb);
EventTimeBuffer* new_rolling_time_buffer(int length, long long event_timeout);

/// @brief formats the cost into a string. 
/// @return a string pointer that must be discarded after us
char* get_large_number_text(double num);
bool inside_box(Vector2 pos, Vector2 topLeft, Vector2 scale);
time_t time_ms();