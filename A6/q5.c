// Sum of array elements
// mpicc q5.c -o q5
// ./q5
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int n = 8;
    int arr[8] = {2, 4, 6, 8, 10, 12, 14, 16};

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
