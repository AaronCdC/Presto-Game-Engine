#ifndef __MEMMANAGER_H__
#define __MEMMANAGER_H__

#include <vector>
#include <malloc.h>
#include <stdarg.h>
#include "..\core.h"
#include "..\DEBUGGER\debugger.h"

extern GameDebugger MainDebugger;

class MemoryManager {

public:
	static void AddToGC(void*);
	static void MemoryManager::MultiAddToGC(int args, ...);
	static void Flush();
private:
	static std::vector<void*> GarbageCollector;

protected:

};

#endif