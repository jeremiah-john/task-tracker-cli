#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jsonObjectCreation.h"
#include "crud.h"
#include "status.h"

//we'll make this a doubly linked list so that deleted a node is simpler
struct task_t {
	int id;
	char description[50];
	enum Status status;
	struct tm createdAtTime;
	struct tm updatedAtTime;
	struct task_t *previousTask;
	struct task_t *nextTask;
};
//hopefully, firstTaskAddress will point to the first task address that was allocated. we need this to traverse the linked list
struct task_t *currentTaskAddress;
struct task_t *returnCurrentTaskAddress()
{
	return currentTaskAddress;
}
void initializeTask(struct task_t *task)
{
	task->id = -1;
	task->status = nullStatus;
	//we will leave linking of Task Node to other nodes in linked list to calling function
}
void initializeLinkedList()
{
	currentTaskAddress = malloc(sizeof(struct task_t));
	if(currentTaskAddress == NULL)
	{
		perror("ERROR: could not allocate memory for tasks\n");
		return;
	}
	currentTaskAddress->previousTask = NULL;
	currentTaskAddress->nextTask = NULL;
	initializeTask(currentTaskAddress);
}
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
void statusEnumToStr(char *statusStr,struct task_t *task)
{
	switch(task->status){
		case todo: strcpy(statusStr, "To-do");break;
		case inProgress: strcpy(statusStr, "In Progress");break;
		case halted: strcpy(statusStr,"Halted");break;
		case done: strcpy(statusStr, "Done");break;
		case nullStatus: strcpy(statusStr, "NULL");break;
	}
}
void addTaskNodeToList()
{
}
void makeNewTask()
{
	struct task_t *newTask = malloc(sizeof(struct task_t));
	initializeTask(newTask);
	newTask->previousTask = currentTaskAddress;
	newTask->nextTask = NULL;
	currentTaskAddress = newTask;
}
void jsonObjToTask(char *jsonObjStr)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];
	//check if jsonObjStr is empty
	if(jsonObjStr[0] == '\0') {return;}
	if(currentTaskAddress->id >= 0) //if currentTaskNode has already been filled, then make another!
	{
		makeNewTask();
	}
	//%s stops after a whitespace, which is problematic for capturing the string values in our JSON object. we will use scansets instead ([^])
	sscanf(jsonObjStr,"{\"id\":%d,\"description\":\"%50[^\"]\",\"status\":\"%15[^\"]\",\"createdAt\":\"%50[^\"]\",\"updatedAt\":\"%50[^\"]\"}\n",&currentTaskAddress->id,currentTaskAddress->description,statusStr,createdAtTimeStr,updatedAtTimeStr);


	if(strptime(createdAtTimeStr, "%a %b %d %I:%M:%S %Y",&(currentTaskAddress->createdAtTime)) == NULL) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	if(strptime(updatedAtTimeStr, "%a %b %d %I:%M:%S %Y",&(currentTaskAddress->updatedAtTime)) == NULL) //if we can't properly convert the date and time
	{
		perror("Error reading date and time!\n");
	}
	currentTaskAddress->status = statusStrToEnum(statusStr);

	addTaskNodeToList();
	
}

void deleteTask(struct task_t *task)
{
	if(task->previousTask != NULL){task->previousTask->nextTask = task->nextTask;}
	if(task->nextTask != NULL){task->nextTask->previousTask = task->previousTask;}
	if(task == currentTaskAddress){currentTaskAddress = task->previousTask;}
	free(task);
}
void taskToJSONObj(char *jsonObjStr, struct task_t *task)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];
	int numCharsWritten; //sprintf returns number of characters written, not including null characters

	//tm must be set for strptime to work
	struct tm tm;
	memset(&tm, 0, sizeof(tm));

	if(task->id != 0)
	{
		statusEnumToStr(statusStr,task);
		if(strftime(createdAtTimeStr,sizeof(createdAtTimeStr), "%a %b %d %I:%M:%S %Y",&(task->createdAtTime)) == 0) //if we can't properly convert the date and time
		{
			perror("Error reading date and time!\n");
		}
		if(strftime(updatedAtTimeStr,sizeof(updatedAtTimeStr),"%a %b %d %I:%M:%S %Y",&(task->updatedAtTime)) == 0) //if we can't properly convert the date and time
		{
			perror("Error reading date and time!\n");
		}
	//using just %s instead of %.*s since we can be sure the createdAt and UpdatedAt strings are null terminated (unless?)
		numCharsWritten = sprintf(jsonObjStr,"{\"id\":%d,\"description\":\"%s\",\"status\":\"todo\",\"createdAt\":\"%s\",\"updatedAt\":\"%s\"}\n",task->id,task->description,
														  createdAtTimeStr,
														  updatedAtTimeStr);
	}
	deleteTask(task);
}

int createTask(char *taskDesc)
{
	time_t currTime;
	time(&currTime);
	struct tm *tempTM; //for holding results of gmtime, to then copy to member struct tms of new task struct

	if(currentTaskAddress->id >= 0) //if currentTaskNode has already been filled, then make another!
	{
		makeNewTask();
	}
	currentTaskAddress->id = nextAvailableTaskID;
	strcpy(currentTaskAddress->description, taskDesc);
	currentTaskAddress->status = todo;
	tempTM = gmtime(&currTime);
	currentTaskAddress->createdAtTime = *tempTM;
	tempTM = gmtime(&currTime);
	currentTaskAddress->updatedAtTime = *tempTM;

	addTaskNodeToList();
	return 0;

}
struct task_t *getTaskByID(int taskID)
{
	struct task_t *taskToReturn = currentTaskAddress;
	while(taskToReturn != NULL)
	{
		if(taskToReturn->id == taskID)
		{
			break;
		}
		taskToReturn = taskToReturn->previousTask;
	}
	return taskToReturn;
}
int updateTask(int taskID, char *newTaskDesc)
{
	time_t currTime;
	time(&currTime);
	struct tm *tempTM; //for holding results of gmtime, to then copy to member struct tms of task struct

    struct task_t *taskToUpdate = getTaskByID(taskID);
	strcpy(taskToUpdate->description, newTaskDesc);
	tempTM = gmtime(&currTime);
	taskToUpdate->updatedAtTime = *tempTM;

	return 0;
}
