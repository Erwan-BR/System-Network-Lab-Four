#include "./functions.h"

int initialize(segment** message, int* semid_dispo, int* semid_init, int* semid_res)
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
    if (-1 == (*semid_dispo = semget(cle, seg_dispo, 0)))
    {
        perror("ERROR on semget (semid_dispo): ");
        return EXIT_FAILURE; 
    }
    if (-1 == (*semid_init = semget(cle, seg_init, 0)))
    {
        perror("ERROR on semget (semid_init): ");
        return EXIT_FAILURE; 
    }
    if (-1 == (*semid_res = semget(cle, res_ok, 0)))
    {
        perror("ERROR on semget (semid_red): ");
        return EXIT_FAILURE; 
    }
    init_rand();
    return EXIT_SUCCESS;
}

int createRandomTable(long* randomTable)
{
    for (int index = 0; maxval > index; index++)
    {
        randomTable[index] = getrand();
    }
    return EXIT_SUCCESS;
}