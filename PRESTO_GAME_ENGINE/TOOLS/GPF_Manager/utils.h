#ifndef __UTILS_H__
#define __UTILS_H__

size_t getLengthText(char* text);
char* mergeStrings(char* char1, char* char2);
char* mergeMultipleStrings(int args, char* str, ...);
NAMEENTRY MCreateNameEntry(char* fname);

#endif