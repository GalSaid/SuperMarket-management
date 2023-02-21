#pragma once

#define NAME_LEN 21
#define BARCODE_LEN 8

typedef enum { eShelf, eFrozen, eFridge, eFruitVegtable, eNofType }PoductType;
static const char* typeTitle[eNofType] = { "Shelf", "Frozen", "Fridge", "FruitVegtable" };

typedef struct
{
	char name[NAME_LEN];
	char barcode[BARCODE_LEN];
	PoductType type;
	float price;
	int amount;
}Product;

void initProduct(Product* pProduct , char* pValidBarcode);
void printProduct(const void* v1);
void getValidBarcode(char* pBarcode);
int ValidBarcode(const char* pBarcode);
PoductType getTypeFromUser();
int compareProductByBarcode(const void* v1, const void* v2);
void freeProduct(void* v1);

int	saveProductToFile(const Product* pProduct, FILE* fp, int isCompress);
int	loadProductFromFile(Product* pProduct, FILE* fp, int isCompress);
char getValidChar(char data);
char* setValidChars(const char* barcode);
