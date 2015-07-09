#include "resmanager.h"

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

ALLEGRO_BITMAP* ResourceManager::LoadBitmap(const char* path, char* name)
{
	ALLEGRO_BITMAP* load = al_load_bitmap(path);
	ASSET newAsset;
	newAsset.ID = rand() % 4294967294 + 1;
	newAsset.NAME = name;
	newAsset.TYPE = T_BITMAP;
	newAsset.ContentPtr = (void*)load;
	ResourceList.push_back(newAsset);
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