#include "./main.h"

int main()
{
    long unsigned int numberOfClients = pow(2, MAX_FORK) - 1;
    int numberOfRequests = numberOfClients * MAX_REQUEST;
    printf("We are going to send %d request, from %ld different clients from fork (so %d requests in total).\n", MAX_REQUEST, numberOfClients, numberOfRequests);
    printf("If you don't see any message starting by \"Error\" before retrieving control of the terminal, it means that everything worked perfectly fine!\n\n");
    printf("It can take a little bit of time (around 30 seconds max) for all forks to communicate with the server.\n");

    segment* message/* = malloc(segsize)*/;
    int* semid_dispo = malloc(sizeof(int));
    int* semid_init = malloc(sizeof(int));
    int* semid_res = malloc(sizeof(int));

    // 0. Intialize ids of segment and generator.
    if (EXIT_FAILURE == initialize(&message, semid_dispo, semid_init, semid_res))
    {
        perror ("ERROR on initialize: ");
        return EXIT_FAILURE;
    }
    
    int numberOfErrors = 0;
    pid_t child_pid, wpid;

    for (int forkNumber = 0; MAX_FORK > forkNumber; forkNumber++)
    {
        child_pid = fork();
    }
    for (int request = 0; MAX_REQUEST > request; request++)
    {
        if (EXIT_FAILURE == sendTableAndCheckEquality(&message, semid_dispo, semid_init, semid_res, request))
        {
            printf("Error on request number %d.\n", request);
            numberOfErrors++;
        }
    }
    if (0 != numberOfErrors)
    {
        printf("Errors : %d/%d\n", numberOfErrors, MAX_REQUEST);
    }

    if (0 == child_pid)
    {
        return EXIT_SUCCESS;
    }

    int status = 0;

    // Father wait for his children.
    while (0 < (wpid = wait(&status)));

    // Detach memory
    shmdt(message);

    free(semid_dispo);
    free(semid_init);
    free(semid_res);

    return EXIT_SUCCESS;
}
