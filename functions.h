#ifndef functions_h
#define functions_h

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "./moodle/segdef.h"

int initialize(segment** message, int* semid_dispo, int* semid_init, int* semid_res);
int createRandomTable(long* randomTable);

#endif /* functions_h */