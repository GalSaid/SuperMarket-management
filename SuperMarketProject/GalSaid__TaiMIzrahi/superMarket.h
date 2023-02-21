#pragma once
#include "address.h"
#include "customer.h"
#include "product.h"
#include "listGen.h"

typedef enum { eSortByName, eSortByShopTimes, eSortByTotalSpend, eNotSorted, eNofSortType } eSortType;
static const char* typeSort[eNofSortType] = { "Name", "Shop times", "Money spent","Not sorted" };

typedef struct
{
	char* name;
	Address address;
	Customer* customersArr;
	eSortType	sortType;
	int numOfCustomers;
	LIST productsLst;
}SuperMarket;

int initSuperMarket(SuperMarket* pSuperMarket, const char* fileName, const char* customersFileName, int isCompress);
void printSuperMarket(const SuperMarket* pSuperMarket);
int addProduct(SuperMarket* pSuperMarket);
Product* checkIfProductExists(const SuperMarket* pSuperMarket ,const char* pBarcode);
int checkIfCustomerExists(const SuperMarket* pSuperMarket, const char* name);
int addCustomer(SuperMarket* pSuperMarket);
void printAllCustomers(const SuperMarket* pSuperMarket);
void printAllPrudocts(const SuperMarket* pSuperMarket);
int shoppingProcess(SuperMarket* pSuperMarket);
void printProductsByType(const SuperMarket* pSuperMarket);
void printCustomerShoppingCart(const SuperMarket* pSuperMarket);
Customer* getCustomer(SuperMarket* pSuperMarket);
int paymentProcess(SuperMarket* pSuperMarket);
int noCustomersInMarket(const SuperMarket* pSuperMarket);
int noProductsInMarket(const SuperMarket* pSuperMarket);
int insertProductToSortedList(LIST* lst, Product* pProduct);
int productCount(const SuperMarket* pSuperMarket);
void sortCustomers(SuperMarket* pMarket, eSortType eType);
void searchCustomer(SuperMarket* pMarket);
void handleCustomerStillShoppingAtExit(SuperMarket* pMarket);
void freeMarket(SuperMarket* pSuperMarket);
