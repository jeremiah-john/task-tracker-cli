#include <stdio.h>
int nextUniqueID = 0;
int createTask(char *taskDesc)
{
	createObject(++nextUniqueID,taskDesc);
}
int readTask(int taskID);
int updateTask(int taskID, char **newTaskDesc);
int deleteTask(int taskID);
int searchTask(char **taskDesc);

