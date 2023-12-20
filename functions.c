#include "./functions.h"

/// @brief Initializa the generator and the server side.
/// @param message Pointer the the shared memeory space.
/// @param semid_dispo Pointer to the integer containg the information of disponibility.
/// @param semid_init Pointer to the integer containg the information of initialization.
/// @param semid_res Pointer to the integer containg the information of the disponibility of the result (server side).
/// @return EXIT_SUCCESS if everything worked fine, EXIT_FAILURE either.
int initialize(segment** message, int* semid_dispo, int* semid_init, int* semid_res)
{
    int shmid;

    if (-1 == (shmid = shmget(cle, 0, 0)))
    {
        perror("ERROR on shmget: ");
        return EXIT_FAILURE; 
    }
    if (((segment*) - 1) == (*message = shmat(shmid, 0, 0)))
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

/// @brief Write random values in a given table.
/// @param randomTable Table that will contains random values.
/// @return EXIT_SUCCESS if everything worked fine, EXIT_FAILURE either. Should always return EXIT_SUCCESS.
int createRandomTable(long* randomTable)
{
    for (int index = 0; maxval > index; index++)
    {
        randomTable[index] = getrand();
    }
    return EXIT_SUCCESS;
}

/// @brief Send a table to the server and check if the mean computed by the server is the same as the one computed by the client.
/// @param message Pointer to the shared space memory.
/// @param semid_dispo Pointer to the integer containg the information of disponibility.
/// @param semid_init Pointer to the integer containg the information of initialization.
/// @param semid_res Pointer to the integer containg the information of the disponibility of the result (server side).
/// @param request Number of the request, to communicate with the server.
/// @return EXIT_SUCCESS if everything worked fine, EXIT_FAILURE either.
int sendTableAndCheckEquality(segment** message, int* semid_dispo, int* semid_init, int* semid_res, int request)
{
    // 1. Acquire seg_dispo
    //acq_sem(*semid_dispo, seg_dispo);
    if (EXIT_FAILURE == sem_call(*semid_dispo, seg_dispo, acquisition))
    {
        perror ("ERROR on sendTableAndCheckEquality (first step): ");
        return EXIT_FAILURE;
    }

    // 2. Initialization of segment and acquisition of seg_init
    segment* s_req = malloc(segsize);
    s_req->pid = getpid();
    s_req->req = request;

    if (EXIT_FAILURE == createRandomTable(s_req->tab))
    {
        perror ("ERROR on createRandomTable: ");
        return EXIT_FAILURE;
    }

    **message = *s_req;
    //acq_sem(*semid_init, seg_init);
    if (EXIT_FAILURE == sem_call(*semid_init, seg_init, acquisition))
    {
        perror ("ERROR on sendTableAndCheckEquality (second step): ");
        return EXIT_FAILURE;
    }

    // 3. Wait res_ok.
    //wait_sem(*semid_res, res_ok);
    if (EXIT_FAILURE == sem_call(*semid_res, res_ok, waiting))
    {
        perror ("ERROR on sendTableAndCheckEquality (third step): ");
        return EXIT_FAILURE;
    }

    // 4. Read the result and free seg_init
    long resultFromServer = (*message)->result;
    //lib_sem(*semid_init, seg_init);
    if (EXIT_FAILURE == sem_call(*semid_init, seg_init, liberation))
    {
        perror ("ERROR on sendTableAndCheckEquality (fourth step): ");
        return EXIT_FAILURE;
    }

    // 5. Wait for liberation of res_ok
    //acq_sem(*semid_res, res_ok);
    //lib_sem(*semid_res, res_ok);
    if (EXIT_FAILURE == sem_call(*semid_res, res_ok, acquisition))
    {
        perror ("ERROR on sendTableAndCheckEquality (fifth step): ");
        return EXIT_FAILURE;
    }
    if (EXIT_FAILURE == sem_call(*semid_res, res_ok, liberation))
    {
        perror ("ERROR on sendTableAndCheckEquality (fifth step): ");
        return EXIT_FAILURE;
    }

    // 6. Free reg_dispo
    // lib_sem(*semid_dispo, seg_dispo);
    if (EXIT_FAILURE == sem_call(*semid_dispo, seg_dispo, liberation))
    {
        perror ("ERROR on sendTableAndCheckEquality (sixth step): ");
        return EXIT_FAILURE;
    }

    // 7. Compute mean (client and server side)
    long mean = 0;
    for (int i = 0; i < maxval ; i++)
    {
        mean += s_req->tab[i];
    }
    mean /= maxval;

    if (mean != resultFromServer)
    {
        printf("ERROR on sendTableAndCheckEquality (means doesn't match).\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/// @brief Implement our own version of the communication with the server. Replace lib_sem, acq_sem and wait_sem.
/// @param id ID of the semoperation that is performed.
/// @param reference Reference of the structure.
/// @param op Operation that is performed. Should be acquisition, waiting or liberation.
/// @return EXIT_SUCCESS if everything (the call to semop in fact) worked fine, EXIT_FAILURE either.
int sem_call(int id, int reference, int op)
{
    struct sembuf sb;

    sb.sem_num = reference;
    sb.sem_op = op;
    sb.sem_flg = 0;

    if (-1 == semop(id, &sb, 1))
    {
        perror("ERROR on wait_sem_is: ");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
