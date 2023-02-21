
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shoppingCart.h"
#define NOT_EXISTS -1
#define DETAIL_PRINT

void initShoppingCart(ShoppingCart * pShoppingCart)
{
	pShoppingCart->items = NULL;
	pShoppingCart->itemsAmount = 0;
}

float printShoppingCart(const ShoppingCart* pCart)
{
	ShoppingItem* pItem;
	float price = 0;
	for (int i = 0; i < pCart->itemsAmount; i++)
	{
		pItem = pCart->items[i];
		printShoppingItem(pItem);
		price += (pItem->price * pItem->amount);
	}
#ifdef DETAIL_PRINT
	printf("Total bill to pay: %.2f\n", price);
#endif
	return price;
}

float getTotalBill(const ShoppingCart * pShoppingCart)//calculate the total sum to pay
{
	float sum = 0, pricePerItem;
	int i, amountOfItem;
	for (i = 0; i < pShoppingCart->itemsAmount; i++)
	{
		pricePerItem = pShoppingCart->items[i]->price; //price of product in cart
		amountOfItem = pShoppingCart->items[i]->amount;//amount of products in cart
		sum += pricePerItem * amountOfItem; 
	}
	return sum;
}

int addItemToShoppingCart(ShoppingCart * pShoppingCart, Product* pProduct)
{
	int itemInd = checkIfProductInCart(pShoppingCart, pProduct->barcode); 
	if (itemInd == NOT_EXISTS) //If you want to add a new product to cart 
	{
		if (!addNewItemToShoppingCart(pShoppingCart, pProduct))
			return 0;
	}
	else //Update tne amount of item in cart
	{
		int amount = getLegalNumberWithTopLimit("How many items do you want?", pProduct->amount);
		pShoppingCart->items[itemInd]->amount += amount;
		pProduct->amount -= amount; //After adding to cart, update the amount in stock
	}

	printf("The item added successfully to cart!\n");
	return 1;
}

int addNewItemToShoppingCart(ShoppingCart * pShoppingCart,Product* pProduct)
{
	ShoppingItem* sI = (ShoppingItem*)malloc(sizeof(ShoppingItem));
	if (!sI)
	{
		printf("Memory allocation failed!\n");
		return 0;
	}
	initShoppingItem(sI, pProduct);
	pShoppingCart->items = (ShoppingItem**)realloc(pShoppingCart->items, sizeof(ShoppingItem*) * (pShoppingCart->itemsAmount+1));
	if (!pShoppingCart->items)
	{
		printf("Memory allocation failed!\n");
		free(sI);
		return 0;
	}
	pShoppingCart->items[pShoppingCart->itemsAmount] = sI;
	pShoppingCart->itemsAmount++;
	pProduct->amount -= sI->amount; //After adding to cart, update the amount in stock
	return 1;
}

int checkIfProductInCart(ShoppingCart *pShoppingCart, char* pBarcode)
{
	int i;
	for (i = 0; i < pShoppingCart->itemsAmount; i++)
		if (!strcmp(pShoppingCart->items[i]->barcode, pBarcode))
			return i;
	return NOT_EXISTS;
	
}

int isEmptyShoppingCart(ShoppingCart* pShoppingCart) //Check if shopping cart is empty
{
	if (pShoppingCart == NULL || pShoppingCart->itemsAmount == 0)
		return 1;
	return 0;
}

void freeShoppingCart(ShoppingCart * pShoppingCart)
{
	generalArrayFunction(pShoppingCart->items, pShoppingCart->itemsAmount, sizeof(ShoppingItem*), freeShoppingItem);
	free(pShoppingCart->items);
}
