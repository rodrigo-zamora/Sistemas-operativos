#include <stdio.h>
#include "omp.h"

int main() {
    printf("Hello world");

    #pragma omp parallel
    {
        printf(" %d", omp_get_thread_num());
        printf("\n");
    }
}