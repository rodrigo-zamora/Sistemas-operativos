#include <stdio.h>
#include "omp.h"

int main() {
    int nthreads, tid;
    #pragma omp parallel private(tid)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d de %d calcula la seccion 1", tid, nthreads);
                printf("\n");
            }

            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d de %d calcula la seccion 2", tid, nthreads);
                printf("\n");
            }

            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d de %d calcula la seccion 3", tid, nthreads);
                printf("\n");
            }

            #pragma omp section
            {
                tid = omp_get_thread_num();
                nthreads = omp_get_num_threads();
                printf("El thread %d de %d calcula la seccion 4", tid, nthreads);
                printf("\n");
            }
        }
    }
}
