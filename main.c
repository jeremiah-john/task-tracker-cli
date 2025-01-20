#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "crud.h"
#include "jsonObjectCreation.h"
#include "status.h"
int main(int argc, char **argv)
{
	//use strcmp(argv[i], "string") == 0 to compare command arguments
	//add will just have argv[2] be the string description of task
	if(readJSONFile() != 0)
	{
		printf("No Previous tasks detected!\n");
	}
	if(argc < 2){
		printf("Hey! give me something to do!\n");
		return EINVAL;
	}
	if(strcmp(argv[1],"add") == 0)
	{
		if(argc < 3)
		{
			printf("Must give description for new task!\n");
			return EINVAL;
		}
		if(argc > 3)
		{
			printf("Too many arguments!\nUse quotation marks (\"\") around description\n");
			return E2BIG;
		}
		int taskCreateResult = createTask(argv[2]);
		if(taskCreateResult > 0)
		{
			printf("Task Added Successfully! (ID: %d)\n",taskCreateResult);

		}
		return writeJSONFile();
	}

	//update will have argv[2] be the task ID(int), and argv[3] be a new string description of task (char*)
	if(strcmp(argv[1],"update") == 0)
	{
		if(argc < 4)
		{
			printf("Must give new description for task!\n");
			return EINVAL;
		}
		if(argc > 4)
		{
			printf("Too many arguments!\nUse quotation marks (\"\") around description\n");
			return E2BIG;
		}
		int taskUpdateResult = updateTask(atoi(argv[2]),argv[3]);
		if(taskUpdateResult == -1)
		{
			printf("Could not update specified task!\n");
			return EINVAL;
		}
		printf("successful update!\n");
		return writeJSONFile();
	}
	//delete will just have argv[2] be task ID(int)
	if(strcmp(argv[1],"delete") == 0)
	{
		if(argc > 3)
		{
			printf("Too many arguments!\nOnly update one task at a time please!"); //we actually could try batch updating and deleting tasks
			return E2BIG;
		}
		if(argc < 3)
		{
			printf("Give a task ID!\n");
			return EINVAL;
		}
		int taskDeleteResult = deleteTask(atoi(argv[2]));
		if(taskDeleteResult == -1)
		{
			printf("Could not delete specified task!\n");
			return EINVAL;
		}
		printf("successful delete!\n");
		return writeJSONFile();
	}
	//mark-in-progress and mark-done will just take argv[2] which is integer of task ID
	if(strcmp(argv[1],"mark-in-progress") == 0)
	{
		enum Status newStatus = inProgress;
		if(argc > 3)
		{
			printf("Too many arguments!\nOnly update one task at a time please!"); //we actually could try batch updating and deleting tasks
			return E2BIG;
		}
		if(argc < 3)
		{
			printf("Give a task ID!\n");
			return EINVAL;
		}
		int taskMarkResult = markTask(atoi(argv[2]),newStatus);
		if(taskMarkResult == -1)
		{
			printf("Could not mark task!\n");
			return EINVAL;
		}
		printf("successful mark-in-progress!\n");
		return writeJSONFile();
	}
	if(strcmp(argv[1],"mark-done") == 0)
	{
		enum Status newStatus = done;
		if(argc > 3)
		{
			printf("Too many arguments!\nOnly update one task at a time please!"); //we actually could try batch updating and deleting tasks
			return E2BIG;
		}
		if(argc < 3)
		{
			printf("Give a task ID!\n");
			return EINVAL;
		}
		int taskMarkResult = markTask(atoi(argv[2]),newStatus);
		if(taskMarkResult == -1)
		{
			printf("Could not mark task!\n");
			return EINVAL;
		}
		printf("successful mark-done!\n");
		return writeJSONFile();
	}
	if(strcmp(argv[1],"mark-halted") == 0)
	{
		enum Status newStatus = halted;
		if(argc > 3)
		{
			printf("Too many arguments!\nOnly update one task at a time please!"); //we actually could try batch updating and deleting tasks
			return E2BIG;
		}
		if(argc < 3)
		{
			printf("Give a task ID!\n");
			return EINVAL;
		}
		int taskMarkResult = markTask(atoi(argv[2]),newStatus);
		if(taskMarkResult == -1)
		{
			printf("Could not mark task!\n");
			return EINVAL;
		}
		printf("successful mark-halted!\n");
		return writeJSONFile();
	}
	//must check if argv[2] is a NULL pointer (in which case, list all tasks), or if it is a string of status of tasks (remember we have an enum for this
	if(strcmp(argv[1],"list") == 0)
	{
		if(argc == 2){
			listTask((todo | inProgress | halted | done));
		}
		else if (argc > 3)
		{
			printf("Too many arguments!\nIf you wish to list all tasks, just use subcommand 'list'\n");
			return E2BIG;
		}
		else
		{
			if(strcmp(argv[2],"done") == 0)
			{
				listTask(done);
			}
			else if(strcmp(argv[2],"todo") == 0)
			{
				listTask(todo);
			}
			else if(strcmp(argv[2],"halted") == 0)
			{
				listTask(halted);
			}
			else if(strcmp(argv[2],"in-progress") == 0)
			{
				listTask(inProgress);
			}
			else
			{
				printf("Incorrect argument for list!\noptions for list: todo,done,in-progress,halted\n");
				return EINVAL;
			}
		}
		return writeJSONFile();
	}

	return 0;
}
