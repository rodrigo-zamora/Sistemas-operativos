/*
Repite el ejercicio anterior, pero ahora una vez introducido el nombre del programa, no deberá esperar a que este termine, si no
deberá permitir que se introduzca el nombre de otro programa que también se lanzará a ejecución, es decir,
los programas se ejecutarán en segundo plano.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char command[100];

    while (1) {
        printf("Introduce un comando: ");
        scanf("%s", command);

        if (strcmp(command, "exit") == 0) {
            break;
        }

        if (fork() == 0) {
            execlp(command, command, NULL);
        } else {
            wait(NULL);
        }
    }

    return 0;
    
}