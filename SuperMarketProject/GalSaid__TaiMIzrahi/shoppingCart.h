#pragma once

#include "shoppingItem.h"
typedef struct 
{
	ShoppingItem** items;
	int itemsAmount;
}ShoppingCart;

void initShoppingCart(ShoppingCart* pShoppingCart);
float printShoppingCart(const ShoppingCart* pCart);
float getTotalBill(const ShoppingCart* pShoppingCart);
int addItemToShoppingCart(ShoppingCart * pShoppingCart, Product* pProduct);
int addNewItemToShoppingCart(ShoppingCart * pShoppingCart, Product* pProduct);
int checkIfProductInCart(ShoppingCart *pShoppingCart, char* pBarcode);
int isEmptyShoppingCart(ShoppingCart* pShoppingCart);
void freeShoppingCart(ShoppingCart* pShoppingCart);


