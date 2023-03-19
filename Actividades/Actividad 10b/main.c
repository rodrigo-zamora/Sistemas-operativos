#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CAPACIDAD 20
#define SOFA 4
#define SILLA_BARBERO 3
#define COORD 3

sem_t sem_max_capacidad;
sem_t sem_sofa;
sem_t sem_silla_barbero;
sem_t sem_coord;
sem_t sem_cliente_listo;
sem_t sem_terminado;
sem_t sem_dejar_silla_b;
sem_t sem_pago;
sem_t sem_recibo;

void *cliente(void *arg) {
    int id = *(int*) arg;
    printf("Cliente %d ha llegado a la barbería.\n", id);

    sem_wait(&sem_max_capacidad);
    printf("Cliente %d entra en la tienda.\n", id);

    sem_wait(&sem_sofa);
    printf("Cliente %d se sienta en el sofá.\n", id);

    sem_wait(&sem_silla_barbero);
    printf("Cliente %d se levanta del sofá.\n", id);
    sem_post(&sem_sofa);

    printf("Cliente %d se sienta en la silla del barbero.\n", id);
    sem_post(&sem_cliente_listo);

    sem_wait(&sem_terminado);
    printf("Cliente %d se levanta de la silla del barbero.\n", id);
    sem_post(&sem_dejar_silla_b);

    printf("Cliente %d paga.\n", id);
    sem_post(&sem_pago);

    sem_wait(&sem_recibo);
    printf("Cliente %d sale de la tienda.\n", id);
    sem_post(&sem_max_capacidad);

    pthread_exit(NULL);
}

void *barbero(void *arg) {
    while (1) {
        sem_wait(&sem_cliente_listo);
        sem_wait(&sem_coord);

        printf("Barbero está cortando el pelo del cliente.\n");
        sleep(2);

        sem_post(&sem_coord);
        sem_post(&sem_terminado);
        printf("\033[1;32mBarber\033[0m: Barbero terminó de cortarel pelo del cliente.\n");
        sem_wait(&sem_dejar_silla_b);
        sem_post(&sem_silla_barbero);
    }

    pthread_exit(NULL);
}

void *cajero(void *arg) {
    while (1) {
        sem_wait(&sem_pago);
        sem_wait(&sem_coord);
        printf("\033[1;36mCashier\033[0m: Cajero está aceptando el pago.\n");
        sleep(1);

        sem_post(&sem_coord);
        sem_post(&sem_recibo);
    }

    pthread_exit(NULL);
}

int main() {
    sem_init(&sem_max_capacidad, 0, MAX_CAPACIDAD);
    sem_init(&sem_sofa, 0, SOFA);
    sem_init(&sem_silla_barbero, 0, SILLA_BARBERO);
    sem_init(&sem_coord, 0, COORD);
    sem_init(&sem_cliente_listo, 0, 0);
    sem_init(&sem_terminado, 0, 0);
    sem_init(&sem_dejar_silla_b, 0, 0);
    sem_init(&sem_pago, 0, 0);
    sem_init(&sem_recibo, 0, 0);

    pthread_t th_clientes[MAX_CAPACIDAD];
    pthread_t th_barberos[3];
    pthread_t th_cajero;

    int ids[MAX_CAPACIDAD];
    for (int i = 0; i < MAX_CAPACIDAD; i++) {
        ids[i] = i + 1;
        pthread_create(&th_clientes[i], NULL, cliente, &ids[i]);
    }
    for (int i = 0; i < 3; i++) {
        pthread_create(&th_barberos[i], NULL, barbero, NULL);
    }
    pthread_create(&th_cajero, NULL, cajero, NULL);

    for (int i = 0; i < MAX_CAPACIDAD; i++) {
        pthread_join(th_clientes[i], NULL);
    }

    pthread_cancel(th_barberos[0]);

    sem_destroy(&sem_max_capacidad);

    return 0;

}