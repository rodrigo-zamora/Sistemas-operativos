#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/wait.h>

void shutdownfile();

int validateuser(char *user, char *password);

#define BUFSIZE 128

int main()
{
    char input[BUFSIZE];
    int noApagar = 1;
    int isValid = 0;

    while (isValid == 0)
    {

        // Verifica si el archivo "shutdown" existe
        if (access("shutdown", F_OK) != -1)
        {
            printf("El sistema se esta apagando, no se puede iniciar sesion\n");
            exit(1);
        }
        
        char user[BUFSIZE];
        char password[BUFSIZE];
        printf("Ingrese su usuario: ");
        fgets(user, BUFSIZE, stdin);
        user[strlen(user) - 1] = '\0';
        printf("Ingrese su contraseña: ");
        fgets(password, BUFSIZE, stdin);
        password[strlen(password) - 1] = '\0';
        isValid = validateuser(user, password);
    }

    execlp("xterm", "xterm","-fa", "Monospace", "-fs", "14","-bg","black","-fg","white","-e", "./shell.out", NULL);
    perror("Error al ejecutar xterm");
    exit(1);
}

int validateuser(char *user, char *password)
{
    FILE *fp;
    fp = fopen("passwd", "r");

    char line[100];
    int found = 0;
    while (fgets(line, 100, fp) != NULL)
    {
        char currentUsername[20];
        char currentPassword[20];
        sscanf(line, "%[^:]:%s", currentUsername, currentPassword);
        if (strcmp(user, currentUsername) == 0 && strcmp(password, currentPassword) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found)
    {
        printf("Login successful\n");
        return 1;
    }
    else
    {
        printf("Login failed\n");
        return 0;
    }
}