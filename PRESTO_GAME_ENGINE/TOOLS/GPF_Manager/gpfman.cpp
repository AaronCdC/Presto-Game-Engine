/* CURRENTLY UNDER DEVELOPMENT */
//TODO: Error control

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <vector>
#include "gpfman.h"
#include "utils.h"

FILE* PDBFILE;
FILE* GPFILE;

char* defaultdbname = "\\db.pdb";

/* Simply open a file to use it as a database */
int OpenDatabase(char* dbpath)
{
	PDBFILE = fopen(dbpath, "w+b");
	return (int) PDBFILE;
}

/* Simply open a file to use it as a package file */
int OpenPackage(char* gpath)
{
	GPFILE = fopen(gpath, "w+b");
	return (int) GPFILE;
}

/* Goes to the beginning of the database file */
void RewindDatabase()
{
	if(!PDBFILE)
		return;
	fseek(PDBFILE, 0L, SEEK_SET);
	return;
}

/* Get the number of packages in the database */
int GetPackageCount()
{
	if(!PDBFILE)
		return -1;
	RewindDatabase();
	int pckgcnt = 0;
	fread((char*)&pckgcnt, 4, 1, PDBFILE);
	return pckgcnt;
}

/* Get the total number of file entries in the database, from all the packages */
/* You can get the number of files within a single package from it's package data entry */
unsigned long long int GetFileCount()
{
	int pkgcnt = GetPackageCount();
	DATAPACKAGE mdata;
	unsigned long long int files = 0;


	for(int x = 0; x < pkgcnt; x++)
	{
		fread(&mdata,sizeof(DATAPACKAGE),1,PDBFILE);
		files += mdata.FILES;
	}

	return files;
}

/* Routine to create a brand new database. */
void CreateDatabase(char* directory, char* dbname)
{
	char* path;
	char* gppath;
	DATAPACKAGE defaultPackage;
	unsigned int nl = 0;

	memset(defaultPackage.GPFNAME.EXT, 0x0, 4);
	memset(defaultPackage.GPFNAME.NAME, 0x0, 17);

	strcpy(defaultPackage.GPFNAME.EXT, "gpf");
	strcpy(defaultPackage.GPFNAME.NAME, "default");

	defaultPackage.CRCC = 0;
	defaultPackage.EXPANDED = 0;
	defaultPackage.FILES = 0;
	defaultPackage.ID = rand() % UINT_MAX;
	defaultPackage.KEY = 0;
	defaultPackage.LDP = sizeof(DATAPACKAGE)+4;

	if(strcmp(directory, "") != 0)
		path = mergeStrings(directory, defaultdbname);
	else
		path = dbname;

	PDBFILE = fopen(path, "w+b");
	if(strcmp(directory, "") != 0)
		gppath = mergeMultipleStrings(5, directory, "\\", defaultPackage.GPFNAME.NAME, ".", defaultPackage.GPFNAME.EXT);
	else
		gppath = mergeMultipleStrings(3, defaultPackage.GPFNAME.NAME, ".", defaultPackage.GPFNAME.EXT);
	GPFILE = fopen(gppath, "w+b");

	int pckgcnt = 1;
	fwrite((char*)&pckgcnt,4,1,PDBFILE);
	fwrite(&defaultPackage, sizeof(DATAPACKAGE), 1, PDBFILE);

	for(int x = 0; x < 1024*sizeof(unsigned int); x++)
		fwrite(&nl,4,1,PDBFILE);

	return;
}

/* Routine to select a package data entry from the database */
void SelectPkgEntry(int Index)
{
	RewindDatabase();
	fseek(PDBFILE,4+(Index*sizeof(DATAPACKAGE)),0L);
	return;
}

/* Routine to find a package file within the database, using it's name. */
int SearchPkgFile(char* name)
{
	int result;
	std::vector<int> results;
	int packagenumber = GetPackageCount();
	DATAPACKAGE dpkg;

	SelectPkgEntry(0);

	if(packagenumber < 2)
	{
		SelectPkgEntry(0);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
			return 0;
		else
			return -1;
	}

	for(int x = 0; x < packagenumber; x++)
	{
		fread(&dpkg,sizeof(DATAPACKAGE),1,PDBFILE);
		if(dpkg.GPFNAME.NAME[0] == name[0] && dpkg.GPFNAME.NAME[1] == name[1])
			results.push_back(x);
	}

	if(results.size() < 2)
	{
		SelectPkgEntry(results[0]);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
			return results[0];
		else
			return -1;
	}

	for(int y = 0; y < results.size(); y++)
	{
		SelectPkgEntry(y);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
			return results[y];
	}

	results.clear();

	return -1;
}

/* Routine to insert a file inside a package file, and update the database */
//TODO:
//Update file count
//Maybe remove LD? Seems unneeded. Search instead for file entries with same pkgid.
void InsertFile(char* file, char* dest_pkg, char* name, char* ext)
{
	char* fbuff;
	unsigned int fpointer = 0;
	unsigned long long int fnumber = 0;
	bool longfile = false;
	size_t read = 0;

	FILEENTRY myfile;
	FILE* fdata = fopen(file, "rb");

	fseek(fdata, 0L, SEEK_END);
	size_t fsize = ftell(fdata);
	fseek(fdata,0L,SEEK_SET);

	if(fsize > 1048576)
		longfile = true;

	memset(&myfile, 0x0, sizeof(FILEENTRY));
	strcpy(myfile.FILENAME.NAME,name);
	strcpy(myfile.FILENAME.EXT,ext);
	myfile.FILESIZE = fsize;

	/* Any file bigger than 1Mb is considered a long file. */
	if(!longfile)
	{
		fbuff = (char*)calloc(fsize, 1);
		fread(fbuff, 1, fsize, fdata);
		fpointer = ftell(GPFILE);
		fwrite(fbuff, 1, fsize, GPFILE);
	}
	else{
		fbuff = (char*)calloc(1048576, 1);
		unsigned long long int passes = ceil((double)(fsize/1048576));
		fpointer = ftell(GPFILE);
		for(unsigned long long int x = 0; x <= passes; x++)
		{
				read = fread(fbuff,1,1048576,fdata);
				fwrite(fbuff, 1, read, GPFILE);
		}
	}

	fclose(fdata);

	myfile.FPOINTER = fpointer;
	myfile.GPFID = SearchPkgFile(dest_pkg);

	fnumber = GetFileCount();
	myfile.ID = fnumber+1;

	fseek(PDBFILE, 4+(GetPackageCount()*sizeof(DATAPACKAGE))+(fnumber*sizeof(unsigned int)), SEEK_SET);
	fwrite(&myfile.ID, sizeof(unsigned int), 1, PDBFILE);

	fseek(PDBFILE, 0L,SEEK_END);
	fwrite(&myfile,sizeof(myfile),1,PDBFILE);
	RewindDatabase();
	free(fbuff);
	return;
}

/* Main function */
int main(int args, char** argv)
{
	CreateDatabase("dbtest", "test");
	fseek(PDBFILE, 0L, SEEK_SET);
	InsertFile("fortest\\textfile.txt", "default", "TEXT", "TXT");
	InsertFile("fortest\\longfile.jpg", "default", "LONGFILE", "JPG");
	printf("%i",sizeof(DATAPACKAGE));
	getchar();
	if(PDBFILE)
		fclose(PDBFILE);
	if(GPFILE)
		fclose(GPFILE);
	return 0;
}