#pragma once


typedef unsigned char BYTE;

float getLegalNumber(char * msg);
int getLegalNumberWithTopLimit(char* msg, int topLimit);
void    generalArrayFunction(void* arr, int count, size_t typeSize, void(*f)(void*));

