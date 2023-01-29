/*
Desarrolla un programa que solicite al usuario una cadena de caracteres, esa cadena será el nombre de un programa1 a ejecutarse,
una vez que este programa termine de ejecutarse permitirá al usuario introducir otro programa a ejecutarse, esto terminará cuando el usuario
introduzca exit. Para este ejercicio utiliza la llamada al sistema system(). En este ejercicio estamos haciendo nuestro primer shell
o intérprete de comandos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    char command[100];

    while (1) {
        printf("Introduce un comando: ");
        scanf("%s", command);

        if (strcmp(command, "exit") == 0) {
            break;
        }

        system(command);
    }

    return 0;
    
}