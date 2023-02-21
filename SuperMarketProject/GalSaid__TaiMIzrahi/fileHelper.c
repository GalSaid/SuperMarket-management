#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHelper.h"
#include "generalStrings.h"


int	 writeStringToFile(const char* str, FILE* fp, const char* msg, int isCompress)
{
	int length = (int)strlen(str);
	if (!isCompress)
	{
		length++;
	}
	if (!writeIntToFile(length, fp, msg))
		return 0;
	if (!writeCharsToFile(str, length, fp, msg))
		return 0;


	return 1;
}

int	 writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

char* readStringFromFile(FILE* fp, const char* msg, int isCompress)
{
	char* str;
	int length;
	if (!readIntFromFile(&length, fp, msg))
		return NULL;
	if (isCompress)
		str = (char*)calloc((length + 1), sizeof(char));
	else
		str = (char*)calloc((length), sizeof(char));
	if (!str)
	{
		puts(msg);
		return NULL;
	}
	if (fread(str, sizeof(char), length, fp) != length)
	{
		free(str);
		puts(msg);
		return NULL;
	}
	return str;
}

int readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg)
{
	int len;
	if (!readIntFromFile(&len, fp, msg))
		return 0;

	if (!readCharsFromFile(arr, len, fp, msg))
		return 0;

	return 1;
}

int	 readCharsFromFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;

}


char* readDynStringFromTextFile(FILE* fp)
{
	char temp[MAX_LENGTH];
	myGets(temp, sizeof(temp), fp);
	return getDynStr(temp);
}

