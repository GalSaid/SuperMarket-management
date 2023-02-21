#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "generalFucns.h"


float getLegalNumber(char * msg)
{
	float res;
	do {
		printf("%s\n", msg);
		scanf("%f", &res);
	} while (res <= 0);
	return res; 

}
int getLegalNumberWithTopLimit(char* msg, int topLimit)
{
	int res;
	do {
		printf("%s max %d\n", msg, topLimit);
		scanf("%d", &res);
	} while (res < 0 || res > topLimit);
	return res;
}

void generalArrayFunction(void* arr, int count, size_t typeSize, void(*f)(void*))
{
	int i;
	for (i = 0; i < count; i++)
		f((char*)arr + i * typeSize);
}