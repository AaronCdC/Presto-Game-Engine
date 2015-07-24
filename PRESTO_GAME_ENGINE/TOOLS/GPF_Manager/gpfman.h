#ifndef __GPF_MAN__
#define __GPF_MAN__

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

#endif