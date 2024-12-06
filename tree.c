#include"tree.h"
#include"factory.h"
#include<string.h>
#include<stdlib.h>
#include"utils.h"
#include<stdio.h>
#include<math.h>

#define tr_names 4

char* tr_prefix[] = {
    "The Great ",
    "Once",
    "The Almighty ",
    "Chipicious ",
    "GNU/",
    "The Holy "
};

char* tr_suffix[] = {
    "cocoak tree",
    "wheat tree",
    "caoco tree",
    "iTree",
    "linux Tree",
    "tree of evil",
};

void* tr_create_instance(FactoryType* this) {
    Tree* tree = malloc(sizeof(Tree));

    if(tree == NULL) {
        printf("failed to init tree\n");
        exit(2);
    }

    tree->height = (int) round(pow(1.4, rand_range(0, 100) * 0.02 -3) * 50 + 3);
    tree->name = calloc(1, sizeof(char) * 50);

    strcat(tree->name, tr_prefix[rand_range(0, tr_names - 1)]);
    strcat(tree->name, tr_suffix[rand_range(0, tr_names - 1)]);

    dyn_add_element(this->instances, tree);
}

int tr_get_cost(FactoryType* this) {
    return 2500 * pow(1.5, this->instances->length);
}

void tr_inspect(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Tree* tree = this->instances->arr[i];
        printf("%s (%d ft)\n", tree->name, tree->height);
    }
}

void tr_free(FactoryType* this) {
    for(int i = 0; i < this->instances->length; i++) {
        Tree* tr = this->instances->arr[i];
        free(tr->name);
        free(tr);
    }
    dyn_clear(this->instances);

    free(this);
}

FactoryType* init_tree() {
    FactoryType* tree = factory_new("Cookie Tree", "tr", 5);

    tree->create_instance = tr_create_instance;
    tree->get_cost = tr_get_cost;
    tree->inspect = tr_inspect;
    tree->free = tr_free;

    return tree;
}