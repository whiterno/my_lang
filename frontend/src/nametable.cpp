#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "nametable.h"

Nametable* nametableCtor(Nametable* parent){
    Nametable* nt = NULL;

    nt           = (Nametable*)calloc(1, sizeof(Nametable));
    nt->parent   = parent;
    nt->capacity = BASIC_NAMETABLE_CAPACITY;
    nt->size     = 0;
    nt->names    = (Name*)calloc(nt->capacity, sizeof(Name));
    if (parent) nt->scope_level = parent->scope_level + 1;
    else        nt->scope_level = 0;

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
    nt->names[nt->size].index = nt->size + nt->scope_level * 1000;
    nt->names[nt->size].line  = line;
    nt->names[nt->size].is_init = init_state;

    nt->size++;
}

void addFuncName(Nametable* nt, Identifier name, size_t line, bool init_state){
    assert(nt);

    while (nt->parent) nt = nt->parent;

    if (isNameInGlobal(nt, name)) return;

    if (nt->size == nt->capacity){
        resizeNametable(nt);
    }

    memcpy(&nt->names[nt->size].name, &name, sizeof(Identifier));
    nt->names[nt->size].index = nt->size + nt->scope_level * 1000;
    nt->names[nt->size].line  = line;
    nt->names[nt->size].is_init = init_state;

    nt->size++;
}

bool isNameInGlobal(Nametable* nt, Identifier name){
    assert(nt);

    if (nt->parent) return isNameInGlobal(nt->parent, name);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0) return true;
    }

    return false;
}


size_t getIndex(Nametable* nt, Identifier name){
    assert(nt);

    for (size_t i = 0; i < nt->size; i++){
        if (strcmp(nt->names[i].name.id, name.id) == 0){
            return nt->names[i].index;
        }
    }

    if(!nt->parent) return -1;

    return getIndex(nt->parent, name);
}

void resizeNametable(Nametable* nt){
    assert(nt);

    nt->capacity = nt->capacity * 2;
    nt->names = (Name*)realloc(nt->names, sizeof(Name) * nt->capacity);
}

void nametableDtor(Nametable* nt){
    free(nt->names);
    free(nt);
}
