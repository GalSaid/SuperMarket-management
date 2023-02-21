#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "generalFucns.h"
#include "fileHelper.h"
#include "SuperFile.h"
#include "myMacros.h"


int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName, const char* customersFileName, int isCompress)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp, "Error open supermarket file to write\n");
	int count = productCount(pMarket);
	if (isCompress)
	{
		int nameLen = strlen(pMarket->name);
		BYTE data[2];
		data[0] = (count >> 2);
		data[1] = (count << 6 | nameLen);
		if (fwrite(data, sizeof(BYTE), 2, fp) != 2)
			CLOSE_RETURN_0(fp);
		if (!writeCharsToFile(pMarket->name, nameLen, fp, "Error write supermarket name\n"))
			CLOSE_RETURN_0(fp);
	}
	else
		if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n", isCompress))
			CLOSE_RETURN_0(fp);

	if (!saveAddressToFile(&pMarket->address, fp, isCompress))
		CLOSE_RETURN_0(fp);

	if (!isCompress)
		if (!writeIntToFile(count, fp, "Error write product count\n"))
			CLOSE_RETURN_0(fp);

	Product* pTemp;
	NODE* pN = pMarket->productsLst.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!saveProductToFile(pTemp, fp, isCompress))
			CLOSE_RETURN_0(fp);
		pN = pN->next;
	}

	fclose(fp);

	saveCustomerToTextFile(pMarket->customersArr, pMarket->numOfCustomers, customersFileName);

	return 1;
}

int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName, const char* customersFileName, int isCompress)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Error open company file\n");
	int count;
	if (isCompress)
	{
		int len;
		BYTE data[2];
		if (fread(data, sizeof(BYTE), 2, fp) != 2)
		{
			printf("Error reading from compress file\n");
			CLOSE_RETURN_0(fp);
		}
		count = (data[0] << 2) | (data[1] >> 6);
		len = data[1] & 0x3F;
		pMarket->name = (char*)calloc(len + 1, sizeof(char));
		if (!pMarket->name)
		{
			printf("Memory allocation failed\n");
			CLOSE_RETURN_0(fp);
		}
		if (!readCharsFromFile(pMarket->name, len, fp, "Error reading supermarket name\n"))
			CLOSE_RETURN_0(fp);
	}
	else
	{
		pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n", isCompress);
		if (!pMarket->name)
			CLOSE_RETURN_0(fp);
	}
	if (!loadAddressFromFile(&pMarket->address, fp, isCompress))
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	if (!isCompress)
	{
		if (!readIntFromFile(&count, fp, "Error reading product count\n"))
			FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	}
	if (count > 0)
	{
		Product* pTemp;
		for (int i = 0; i < count; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productsLst, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			if (!loadProductFromFile(pTemp, fp, isCompress))
			{
				L_free(&pMarket->productsLst, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
			if (!insertProductToSortedList(&pMarket->productsLst, pTemp))
			{
				L_free(&pMarket->productsLst, freeProduct);
				FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
			}
		}
	}
	fclose(fp);

	pMarket->customersArr = loadCustomerFromTextFile(customersFileName, &pMarket->numOfCustomers);
	if (!pMarket->customersArr)
		return 0;

	return	1;

}
