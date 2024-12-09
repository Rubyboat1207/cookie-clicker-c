#include "utils.h"
#include<stdlib.h>
#include<memory.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>

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


time_t time_ms() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long long milliseconds = ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;

    return milliseconds;
}


void etb_add(EventTimeBuffer *rtb)
{
    for(int i = 0; i < rtb->length; i++) {
        if(rtb->times[i] == 0) {
            rtb->times[i] = time_ms();
            return;
        }
    }

    time_t now = time_ms();
    for(int i = 0; i < rtb->length; i++) {
        if(now - rtb->times[i] > rtb->event_timeout) {
            rtb->times[i] = time_ms();
            return;
        }
    }

    printf("WARN: No room in the event time buffer.\n");
}

int etb_events_count(EventTimeBuffer *rtb)
{
    int count = 0;
    time_t now = time_ms();
    for(int i = 0; i < rtb->length; i++) {
        if(rtb->times[i] != 0) {
            if(now - rtb->times[i] < rtb->event_timeout) {
                count++;
            }else {
                rtb->times[i] = 0;
            }
        }
    }

    return count;
}

EventTimeBuffer *new_rolling_time_buffer(int length, long long event_timeout)
{
    EventTimeBuffer* rtb = calloc(1, sizeof(EventTimeBuffer));
    rtb->times = calloc(length, sizeof(long long));
    rtb->length = length;
    rtb->event_timeout = event_timeout;

    return rtb;
}

char *get_large_number_text(double num)
{
    // OH MY GOD REMEMBER TO FREE THIS LATER <3
    char* buf = calloc(1, 10);

    if(num < 1000) {
        sprintf_s(buf, 10, "%d", (int) num);
    }else if(num < 1000000) {
        sprintf_s(buf, 10, "%.2fk", num/1000.0);
    }else if(num < 1000000000) {
        sprintf_s(buf, 10, "%.2fm", round(num * 100000)/1000000);
    }else {
        strcpy(buf, "a lot");
    }

    return buf;
}

bool inside_box(Vector2 pos, Vector2 topLeft, Vector2 scale) {
    return pos.x > topLeft.x && pos.x < topLeft.x + scale.x && pos.y > topLeft.y && pos.y < topLeft.y + scale.y;
}