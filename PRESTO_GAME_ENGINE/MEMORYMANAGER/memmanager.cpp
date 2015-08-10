#include "memmanager.h"
#include <initializer_list>

std::vector<void*> MemoryManager::GarbageCollector(0);

/* Adds a pointer to the GC. MUST BE ALLOCATED USING malloc/calloc */
void MemoryManager::AddToGC(void* myThing)
{
	if(__DEBUG__MODE__)
		MainDebugger.Log("Adding 1 element to GC: %p", "Info", myThing);
	MemoryManager::GarbageCollector.push_back(myThing);
	return;
}

/* Adds multiple pointers to the GC. MUST BE ALLOCATED USING malloc/calloc */
/* NOTE: C++11 is not fully supported in VC++2010. I'm using va_lists. */
void MemoryManager::MultiAddToGC(int args, ...)
{
	if(args < 1)
		return;
	
	va_list myThings;

#ifdef _DEBUG
		MainDebugger.Log("Adding %i element to GC", "Info", args);
#endif

		va_start(myThings, args);

		for(int x = 0; x < args; x++)
			MemoryManager::GarbageCollector.push_back(va_arg(myThings, void*));

		va_end(myThings);

	return;
}

/* Cleans the GC, freeing the memory. */
void MemoryManager::Flush()
{
	int lasterror = 0;

#ifdef _DEBUG
		MainDebugger.Log("Call to MemoryManager::Flush() with %i items in the GC", "Info", MemoryManager::GarbageCollector.size());
#endif

	for(unsigned int x = 0; x < MemoryManager::GarbageCollector.size(); x++)
	{
		errno = lasterror = 0;
		free(GarbageCollector[x]);
		lasterror = errno;
		if(lasterror > 0)
			MainDebugger.Log("MemoryManager::Flush() Error: %s (%i : %p)", "Error", strerror(lasterror), x, GarbageCollector[x]);
	}
	GarbageCollector.clear();
	return;
}