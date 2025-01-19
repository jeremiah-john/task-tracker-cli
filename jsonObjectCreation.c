#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "jsonObjectCreation.h"
#include "crud.h"
//when we create json object, it should also have status, createdAt, and updatedAt properties	
FILE* json;

int readJSONFile()
{
	json = fopen("tasks.json","r");
	char nextJSONObject[200];

	if(json == NULL)
	{
		perror("Failed to open JSON file!");
		fflush(stderr);
		return 1;
	}

	int nextChar = fgetc(json);
	/* test if file is empty using fgetc
	 * (feof only looks at result of last File I/O operation, based on whether EOF indicator has been set)
	 */
	while(nextChar != EOF)
	{
		if(nextChar == '{')
		{
			ungetc(nextChar,json);
			fgets(nextJSONObject,sizeof(nextJSONObject),json); //nextJSONObject will include newline
			jsonObjToTask(nextJSONObject);
			memset(nextJSONObject,0,sizeof(nextJSONObject)); //paranoid precaution to make sure no leftover chars from last fgets
		}
		nextChar = fgetc(json);
	}
	return fclose(json);
}
int writeJSONFile()
{
	json = fopen("tasks.json","w");
	int taskIndex = 0;
	char jsonObj[200] = {0};
	fprintf(json,"[\n");
	while(1) //while no error is returned trying to convert our task to a JSON object
	{
		taskToJSONObj(jsonObj,taskIndex);
		if(jsonObj[0] == 0) {break;}
		if(taskIndex >= 1){fprintf(json,",\n");} //if we have more than one task, then we need a comma to separate the subsequent JSON Objects
		fprintf(json,jsonObj);
		taskIndex++;
		memset(jsonObj,0,sizeof(jsonObj));
	}
	fprintf(json,"]\n");
	return fclose(json);
}
/*(int createObject(char *description)
{
	time(&globalTime);
	timeAndDate = localtime(&globalTime);
	char timeAndDateStr[50];
	strcpy(timeAndDateStr,asctime(timeAndDate));
	int timeAndDateStrLen = strcspn(timeAndDateStr,"\n");
	//we shall generate a unique ID using rand() and srand() (we are trying to avoid external libraries, such as libuuid)

	int id = getNextAvailableTaskID();
	fseek(json,0,SEEK_END); //so we are at the end of the file and don't overwrite any other objects

	int retVal = fprintf(json,"{\"id\":%d,\"description\":\"%s\",\"status\":\"todo\",\"createdAt\":\"%.*s\",\"updatedAt\":\"%.*s\"}\n",id,description,timeAndDateStrLen,asctime(timeAndDate),timeAndDateStrLen,asctime(timeAndDate));
	if(retVal < 0)
	{
		perror("Failed to write JSON object literal to file!");
		return 1;
	}
	return fclose(json);

}*/


