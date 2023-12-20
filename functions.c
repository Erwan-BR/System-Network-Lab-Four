#include "./functions.h"

int initialize(segment** message)
{
    int shmid;

    if (-1 == (shmid = shmget(cle, 0, 0)))
    {
        perror("ERROR on shmget: ");
        return EXIT_FAILURE; 
    }
    if (((segment*) -1) == (*message = shmat(shmid, 0, 0)))
    {
        perror("ERROR on shmat: ");
        return EXIT_FAILURE; 
    }
    //struct sembuf sb;
    return EXIT_SUCCESS;
}
