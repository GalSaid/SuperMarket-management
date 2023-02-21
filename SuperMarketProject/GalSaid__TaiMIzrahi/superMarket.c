#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "superMarket.h"
#include "generalStrings.h"
#include "generalFucns.h"
#include "superFile.h"
#include "myMacros.h"
#define NOT_EXIST -1

const int (*compare[])(const void*, const void*) = { compareCustomerByName, compareCustomerByShopTimes, compareCustomerByTotalSpend }; //Arr of pointers of compare functions

int initSuperMarket(SuperMarket * pSuperMarket,const char* fileName, const char* customersFileName, int isCompress)
{
	pSuperMarket->numOfCustomers = 0;
	pSuperMarket->customersArr = NULL;
	pSuperMarket->sortType = eNotSorted;
	L_init(&pSuperMarket->productsLst);

	if (loadSuperMarketFromFile(pSuperMarket, fileName, customersFileName, isCompress) == 1)
	{
		printf("Supermarket successfully loaded from files\n");
		return 1;
	}

	pSuperMarket->name = getStrExactLength("Enter market name");
	return initAddress(&pSuperMarket->address);
}

void printSuperMarket(const SuperMarket * pSuperMarket)
{
	printf("Super Market name: %s ", pSuperMarket->name);
	printAddress(&pSuperMarket->address);
	printAllPrudocts(pSuperMarket);
	printAllCustomers(pSuperMarket);
}

int addProduct(SuperMarket * pSuperMarket)
{
	char barcode[BARCODE_LEN];
	getValidBarcode(barcode);//Get valid barcode from user
	Product* pProduct = checkIfProductExists(pSuperMarket, barcode);
	if (pProduct) 
	{
		int amount = (int)getLegalNumber("How many items add to stock?"); 
		pProduct->amount += amount; //update the amount in stock of the product
		printf("The amount in stock updated successfully\n");
		return 1;
	}
	pProduct = (Product*)malloc(sizeof(Product)); //If doesn't exists make a new dynamic allocation
	if (!pProduct)
	{
		printf("Memory allocation failed!\n");
		return 0;
	}
	initProduct(pProduct, barcode);
	
	if (!insertProductToSortedList(&pSuperMarket->productsLst,pProduct)) //Put the new product in the list
	{
		free(pProduct);
		return 0;
	}
	return 1;
}

int insertProductToSortedList(LIST* lst, Product* pProduct) // Insert the data to exact place,keep the list sorted
{
	CHECK_RETURN_0(lst);
	NODE* ptr = &lst->head;
	int res;
	while (ptr->next)
	{
		res = compareProductByBarcode(pProduct,ptr->next->key);
		if (res > 0)
			ptr = ptr->next;
		else
			break;
	}
	if (!L_insert(ptr, pProduct))
		return 0;//Error in creating node
	return 1;
}

Product* checkIfProductExists(const SuperMarket* pSuperMarket , const char* pBarcode)
{
	NODE* pNode = pSuperMarket->productsLst.head.next;
	Product temp;
	strcpy(temp.barcode, pBarcode);
	while (pNode)
	{
		if (!compareProductByBarcode(&temp, pNode->key))
			return (Product*)pNode->key;
		pNode = pNode->next;
	}
	return NULL;
}

int checkIfCustomerExists(const SuperMarket * pSuperMarket, const char * name)
{
	int i;
	for (i = 0; i < pSuperMarket->numOfCustomers; i++) //Check if the customer exists 
		if (!strcmp(pSuperMarket->customersArr[i].name, name)) //Compare by name
			return i;
	return NOT_EXIST;
}

int addCustomer(SuperMarket * pSuperMarket)
{
	Customer customer;
	if (!initCustomer(&customer))
		return 0;
	if(checkIfCustomerExists(pSuperMarket , customer.name)!=NOT_EXIST)
	{
		printf("This customer already in market\n");
		freeCustomer(&customer);
		return 0;
	}
	pSuperMarket->customersArr = (Customer*)realloc(pSuperMarket->customersArr, sizeof(Customer)*(pSuperMarket->numOfCustomers + 1)); //Enlarge the array dynamically 
	if (!pSuperMarket->customersArr)
	{
		printf("Memory allocation failed!\n");
		freeCustomer(&customer);
		return 0;
	}
	pSuperMarket->customersArr[pSuperMarket->numOfCustomers] = customer; //Add the customer to the array
	pSuperMarket->numOfCustomers++;
	printf("Customer added successfully!\n");
	return 1;
}

void printAllCustomers(const SuperMarket * pSuperMarket)
{
	printf("There are %d listed customers\n", pSuperMarket->numOfCustomers);
	generalArrayFunction(pSuperMarket->customersArr, pSuperMarket->numOfCustomers, sizeof(Customer), printCustomer);
}

void printAllPrudocts(const SuperMarket * pSuperMarket)
{
	int numOfProducts = productCount(pSuperMarket);
	if (!numOfProducts)
		return;
	printf("There are %d products\n",numOfProducts);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");
	L_print(&pSuperMarket->productsLst, printProduct);
}

int productCount(const SuperMarket* pSuperMarket)
{
	int count = 0;
	NODE* pN = pSuperMarket->productsLst.head.next; //first Node

	while (pN != NULL)
	{
		count++;
		pN = pN->next;
	}
	return count;
}

int shoppingProcess(SuperMarket * pSuperMarket)
{
	ShoppingCart *sC;
	if (noCustomersInMarket(pSuperMarket) || noProductsInMarket(pSuperMarket))
		return 0;
	int customerInd;
	Product* pProduct;
	printAllCustomers(pSuperMarket);
	printf("Who is shopping? enter customer's name:\n");
	char* name = getStrExactLength();
	if (!name)
	{
		printf("Memory allocation failed!\n");
		return 0;
	}
	customerInd = checkIfCustomerExists(pSuperMarket, name);
	if(customerInd == NOT_EXIST)
	{
		printf("Error! customer doesn't exist!\n");
		return 0;
	}
	if (!pSuperMarket->customersArr[customerInd].shoppingCart) //The shopping cart is empty
	{
		pSuperMarket->customersArr[customerInd].shoppingCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));//Create cart by dynamic allocation
		if (!pSuperMarket->customersArr[customerInd].shoppingCart)
		{
			printf("Memory allocation failed!\n"); 
			return 0;
		}
		initShoppingCart(pSuperMarket->customersArr[customerInd].shoppingCart);//Make empty shopping cart
	}
	char choice[MAX_LENGTH];
	char barcode[BARCODE_LEN];
	printAllPrudocts(pSuperMarket);
	printf("Do you want to shop for a product? y/Y, anything else to exit!!\n");
	myGets(choice, MAX_LENGTH,stdin);
	while (!strcmp(choice,"Y") || !strcmp(choice, "y"))
	{
		getValidBarcode(barcode);
		pProduct = checkIfProductExists(pSuperMarket, barcode);
		if (!pProduct)
			printf("No such product\n");
		else
		{
			sC = pSuperMarket->customersArr[customerInd].shoppingCart;
			if (!addItemToShoppingCart(sC, pProduct))
				return 0;
		}
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\n");
		getchar();
		myGets(choice, MAX_LENGTH,stdin);

	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

void printProductsByType(const SuperMarket * pSuperMarket) 
{
	if (noProductsInMarket(pSuperMarket)) //Check if there are no product , and if the list in null
		return;
	int counter = 0;
	PoductType type = getTypeFromUser();
	NODE* pNode = pSuperMarket->productsLst.head.next;
	Product* pProduct;
	while (pNode)
	{
		pProduct = (Product*)pNode->key;
		if (pProduct->type == type)
		{
			printProduct(pProduct);
			counter++;
		}
		pNode = pNode->next;
	}
	if (!counter) //If there is no product of this type 
		printf("There are no product of type %s in market %s\n", typeTitle[type], pSuperMarket->name);
}

void printCustomerShoppingCart(const SuperMarket * pSuperMarket)
{
	if (noCustomersInMarket(pSuperMarket))
		return;
	Customer* customer = getCustomer(pSuperMarket);
	
	if (!customer) //If the customer not exists
		printf("This customer doesn't exists\n");
	else if (isEmptyShoppingCart(customer->shoppingCart))
		printf("Customer cart is empty!\n");
	else 
		printShoppingCart(customer->shoppingCart);
}

Customer* getCustomer(SuperMarket* pSuperMarket)
{
	printAllCustomers(pSuperMarket);//Show to the users all the customers
	printf("Who is shopping? Enter cutomer name\n");
	char* name = getStrExactLength(); //Get customer's name from the user
	int cusInd = checkIfCustomerExists(pSuperMarket, name);
	free(name); //Free after use
	if (cusInd == NOT_EXIST) //If the customer not exists
		return NULL;
	else
		return &pSuperMarket->customersArr[cusInd];
}


int paymentProcess(SuperMarket * pSuperMarket)
{
	if (noCustomersInMarket(pSuperMarket))
		return 0;
	Customer* customer = getCustomer(pSuperMarket);
	if (!customer) //If the customer not exists
	{
		printf("This customer doesn't exists\n");
		return 0;
	}
	else if (isEmptyShoppingCart(customer->shoppingCart))
	{
		printf("Customer cart is empty\n");
		return 0;
	}
	else
		pay(customer);
	return 1;
}

void sortCustomers(SuperMarket* pMarket, eSortType eType)
{
	qsort(pMarket->customersArr, pMarket->numOfCustomers, sizeof(Customer), compare[eType]);
	pMarket->sortType = eType;
}

void searchCustomer(SuperMarket* pMarket)
{
	if (pMarket->sortType == eNotSorted)
	{
		printf("The search cannot be performed, array not sorted");
		return;
	}
	if (pMarket->numOfCustomers == 0)
	{
		printf("The search cannot be performed, there are no customers");
		return;
	}
	Customer customer;
	if (pMarket->sortType == eSortByName)
		customer.name = getStrExactLength("Enter customer name\n");
	else if (pMarket->sortType == eSortByShopTimes)
	{
		printf("Enter time in market\n");
		scanf("%d", &customer.shopTimes);
	}
	else
	{
		printf("Enter spent amount\n");
		scanf("%d", &customer.totalSpend);
	}
	Customer* pCustomer = (Customer*)bsearch(&customer, pMarket->customersArr, pMarket->numOfCustomers, sizeof(Customer), compare[pMarket->sortType]);
	if (pMarket->sortType == eSortByName)
		free(customer.name);
	if (!pCustomer)
		printf("Customer was not found\n");
	else
	{
		printf("Customer found, ");
		printCustomer(pCustomer);
	}
}

int noCustomersInMarket(const SuperMarket* pSuperMarket) 
{
	if (!pSuperMarket->numOfCustomers) //Check if there is no customers in the market
	{
		printf("No customer listed to market\n");
		return 1;
	}
	return 0;
}

int noProductsInMarket(const SuperMarket* pSuperMarket)
{
	if (!&pSuperMarket->productsLst)
		return 1;
	if (!pSuperMarket->productsLst.head.next) //Check if there is no products in the market
	{
		printf("No products in market - cannot shop\n");
		return 1;
	}
	return 0;
}

void handleCustomerStillShoppingAtExit(SuperMarket* pMarket)
{
	Customer* pCust;
	for (int i = 0; i < pMarket->numOfCustomers; i++)
	{
		pCust = &pMarket->customersArr[i];
		if (pCust->shoppingCart)
		{
			printf("Market is closing must pay!!!\n");
			pay(pCust); //will free every thing and update shope info
		}
	}
}
void freeMarket(SuperMarket* pSuperMarket)
{
	generalArrayFunction(pSuperMarket->customersArr, pSuperMarket->numOfCustomers, sizeof(Customer), freeCustomer);
	free(pSuperMarket->name);
	freeAddress(&pSuperMarket->address);
	L_free(&pSuperMarket->productsLst, freeProduct);
	free(pSuperMarket->customersArr);
}



