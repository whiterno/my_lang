#include <stdio.h>

#include "lexer.h"

int main(){
    Token* token  = lexer("test1.lan");
    printTokens(token);
}
