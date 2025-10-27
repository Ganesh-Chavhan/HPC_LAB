// Matrix-Vector Multiplication
// Compile: mpicc q1.c -o matvec
// Run: mpirun --oversubscribe -n 4 ./matvec
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4

int main(int argc, char** argv) {
    int rank, size;
    int A[N][N], x[N], y[N];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;

    int a_sub[rows_per_proc][N]; // local matrix A
    int y_sub[rows_per_proc]; // local vector y

    // Initialize matrix and vector only in rank 0
    if (rank == 0) {
        srand(time(NULL));
        printf("Matrix A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }

        printf("Vector x:\n");
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 10;
            printf("%d ", x[i]);
        }
        printf("\n");
    }

    // Scatter rows of A to all processes
    MPI_Scatter(A, rows_per_proc * N, MPI_INT, a_sub, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);
    // Broadcast vector x to all processes
    MPI_Bcast(x, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Local computation
    for (int i = 0; i < rows_per_proc; i++) {
        y_sub[i] = 0;
        for (int j = 0; j < N; j++)
            y_sub[i] += a_sub[i][j] * x[j];
    }

    // Gather results
    MPI_Gather(y_sub, rows_per_proc, MPI_INT, y, rows_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result Vector y = A * x:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", y[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
