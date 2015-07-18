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

extern bool __DEBUG__MODE__;

class ResourceManager {
private:
	
public:
	ResourceManager();
	~ResourceManager();
	ALLEGRO_BITMAP* LoadBitmap(const char* path, char* name);
	ALLEGRO_BITMAP* FetchAsBitmap(int ID);
	int FindByName(char* name);
	int FindByID(int ID);
protected:
	std::vector<ASSET> ResourceList;
};

#endif