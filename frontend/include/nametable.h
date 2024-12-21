#ifndef NAMETABLE_H
#define NAMETABLE_H

#include "syntax_tree.h"

const size_t BASIC_NAMETABLE_CAPACITY = 4;

struct Name{
    Identifier  name;
    size_t      index;
    bool        is_init;
};

struct Nametable{
    Name*       names;
    size_t      size;
    size_t      capacity;
};

Nametable* nametableCtor();
bool isNameInNametable(Nametable* nt, Identifier name);
bool isInit(Nametable* nt, Identifier name);
void initName(Nametable* nt, Identifier name);
void addNewName(Nametable* nt, Identifier name, bool init_state);
size_t getIndex(Nametable* nt, Identifier name);
void resizeNametable(Nametable* nt);


#endif
