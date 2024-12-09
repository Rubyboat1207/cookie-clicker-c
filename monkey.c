#include"monkey.h"
#include"factory.h"
#include<string.h>
#include<stdlib.h>
#include"utils.h"
#include<stdio.h>
#include<math.h>

#define mk_names 5

const char* monkey_prefix[] = {
    "ooh",
    "geeb",
    "gal",
    "eeh",
    "ooooo"
};

const char* monkey_suffix[] = {
    "ooh",
    "oob",
    "oon",
    "aaah",
    "ehehe"
};

#define mk_activity_count 13
const char* monkey_activities[] = {
    "eating bananas",
    "throwing their own poop",
    "staring at other monkeys",
    "swinging on the monkey bars",
    "chasing girl monkeys",
    "chasing boy monkeys",
    "rasing a child",
    "swimming in the lake",
    "swimming in their own poop",
    "mauling toddlers",
    "getting shot",
    "eating dirt",
    "curing cancer"
};

#define mk_times 8
const char* monkey_times[] = {
    "during the afternoon",
    "at midnight",
    "in the morning",
    "at lunch",
    "while everyone else is asleep",
    "at dinner time",
    "while pooping",
    "after almost dying"
};

void* mk_create_instance(FactoryType* this) {
    Monkey* monkey = malloc(sizeof(Monkey));

    if(monkey == NULL) {
        printf("failed to init monkey\n");
        exit(2);
    }

    monkey->name = calloc(1, sizeof(char) * 10);
    strcat(monkey->name, monkey_prefix[rand_range(0, mk_names - 1)]);
    strcat(monkey->name, monkey_suffix[rand_range(0, mk_names - 1)]);

    monkey->activity = monkey_activities[rand_range(0, mk_activity_count - 1)];
    monkey->time = monkey_times[rand_range(0, mk_times - 1)];

    if(this->instances->length == 0) {
        monkey->with = "a monkey from a different school";
    }else {
        monkey->with = ((Monkey*) this->instances->arr[rand_range(0, this->instances->length - 1)])->name;
    }

    if(this->instances->length == 5) {
        ((Monkey*) this->instances->arr[0])->with = monkey->name;
    }
    

    dyn_add_element(this->instances, monkey);
}

int mk_get_cost(FactoryType* this) {
    return 800 * pow(1.5, this->instances->length);
}

void mk_inspect(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Monkey* monkey = this->instances->arr[i];
        printf("%s (favorite activity: %s with %s %s)\n",
            monkey->name,
            monkey->activity, 
            monkey->with, 
            monkey->time
        );
    }
}

void mk_free(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Monkey* mk = this->instances->arr[i];
        free(mk->name);
        free(mk);
    }
    dyn_clear(this->instances);

    free(this);
}

FactoryType* init_monkey() {
    FactoryType* monkey = factory_new("Typewriter Monkey", "mk", 1, NULL);

    monkey->create_instance = mk_create_instance;
    monkey->get_cost = mk_get_cost;
    monkey->inspect = mk_inspect;
    monkey->free = mk_free;

    return monkey;
}