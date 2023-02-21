#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "generalStrings.h"
#include "shoppingCart.h"
#include "fileHelper.h"


int initCustomer(Customer * pCustomer)
{
	do {
		printf("Enter name of customer\n");
		pCustomer->name = getStrExactLength(); //Return dynamic allocation of name
		if (!(pCustomer->name)) //Check if memory allocation failed
			return 0;
	} while (checkEmptyString(pCustomer->name));
	pCustomer->shoppingCart = NULL; //init empty shopping cart 
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	return 1;
}

void printCustomer(const Customer * pCustomer)
{
	printf("Customer: %-20s\t", pCustomer->name);
	char msg[MAX_LENGTH];
	sprintf(msg, "has shopped %-2d times spent : %.2f IS\t", pCustomer->shopTimes, pCustomer->totalSpend);
	printf("%-50s",msg);
	if (isEmptyShoppingCart(pCustomer->shoppingCart))
		printf("Shopping cart is empty\n");
	else
		printf("Doing shopping now!!\n");


}

void pay(Customer* pCustomer)
{
	if (!pCustomer->shoppingCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	pCustomer->totalSpend += printShoppingCart(pCustomer->shoppingCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	pCustomer->shopTimes++;
	freeShoppingCart(pCustomer->shoppingCart);
	free(pCustomer->shoppingCart);
	pCustomer->shoppingCart = NULL; //After free the shopping cart , the customer dont have cart (make it NULL)
}


int compareCustomerByName(const void* v1, const void* v2)
{
	const Customer* c1 = (const Customer*)v1;
	const Customer* c2 = (const Customer*)v2;
	return strcmp(c1->name, c2->name);
}

int compareCustomerByShopTimes(const void* v1, const void* v2)
{
	const Customer* c1 = (const Customer*)v1;
	const Customer* c2 = (const Customer*)v2;
	return c1->shopTimes - c2->shopTimes;
}

int compareCustomerByTotalSpend(const void* v1, const void* v2)
{
	const Customer* c1 = (const Customer*)v1;
	const Customer* c2 = (const Customer*)v2;
	if (c1->totalSpend - c2->totalSpend > 0)
		return 1;
	else if (c1->totalSpend - c2->totalSpend < 0)
		return -1;
	return 0;
}


int	saveCustomerToTextFile(const Customer* customerArr, int  customerCount, const char* customersFileName)
{
	FILE* fp;

	fp = fopen(customersFileName, "w");
	if (!fp) {
		printf("Error open customers file to write\n");
		return 0;
	}

	fprintf(fp, "%d\n", customerCount);
	for (int i = 0; i < customerCount; i++)
	{
		fprintf(fp, "%s\n%d %.2f\n", customerArr[i].name, customerArr[i].shopTimes,
			customerArr[i].totalSpend);
	}
	fclose(fp);
	return 1;
}

Customer* loadCustomerFromTextFile(const char* customersFileName, int* pCount)
{
	FILE* fp;

	fp = fopen(customersFileName, "r");
	if (!fp) {
		printf("Error open customers file to write\n");
		return NULL;
	}
	Customer* customerArr = NULL;
	int customerCount;
	fscanf(fp, "%d\n", &customerCount);
	if (customerCount > 0)
	{
		customerArr = (Customer*)calloc(customerCount, sizeof(Customer)); //cart will be NULL!!!
		if (!customerArr)
		{
			fclose(fp);
			return NULL;
		}
		for (int i = 0; i < customerCount; i++)
		{
			customerArr[i].name = readDynStringFromTextFile(fp);
			if (!customerArr[i].name)
			{
				for (int j = 0; j < i; j++)
					free(customerArr[j].name);
				free(customerArr);
				fclose(fp);
				return NULL;
			}
			fscanf(fp, "%d %f", &customerArr[i].shopTimes, &customerArr[i].totalSpend);
			fgetc(fp); //clean the enter
		}
	}

	fclose(fp);
	*pCount = customerCount;
	return customerArr;
}



void freeCustomer(void* v)
{
	Customer* pCust = (Customer*)v;
	if (pCust->shoppingCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}