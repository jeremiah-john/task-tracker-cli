#include <stdio.h>
#include <string.h>
int main(int argc, char **argv)
{
	//use strcmp(argv[i], "string") == 0 to compare command arguments
	if(strcmp(argv[1],"add") == 0)
	{
		printf("successful add!\n");
	}


	if(strcmp(argv[1],"update") == 0)
	{
		printf("successful update!\n");
	}
	if(strcmp(argv[1],"delete") == 0)
	{
		printf("successful delete!\n");
	}
	if(strcmp(argv[1],"mark-in-progress") == 0)
	{
		printf("successful mark-in-progress!\n");
	}
	if(strcmp(argv[1],"mark-done") == 0)
	{
		printf("successful mark-done!\n");
	}


	if(strcmp(argv[1],"list") == 0)
	{
		printf("successful list!\n");
	}

	return 0;
}
