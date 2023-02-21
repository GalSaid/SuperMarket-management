#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "address.h"
#include "generalStrings.h"
#include "generalFucns.h"
#include "fileHelper.h"


#define NUM_OF_ELEMENTS 3 //elements of the struct address

int initAddress(Address * pAdd)
{
	int fCountinue = 1;
	char tempAddress[MAX_LENGTH];
	do
	{
		printf("Enter address data\n");
		printf("Format: street#house number#city\n");
		printf("street and city can have spaces\n");
		myGets(tempAddress, MAX_LENGTH,stdin); //Get address from user
		if (!checkValidFormat(tempAddress, '#')) //Check if the input address is valid by the format
			printf("!!!Incorrect address format!!!\n"); 
		else
			fCountinue = 0;
	} while (fCountinue);
	char res[NUM_OF_ELEMENTS][MAX_LENGTH]; //Static array of words
	splitWords(tempAddress, res,NUM_OF_ELEMENTS,"#"); //Split the address into array of words;
	pAdd->street = changeSpaces(res[0]); //Send the place in the array that refer to the street , and return valid street
	if (!pAdd->street) //The func include's _strdup . check if the dynamic allocation failed.
	{
		printf("Memory allocation failed!\n");
		return 0;
	}
	sscanf(res[1], "%d", &pAdd->num);
	pAdd->city = changeSpaces(res[2]);
	if (!pAdd->city) //The func include's _strdup . check if the dynamic allocation failed.
	{
		printf("Memory allocation failed!\n");
		free(pAdd->street);
		return 0;
	}
	return 1;
}

int checkValidFormat(const char* str, char ch) //Check if the input address from the user is according to the format
{
	int counter = 0;
	while (*str)
	{
		if (*str == ch)
			counter++; //Counting the number of the '#'
		else if (counter == 1) //Check if entered valid house number
		{
			if (!isdigit(*str))//Check if char is number
			{
				return 0;//Return 0 if not number
			}
		}
		str++;
	}

	return counter == 2; //Return 1 if valid , 0 if not
}

void printAddress(const Address * pAdd)
{
	printf("Address: ");
	printf("%s %d, %s\n", pAdd->street, pAdd->num, pAdd->city);
}

int saveAddressToFile(const Address* pAdd, FILE* fp, int isCompress)
{
	if (isCompress)
	{
		BYTE data = pAdd->num;
		if (fwrite(&data, sizeof(BYTE), 1, fp) != 1)
			return 0;
	}
	else
		if (!writeIntToFile(pAdd->num, fp, "error write house number\n"))
			return 0;

	if (!writeStringToFile(pAdd->street, fp, "error write street name\n", isCompress))
		return 0;

	if (!writeStringToFile(pAdd->city, fp, "error write city name\n", isCompress))
		return 0;
	return 1;
}

int loadAddressFromFile(Address* pAdd, FILE* fp, int isCompress)
{
	if (isCompress)
	{
		BYTE data;
		if (fread(&data, sizeof(BYTE), 1, fp) != 1)
		{
			printf("error read house number\n");
			return 0;
		}
		pAdd->num = data;

	}
	else
		if (!readIntFromFile(&pAdd->num, fp, "error read house number\n"))
			return 0;

	pAdd->street = readStringFromFile(fp, "error read street name\n", isCompress);
	if (!pAdd->street)
		return 0;

	pAdd->city = readStringFromFile(fp, "error read city name\n", isCompress);
	if (!pAdd->city)
	{
		free(pAdd->street);
		return 0;
	}

	return 1;
}

void freeAddress(Address* pAdd)
{
	free(pAdd->street);
	free(pAdd->city);
	
}



