#ifndef __RESMANAGER_H__
#define __RESMANAGER_H__

#include "..\core.h"
#include <vector>
#include <cstring>

#define T_BITMAP 0
#define T_AUDIO 1
#define T_UNKNOWN 1000

#pragma pack(push, 1)
typedef struct ASSET {
	unsigned long int ID;
	char* NAME;
	unsigned int TYPE;
	void* ContentPtr;
}ASSET;
#pragma pack(pop)

/* NAMEENTRY: Structure used for file names inside the DB */
#pragma pack(push, 1)
typedef struct nameentry
{
	char NAME[17];
	char EXT[4];
}NAMEENTRY;
#pragma pack(pop)

/* DATAPACKAGE: Structure with information related to a package file */
#pragma pack(push, 1)
typedef struct _dp
{
	unsigned int ID;
	NAMEENTRY GPFNAME;
	unsigned int LDP;
	unsigned long long int CRCC;
	unsigned long long int KEY;
	unsigned int FILES;
	unsigned char EXPANDED;
}DATAPACKAGE;
#pragma pack(pop)

/* FILEENTRY: Structure with information about a single file */
#pragma pack(push, 1)
typedef struct fileentry
{
	unsigned int ID;
	NAMEENTRY FILENAME;
	unsigned int GPFID;
	unsigned long long int FPOINTER;
	size_t FILESIZE;
}FILEENTRY;
#pragma pack(pop)

extern bool __DEBUG__MODE__;
extern GameDebugger MainDebugger;

class ResourceManager {
private:
	
public:
	ResourceManager();
	~ResourceManager();

	ALLEGRO_BITMAP* LoadBitmap(const char* path, char* name);
	ALLEGRO_BITMAP* FetchAsBitmap(int ID);
	int FindByName(char* name);
	int FindByID(int ID);
	int OpenDatabase(char* dbpath);
	int OpenPackage(char* gpath);
	void RewindDatabase();
	int GetPackageCount();
	unsigned long long int GetFileCount();
	unsigned int GetPkgFileCount(int pkgid);
	void SelectPkgEntry(int Index);
	int SearchPkgFile(char* name);
	DATAPACKAGE GetPkgEntryByIndex(int Index);
	DATAPACKAGE* GetPkgEntryByName(char* pkgname);
	unsigned int* GetDPTableByPkgId(int Index);
	unsigned int* GetDPTableByPkgName(char* pkgname);
	FILEENTRY* SearchFileByPkgID(int ID, NAMEENTRY fname);
	FILEENTRY* SearchFileByPkgName(char* pkgnm, NAMEENTRY* fname);
	char* ExtractToMemory(FILEENTRY* mfile);
	int ExtractToDisk(FILEENTRY* mfile);
	void OpenResourceDB(char*);
protected:
	std::vector<ASSET> ResourceList;
};

#endif