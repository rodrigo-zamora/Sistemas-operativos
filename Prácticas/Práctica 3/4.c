#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ITERACIONES 2000000000
#define PI 4

int main(){
    long long start_ts;
    long long stop_ts;
    int elapsed_time;
    struct timeval ts;
    int i, num_procesos;
    double num_elevado, pi_aproximado, suma_global = 0;
    double *sumas_parciales;
    pid_t pid;
    key_t clave;
    int shmid;
    size_t tam_memoria;
    
    printf("Introduce el número de procesos a utilizar: ");
    scanf("%d", &num_procesos);
    
    // Creación de la memoria compartida
    tam_memoria = num_procesos * sizeof(double);
    clave = ftok("/bin/ls", 'R');
    shmid = shmget(clave, tam_memoria, 0777 | IPC_CREAT);
    sumas_parciales = (double *) shmat(shmid, 0, 0);
    
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial
    
    for (i = 0; i < num_procesos; i++) {
        pid = fork(); // Se crea un nuevo proceso hijo
        
        if (pid == -1) {
            printf("Error al crear proceso hijo\n");
            return 1;
        } else if (pid == 0) { // Código del proceso hijo
            double sumatoria = 0;
            
            // Cada proceso calcula una parte de la sumatoria
            for (int j = i; j < ITERACIONES; j += num_procesos) {
                num_elevado = pow(-1, j);
                sumatoria += (num_elevado / (2 * j + 1));
            }
            
            sumas_parciales[i] = sumatoria; // Se guarda la sumatoria parcial en la memoria compartida
            exit(0); // Se finaliza el proceso hijo
        }
    }
    
    for (i = 0; i < num_procesos; i++) {
        wait(NULL); // Espera a que todos los procesos hijos terminen
        suma_global += sumas_parciales[i]; // Suma las sumatorias parciales
    }
    
    // Liberación de la memoria compartida
    shmdt((char *) sumas_parciales);
    shmctl(shmid, IPC_RMID, 0);
    
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
    
    printf("Resultado: %lf\n", suma_global);
    pi_aproximado = suma_global * PI;
    printf("Aproximación de pi: %lf\n", pi_aproximado);
    elapsed_time = (int) (stop_ts - start_ts);
    printf("Proceso %d, %d microsegundos\n", getpid(), elapsed_time);
    return 0;
}