//  matrix matrix multiplication
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4

int main(int argc, char** argv) {
    int rank, size;
    int A[N][N], B[N][N], C[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;
    int a_sub[rows_per_proc][N];
    int c_sub[rows_per_proc][N];

    // Initialize matrices in rank 0
    if (rank == 0) {
        srand(time(NULL));
        printf("Matrix A:\n");
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
                if (j == N-1) printf("\n");
            }

        printf("Matrix B:\n");
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                B[i][j] = rand() % 10;
                printf("%d ", B[i][j]);
                if (j == N-1) printf("\n");
            }
    }

    // Scatter rows of A
    MPI_Scatter(A, rows_per_proc * N, MPI_INT, a_sub, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);
    // Broadcast matrix B
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Local computation
    for (int i = 0; i < rows_per_proc; i++)
        for (int j = 0; j < N; j++) {
            c_sub[i][j] = 0;
            for (int k = 0; k < N; k++)
                c_sub[i][j] += a_sub[i][k] * B[k][j];
        }

    // Gather results
    MPI_Gather(c_sub, rows_per_proc * N, MPI_INT, C, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result Matrix C = A * B:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%d ", C[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
