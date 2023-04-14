#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "semaphores.h"

#define MAX_PRIMES 100

int is_prime(int num);
void buscar_primos(int start, int end, int* shared_mem, SEM_ID sem_id, int msg_qid);
void mostrar_primos(int n1, int n2, int msg_qid);

struct msg_buf {
    long mtype;
    int prime;
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s n1 n2\n", argv[0]);
        return 1;
    }

    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);

    if (n1 < 1 || n2 < 1 || n1 > n2) {
        printf("n1 and n2 must be greater than 1 and n1 must be less than n2.\n");
        return 1;
    }

    int range = n2 - n1 + 1;
    int chunk_size = (range + 3) / 4;

    // Crear segmento de memoria compartida
    int shm_id = shmget(IPC_PRIVATE, range * sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        return 1;
    }

    int* shared_mem = (int*) shmat(shm_id, NULL, 0);
    if (shared_mem == (int*) -1) {
        perror("shmat");
        return 1;
    }

    // Inicializar la memoria compartida con ceros
    for (int i = 0; i < range; i++) {
        shared_mem[i] = 0;
    }

    // Crear semáforo
    SEM_ID sem_id = createsem(IPC_PRIVATE, 1);
    if (sem_id == -1) {
        perror("createsem");
        return 1;
    }

    // Crear cola de mensajes
    int msg_qid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msg_qid == -1) {
        perror("msgget");
        return 1;
    }

    // Crear procesos buscadores
    for (int i = 0; i < 4; i++) {
        int start = n1 + i * chunk_size;
        int end = (i == 3) ? n2 : (start + chunk_size - 1);

        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo (buscador)
            buscar_primos(start, end, shared_mem, sem_id, msg_qid);
            exit(0);
        }
    }

    // Esperar a que todos los procesos buscadores terminen
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    // Mostrar números primos en orden ascendente
    mostrar_primos(n1, n2, msg_qid);

    // Liberar recursos
    shmdt(shared_mem);
    shmctl(shm_id, IPC_RMID, NULL);
    erasesem(sem_id);
    msgctl(msg_qid, IPC_RMID, NULL);

    return 0;
}

void buscar_primos(int start, int end, int* shared_mem, SEM_ID sem_id, int msg_qid) {
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            // Esperar a que el semáforo esté disponible
            semwait(sem_id);

            // Escribir el número primo en la memoria compartida
            shared_mem[i - start] = i;

            // Liberar el semáforo
            semsignal(sem_id);
        }
    }
}

void mostrar_primos(int n1, int n2, int msg_qid) {
    int range = n2 - n1 + 1;
    int primes_found = 0;

    while (primes_found < range) {
        // Leer un mensaje de la cola de mensajes
        struct msg_buf msg;
        if (msgrcv(msg_qid, &msg, sizeof(int), 0, 0) == -1) {
            perror("msgrcv");
            return;
        }

        // Imprimir el número primo
        printf("%d\n", msg.prime);
        primes_found++;
    }

    }

int is_prime(int num) {
    if (num == 1) {
        return 0;
    }

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}

