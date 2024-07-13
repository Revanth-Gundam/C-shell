#include "headers.h"

int runner(char *input, int background, char *home, char *prev_dir)
{
    char* token = strtok(input, " \t\n");
    char* args[100];
    args[0] = token;
    args[1] = NULL;
    for(int i = 0; token != NULL; i++)
    {
        token = strtok(NULL, " \t\n");
        args[i + 1] = token;
        args[i + 2] = NULL;
    }

    // printf("bg = %d\n", background);
    int pid = fork();

    if (pid < 0)
    {
        perror("Error");
        return -1;
    }
    else if (pid)
    {
        if (background == 1)
        {
            printf("%s started with PID %d\n", args[0], pid);

            return pid;
        }
        else
        {
            int status;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(0, pid);

            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(0, getpgid(0));

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status))
                return pid;

            if (WEXITSTATUS(status) == EXIT_FAILURE)
                return -1;
        }
    }
    else
    {
        setpgid(0, 0);

        if (execvp(args[0], args) < 0)
        {
            perror("Error");
            exit(1);
        }
    }

    return 0;
}