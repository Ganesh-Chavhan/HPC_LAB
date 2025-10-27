// Deadlock between two processes
// Compile: mpicc q3.c -o deadlock
// run code: mpirun --oversubscribe -n 2 ./deadlock
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, data = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = 100;
        // Synchronous send will block until the receiver posts a receive
        printf("P0 sending...\n"); fflush(stdout);
        MPI_Ssend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 received...\n"); fflush(stdout);
    } else if (rank == 1) {
        data = 200;
        // Same order → deadlock occurs
        printf("P1 sending...\n"); fflush(stdout);
        MPI_Ssend(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P1 received...\n"); fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}

