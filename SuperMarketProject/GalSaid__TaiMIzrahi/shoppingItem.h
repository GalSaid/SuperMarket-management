#pragma once
#include "product.h"
typedef struct
{
	char* barcode;
	float price;
	int amount;
}ShoppingItem;

void initShoppingItem(ShoppingItem* pShoppingItem, Product* pProduct);
void printShoppingItem(const ShoppingItem* pShoppingItem);
void freeShoppingItem(void* v1);