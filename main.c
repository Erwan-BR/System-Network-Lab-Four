#include "./main.h"

int main()
{
    printf("Work in progress...\n");

    segment* message = malloc(segsize);
    int* semid_dispo = malloc(sizeof(int));
    int* semid_init = malloc(sizeof(int));
    int* semid_res = malloc(sizeof(int));

    if (EXIT_FAILURE == initialize(&message, semid_dispo, semid_init, semid_res))
    {
        perror ("ERROR on initialize: ");
        return EXIT_FAILURE;
    }
    
    for (int request = 0; max_request > request; request++)
    {
        /*wait_sem(*semid_dispo, seg_dispo);
        acq_sem(*semid_dispo, seg_dispo);*/

        segment* s_req = malloc(segsize);
        s_req->pid = getpid();
        s_req->req = request;
        //s_req.tab = randomTable;
        //long randomTable[maxval];

        if (EXIT_FAILURE == createRandomTable(s_req->tab))
        {
            perror ("ERROR on createSegment: ");
            return EXIT_FAILURE;
        }

        *message = *s_req;
        //acq_sem(*semid_init, seg_init);
        
        long moyenne = 0;
        for (int i = 0; i < maxval ; i++)
        {
            moyenne += s_req->tab[i];
        }
        moyenne /= maxval;

        printf("Moyenne of table at request number %d: %ld\n", request, moyenne);

        wait_sem(*semid_res, res_ok);
    }

    return EXIT_SUCCESS;
}
