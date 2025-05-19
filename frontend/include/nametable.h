#ifndef NAMETABLE_H
#define NAMETABLE_H

#include "syntax_tree.h"

const size_t BASIC_NAMETABLE_CAPACITY = 4;

struct Name{
    Identifier  name;
    size_t      index;
    size_t      line;
    bool        is_init;
};

struct Nametable{
    Name*       names;
    Nametable*  parent;
    size_t      size;
    size_t      capacity;
    size_t      scope_level;
};

Nametable* nametableCtor(Nametable* parent);
bool isNameInNametable(Nametable* nt, Identifier name);
bool isNameInGlobal(Nametable* nt, Identifier name);
bool isInit(Nametable* nt, Identifier name);
void initName(Nametable* nt, Identifier name);
void addNewName(Nametable* nt, Identifier name, size_t line, bool init_state);
void addFuncName(Nametable* nt, Identifier name, size_t line, bool init_state);
size_t getIndex(Nametable* nt, Identifier name);
void resizeNametable(Nametable* nt);
void nametableDtor(Nametable* nt);

#endif
