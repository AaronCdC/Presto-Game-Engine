#ifndef __GPF_MAN__
#define __GPF_MAN__

#pragma pack(push, 1)
typedef struct nameentry
{
	char NAME[16];
	char EXT[3];
}NAMEENTRY;
#pragma pack(pop)

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