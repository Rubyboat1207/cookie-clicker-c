#pragma once
#include"factory.h"

typedef struct {
    char* name;
    int age;
} Grandma;

FactoryType* init_grandma();