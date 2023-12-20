#include "./main.h"

int main()
{
    printf("Work in progress...\n");

    segment* message = malloc(segsize);
    int* semid_dispo = malloc(sizeof(int));
    int* semid_init = malloc(sizeof(int));
    int* semid_res = malloc(sizeof(int));

    // 0. Intialize ids of segment and generator.
    if (EXIT_FAILURE == initialize(&message, semid_dispo, semid_init, semid_res))
    {
        perror ("ERROR on initialize: ");
        return EXIT_FAILURE;
    }
    
    for (int request = 0; MAX_REQUEST > request; request++)
    {
        if (EXIT_FAILURE == sendTableAndCheckEquality(&message, semid_dispo, semid_init, semid_res, request))
        {
            printf("Error on request number %d\n", request);
        }
    }
    // Detach memory
    shmdt(message);

    return EXIT_SUCCESS;
}
