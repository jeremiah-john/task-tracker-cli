#ifndef STATUS_H
#define STATUS_H
enum Status {
	todo = 1,
	inProgress = 2,
	halted = 3,
	done = 4,
	nullStatus = 0
};
int markTask(int taskIndex,enum Status newStatus);
#endif
