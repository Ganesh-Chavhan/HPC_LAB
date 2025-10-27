// Sum of array elements
// Compile: mpicc q5.c -o sum
// run code: mpirun --oversubscribe -n 2 ./sum
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    int n = 8;
    int arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int half = n / 2;
    int local_sum = 0;

    if (rank == 0)
        for (int i = 0; i < half; i++)
            local_sum += arr[i];
    else if (rank == 1)
        for (int i = half; i < n; i++)
            local_sum += arr[i];

    int total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Final sum of array = %d\n", total_sum);

    MPI_Finalize();
    return 0;
}
