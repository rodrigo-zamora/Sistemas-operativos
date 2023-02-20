#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define PI 4
#define HILOS 4
#define ITERACIONES 2000000000 / HILOS
double sumatoria = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *calc_sum(void *arg)
{
    int i;
    double num_elevado, sumatoria_local = 0;
    int id_hilo = *(int *)arg;

    for (i = id_hilo; i < ITERACIONES; i += HILOS)
    {
        num_elevado = pow(-1, i);
        sumatoria_local += (num_elevado / (2 * i + 1));
    }

    printf("pi = %f\n", sumatoria_local);
    pthread_mutex_lock(&mutex);
    sumatoria += sumatoria_local;
    pthread_mutex_unlock(&mutex);
}

int main()
{
    long long start_ts;
    long long stop_ts;
    int elapsed_time;
    struct timeval ts;
    int i;
    double pi_aproximado;
    pthread_t hilos[HILOS];
    int id_hilos[HILOS];

    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec; 
    // Tiempo inicial
    // INICIO DE HILOS

    for (i = 0; i < HILOS; i++)
    {
        id_hilos[i] = i;
        pthread_create(&hilos[i], NULL, calc_sum,  &id_hilos[i]);
    }

    for (i = 0; i < HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }
    // FIN DE HILOS
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final

    printf("Resultado: %lf\n", sumatoria);
    pi_aproximado = sumatoria * PI;
    printf("Aproximación de pi: %lf\n", pi_aproximado);
    elapsed_time = (int)(stop_ts - start_ts);
    printf("Proceso %d, %d microsegundos\n", getpid(), elapsed_time);
    printf("Segundos: %f\n", (double)elapsed_time / 1000000.0);
    return 0;
}
