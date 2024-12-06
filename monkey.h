#pragma once
#include"factory.h"

typedef struct {
    char* name;
    const char* activity;
    char* with;
    const char* time;
} Monkey;

FactoryType* init_monkey();