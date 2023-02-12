
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>

#define NTHREADS 6
int shutdown();
int main()
{

    // Elimina el archivo "shutdown" si existe
    if (access("shutdown", F_OK) != -1)
    {
        remove("shutdown");
    }

    pid_t child_pids[NTHREADS];
    while (shutdown())
    {
        for (int i = 0; i < NTHREADS; i++)
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                // Ejecutar getty en una nueva ventana xterm
                execlp("xterm", "xterm", "-bg", "black", "-fg", "white", "-fa", "Monospace", "-fs", "14", "-e", "./getty.out", NULL);
                perror("Error al ejecutar xterm");
                exit(1);
            }
            else if (pid > 0)
            {
                child_pids[i] = pid;
            }
            else
            {
                perror("Error al crear el proceso hijo");
                exit(1);
            }
        }
        // Esperar a que todos los procesos hijos terminen
        for (int i = 0; i < NTHREADS; i++)
        {
            waitpid(child_pids[i], NULL, 0);
        }
        // Matar a todos los procesos hijos
        for (int i = 0; i < NTHREADS; i++)
        {
            kill(child_pids[i], SIGKILL);
        }
    }
    return 0;
}

int shutdown()
{
    int noApagar = 1;

    // Verifica si el archivo "shutdown" existe, si existe, eliminarlo
    if (access("shutdown", F_OK) != -1)
    {
        noApagar = 0;
        remove("shutdown");
    }
    return noApagar;
}
// int main()
// {
//     pid_t child_pids[NTHREADS];
//     while (shutdown())
//     {
//         for (int i = 0; i < NTHREADS; i++)
//         {
//             pid_t pid = fork();

//             if (pid == 0)
//             {
//                 // Ejecutar getty en una neuva ventana xterm
//                 execlp("xterm", "xterm","-bg","black","-fg","white","-fa", "Monospace", "-fs", "14", "-e","./getty.out", NULL);
//                 perror("Error al ejecutar xterm");
//             }
//             else
//             {
//                 perror("Error al crear el proceso hijo");
//             }
//         }
//         // Esperar a que todos los procesos hijos terminen
//         for (int i = 0; i < NTHREADS; i++)
//         {
//             waitpid(child_pids[i], NULL, 0);
//         }
//         // Matar a todos los procesos hijos
//         for (int i = 0; i < NTHREADS; i++)
//         {
//             kill(child_pids[i], SIGKILL);
//         }
//     }
// }

// int shutdown()
// {
//     int noApagar = 1;

//     // Verifica si el archivo "shutdown" existe
//     if (access("shutdown", F_OK) != -1)
//     {
//         noApagar = 0;
//         // Elimina el archivo "shutdown"
//         if (remove("shutdown") != 0)
//         {
//             perror("Error al eliminar el archivo");
//             exit(1);
//         }
//         exit(0);
//     }
//     return noApagar;
// }
