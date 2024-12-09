#include"events.h"
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include"utils.h"
#include<math.h>

RandomEventType* new_event(void (*on_triggered)(), void(*on_timeout)(), bool(*on_clicked)(double*)) {
    RandomEventType* event = calloc(1, sizeof(RandomEventType));

    event->on_triggered = on_triggered;
    event->on_timeout = on_timeout;
    event->on_clicked = on_clicked;
}

#define EVENT_SIZE 1
RandomEventType* events[EVENT_SIZE];

void golden_cookie_on_triggered() {
    printf("A Golden cookie has appeared! Press c to capture it!\n");
}

void golden_cookie_on_timeout() {
    printf("The cookie has disappeared!\n");
}

bool golden_cookie_clicked(double* cookies) {
    if(strcmp("", "c") == 0) {
        int add = rand_range(0, 100) * 100 * fmin((*cookies / 10000), 1);
        *cookies += add;

        printf("You got %d cookies from a golden cookie!\n", add);

        return true;
    }
    return false;
}

void init_events() {
    events[0] = new_event(golden_cookie_on_triggered, golden_cookie_on_timeout, golden_cookie_clicked);
}

RandomEventType* get_random_event() {
    if(events[0] == NULL) {
        init_events();
    }

    return events[rand_range(0, EVENT_SIZE - 1)];
}