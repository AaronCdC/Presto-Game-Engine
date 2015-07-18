/* -- UNDER DEVELOPMENT -- */

#include <stdio.h>
#include "gpfman.h"

FILE* PDBFILE;
FILE* GPFILE;

int OpenDatabase(char* dbpath)
{
	PDBFILE = fopen(dbpath, "rwb");
	return (int) PDBFILE;
}

int OpenPackage(char* gpath)
{
	GPFILE = fopen(gpath, "rwb");
	return (int) GPFILE;
}

void RewindDatabase()
{
	if(!PDBFILE)
		return;
	fseek(PDBFILE, 0L, SEEK_SET);
	return;
}

int GetPackageCount()
{
	if(!PDBFILE)
		return -1;
	RewindDatabase();
	int pckgcnt = 0;
	fread((char*)&pckgcnt, 4, 1, PDBFILE);
	return pckgcnt;
}

int main(int args, char** argv)
{
	return 0;
}