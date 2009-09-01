#include "globals.h"
CRITICAL_SECTION gStreamMutex;

void InitGlobals()
{
	//Init global vars
	gInitMutex(&gStreamMutex);
}