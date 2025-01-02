//the following functions are utilized in the process of CRUD operations on C object representations of tasks
#ifndef CRUD_H
#define CRUD_H
int createTask(char *taskDesc);
int updateTask(int taskID, char *newTaskDesc);
/*int readTask(int taskID);
int updateTask(int taskID, char *newTaskDesc);
int deleteTask(int taskID);
int searchTask(char *taskDesc);
int getNextAvailableTaskIndex();*/
/*Note to self: declare your structures and other data types BEFORE declaring functions which use said data types
  structs are declared as soon as they are named anywhere, so the struct task_t in the parameter list will be treated as it's own declaration
  which will then conflict with our actual intended declaration if it is declared later */
struct task_t;
struct task_t *returnCurrentTaskAddress();
//these two functions are utilized in the process of converting the tasks from the JSON file to C objects that are easier to modify and vice versa
void jsonObjToTask(char *jsonObjStr);
void taskToJSONObj(char *jsonObjStr, struct task_t *task);
void initializeLinkedList();

#endif