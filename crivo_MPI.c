#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

void sieve_mpi(int n, int rank, int size) {
    int sqrt_n = (int)sqrt(n);
    int block_size = (n - sqrt_n) / size + 1;

    int *prime = (int *)malloc((n+1) * sizeof(int));
    int *block_prime = (int *)malloc(block_size * sizeof(int));

    for (int i = 0; i <= n; i++)
        prime[i] = 1;

    prime[0] = prime[1] = 0;

    if (rank == 0) {
        for (int p = 2; p <= sqrt_n; p++) {
            if (prime[p] == 1) {
                for (int i = p * p; i <= n; i += p)
                    prime[i] = 0;
            }
        }
    }

    MPI_Bcast(prime, n + 1, MPI_INT, 0, MPI_COMM_WORLD);

    int start = sqrt_n + 1 + rank * block_size;
    int end = start + block_size - 1;

    if (end > n)
        end = n;

    for (int i = start; i <= end; i++)
        block_prime[i - start] = prime[i];

    for (int p = 2; p <= sqrt_n; p++) {
        if (prime[p] == 1) {
            for (int i = p * p; i <= n; i += p) {
                if (i >= start && i <= end)
                    block_prime[i - start] = 0;
            }
        }
    }

    MPI_Gather(block_prime, block_size, MPI_INT, prime + sqrt_n + 1, block_size, MPI_INT, 0, MPI_COMM_WORLD);

    free(prime);
    free(block_prime);
}

int main(int argc, char *argv[]) {
    int n = 100000000;
    int rank, size;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    start = MPI_Wtime();
    sieve_mpi(n, rank, size);
    end = MPI_Wtime();

    if (rank == 0) {
        printf("MPI Time: %f seconds\n", end - start);
    }

    MPI_Finalize();

    return 0;
}
