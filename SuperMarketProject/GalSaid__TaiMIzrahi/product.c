#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "product.h"
#include "generalStrings.h"
#include "generalFucns.h"
#include "fileHelper.h"



void initProduct(Product * pProduct ,char* pValidBarcode)
{
	strcpy(pProduct->barcode, pValidBarcode);
	getchar();
	printf("Please enter product's name up to %d chars\n", NAME_LEN - 1);
	myGets(pProduct->name, NAME_LEN,stdin);
	pProduct->type = getTypeFromUser();
	pProduct->price = getLegalNumber("Enter product's price:");
	pProduct->amount = (int)getLegalNumber("Enter number of items to add:");
	
}

void printProduct(const void* v1)
{
	const Product* pProduct = (const Product*)v1;
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %10d\n", typeTitle[pProduct->type], pProduct->price, pProduct->amount);
}

void getValidBarcode(char* pBarcode)
{
	char temp[MAX_LENGTH]; //Uses a bigger string temporarily 
	int valid = 1;
	int digitsCounter = 0;
	printf("Enter product barcode ");
	do
	{
		printf("Code should be of %d length exactly\n", BARCODE_LEN - 1);
		printf("UPPER CASE letter and digits\n");
		printf("Must have 3 to 5 digits\n");
		printf("First and last chars must be UPPER CASE letter\n");
		printf("For example A12B40C\n");
		scanf("%s", temp);
	} while ((strlen(temp) != BARCODE_LEN - 1) || !ValidBarcode(temp)); 
	strcpy(pBarcode, temp); //Copies the valid barcode from temp string into pBarcode

}

int ValidBarcode(const char* pBarcode)
{
	int i, digitsCounter = 0;
	for (i = 0; i < BARCODE_LEN - 1; i++)
	{
		if (isdigit(pBarcode[i]))
			digitsCounter++; //Counts number of digits in pBarcode
		else if (digitsCounter > 5 || islower(pBarcode[i])) 
			return 0;
	}
	if (isdigit(pBarcode[0]) || isdigit(pBarcode[BARCODE_LEN - 1]) || digitsCounter < 3) //If barcode starts and ends with digit, or number of digits lower than 3
		return 0; //Not valid barcode
	return 1;
}

PoductType getTypeFromUser() //Gets type of product type from the user and return enum of it
{
	int val;
	printf("Please enter a product type\n");
	do {
		for (int i = 0; i < eNofType; i++)
			printf("Enter %d for %s\n", i, typeTitle[i]);
		scanf("%d", &val);
	} while (val < 0 || val >= eNofType);
	return (PoductType)val; //casting to productType
}

int compareProductByBarcode(const void* v1, const void* v2)
{
	const Product* p1 = (const Product*)v1;
	const Product* p2 = (const Product*)v2;
	return strcmp(p1->barcode,p2->barcode);
}

int	saveProductToFile(const Product* pProduct, FILE* fp, int isCompress)
{
	if (isCompress)
	{
		BYTE data[3];
		char* temp;
		temp = setValidChars(pProduct->barcode);
		data[0] = ((temp[0] << 2) | (temp[1] >> 4));
		data[1] = ((temp[1] << 4) | (temp[2] >> 2));
		data[2] = ((temp[2] << 6) | (temp[3]));
		if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
			return 0;
		int len = strlen(pProduct->name);
		data[0] = ((temp[4] << 2) | (temp[5] >> 4));
		data[1] = ((temp[5] << 4) | (temp[6] >> 2));
		data[2] = ((temp[6] << 6) | (len << 2) | (pProduct->type));
		if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
			return 0;
		if (!writeCharsToFile(pProduct->name, len, fp, "Error saving product's name to file\n"))
			return 0;
		data[0] = pProduct->amount;
		int priceNoPennis = (int)pProduct->price;
		int pennis = (pProduct->price - priceNoPennis) * 100;
		data[1] = ((pennis << 1) | (priceNoPennis >> 8));
		data[2] = priceNoPennis;
		if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
			return 0;
	}
	else
	{
		if (fwrite(pProduct, sizeof(Product), 1, fp) != 1)
		{
			puts("Error saving product to file\n");
			return 0;
		}
	}
	return 1;
}

char* setValidChars(const char* barcode)
{
	char validBar[BARCODE_LEN + 1];
	strcpy(validBar, barcode);
	int i = 0;
	while (validBar[i])
	{
		if (validBar[i] > '9')
			validBar[i] = validBar[i] - 'A' + 10;
		else
			validBar[i] = validBar[i] -= '0';
		i++;
	}
	return validBar;
}

int	loadProductFromFile(Product* pProduct, FILE* fp, int isCompress)
{
	if (isCompress)
	{
		BYTE data[3];
		if (fread(data, sizeof(BYTE), 3, fp) != 3)
		{
			puts("Error reading product from file\n");
			return 0;
		}
		int num;
		pProduct->barcode[0] = getValidChar(data[0] >> 2);
		pProduct->barcode[1] = getValidChar(((data[1] >> 4) | (data[0] & 0x3) << 4));
		pProduct->barcode[2] = getValidChar(((data[1] & 0xF) << 2) | (data[2] >> 6));
		pProduct->barcode[3] = getValidChar(data[2] & 0x3F);
		if (fread(data, sizeof(BYTE), 3, fp) != 3)
		{
			puts("Error reading product from file\n");
			return 0;
		}
		pProduct->barcode[4] = getValidChar(data[0] >> 2);
		pProduct->barcode[5] = getValidChar(((data[1] >> 4) | (data[0] & 0x3) << 4));
		pProduct->barcode[6] = getValidChar(((data[1] & 0xF) << 2) | (data[2] >> 6));
		int nameLen = ((data[2] >> 2) & 0xF);
		pProduct->type = data[2] & 0x3;
		if (!readCharsFromFile(pProduct->name, nameLen, fp, "Error reading product from file\n"))
			return 0;
		pProduct->name[nameLen] = '\0';
		if (fread(data, sizeof(BYTE), 3, fp) != 3)
		{
			puts("Error reading product from file\n");
			return 0;
		}
		pProduct->amount = data[0];
		int pennis = data[1] >> 1;
		int priceNoPennis = (data[1] & 0x1) << 8 | data[2];
		pProduct->price = priceNoPennis + (float)pennis / 100;


	}
	else
	{
		if (fread(pProduct, sizeof(Product), 1, fp) != 1)
		{
			puts("Error reading product from file\n");
			return 0;
		}
	}
	return 1;
}

char getValidChar(char data)
{
	if (data > 9)
		return data - 10 + 'A';
	return data + '0';
}

void freeProduct(void* v1)
{
	free(v1);
}

