#ifndef STATUS_H
#define STATUS_H
// the Status enumeration values are bitshifted so that the code to list all tasks and tasks by status is easier to understand
// essentially acting as flags as in C# code
enum Status {
	todo = 1 << 0,
	inProgress = 1 << 1,
	halted = 1 << 2,
	done = 1 << 3,
	nullStatus = 0
};
int markTask(int taskIndex,enum Status newStatus);
int listTask(enum Status searchStatus);
#endif
