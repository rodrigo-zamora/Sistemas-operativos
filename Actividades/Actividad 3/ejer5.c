/*
Repite el ejercicio anterior, pero ahora no podrás usar la llamada al sistema system(), por lo que será necesario que crees un nuevo proceso (hijo)
el cuál ejecutará el programa que indicaste; Una vez que termine el programa de ejecutarse permitirá al usuario introducir otro programa a ejecutarse,
esto terminará cuando el usuario introduzca exit.
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