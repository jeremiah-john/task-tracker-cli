#include <stdio.h>
#include "jsonObjectCreation.h"
#include "crud.h"
int nextUniqueID = 0;

int createTask(char *taskDesc)
{
	createObject(++nextUniqueID,taskDesc);
}

