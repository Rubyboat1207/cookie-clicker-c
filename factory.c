#include "factory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include <stdbool.h>

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

void ft_default_free(FactoryType* this) {
    printf("%s has not declared a \"free\" function", this->name);
}

char* ft_default_print_cost(FactoryType* this) {
    return get_large_number_text(this->get_cost(this) + 0.0);
}

Texture2D ft_default_texture = {0};
bool ft_default_loaded = false;

FactoryType* factory_new(
    char* name,
    char* abbreviation,
    double cookies_per_second,
    Texture2D* texture
) {
    if(texture == NULL) {
        if(!ft_default_loaded) {
            ft_default_texture = LoadTexture("./assets/missing.png");
            ft_default_loaded = true;
        }
        texture = &ft_default_texture;
    }
    FactoryType* this = calloc(1, sizeof(FactoryType));

    this->name = name;
    this->abbreviation = abbreviation;
    this->cookies_per_second = cookies_per_second;
    this->texture = texture;

    this->create_instance = ft_default_create_instance;
    this->get_cost = ft_default_get_cost;
    this->inspect = ft_default_inspect;
    this->free = ft_default_free;
    this->get_print_cost = ft_default_print_cost;
    this->instances = new_dynamic_array();

    return this;
}