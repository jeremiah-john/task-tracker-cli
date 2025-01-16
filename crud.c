#define _XOPEN_SOURCE
#define MAX_TASKS 100
#define DEBUG
#define DETAILED_DEBUG //for generating logs that might be lengthy or in-depth
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
} tasks[MAX_TASKS] = {0};
int nextAvailableTaskIndex = 0;
int nextAvailableTaskID = 1;
enum Status statusStrToEnum(char *statusStr)
{
	if(strcmp(statusStr,"To-do") == 0)
	{
		#ifdef DEBUG
			printf("statusStrToEnum: Task status is To-do\n");
		#endif
		return todo;
	}
	if(strcmp(statusStr,"In Progress") == 0)
	{
		#ifdef DEBUG
			printf("statusStrToEnum: Task status is In Progress\n");
		#endif
		return inProgress;
	}
	if(strcmp(statusStr,"Halted") == 0)
	{
		#ifdef DEBUG
			printf("statusStrToEnum: Task status is Halted\n");
		#endif
		return halted;
	}
	if(strcmp(statusStr,"Done") == 0)
	{
		#ifdef DEBUG
			printf("statusStrToEnum: Task status is Done\n");
		#endif
		return done;
	}
	#ifdef DEBUG
		printf("statusStrToEnum: Task status is NULL\n");
	#endif
	return nullStatus;
}
//perhaps lets make this return a string? (be wary of how passing by reference or value works for char arrays)
void statusEnumToStr(char *statusStr,int index)
{
	switch(tasks[index].status){
		case todo:
			#ifdef DEBUG
				printf("statusEnumToStr: Task status is todo\n");
			#endif
			strcpy(statusStr, "To-do");
			break;
		case inProgress:
			#ifdef DEBUG
				printf("statusEnumToStr: Task status is inProgress\n");
			#endif 
			strcpy(statusStr, "In Progress");
			break;
		case halted:
			#ifdef DEBUG
				printf("statusEnumToStr: Task status is halted\n");
			#endif 
			strcpy(statusStr,"Halted");
			break;
		case done:
			#ifdef DEBUG
				printf("statusEnumToStr: Task status is done\n");
			#endif 
			strcpy(statusStr, "Done");
			break;
		case nullStatus:
			#ifdef DEBUG
				printf("statusEnumToStr: Task status is nullStatus\n");
			#endif 
			strcpy(statusStr, "NULL");
			break;
	}
}
int getTaskIndexByID(int taskID)
{
	#ifdef DEBUG
		printf("getTaskIndexByID: searching for task (%d)\n",taskID);
	#endif
	for(int i = 0; i < MAX_TASKS; i++)
	{
		#ifdef DETAILED_DEBUG
				printf("getTaskIndexByID: current index is (%d)\n",i);
		#endif
		if(tasks[i].id == taskID)
		{
			#ifdef DEBUG
				printf("getTaskIndexByID: Task with id (%d) has been found!\n",taskID);
			#endif
			return i;
		}
	}
	#ifdef DEBUG
		printf("getTaskIndexByID: Task with id (%d) has NOT been found!\n",taskID);
	#endif
	return -1;
}

int markTask(int taskID, enum Status newStatus)
{
	int taskIndex = getTaskIndexByID(taskID);
	if (taskIndex == -1)
	{
		printf("ERROR: no task found by that ID!\n");
		return -1;
	}
	tasks[taskIndex].status = newStatus;

	#ifdef DEBUG
		printf("markTask: status (numerical value) of task with ID (%d) is now (%d)\n",taskID, (int) tasks[taskIndex].status);
	#endif
	return 0;
}
void jsonObjToTask(char *jsonObjStr)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];
	//check if jsonObjStr is empty
	if(jsonObjStr[0] == '\0') {return;}
	//%s stops after a whitespace, which is problematic for capturing the string values in our JSON object. we will use scansets instead ([^])
	int numArgsFilled = sscanf(jsonObjStr,"{\"id\":%d,\"description\":\"%50[^\"]\",\"status\":\"%15[^\"]\",\"createdAt\":\"%50[^\"]\",\"updatedAt\":\"%50[^\"]\"}\n",&tasks[nextAvailableTaskIndex].id,tasks[nextAvailableTaskIndex].description,statusStr,createdAtTimeStr,updatedAtTimeStr);
	#ifdef DEBUG
		printf("jsonObjToTask: number of task fields filled for this task is %d\n",numArgsFilled);
	#endif

	if(strptime(createdAtTimeStr, "%a %b %d %I:%M:%S %Y",&tasks[nextAvailableTaskIndex].createdAtTime) == NULL) //if we can't properly convert the date and time
	{
		#ifdef DEBUG
			printf("jsonObjToTask: createdAtTimeStr extracted from json file is: (%s)\n",createdAtTimeStr);
		#endif
		perror("Error reading date and time!\n");
	}
	if(strptime(updatedAtTimeStr, "%a %b %d %I:%M:%S %Y",&tasks[nextAvailableTaskIndex].updatedAtTime) == NULL) //if we can't properly convert the date and time
	{
		#ifdef DEBUG
			printf("jsonObjToTask: updatedAtTimeStr extracted from json file is: (%s)\n",createdAtTimeStr);
		#endif
		perror("Error reading date and time!\n");
	}
	tasks[nextAvailableTaskIndex].status = statusStrToEnum(statusStr);
	nextAvailableTaskID = tasks[nextAvailableTaskIndex].id + 1;
	nextAvailableTaskIndex++;
	#ifdef DEBUG
		printf("jsonObjToTask: status (numerical value) of task with ID (%d) is now (%d)\n",tasks[nextAvailableTaskIndex].id, (int) tasks[nextAvailableTaskIndex].status );
		printf("jsonObjToTask: new values for:\n nextAvailableTaskID: %d\nnextAvailableTaskIndex%d\n",nextAvailableTaskID,nextAvailableTaskIndex);
	#endif
}
void taskToJSONObj(char *jsonObjStr, int index)
{
	char statusStr[15];
	char createdAtTimeStr[50];
	char updatedAtTimeStr[50];

	//tm must be set for strftime to work
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
		int numCharsWritten = printf(jsonObjStr,"{\"id\":%d,\"description\":\"%s\",\"status\":\"%s\",\"createdAt\":\"%s\",\"updatedAt\":\"%s\"}\n",tasks[index].id,tasks[index].description,
														  statusStr,
														  createdAtTimeStr,
														  updatedAtTimeStr);
		#ifdef DEBUG
			printf("taskToJSONObj: number of Characters written to file for task with ID (%d): %d\n",tasks[index].id,numCharsWritten);
		#endif
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

	return tasks[nextAvailableTaskIndex].id;

}

int updateTask(int taskID, char *newTaskDesc)
{
	time_t currTime;
	time(&currTime);
	struct tm *tempTM; //for holding results of gmtime, to then copy to member struct tms of task struct
	#ifdef DEBUG
		printf("updaateTask: attempting to update task with ID (%d)\n", taskID);
	#endif
    int taskIndex = getTaskIndexByID(taskID);
    if (taskIndex == -1)
	{
		printf("ERROR: no task found by that ID!\n");
		return -1;
	}
	strcpy(tasks[taskIndex].description, newTaskDesc);
	tempTM = gmtime(&currTime);
	tasks[taskIndex].updatedAtTime = *tempTM;

	return 0;
}
int deleteTask(int taskID)
{
	int taskIndex = getTaskIndexByID(taskID);
	if (taskIndex == -1)
	{
		printf("ERROR: no task found by that ID!\n");
		return -1;
	}
	for(int i = taskIndex; i < MAX_TASKS - 1; i++)
	{
		tasks[i] = tasks[i+1];
	}
	memset(&tasks[MAX_TASKS-1],0,sizeof(struct task_t));
	return 0;
}
