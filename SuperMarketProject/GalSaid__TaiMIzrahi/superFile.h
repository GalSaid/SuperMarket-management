#pragma once

#include <stdio.h>
#include "Supermarket.h"


int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName, const char* customersFileName, int isCompress);
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName, const char* customersFileName, int isCompress);


