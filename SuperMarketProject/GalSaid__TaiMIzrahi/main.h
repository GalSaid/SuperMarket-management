#pragma once

#define EXIT -1
#define SUPER_FILE_NAME "SuperMarket.bin"
#define CUSTOMER_FILE_NAME "Customers.txt"

typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping, ePrintCart, eCustomerPay,
	eSortCustomers, eSearchCustomer, ePrintProductByType, eNofOptions
} eMenuOptions;

const char* menuStrings[eNofOptions];

int menu();