#pragma once

typedef struct
{
	char* street;
	char* city;
	int num;
}Address;

int initAddress(Address* pAdd);
int checkValidFormat(const char* str, char ch);
void printAddress(const Address* pAdd);
void freeAddress(Address* pAdd);
int	saveAddressToFile(const Address* pAdd, FILE* fp, int isCompress);
int	loadAddressFromFile(Address* pAdd, FILE* fp, int isCompress);
