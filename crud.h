int createTask(char *taskDesc);
/*int readTask(int taskID);
int updateTask(int taskID, char **newTaskDesc);
int deleteTask(int taskID);
int searchTask(char *taskDesc);
int getNextAvailableTaskIndex();*/
void jsonObjToTask(char *jsonObjStr);
void taskToJSONObj(char *jsonObjStr, int index);
