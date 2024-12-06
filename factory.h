#pragma once
#include<time.h>
#include"utils.h"

typedef struct FactoryType{
    const char* name;
    char* abbreviation;
    int (*get_cost)(struct FactoryType*);
    void (*inspect)(struct FactoryType*);
    void* (*create_instance)(struct FactoryType*);
    /// @brief formats the cost into a string. 
    /// @return a string pointer that must be discarded after use
    char* (*get_print_cost)(struct FactoryType*);
    void (*free)(struct FactoryType*);
    DynArr* instances;
    double cookies_per_second;
} FactoryType;

FactoryType* factory_new(
    char* name,
    char* abbreviation,
    double cookies_per_second
);