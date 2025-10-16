// Ring communication
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size, send_data, recv_data;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int left = (rank - 1 + size) % size;
    int right = (rank + 1) % size;

    send_data = rank;

    MPI_Send(&send_data, 1, MPI_INT, right, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_data, 1, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received %d from process %d\n", rank, recv_data, left);

    MPI_Finalize();
    return 0;
}
