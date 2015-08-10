#include "resmanager.h"
#include "utils.h"

char* defaultdbname = "\\db.pdb";

FILE* PDBFILE;
FILE* GPFILE;

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ALLEGRO_BITMAP* ResourceManager::LoadBitmap(const char* path, char* name)
{
	ALLEGRO_BITMAP* load = al_load_bitmap(path);
	if(!load)
	{
		MainDebugger.Log("Failed loading resource: %s (%s) - RETURNING NULL",info_type_error, path, name);
		return NULL;
	}
	ASSET newAsset;
	newAsset.ID = rand() % 4294967294 + 1;
	newAsset.NAME = name;
	newAsset.TYPE = T_BITMAP;
	newAsset.ContentPtr = (void*)load;
	ResourceList.push_back(newAsset);
	if(__DEBUG__MODE__)
		MainDebugger.Log("New resource loaded: %i %i \"%s\" %i 0x%p",info_type_message, ResourceList.size()-1, newAsset.ID, newAsset.NAME, newAsset.TYPE, newAsset.ContentPtr);
	return load;
}

ALLEGRO_BITMAP* ResourceManager::FetchAsBitmap(int ID)
{
	if(ResourceList[ID].TYPE == T_BITMAP)
		return (ALLEGRO_BITMAP*)ResourceList[ID].ContentPtr;
	else
		return NULL;
}

int ResourceManager::FindByName(char* name)
{
	for(int x = 0; x < ResourceList.size(); x++)
	{
		if(strcmp(ResourceList[x].NAME, name) == 0)
			return x;
	}
	return -1;
}

int ResourceManager::FindByID(int ID)
{
	for(int x = 0; x < ResourceList.size(); x++)
	{
		if(ResourceList[x].ID == ID)
			return x;
	}
	return -1;
}


/***************************************************
	GPF MANAGER ROUTINES
***************************************************/

/* Simply open a file to use it as a database */
int ResourceManager::OpenDatabase(char* dbpath)
{
	PDBFILE = fopen(dbpath, "rb");
	return (int) PDBFILE;
}

/* Simply open a file to use it as a package file */
int ResourceManager::OpenPackage(char* gpath)
{
	GPFILE = fopen(gpath, "rb");
	return (int) GPFILE;
}

/* Goes to the beginning of the database file */
void ResourceManager::RewindDatabase()
{
	if(!PDBFILE)
		return;
	fseek(PDBFILE, 0L, SEEK_SET);
	return;
}

/* Get the number of packages in the database (First 4 bytes in the database) */
int ResourceManager::GetPackageCount()
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
unsigned long long int ResourceManager::GetFileCount()
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
unsigned int ResourceManager::GetPkgFileCount(int pkgid)
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

/* Routine to select a package data entry from the database. */
void ResourceManager::SelectPkgEntry(int Index)
{
	RewindDatabase();
	fseek(PDBFILE,4+(Index*sizeof(DATAPACKAGE)),0L);
	return;
}

/* Routine to find a package file within the database, using it's name. */
int ResourceManager::SearchPkgFile(char* name)
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
DATAPACKAGE ResourceManager::GetPkgEntryByIndex(int Index)
{
	DATAPACKAGE myPkg;
	RewindDatabase();
	fseek(PDBFILE,4+(Index*sizeof(DATAPACKAGE)),0L);
	fread(&myPkg, sizeof(DATAPACKAGE), 1, PDBFILE);
	return myPkg;
}

/* Routine to get the DP entry by it's name. */
/*	-!-!-! U N T E S T E D !-!-!- */
DATAPACKAGE* ResourceManager::GetPkgEntryByName(char* pkgname)
{
	DATAPACKAGE* myPkg = (DATAPACKAGE*)calloc(sizeof(DATAPACKAGE),1);
	int pkgid = SearchPkgFile(pkgname);
	RewindDatabase();
	fseek(PDBFILE,4+(pkgid*sizeof(DATAPACKAGE)),0L);
	fread(myPkg, sizeof(DATAPACKAGE), 1, PDBFILE);
	return myPkg;
}

/* Routine to get the DP table from it's package ID. */
unsigned int* ResourceManager::GetDPTableByPkgId(int Index)
{
	unsigned int* dpdescriptor = (unsigned int*)calloc(sizeof(unsigned int),1024);
	DATAPACKAGE myPkg = GetPkgEntryByIndex(Index);
	fseek(PDBFILE, myPkg.LDP, SEEK_SET);
	fread(dpdescriptor, sizeof(unsigned int), 1024, PDBFILE);
	return dpdescriptor;
}

/* Routine to get the DP table from it's package name. */
unsigned int* ResourceManager::GetDPTableByPkgName(char* pkgname)
{
	unsigned int* dpdescriptor = (unsigned int*)calloc(sizeof(unsigned int),1024);
	DATAPACKAGE* myPkg = GetPkgEntryByName(pkgname);
	fseek(PDBFILE, myPkg->LDP, SEEK_SET);
	fread(dpdescriptor, sizeof(unsigned int), 1024, PDBFILE);
	return dpdescriptor;
}

/* Routine to get a pointer to a file entry from a package ID and file name. */
FILEENTRY* ResourceManager::SearchFileByPkgID(int ID, NAMEENTRY fname)
{
	FILEENTRY* myFile = (FILEENTRY*)calloc(sizeof(FILEENTRY),1);
	//bufferlist.push_back((char*)myFile);
	std::vector<int> results;
	unsigned int* dptable = GetDPTableByPkgId(ID);
	bool found = false;

	for(int x = 0; x < 1024; x++)
	{
		if(dptable[x] > 0)
		{
			fseek(PDBFILE, dptable[x], SEEK_SET);
			fread(myFile, sizeof(FILEENTRY), 1, PDBFILE);
			if(strcmp(myFile->FILENAME.EXT, fname.EXT) == 0)
				if(myFile->FILENAME.NAME[0] == fname.NAME[0])
					results.push_back(dptable[x]);
		}
	}


	if(results.size() == 0)
		return 0L;

	for(int y = 0; y < results.size(); y++)
	{
		fseek(PDBFILE, results[y], SEEK_SET);
		fread(myFile, sizeof(FILEENTRY), 1, PDBFILE);
		if(strcmp(myFile->FILENAME.NAME, fname.NAME) == 0)
		{
			found = true;
			break;
		}
	}

	results.clear();

	if(found)
		return myFile;
	else
	{
		free(myFile);
		return 0L;
	}
}

/* Routine to get a pointer to a file entry from a package name and file name. */
FILEENTRY* ResourceManager::SearchFileByPkgName(char* pkgnm, NAMEENTRY* fname)
{
	FILEENTRY* myFile = (FILEENTRY*)calloc(sizeof(FILEENTRY),1);
	std::vector<int> results;
	unsigned int* dptable = GetDPTableByPkgName(pkgnm);
	bool found = false;

	for(int x = 0; x < 1024; x++)
	{
		if(dptable[x] > 0)
		{
			fseek(PDBFILE, dptable[x], SEEK_SET);
			fread(myFile, sizeof(FILEENTRY), 1, PDBFILE);
			if(strcmp(myFile->FILENAME.EXT, fname->EXT) == 0)
				if(myFile->FILENAME.NAME[0] == fname->NAME[0])
					results.push_back(dptable[x]);
		}
	}


	if(results.size() == 0)
		return 0L;

	for(int y = 0; y < results.size(); y++)
	{
		fseek(PDBFILE, results[y], SEEK_SET);
		fread(myFile, sizeof(FILEENTRY), 1, PDBFILE);
		if(strcmp(myFile->FILENAME.NAME, fname->NAME) == 0)
		{
			found = true;
			break;
		}
	}

	results.clear();

	if(found)
		return myFile;
	else
	{
		free(myFile);
		return 0L;
	}
}

/* Routine to extract a file to the memory */
char* ResourceManager::ExtractToMemory(FILEENTRY* mfile)
{
	char* buffer;
	buffer = (char*)calloc(1, mfile->FILESIZE);
	fseek(GPFILE, mfile->FPOINTER, SEEK_SET);
	if(fread(buffer, 1, mfile->FILESIZE, GPFILE) == 0)
	{
		free(buffer);
		return 0L;
	}
	return buffer;
}

/* Routine to extract a file to the hard disk */
int ResourceManager::ExtractToDisk(FILEENTRY* mfile)
{
	char* buffer;
	FILE* temp = fopen(HelpUtils::mergeMultipleStrings(3,mfile->FILENAME.NAME, ".", mfile->FILENAME.EXT), "w+b");
	buffer = (char*)calloc(1, mfile->FILESIZE);
	fseek(GPFILE, mfile->FPOINTER, SEEK_SET);
	if(fread(buffer, 1, mfile->FILESIZE, GPFILE) == 0)
	{
		free(buffer);
		return -1;
	}
	if(fwrite(buffer,1,mfile->FILESIZE,temp) == 0);
	{
		free(buffer);
		return -1;
	}
	free(buffer);
	fclose(temp);
	return 1;
}

void ResourceManager::OpenResourceDB(char* path)
{
	char* fullpath = HelpUtils::mergeStrings(path, "\\db.pdb");
	OpenDatabase(fullpath);
	if(PDBFILE)
		MainDebugger.Log("Open database %s", "Info", fullpath);
	else
		MainDebugger.Log("Failed to open database %s", "Info", fullpath);
	free(fullpath);
	return;
}