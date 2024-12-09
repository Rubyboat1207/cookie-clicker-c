#pragma once
#include<time.h>
#include"utils.h"
#include"raylib.h"

typedef struct FactoryType{
    const char* name;
    char* abbreviation;
    int (*get_cost)(struct FactoryType*);
    void (*inspect)(struct FactoryType*);
    void* (*create_instance)(struct FactoryType*);
    char* (*get_print_cost)(struct FactoryType*);
    void (*free)(struct FactoryType*);
    DynArr* instances;
    double cookies_per_second;
    Texture2D* texture;
} FactoryType;

FactoryType* factory_new(
    char* name,
    char* abbreviation,
    double cookies_per_second,
    Texture2D* texture
);