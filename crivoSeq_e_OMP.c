#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int num_threads = 4;

void crivo_sequential(int n, int *prime) {
    for (int i = 0; i <= n; i++)
        prime[i] = 1;

    prime[0] = prime[1] = 0;

    for (int p = 2; p <= sqrt(n); p++) {
        if (prime[p] == 1) {
            for (int i = p * p; i <= n; i += p)
                prime[i] = 0;
        }
    }
}

void crivo_openmp(int n, int *prime) {
    for (int i = 0; i <= n; i++)
        prime[i] = 1;

    prime[0] = prime[1] = 0;

    int sqrt_n = (int)sqrt(n);

    #pragma omp parallel num_threads(num_threads)
    {
        int id = omp_get_thread_num();
        int p;

        for (p = 2 + id; p <= sqrt_n; p += num_threads) {
            if (prime[p] == 1) {
                for (int i = p * p; i <= n; i += p)
                    prime[i] = 0;
            }
        }
    }
}

void print_primes(int n, int *prime) {
    for (int i = 2; i <= n; i++) {
        if (prime[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int main() {
    int n = 100000000;
    int *prime_seq = (int *)malloc((n + 1) * sizeof(int));
    int *prime_omp = (int *)malloc((n + 1) * sizeof(int));
    double start, end, time_seq, time_omp;

    // sequencial
    start = omp_get_wtime();
    crivo_sequential(n, prime_seq);
    end = omp_get_wtime();
    time_seq = end-start;
    printf("Tempo sequencial: %f seconds\n", time_seq);
    //print_primes(n, prime_seq); // descomentar para printar os primos

    // OpenMP
    start = omp_get_wtime();
    crivo_openmp(n, prime_omp);
    end = omp_get_wtime();
    time_omp = end-start;
    printf("Tempo OpenMP (%d threads): %f seconds\n", num_threads, time_omp);
    //print_primes(n, prime_omp); // descomentar para printar os primos

    printf("Speedup: %f\n", time_seq/time_omp);
    printf("Eficiencia: %f\n", (time_seq/time_omp)/num_threads);

    // Verificar se as implementacoes dao o mesmo resultado
    int results_match = 1;
    for (int i = 2; i <= n; i++) {
        if (prime_seq[i] != prime_omp[i]) {
            results_match = 0;
            printf("Mismatch at %d: Sequencial = %d, OpenMP = %d\n", i, prime_seq[i], prime_omp[i]);
            break;
        }
    }

    if (results_match) {
        printf("Os resultados sao iguais!\n");
    } else {
        printf("Os resultados sao diferentes.\n");
    }

    free(prime_seq);
    free(prime_omp);

    return 0;
}
