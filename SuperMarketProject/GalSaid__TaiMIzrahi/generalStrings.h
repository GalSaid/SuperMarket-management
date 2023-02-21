#pragma once

#define MAX_LENGTH 255

char* getStrExactLength();
char* myGets(char* buf, int size,FILE* fp);
char* changeSpaces(char* str);
int numOfWords(char* str);
void splitWords(char* tempAddress, char wordsArray[][MAX_LENGTH], int numOfWords, char* delimiters);
int	checkEmptyString(char* str);
char* getDynStr(char* str);
void printMessage(const char* str, ...);