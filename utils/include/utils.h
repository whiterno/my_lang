#ifndef UTILS_H
#define UTILS_H

int readFile(const char* filename, char** txt);
size_t fileSize(const char* filename);
void fprintfnTab(FILE* fp, int n);
void clearBuffer();
bool cmpf(double num1, double num2);
void skipSpaces(char** string, size_t* line);
void skipComments(char** string, size_t* line);

#endif
