#include "factory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void* ft_default_create_instance(FactoryType* this) {
    printf("%s has not declared a \"create_instance\" function", this->name);
}

int ft_default_get_cost(FactoryType* this) {
    printf("%s has not declared a \"get_cost\" function", this->name);
    return 0;
}

void ft_default_inspect(FactoryType* this) {
    printf("%s has not declared a \"inspect\" function", this->name);
}

char* ft_default_get_print_cost(FactoryType* this) {
    int cost = this->get_cost(this);

    // OH MY GOD REMEMBER TO FREE THIS LATER <3
    char* buf = calloc(1, 10);

    if(cost < 1000) {
        sprintf_s(buf, 10, "%d", cost);
    }else if(cost < 1000000) {
        sprintf_s(buf, 10, "%.2fk", cost/1000.0);
    }else if(cost < 1000000000) {
        sprintf_s(buf, 10, "%.2fm", round(cost * 100000)/1000000);
    }else {
        strcpy(buf, "a lot");
    }

    return buf;

}

void ft_default_free(FactoryType* this) {
    printf("%s has not declared a \"free\" function", this->name);
}

FactoryType* factory_new(
    char* name,
    char* abbreviation,
    double cookies_per_second
) {
    FactoryType* this = calloc(1, sizeof(FactoryType));

    this->name = name;
    this->abbreviation = abbreviation;
    this->cookies_per_second = cookies_per_second;

    this->create_instance = ft_default_create_instance;
    this->get_cost = ft_default_get_cost;
    this->inspect = ft_default_inspect;
    this->get_print_cost = ft_default_get_print_cost;
    this->free = ft_default_free;
    this->instances = new_dynamic_array();

    return this;
}