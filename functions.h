#ifndef functions_h
#define functions_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "./moodle/segdef.h"

#define acquisition     -1
#define waiting         0
#define liberation      1

int initialize(segment** message, int* semid_dispo, int* semid_init, int* semid_res);
int createRandomTable(long* randomTable);
int sendTableAndCheckEquality(segment** message, int* semid_dispo, int* semid_init, int* semid_res, int request);
int sem_call(int id, int reference, int op);

#endif /* functions_h */