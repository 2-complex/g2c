
#include "appenvironment.h"
#include "memoryapp.h"

#include "macbank.h"

int main()
{
	AppEnvironment e;
	MemoryApp app;
	e.app = &app;
	
	MacFileSystemBank bank;
	app.bank = &bank;
	
	e.mainLoop();
	
	return 0;
}

