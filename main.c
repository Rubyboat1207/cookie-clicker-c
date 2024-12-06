#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include"utils.h"
#include"factory.h"
#include"grandma.h"
#include"monkey.h"
#include"tree.h"
#include"events.h"

double cookies = 250;

void shop();

bool strEq(char* a, char* b) {
    return strcmp(a, b) == 0;
}

void clean_input_buffer(char* data) {
    size_t len = strlen(data);
    if (len > 0 && data[len - 1] == '\n') {
        data[len - 1] = '\0';
    } else {
        // If there's no newline, clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

time_t time_ms() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    long long milliseconds = ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;

    return milliseconds;
}

#define FACTORY_TYPE_COUNT 3
FactoryType* factory_types[FACTORY_TYPE_COUNT];

void init_factory_types() {
    factory_types[0] = init_grandma();
    factory_types[1] = init_monkey();
    factory_types[2] = init_tree();
}

int event_countdown = 0;
RandomEventType* active_event = NULL;

#define DATA_SIZE 5
int main() {
    char data[DATA_SIZE];
    time_t last_loop_time = time_ms();
    srand(last_loop_time);

    init_factory_types();

    while(1) {
        if(rand_range(0, 9000) == 5 && active_event == NULL) {
            active_event = get_random_event();
            active_event->on_triggered();
            event_countdown = 50;
        }
        fgets(data, DATA_SIZE, stdin);
        clean_input_buffer(data);
        if(active_event != NULL) {
            if(active_event->on_clicked(data, &cookies)) {
                active_event = NULL;
            }

            if(event_countdown == 0) {
                active_event->on_timeout();
                active_event = NULL;
            }
            event_countdown -= 1;
        }
        


        

        time_t cur_time = time_ms();

        int diff = cur_time - last_loop_time;
        last_loop_time = cur_time;



        for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
            FactoryType* type = factory_types[i];

            double count = type->instances->length * (diff / 1000.0) * type->cookies_per_second;

            if(count > 1) {
                printf("Your %ss have produced %d cookies.\n", type->name, (int) round(count));
            }

            cookies += count;
        }
        
        if(strEq(data, "")) {
            cookies += 1;
        }else if(strEq(data, "sh")) {
            shop();
        }else if(strEq(data, "q")) {
            break;
        }

        for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
            FactoryType* type = factory_types[i];

            if(strEq(data, type->abbreviation)) {
                type->inspect(type);
            }
        }

        printf("you have: %d cookies\n> ", (int) round(cookies));
    }

    for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
        factory_types[i]->free(factory_types[i]);
    }

    printf("all resources freed");

    return 0;
}

void shop() {
    while(1) {
        printf("---- SHOP ----\n\nCOOKIES: %d\n\n", (int) round(cookies));
        printf("0 - Exit\n");
        for(int i = 0; i < FACTORY_TYPE_COUNT; i++) {
            char* cost = factory_types[i]->get_print_cost(factory_types[i]);
            printf("%d - %s $%s (you own: %d)\n", i + 1, factory_types[i]->name, cost, factory_types[i]->instances->length);
            free(cost);
        }

        char val[10];
        fgets(val, 10, stdin);
        clean_input_buffer(val);

        if(strEq(val, "0")) {
            return;
        }

        int num = atoi(val);
        
        if(num <= FACTORY_TYPE_COUNT && num > 0) {
            FactoryType* factory_type = factory_types[num - 1];
            int cost = factory_type->get_cost(factory_type);
            if(cost > cookies) {
                printf("You dont have enough money.\n");
                continue;
            }
            cookies -= cost;
            factory_type->create_instance(factory_type);
        }
    }
    
}