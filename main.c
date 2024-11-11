#include <stdio.h>
#include <string.h>
#include "crud.h"
#include "jsonObjectCreation.h"
int main(int argc, char **argv)
{
	//use strcmp(argv[i], "string") == 0 to compare command arguments
	//add will just have argv[2] be the string description of task
	if(readJSONFile() != 0)
	{
		return 1;
	}
	if(strcmp(argv[1],"add") == 0)
	{
		printf("successful add!\n");
		createTask(argv[2]);
		return(writeJSONFile());
	}

	//update will have argv[2] be the task ID(int), and argv[3] be a new string description of task (char*)
	if(strcmp(argv[1],"update") == 0)
	{
		printf("successful update!\n");
	}
	//delete will just have argv[2] be task ID(int)
	if(strcmp(argv[1],"delete") == 0)
	{
		printf("successful delete!\n");
	}
	//mark-in-progress and mark-done will just take argv[2] which is integer of task ID
	if(strcmp(argv[1],"mark-in-progress") == 0)
	{
		printf("successful mark-in-progress!\n");
	}
	if(strcmp(argv[1],"mark-done") == 0)
	{
		printf("successful mark-done!\n");
	}

	//must check if argv[2] is a NULL pointer (in which case, list all tasks), or if it is a string of status of tasks (remember we have an enum for this
	if(strcmp(argv[1],"list") == 0)
	{
		printf("successful list!\n");
	}

	return 0;
}
