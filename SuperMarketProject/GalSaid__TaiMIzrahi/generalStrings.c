#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "generalStrings.h"




char* getStrExactLength()
{
	char* theStr = NULL;
	size_t len;
	char inpStr[MAX_LENGTH]; //variable to hold the string that the user gives

	//Ask for a string from the user
	myGets(inpStr, sizeof(inpStr),stdin);

	return getDynStr(inpStr);
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}



char* myGets(char* buf, int size, FILE* fp) //Gets a size and place to store the string
{
	if (buf != NULL && size > 0)
	{
		if (fgets(buf, size, fp)) //fgets return the string from user 
		{
			buf[strcspn(buf, "\n")] = '\0'; // Replace '\n' with '\0'
			return buf; 
		}
		*buf = '\0';
	}
	return NULL; 

}



char* changeSpaces(char* str)
{
	char* word;
	int size;
	int notLastWordInd , firstWord = 1;
	char temp[MAX_LENGTH];
	strcpy(temp, str);
	notLastWordInd = numOfWords(temp)-1;
	word = strtok(str, " ");
	if (!notLastWordInd)
	{
		word[0] = toupper(word[0]);
		strcpy(temp, word);
	}
	else
	{
		while (word != NULL)
		{
			if (notLastWordInd)
			{
				word[0] = toupper(word[0]); //change the first letter in word, to capital letter
				notLastWordInd--;

				if (firstWord)
				{
					strcpy(temp, word);
					firstWord = 0;
				}
				else
					strcat(temp, word);
				strcat(temp, "  "); //Two spaces
			}
			else //Last word in sentence
			{
				word[0] = tolower(word[0]); //change the first letter in last word, to lower letter
				strcat(temp, word);
			}
			word = strtok(NULL, " ");
		}
	}
	char* res = _strdup(temp);//Dynamic allocation of temp 
	return res;
}

int numOfWords(char* str) //Count the number of words in sentence
{
	int numOfWords = 0;
	char* word = strtok(str, " ");
	while (word)
	{
		numOfWords++;
		word = strtok(NULL, " ");
	}
	return numOfWords;
}

void splitWords(char* tempAddress, char wordsArray[][MAX_LENGTH] , int numOfWords , char* delimiters) //Split a string into words array by delimiters
{
	int i = 0;
	char* word;
	word = strtok(tempAddress, delimiters);
	while (word && i < numOfWords)
	{
		strcpy(wordsArray[i], word);
		word = strtok(NULL, delimiters);
		i++;
	}
}

int	checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}
	return 1;
}

void printMessage(const char* str, ...)
{
	va_list list;
	va_start(list, str);
	char* word = str;
	while (word)
	{
		printf("%s ", word);
		word = va_arg(list, char*);
	}
	va_end(list);
}