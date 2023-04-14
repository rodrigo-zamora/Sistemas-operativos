#include <stdio.h>
#include "omp.h"

#define CHUNKSIZE 2
#define N 10

int main()
{
    int i, chunk;
    int nthreads, tid;

    int a[N], b[N], c[N];

    for (i = 0; i < N; i++)
        a[i] = b[i] = i * 1.0;

    chunk = CHUNKSIZE;

#pragma omp parallel shared(a, b, c, chunk) private(i, tid)
    {
#pragma omp for schedule(static, chunk)

        for (i = 0; i < N; i++)
        {
            tid = omp_get_thread_num();
            nthreads = omp_get_num_threads();
            c[i] = a[i] + b[i];
            printf("El thread %d, de %d threads, calcula la iteración i = %d\n", tid, nthreads, i);
        }
    }
}