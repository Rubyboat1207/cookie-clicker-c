#pragma once
#include"factory.h"

typedef struct {
    char* name;
    int height;
} Tree;

FactoryType* init_tree();