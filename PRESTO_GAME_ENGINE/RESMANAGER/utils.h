#ifndef __UTILS_H__
#define __UTILS_H__

#include "resmanager.h"

class HelpUtils {

public:
	static size_t getLengthText(char* text);
	static char* mergeStrings(char* char1, char* char2);
	static char* mergeMultipleStrings(int args, ...);
	static void* MCreateNameEntry(char* fname);

};

#endif