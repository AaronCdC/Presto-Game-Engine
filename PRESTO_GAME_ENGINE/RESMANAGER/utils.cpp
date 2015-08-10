/* Some functions I coded years ago for a C library
aimed to easily manipulate C strings and paths. */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"

/* This function get the number of characters
in a C string, if it's properly formated. */
size_t HelpUtils::getLengthText(char* text)
{
	unsigned int counter = -1;
	char cChar = 1;
	while(cChar != '\0')
	{
		counter+=1;
		cChar = text[counter];
	}
	return counter;
}

/* C function to merge two strings. */
char* HelpUtils::mergeStrings(char* char1, char* char2)
{
	char* result;
	size_t c1size = getLengthText(char1);
	size_t c2size = getLengthText(char2);

	result=(char*)calloc(c1size+c2size+1,1);
	memcpy(result,char1,c1size);
	memcpy(result+c1size,char2,c2size);

	return result;
}

/* C function to merge multiple, undefined number of strings.*/
char* HelpUtils::mergeMultipleStrings(int args, ...)
{
	va_list chrstr;
	char* result;
	int total = 0;
	unsigned int totalChars = 0;
	va_start(chrstr, args);
	for(int x = 0; x < args; x++)
		totalChars += getLengthText((char*)va_arg(chrstr, char*));
	va_end(chrstr);

	result = (char*)calloc(1, totalChars+1);

	va_start(chrstr, args);
	for(int x = 0; x < args; x++)
	{
		char* buf = (char*)va_arg(chrstr, char*);
		int cSize = getLengthText(buf);
		memcpy(result+total,buf,cSize);
		total += cSize;
	}
	va_end(chrstr);
	return result;
}

/* Create a NAMEENTRY in the HEAP and return a pointer to it. */
void* HelpUtils::MCreateNameEntry(char* fname)
{
	NAMEENTRY* result;
	char* name = (char*)calloc(17,1);
	char* ext = (char*)calloc(4,1);
	int tsize = getLengthText(fname), y = 0;
	bool extm = false;

	result = (NAMEENTRY*)calloc(sizeof(NAMEENTRY),1);

	for(int x = 0; x < tsize; x++)
	{
		if(fname[x] == '.')
		{
			extm = true;
			continue;
		}

		if(x >= 17 && !extm)
			break;

		if(!extm)
			name[x] = fname[x];
		else
		{
			ext[y] = fname[x];
			y++;
			if(y >= 4)
				break;
		}
			
	}
	
	strcpy(result->NAME, name);
	strcpy(result->EXT, ext);

	return (void*)result;
}