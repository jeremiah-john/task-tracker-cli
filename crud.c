#define _XOPEN_SOURCE
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "jsonObjectCreation.h"
#include "crud.h"
#include "status.h"
struct task_t {
	int id;
	char description[50];
	enum Status status;
	struct tm createdAtTime;
	struct tm updatedAtTime;
} tasks[100] = {0};
int nextAvailableTaskIndex = 0;
int nextAvailableTaskID = 1;
enum Status statusStrToEnum(char *statusStr)
{
	if(strcmp(statusStr,"To-do") == 0)
	{
		return todo;
	}
	if(strcmp(statusStr,"In Progress") == 0)
	{
		return inProgress;
	}
	if(strcmp(statusStr,"Halted") == 0)
	{
		return halted;
	}
	if(strcmp(statusStr,"Done") == 0)
	{
		return done;
	}
	return nullStatus;
}
void statusEnumToStr(char *statusStr,int index)
{
	switch(tasks[index].status){
		case todo: strcpy(statusStr, "To-do");break;
		case inProgress: strcpy(statusStr, "In Progress");break;
		case halted: strcpy(statusStr,"Halted");break;
		case done: strcpy(statusStr, "Done");break;
		case nullStatus: strcpy(statusStr, "NULL");break;
	}
}
void jsonObjToTask(char *jsonObjStr)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];
	//%s stops after a whitespace, which is problematic for capturing the string values in our JSON object. we will use scansets instead ([^])
	sscanf(jsonObjStr,"{\"id\":%d,\"description\":\"%50[^\"]\",\"status\":\"%15[^\"]\",\"createdAt\":\"%50[^\"]\",\"updatedAt\":\"%50[^\"]\"}\n",&tasks[nextAvailableTaskIndex].id,tasks[nextAvailableTaskIndex].description,statusStr,createdAtTimeStr,updatedAtTimeStr);
	if(strptime(createdAtTimeStr, "%a %b %d %I:%M:%S %Y",&tasks[nextAvailableTaskIndex].createdAtTime) == NULL) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	if(strptime(updatedAtTimeStr, "%a %b %d %I:%M:%S %Y",&tasks[nextAvailableTaskIndex].updatedAtTime) == NULL) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	//don't forget to set the staus enum of the task!
	nextAvailableTaskID = tasks[nextAvailableTaskIndex].id + 1;
	nextAvailableTaskIndex++;
}
void taskToJSONObj(char *jsonObjStr, int index)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];
	int numCharsWritten; //sprintf returns number of characters written, not including null characters

	//tm must be set for strptime to work
	struct tm tm;
	memset(&tm, 0, sizeof(tm));
	if(tasks[index].id != 0)
	{
		statusEnumToStr(statusStr,index);
		if(strftime(createdAtTimeStr,sizeof(createdAtTimeStr), "%a %b %d %I:%M:%S %Y",&tasks[index].createdAtTime) == 0) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	if(strftime(updatedAtTimeStr,sizeof(updatedAtTimeStr),"%a %b %d %I:%M:%S %Y",&tasks[index].updatedAtTime) == 0) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	//using just %s instead of %.*s since we can be sure the createdAt and UpdatedAt strings are null terminated (unless?)
		numCharsWritten = sprintf(jsonObjStr,"{\"id\":%d,\"description\":\"%s\",\"status\":\"todo\",\"createdAt\":\"%s\",\"updatedAt\":\"%s\"}\n",tasks[index].id,tasks[index].description,
														  statusStr,
														  createdAtTimeStr,
														  updatedAtTimeStr);
	}
}

int createTask(char *taskDesc)
{
	time_t currTime;
	time(&currTime);
	struct tm *tempTM; //for holding results of gmtime, to then copy to member struct tms of new task struct
	tasks[nextAvailableTaskIndex].id = nextAvailableTaskID;
	strcpy(tasks[nextAvailableTaskIndex].description, taskDesc);
	tasks[nextAvailableTaskIndex].status = todo;
	tempTM = gmtime(&currTime);
	tasks[nextAvailableTaskIndex].createdAtTime = *tempTM;
	tempTM = gmtime(&currTime);
	tasks[nextAvailableTaskIndex].updatedAtTime = *tempTM;

	return 0;

}

