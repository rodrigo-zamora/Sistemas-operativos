#include <pthread_utils.h>
#include <stdlib.h>
#include <libsem.h>

// La estructura del semáforo
// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B)   __asm__ __volatile__(   \
                            "   lock xchg %1,%0 ;\n"    \
                            : "=ir"  (A)            \
                            : "m"  (B), "ir" (A)        \
                            );

int g=0;
int l = 1;

SEMAPHORE semaphore_get(int val)
{
    // Reservar memoria dinámica para la estructura del semáforo
	SEMAPHORE s = (SEMAPHORE) malloc(sizeof(SEMAPHORE)); 
	// Inicializar el contador del semáforo con el valor de val
    s->count = val; 

	// Crear una cola (ver función queue_create()) y asignarla al apuntador de la cola en el semáforo
    s->queue = queue_create(); 
    return s;
}

void semaphore_destroy(SEMAPHORE s)
{	
	// Destruir la cola
    queue_destroy(s->queue);
	// Liberar la memoria del semáforo
    free(s);
}

void semaphore_wait(SEMAPHORE s)
{

	// Aquí debes implementar la función semaphore_wait() haciendo que el hilo que tiene que esperar
	// se bloquée de manera que no haya espera ocupada
	//	- Antes de bloquearlo hay que:
    int tid = pthread_self(); //	-	1.- Obtener el tid del hilo (pthread_self) y guardarlo en una variable.
    atomic_xchg(l, g ); // Entrar a sección crítica

    // Si el contador del semáforo es menor o igual a cero, encolar el hilo actual y bloquearlo
    if (s->count <= 0) {
        queue_offer(s->queue, (void*)tid); //	-	2.- Guardar el tid en la cola del semáforo con queue_offer
        atomic_xchg(0, g ); //			Un detalle muy importante es que antes de que el hilo se bloquée, debe liberar el atomic_xchg()
        block_thread();  //	-	3.- Finalmente bloquear al hilo (block_thread)
		// Recuerda quye se debe actualizar el contador del semáforo
	// Recuerda que esta función debe ejecutarse de manera atómica
    } 
    // Si el contador del semáforo es mayor a cero, decrementarlo y salir de sección crítica
    else {
        s->count--;
        atomic_xchg(0, g ); // Salir de sección crítica
    }
}

void semaphore_signal(SEMAPHORE s)
{
	// Aquí hay que implementar la función semaphore_signal() para que un hilo que esté bloqueado en
	// Recuerda que esta función debe ejecutarse de manera atómica
    atomic_xchg(1, g ); // Entrar a sección crítica


	// el semáforo s->queue se desbloquée y se actualize el contador del semáforo-
    if (!queue_is_empty(s->queue)) {
        int* tid_ptr = (int*) queue_poll(s->queue); // Obtener el siguiente ID de hilo de la cola
        unblock_thread(*tid_ptr); // Desbloquear el hilo correspondiente
    }
    // Incrementar el contador del semáforo y salir de sección crítica
    s->count++;
    atomic_xchg(0, g ); // Salir de sección crítica
}