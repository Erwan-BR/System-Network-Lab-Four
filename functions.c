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

int sendTableAndCheckEquality(segment** message, int* semid_dispo, int* semid_init, int* semid_res, int request)
{
    // 1. Acquire seg_disp
    acq_sem(*semid_dispo, seg_dispo);

    // 2. Initialization of segment and acquisition of seg_init
    segment* s_req = malloc(segsize);
    s_req->pid = getpid();
    s_req->req = request;

    if (EXIT_FAILURE == createRandomTable(s_req->tab))
    {
        perror ("ERROR on createSegment: ");
        return EXIT_FAILURE;
    }

    **message = *s_req;
    acq_sem(*semid_init, seg_init);

    // 3. Wait res_ok.
    wait_sem(*semid_res, res_ok);

    // 4. Read the result and free seg_init
    long resultFromServer = (*message)->result;
    lib_sem(*semid_init, seg_init);

    // 5. Wait for liberation of res_ok
    acq_sem(*semid_res, res_ok);
    lib_sem(*semid_res, res_ok);

    // 6. Free reg_dispo
    lib_sem(*semid_dispo, seg_dispo);

    // 7. Compute mean (client and server side)
    long mean = 0;
    for (int i = 0; i < maxval ; i++)
    {
        mean += s_req->tab[i];
    }
    mean /= maxval;

    if (mean != resultFromServer)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}