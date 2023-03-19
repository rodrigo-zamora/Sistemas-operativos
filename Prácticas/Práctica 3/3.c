#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

#define ITERACIONES 20000000000
#define PI 4
#define NUM_HILOS 4 // Número de procesos a crear

double sumas_parciales[NUM_HILOS]; // Arreglo para guardar las sumas parciales de cada hilo

void calcular_suma_parcial(void *arg)
{
    int id_hilo = *((int *)arg);
    int i;
    double num_elevado;
    double *sumatoria_parcial = &sumas_parciales[id_hilo]; // Puntero a la variable de sumatoria parcial del hilo

    // Calcular la suma parcial correspondiente al hilo
    for (i = id_hilo; i < ITERACIONES; i += NUM_HILOS)
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
    int i, j;
    double sumatoria = 0;
    double pi_aproximado;

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

    // Crear procesos hijos para calcular las sumas parciales
    for (i = 0; i < NUM_HILOS; i++)
    {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        int pid = clone(calcular_suma_parcial,  // Función que ejecutará el proceso hijo
                        malloc(16384) + 16384,  // Pila para el proceso hijo
                        CLONE_VM | CLONE_FILES, // Flags para crear el proceso hijo
                        arg);                   // Argumento que pasamos a la función calcular_suma_parcial
        if (pid == -1)
        {
            perror("Error al crear proceso hijo");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que terminen todos los procesos hijos
    for (i = 0; i < NUM_HILOS; i++)
    {
        wait(NULL);
    }

    // Sumar todas las sumas parciales
    for (j = 0; j < NUM_HILOS; j++)
    {
        sumatoria += sumas_parciales[j];
    }

    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

    printf("Resultado: %lf\n", sumatoria);
    pi_aproximado = sumatoria * PI;
    printf("Aproximación de pi: %lf\n", pi_aproximado);
    elapsed_time = (int)(stop_ts - start_ts);
    printf("Proceso %d, %d microsegundos\n", getpid(), elapsed_time);

    return 0;
}