#pragma once
#include "shoppingCart.h"
typedef struct
{
	char* name;
	int shopTimes;
	float totalSpend;
	ShoppingCart* shoppingCart;

}Customer;

int initCustomer(Customer* pCustomer);
void printCustomer(const Customer* pCustomer);
void pay(Customer* pCustomer);
int compareCustomerByName(const void* v1, const void* v2);
int compareCustomerByShopTimes(const void* v1, const void* v2);
int compareCustomerByTotalSpend(const void* v1, const void* v2);
int	saveCustomerToTextFile(const Customer* arr, int  customerCount, const char* customersFileName);
Customer* loadCustomerFromTextFile(const char* customersFileName, int* pCount);
void freeCustomer(void* v);
