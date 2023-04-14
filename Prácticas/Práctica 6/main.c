// Incluir las bibliotecas necesarias
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include "semaphores.h"

// Declarar funciones
int is_prime(int num);
void buscar_primos(int start, int end, int* shared_mem, SEM_ID sem_id);

int main(int argc, char *argv[]) {
    // Verificar si el número de argumentos es correcto
    if (argc != 3) {
        printf("Usage: %s n1 n2\n", argv[0]);
        return 1;
    }

    // Obtener los límites del rango a buscar (n1 y n2) desde los argumentos
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);

    // Verificar si los límites del rango son válidos
    if (n1 < 1 || n2 < 1 || n1 > n2) {
        printf("n1 and n2 must be greater than 1 and n1 must be less than n2.\n");
        return 1;
    }

    // Calcular el tamaño del rango y dividir en 4 partes iguales
    int range = n2 - n1 + 1;
    int chunk_size = (range + 3) / 4;

    // Crear un segmento de memoria compartida y adjuntarlo al proceso actual
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

    // Crear un semáforo para controlar el acceso a la memoria compartida
    SEM_ID sem_id = createsem(IPC_PRIVATE, 1);
    if (sem_id == -1) {
        perror("createsem");
        return 1;
    }

    // Crear 4 procesos hijos (buscadores) para buscar números primos en paralelo
    for (int i = 0; i < 4; i++) {
        int start = n1 + i * chunk_size;
        int end = (i == 3) ? n2 : (start + chunk_size - 1);

        pid_t pid = fork();
        if (pid == 0) { // Proceso hijo (buscador)
            buscar_primos(start, end, shared_mem, sem_id);
            exit(0);
        }
    }

    // Esperar a que todos los procesos buscadores terminen
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    // Imprimir los números primos encontrados en orden ascendente
    for (int i = n1 - 1; i < n2; i++) {
        if (shared_mem[i] != 0) {
            printf("%d \n", shared_mem[i]);
        }
    }
    printf("\n");

    // Liberar recursos: desadjuntar la memoria compartida, eliminar el segmento de memoria compartida y eliminar el semáforo
    shmdt(shared_mem);
    shmctl(shm_id, IPC_RMID, NULL);
    erasesem(sem_id);

    return 0;
}

// Función para determinar si un número es primo
int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Función para buscar números primos en un rango específico y almacenarlos en la 
void buscar_primos(int start, int end, int* shared_mem, SEM_ID sem_id) {
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            semwait(sem_id);
            shared_mem[i - 1] = i;
            semsignal(sem_id);
        }
    }
}
