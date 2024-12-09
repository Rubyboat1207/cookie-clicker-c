#include"grandma.h"
#include"factory.h"
#include<string.h>
#include<stdlib.h>
#include"utils.h"
#include<stdio.h>
#include<math.h>

#define gm_names 7
const char* grandma_prefix[] = {
    "ger",
    "del",
    "she",
    "mel",
    "maur",
    "turt",
    "lens",
};

const char* grandma_suffix[] = {
    "trude",
    "iese",
    "han",
    "ody",
    "neise",
    "le",
    "iggy"
};

void* gm_create_instance(FactoryType* this) {
    Grandma* gm = malloc(sizeof(Grandma));

    if(gm == NULL) {
        printf("failed to init grandma\n");
        exit(2);
    }

    gm->age = rand_range(0, 20) + 70;
    gm->name = calloc(1, sizeof(char) * 10);
    strcat(gm->name, grandma_prefix[rand_range(0, gm_names - 1)]);
    strcat(gm->name, grandma_suffix[rand_range(0, gm_names - 1)]);

    dyn_add_element(this->instances, gm);
}

int gm_get_cost(FactoryType* this) {
    return 100 * pow(1.5, this->instances->length);
}

void gm_inspect(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Grandma* grandma = this->instances->arr[i];
        printf("%s (age %d)\n", grandma->name, grandma->age);
    }
}

void gm_free(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Grandma* gm = this->instances->arr[i];
        free(gm->name);
        free(gm);
    }
    dyn_clear(this->instances);

    free(this);
}

Texture2D gm_texture;

FactoryType* init_grandma() {
    gm_texture = LoadTexture("./assets/Grandma.png");
    FactoryType* grandmas = factory_new("Grandma", "gm", 0.2, &gm_texture);

    grandmas->create_instance = gm_create_instance;
    grandmas->get_cost = gm_get_cost;
    grandmas->inspect = gm_inspect;
    grandmas->free = gm_free;

    return grandmas;
}