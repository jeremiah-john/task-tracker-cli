#include <stdio.h>
#include "status.h"
//when we create json object, it should also have status, createdAt, and updatedAt properties
int createObject(int id, char **description);
int updateObjectDesc(int id, char **description);
int updateObjectStatus(int id, Status status);
