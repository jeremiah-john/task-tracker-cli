#include <stdio.h>
#include <time.h>
#include <string.h>
#include "status.h"
#include "jsonObjectCreation.h"
//when we create json object, it should also have status, createdAt, and updatedAt properties	
FILE* json;
time_t globalTime;
struct tm *timeAndDate;

int initJSON()
{
	json = fopen("tasks.json","a+");
	//a+ means that writing to this file is always going to be at the end of the file
	//regardless of where fseek has set the internal file pointer before
	if(json == NULL)
	{
		perror("Failed to open JSON file!\n");
		return 1;
	}
	return 0;
}
int createObject(char *description)
{
	time(&globalTime);
	timeAndDate = localtime(&globalTime);
	char timeAndDateStr[50];
	strcpy(timeAndDateStr,asctime(timeAndDate));
	int timeAndDateStrLen = strcspn(timeAndDateStr,"\n");

	int id = 0;
	fscanf(json,"{\"id\":%d",&id);
	id++;
	fseek(json,0,SEEK_END); //so we are at the end of the file and don't overwrite any other objects

	int retVal = fprintf(json,"{\"id\":%d,\"description\":\"%s\",\"status\":\"todo\",\"createdAt\":\"%.*s\",\"updatedAt\":\"%.*s\"}\n",id,description,timeAndDateStrLen,asctime(timeAndDate),timeAndDateStrLen,asctime(timeAndDate));
	if(retVal < 0)
	{
		perror("Failed to write JSON object literal to file!");
		return 1;
	}
	return fclose(json);

}
int updateObjectDesc(int id, char **description)
{
	
}

