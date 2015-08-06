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

/* Get the number of packages in the database (First 4 bytes in the database) */
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

/* Routine to get the number of files in a package */
unsigned int GetPkgFileCount(int pkgid)
{
	int pkgcnt = GetPackageCount();
	int rb = 0;
	DATAPACKAGE myPkg;
	if(pkgcnt < pkgid)
		return 0;
	fseek(PDBFILE, sizeof(int)+(pkgid*sizeof(DATAPACKAGE)), SEEK_SET);
	fread(&myPkg, sizeof(DATAPACKAGE), 1, PDBFILE);
	fseek(PDBFILE, 0L, SEEK_SET);
	return myPkg.FILES;
}

/* Routine to update the number of files in a package */
void UpdatePkgFileCount(int pkgid, unsigned int fnumber)
{
	unsigned int offset = (sizeof(unsigned int)*2) + sizeof(NAMEENTRY) + sizeof(unsigned long long int)*2;
	int pkgcnt = GetPackageCount();
	int rb = 0;
	if(pkgcnt < pkgid)
		return;
	fseek(PDBFILE, sizeof(int)+(pkgid*sizeof(DATAPACKAGE)) + offset, SEEK_SET);
	fwrite(&fnumber, sizeof(unsigned int), 1, PDBFILE);
	fseek(PDBFILE, 0L, SEEK_SET);
	return;
}

/* Routine to create a brand new database. The database includes one package file. */
int CreateDatabase(char* directory, char* dbname)
{
	char* path;
	char* gppath;
	DATAPACKAGE defaultPackage;
	unsigned int nl = 0;
	int nlngth = getLengthText(dbname);

	if(nlngth >= 16 || nlngth < 3)
		return -1;

	memset(defaultPackage.GPFNAME.EXT, 0x0, 4);
	memset(defaultPackage.GPFNAME.NAME, 0x0, 17);

	strcpy(defaultPackage.GPFNAME.EXT, "gpf");
	strcpy(defaultPackage.GPFNAME.NAME, dbname);

	defaultPackage.CRCC = 0; //Unused for now. Not really needed but can be usefull for file integrity checks.
	defaultPackage.EXPANDED = 0;
	defaultPackage.FILES = 0;
	defaultPackage.ID = rand() % UINT_MAX;
	defaultPackage.KEY = 0; //Unused until the very last part of the development
	defaultPackage.LDP = sizeof(DATAPACKAGE)+4; //This is our very first package.

	//If there is a path, merge the name of the database with the path.
	if(strcmp(directory, "") != 0)
		path = mergeStrings(directory, defaultdbname);
	else
		path = dbname;

	PDBFILE = fopen(path, "w+b");

	//Build the path to the package file.
	if(strcmp(directory, "") != 0)
		gppath = mergeMultipleStrings(5, directory, "\\", defaultPackage.GPFNAME.NAME, ".", defaultPackage.GPFNAME.EXT);
	else
		gppath = mergeMultipleStrings(3, defaultPackage.GPFNAME.NAME, ".", defaultPackage.GPFNAME.EXT);
	GPFILE = fopen(gppath, "w+b");

	//Generate the default stuff in the database.
	int pckgcnt = 1;
	fwrite((char*)&pckgcnt,4,1,PDBFILE);
	fwrite(&defaultPackage, sizeof(DATAPACKAGE), 1, PDBFILE);

	for(int x = 0; x < 1024*sizeof(unsigned int); x++)
		fwrite(&nl,4,1,PDBFILE);

	return 0;
}

/* Routine to select a package data entry from the database. */
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

	//This means "Go to the very first package"
	SelectPkgEntry(0);

	//If there is only one package file, check it.
	if(packagenumber < 2)
	{
		SelectPkgEntry(0);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
			return 0; //Found match
		else
			return -1; //Not found
	}

	//If there are more, check if the first 2 letters of their name match.
	//If they do match, store them in a list.
	for(int x = 0; x < packagenumber; x++)
	{
		fread(&dpkg,sizeof(DATAPACKAGE),1,PDBFILE);
		if(dpkg.GPFNAME.NAME[0] == name[0] && dpkg.GPFNAME.NAME[1] == name[1])
			results.push_back(x);
	}

	//If the list of matches has only one match, check it.
	if(results.size() < 2)
	{
		SelectPkgEntry(results[0]);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
		{
			results.clear();
			return results[0]; //Found match
		}
		else{
			results.clear();
			return -1; //Not found
		}
	}

	//If there are more than one matches, check them one by one.
	for(int y = 0; y < results.size(); y++)
	{
		SelectPkgEntry(y);
		fread(&dpkg, sizeof(DATAPACKAGE), 1, PDBFILE);
		if(strcmp(dpkg.GPFNAME.NAME, name)==0)
		{
			results.clear();
			return results[y]; //Found match
		}
	}

	//If this point is reached, no matches were found.
	results.clear();
	return -1;
}

/* Routine to get the DP entry by it's index. */
/*	-!-!-! U N T E S T E D !-!-!- */
DATAPACKAGE GetPkgEntryByIndex(int Index)
{
	DATAPACKAGE myPkg;
	RewindDatabase();
	fseek(PDBFILE,4+(Index*sizeof(DATAPACKAGE)),0L);
	fread(&myPkg, sizeof(DATAPACKAGE), 1, PDBFILE);
	return myPkg;
}

/* Routine to get the DP entry by it's name. */
/*	-!-!-! U N T E S T E D !-!-!- */
DATAPACKAGE GetPkgEntryByName(char* pkgname)
{
	DATAPACKAGE myPkg;
	int pkgid = SearchPkgFile(pkgname);
	RewindDatabase();
	fseek(PDBFILE,4+(pkgid*sizeof(DATAPACKAGE)),0L);
	fread(&myPkg, sizeof(DATAPACKAGE), 1, PDBFILE);
	return myPkg;
}

/* Routine to get the DP table from it's package ID. */
/*	-!-!-! U N T E S T E D !-!-!- */
unsigned int* GetDPTableByPkgId(int Index)
{
	unsigned int dpdescriptor[1024];
	DATAPACKAGE myPkg = GetPkgEntryByIndex(Index);
	fseek(PDBFILE, myPkg.LDP, SEEK_SET);
	fread(&dpdescriptor, sizeof(unsigned int), 1024, PDBFILE);
	return dpdescriptor;
}

/* Routine to get the DP table from it's package name. */
/*	-!-!-! U N T E S T E D !-!-!- */
unsigned int* GetDPTableByPkgName(char* pkgname)
{
	unsigned int dpdescriptor[1024];
	DATAPACKAGE myPkg = GetPkgEntryByName(pkgname);
	fseek(PDBFILE, myPkg.LDP, SEEK_SET);
	fread(&dpdescriptor, sizeof(unsigned int), 1024, PDBFILE);
	return dpdescriptor;
}

/* Routine to get a pointer to a file entry from a package ID and file name. */
/*	-!-!-! U N T E S T E D !-!-!- */
FILEENTRY* SearchFileByPkgID(int ID, NAMEENTRY fname)
{
	FILEENTRY myFile;
	std::vector<int> results;
	unsigned int* dptable = GetDPTableByPkgId(ID);
	bool found = false;

	for(int x = 0; x < 1024; x++)
	{
		if(dptable[x] > 0)
		{
			fseek(PDBFILE, dptable[x], SEEK_SET);
			fread(&myFile, sizeof(FILEENTRY), 1, PDBFILE);
			if(strcmp(myFile.FILENAME.EXT, fname.EXT) == 0)
				if(myFile.FILENAME.NAME[0] == fname.NAME[0])
					results.push_back(dptable[x]);
		}
	}


	for(int y = 0; y < results.size(); y++)
	{
		fseek(PDBFILE, results[y], SEEK_SET);
		fread(&myFile, sizeof(FILEENTRY), 1, PDBFILE);
		if(strcmp(myFile.FILENAME.NAME, fname.NAME) == 0)
		{
			found = true;
			break;
		}
	}

	results.clear();

	if(found)
		return &myFile;
	else
		return 0L;
}

/* Routine to insert a file inside a package file, and update the database */
void InsertFile(char* file, char* dest_pkg, char* name, char* ext)
{
	char* fbuff;
	unsigned int fpointer = 0;
	unsigned long long int fnumber = 0;
	bool longfile = false;
	size_t read = 0;
	unsigned int eofp = 0;
	int pkgcnt = GetPackageCount();
	unsigned int fcnt = 0;

	FILEENTRY myfile;
	FILE* fdata = fopen(file, "rb");

	fseek(fdata, 0L, SEEK_END);
	size_t fsize = ftell(fdata);
	fseek(fdata,0L,SEEK_SET);

	//Check if we have a long file.
	if(fsize > 1048576)
		longfile = true;

	memset(&myfile, 0x0, sizeof(FILEENTRY));
	strcpy(myfile.FILENAME.NAME,name);
	strcpy(myfile.FILENAME.EXT,ext);
	myfile.FILESIZE = fsize;

	/* Any file bigger than 1Mb is considered a long file. */
	if(!longfile)
	{
		//Normal, small files can be copied in a single pass.
		fbuff = (char*)calloc(fsize, 1);
		fread(fbuff, 1, fsize, fdata);
		fpointer = ftell(GPFILE);
		fwrite(fbuff, 1, fsize, GPFILE);
	}
	else{
		//If the file is too big, use multiple passes in a 1Mb buffer.
		//Most computers have a huge RAM capacity by now, but we don't want to waste too much memory.
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

	//Fill the file descriptor.
	myfile.FPOINTER = fpointer;
	myfile.GPFID = SearchPkgFile(dest_pkg);

	fnumber = GetFileCount();
	myfile.ID = fnumber+1;

	//fseek(PDBFILE, 4+(GetPackageCount()*sizeof(DATAPACKAGE))+(fnumber*sizeof(unsigned int)), SEEK_SET);
	//fwrite(&myfile.ID, sizeof(unsigned int), 1, PDBFILE);

	fseek(PDBFILE, 0L,SEEK_END);
	eofp = ftell(PDBFILE); //Gets the current FD entry pointer, which is the EOF.
	fwrite(&myfile,sizeof(myfile),1,PDBFILE); //Write the file descriptor at the EOF.
	RewindDatabase(); //Goes to the first byte of the database file.

	fcnt = GetPkgFileCount(myfile.GPFID);
	fseek(PDBFILE, sizeof(int)+(sizeof(DATAPACKAGE)*(pkgcnt))+(sizeof(int)*(fcnt)), SEEK_SET);
	fwrite(&eofp, sizeof(unsigned int), 1, PDBFILE); //Updates the FD pointer table.

	fcnt++;
	UpdatePkgFileCount(myfile.GPFID,fcnt); //Update the number of files in the package descriptor.

	free(fbuff);
	return;
}

/* Main function */
int main(int args, char** argv)
{
	CreateDatabase("dbtest", "test");
	fseek(PDBFILE, 0L, SEEK_SET);
	InsertFile("fortest\\textfile.txt", "test", "TEXT", "TXT");
	InsertFile("fortest\\longfile.jpg", "test", "LONGFILE", "JPG");

	getchar();
	if(PDBFILE)
		fclose(PDBFILE);
	if(GPFILE)
		fclose(GPFILE);
	return 0;
}