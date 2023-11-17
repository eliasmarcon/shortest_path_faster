#include "distributed_spf.h"
#include <stdbool.h>
#include <mpi.h>
int rank, size;

/**
 * @brief The main function of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return int The exit status of the program.
 */
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int attr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (argc > 1)
    {
        attr = atoi(argv[1]);
    }
    else
    {
        attr = 2;
    }
    
    MPI_Finalize();

    return 0;
}