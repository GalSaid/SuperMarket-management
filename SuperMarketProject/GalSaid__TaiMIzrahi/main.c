#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "generalFucns.h"
#include "Supermarket.h"
#include "generalStrings.h"
#include "SuperFile.h"



int menu();
eSortType sortMenu();


const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Pay",
								"Sort Customers" , "Search a Customer" , "Print Product By Type" };

int main(int argc, char* argv[])
{
	SuperMarket	market;
	if (argc != 3)
		return 0;
	char filename[MAX_LENGTH];
	int flag;
	int count = 0;
	count += sscanf(argv[1], "%d", &flag);
	count += sscanf(argv[2], "%s", filename);
	if (count != 2)
	{
		printf("Error in reading main parameters\n");
		return 0;
	}
	if (!initSuperMarket(&market, filename, CUSTOMER_FILE_NAME, flag))
	{
		printf("error init  Super Market");
		return 0;
	}
	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;


		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if (!shoppingProcess(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			printCustomerShoppingCart(&market);
			break;

		case eCustomerPay:
			if (!paymentProcess(&market))
				printf("Error in payment\n");
			break;

		case eSortCustomers:
			sortCustomers(&market, sortMenu());
			break;

		case eSearchCustomer:
			searchCustomer(&market);
			break;

		case ePrintProductByType:
			printProductsByType(&market);
			break;

		case EXIT:
			printMessage("Thank", "You", "For", "Shopping", "With", "Us", NULL);
			printf("\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);


	handleCustomerStillShoppingAtExit(&market);

	if (!saveSuperMarketToFile(&market, filename, CUSTOMER_FILE_NAME, flag))
		printf("Error saving supermarket to file\n");

	freeMarket(&market);


	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

eSortType sortMenu()
{
	int option;
	do
	{
		printf("Base on what field do you want to sort?\n");
		for (int i = 1; i < eNofSortType; i++)
			printf("%d - %s\n", i, typeSort[i - 1]);
		scanf("%d", &option);
	} while (option < 1 || option >= eNofSortType);
	return (eSortType)(option - 1);
}