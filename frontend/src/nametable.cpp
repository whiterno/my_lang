#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "nametable.h"

Nametable* nametableCtor(){
    Nametable* nt = NULL;

    nt           = (Nametable*)calloc(1, sizeof(Nametable));
    nt->capacity = BASIC_NAMETABLE_CAPACITY;
    nt->size     = 0;
    nt->names    = (Name*)calloc(nt->capacity, sizeof(Name));

    return nt;
}

bool isNameInNametable(Nametable* nt, Identifier name){
    assert(nt);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0) return true;
    }

    return false;
}

bool isInit(Nametable* nt, Identifier name){
    assert(nt);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0){
            return nt->names[i].is_init;
        }
    }

    return false;
}

void initName(Nametable* nt, Identifier name){
    assert(nt);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0){
            nt->names[i].is_init = true;
        }
    }
}

void addNewName(Nametable* nt, Identifier name, size_t line, bool init_state){
    assert(nt);

    if (nt->size == nt->capacity){
        resizeNametable(nt);
    }

    memcpy(&nt->names[nt->size].name, &name, sizeof(Identifier));
    nt->names[nt->size].index = nt->size;
    nt->names[nt->size].line  = line;
    nt->names[nt->size].is_init = init_state;
}

size_t getIndex(Nametable* nt, Identifier name){
    assert(nt);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0){
            return nt->names[i].index;
        }
    }
}

void resizeNametable(Nametable* nt){
    assert(nt);

    nt->capacity = nt->capacity * 2;
    nt->names = (Name*)realloc(nt->names, sizeof(Name) * nt->capacity);
}
