#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/wait.h>

#define MAXCMDLEN 80
#define MAXARGS 5

void execute(char *cmd);

int main()
{
    char cmd[MAXCMDLEN];
    pid_t pid;

    while (1)
    {
        printf("\033[0;35mShell \033[0;36m%s>\033[0m ", getcwd(NULL, 0));
        scanf("%[^\n]%*c", cmd);
        printf("\n");
        if (strcmp(cmd, "exit") == 0)
        {
            exit(1);
        }
        execute(cmd);
    }
}

void execute(char *cmd)
{
    char *args[MAXARGS + 1];
    char *token;
    int i = 0;

    // Tokenizar la cadena de comandos
    token = strtok(cmd, " ");
    while (token != NULL)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        if (strcmp(args[0], "cd") == 0)
        {
            chdir(args[1]);
            return;
        }

        // Si el argumento es & entonces el proceso se ejecuta en background
        if (strcmp(args[1], "&") == 0)
        {
            args[1] = NULL;
            pid_t pid = fork();
            if (pid == 0)
            {
                execvp(args[0], args);
                exit(1);
            }
            else
            {
                return;
            }
        }

        if (i == 1)
        {
            execlp(args[0], args[0], NULL);
        }
        else
        {
            execvp(args[0], args);
        }

        exit(1);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}