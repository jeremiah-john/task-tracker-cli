//the following functions are utilized in the process of CRUD operations on C object representations of tasks
int createTask(char *taskDesc);
int updateTask(int taskID, char *newTaskDesc);
/*int readTask(int taskID);
int updateTask(int taskID, char *newTaskDesc);
int deleteTask(int taskID);
int searchTask(char *taskDesc);
int getNextAvailableTaskIndex();*/
//these two functions are utilized in the process of converting the tasks from the JSON file to C objects that are easier to modify and vice versa
void jsonObjToTask(char *jsonObjStr);
void taskToJSONObj(char *jsonObjStr, int index);
