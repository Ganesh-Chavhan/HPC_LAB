// Compile: gcc -fopenmp q2.c -o mm_scalar
// Run: ./mm_scalar

// This program multiplies a scalar by a matrix.
// It uses OpenMP to parallelize the loop.
// The matrix is initialized with values from 0 to 99.
// The scalar is 2.5.
// The program prints the time taken to multiply the scalar by the matrix.
// It also prints the first and last elements of the resulting matrix.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int rows = 4000, cols = 4000;
    double scalar = 2.5;
    double *A = malloc(rows * cols * sizeof(double));
    double *B = malloc(rows * cols * sizeof(double));

    for (int i=0;i<rows*cols;i++) A[i] = i % 100;

    double t0 = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int idx = 0; idx < rows * cols; ++idx) {
        B[idx] = scalar * A[idx];
    }

    double t1 = omp_get_wtime();
    printf("Time: %f sec\n", t1 - t0);
    printf("Sample B[0]=%f B[last]=%f\n", B[0], B[rows*cols-1]);

    free(A); free(B);
    return 0;
}

