#include "./main.h"

int main()
{
    printf("Work in progress...\n");

    segment* message;

    if (EXIT_FAILURE == initialize(&message))
    {
        perror ("ERROR on initialize");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
