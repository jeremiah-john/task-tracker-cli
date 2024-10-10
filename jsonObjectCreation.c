#include <stdio.h>
#include <time.h>
#include "status.h"
#include "jsonObjectCreation.h"
//when we create json object, it should also have status, createdAt, and updatedAt properties	
FILE* json;
time_t globalTime;
struct tm *timeAndDate;

int initJSON()
{
	json = fopen("tasks.json","a+");
	if(json == NULL)
	{
		perror("Failed to open JSON file!\n");
		return 1;
	}
	return 0;
}
int createObject(int id, char *description)
{
	time(&globalTime);
	timeAndDate = localtime(&globalTime);

	int retVal = fprintf(json,"{\"id\":%d,\"description\":%s,\"status\":\"todo\",\"createdAt\":%s,\"updatedAt\":%s}\n",id,description,asctime(timeAndDate),asctime(timeAndDate));
	if(retVal < 0)
	{
		perror("Failed to write JSON object literal to file!");
		return 1;
	}
	return 0;

}
int updateObjectDesc(int id, char **description)
{
	
}

