#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "shoppingItem.h"
#include "generalFucns.h"

void initShoppingItem(ShoppingItem* pShoppingItem, Product* pProduct)
{
	
	pShoppingItem->barcode = pProduct->barcode;
	pShoppingItem->price = pProduct->price;
	pShoppingItem->amount = getLegalNumberWithTopLimit("How many items do you want?", pProduct->amount);
	 
}
void printShoppingItem(const ShoppingItem* pShoppingItem)
{
	printf("Item %s amount in cart: %d price per item: %.2f \n", pShoppingItem->barcode, pShoppingItem->amount, pShoppingItem->price);
}

void freeShoppingItem(void* v1)
{
	ShoppingItem* sI = *(ShoppingItem**)v1;
	free(sI);
}