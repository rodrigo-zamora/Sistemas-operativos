#include <stdio.h>
#include "omp.h"

long fib (int n) { return (n < 2) ? n : fib(n-1) + fib(n-2); }

int main() {
    int n = 45;
    #pragma omp parallel
    {
        int t = omp_get_thread_num();
        printf("%d: %ld, ", t, fib(n + t));
    }
    printf("\n");
    return 0;
}