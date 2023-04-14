#include <sys/sem.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 

#define SEM_ID int

void semwait(int semid)
{
	struct sembuf s;
	s.sem_num=0; // Primero y único elemento del semáforo
	s.sem_op=-1;
	s.sem_flg=SEM_UNDO;
	
	semop(semid,&s,1);
	return;
}
	
void semsignal(int semid)
{
	struct sembuf s;
	s.sem_num=0; // Primero y único elemento del semáforo
	s.sem_op=1;
	s.sem_flg=SEM_UNDO;
	
	semop(semid,&s,1);
	return;
}

int createsem(int key, int value)
{
	int semid;
	semid=semget(key,1,0666|IPC_CREAT); // Solicitar un arreglo de semáforos al SO
	semctl(semid,0,SETVAL,value);	// Inicializar el semáforo en 1
	return(semid);
}

void erasesem(int semid)
{
	semctl(semid,0,IPC_RMID,0);	// Eliminar el arreglo de semaforos
	return;
}

// #define SEMAPHORES_H
// #define SEMAPHORES_H

// #include <sys/types.h>
// #include <sys/ipc.h>
// #include <sys/sem.h>
// #include <stdio.h>

// union semun {
//     int val;
//     struct semid_ds *buf;
//     unsigned short *array;
// };

// int sem_create(key_t key, int init_value) {
//     int sem_id;
//     union semun arg;

//     sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
//     if (sem_id == -1) {
//         perror("sem_create");
//         return -1;
//     }

//     arg.val = init_value;
//     if (semctl(sem_id, 0, SETVAL, arg) == -1) {
//         perror("sem_create: semctl");
//         return -1;
//     }

//     return sem_id;
// }

// int sem_wait(int sem_id) {
//     struct sembuf sb;

//     sb.sem_num = 0;
//     sb.sem_op = -1;
//     sb.sem_flg = 0;

//     if (semop(sem_id, &sb, 1) == -1) {
//         perror("sem_wait");
//         return -1;
//     }

//     return 0;
// }

// int sem_signal(int sem_id) {
//     struct sembuf sb;

//     sb.sem_num = 0;
//     sb.sem_op = 1;
//     sb.sem_flg = 0;

//     if (semop(sem_id, &sb, 1) == -1) {
//         perror("sem_signal");
//         return -1;
//     }

//     return 0;
// }

// int sem_remove(int sem_id) {
//     if (semctl(sem_id, 0, IPC_RMID) == -1) {
//         perror("sem_remove");
//         return -1;
//     }

//     return 0;
// }

// #endif // SEMAPHORES_H
