/* Some functions I coded years ago for a C library
aimed to easily manipulate C strings and paths. */

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

/* This function get the number of characters
in a C string, if it's properly formated. */
size_t getLengthText(char* text)
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
char* mergeStrings(char* char1, char* char2)
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
char* mergeMultipleStrings(int args, char* str, ...)
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