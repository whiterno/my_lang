#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#include "utils.h"

#include "syntax_tree.h"

int readFile(const char* filename, char** txt){
    assert(filename);
    assert(txt);

    size_t file_size = fileSize(filename);
    int fp = open(filename, O_RDONLY);

    if (fp == -1){
        return FILE_NOT_OPEN;
    }

    (*txt) = (char*)calloc(file_size + 1, sizeof(char));
    read(fp, *txt, file_size);
    close(fp);

    (*txt)[file_size] = '\0';

    return NO_ERROR;
}

size_t fileSize(const char* filename){
    assert(filename);

    struct stat file_stat;
    stat(filename, &file_stat);
    return file_stat.st_size;
}

void fprintfnTab(FILE* fp, int n){
    for (int i = 0; i < n; i++){
        fputc('\t', fp);
    }
}

void clearBuffer(){
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF){}
}

bool cmpf(double num1, double num2){
    if (fabs(num1 - num2) < 0.001){
        return 1;
    }

    return 0;
}

void skipSpaces(char** string, size_t* line){
    while (isspace(**string)){
        if (**string == '\n') (*line)++;

        (*string)++;
    }

    if (**string == '/' && *(*string + 1) == '/'){
        skipComments(string, line);
    }
}

void skipComments(char** string, size_t* line){
    while (**string != '\n'){
        (*string)++;
    }

    skipSpaces(string, line);
}
