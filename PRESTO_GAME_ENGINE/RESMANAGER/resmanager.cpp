/*
 -- CURRENTLY UNDER DEVELOPMENT --
*/

#include "resmanager.h"

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