#ifndef functions_h
#define functions_h

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "./moodle/segdef.h"

int initialize(segment** message);

#endif /* functions_h */