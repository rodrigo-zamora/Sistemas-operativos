#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITERACIONES 2000000000
#define PI 4
#define NUM_HILOS 4

double sumatoria = 0;

void calcular_sumatoria(double *sumatoria_parcial, int hilo_id)
{
    double num_elevado;
    int i;
    double inicio = hilo_id * (double)ITERACIONES / NUM_HILOS;
    double fin = (hilo_id + 1) * (double)ITERACIONES / NUM_HILOS;

    for (i = inicio; i < fin; i++)
    {
        num_elevado = pow(-1, i);
        *sumatoria_parcial += (num_elevado / (2 * i + 1));
    }
}

int main()
{
    long long start_ts;
    long long stop_ts;
    int elapsed_time;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

    // Arreglo de enteros para almacenar los IDs de los hilos
    int hilos[NUM_HILOS];
    int i;
    for (i = 0; i < NUM_HILOS; i++)
    {
        hilos[i] = clone((void *)calcular_sumatoria, (void *)&sumatoria, CLONE_VM, (void *)i);
    }

    // Esperar a que terminen todos los hilos
    for (i = 0; i < NUM_HILOS; i++)
    {

        if (hilos[i] == -1)
        {
            perror("Error al crear el hilo");
            exit(1);
        }
        waitpid(hilos[i], NULL, 0);
    }

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

    printf("Resultado: %lf\n", sumatoria);
    double pi_aproximado = sumatoria * PI;
    printf("Aproximación de pi: %lf\n", pi_aproximado);
    elapsed_time = (int)(stop_ts - start_ts);
    printf("Proceso %d, %d microsegundos\n", getpid(), elapsed_time);
    return 0;
}
