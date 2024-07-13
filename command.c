// if input string is read as warp go into warp.c
#include "headers.h"

void command(char *input, char *home, char *prev_dir, int add_to_past)
{
    if (add_to_past == 1)
    {
        add_past(home, input);
    }
    char copy2[100];

    strcpy(copy2, input);
    // split using strtok in case of & and ; and store them in commands
    char *commands[100];
    char *token2 = strtok(copy2, "&;");

    int token_i = 0;
    while (token2 != NULL)
    {
        // printf("token2: %s\n", token2);
        commands[token_i] = token2;
        token2 = strtok(NULL, "&;");
        token_i++;
    }
    int bg[token_i]; // foreground is -1 and background is 1
    int che = 0;
    for (int kkk = 0; kkk < token_i; kkk++)
    {
        bg[kkk] = -1;
    }
    for (int kkk = 0; input[kkk] != '\0'; kkk++)
    {
        if (input[kkk] == '&')
        {
            bg[che] = 1;
            che++;
        }
        else if (input[kkk] == ';')
        {
            bg[che] = -1;
            che++;
        }
    }

    // printf("no of commands: %d\n", token_i);

    for (int jj = 0; jj < token_i; jj++)

    {
        // printf ("jj = %d\n", jj);
        // printf ("command = %s\n", commands[jj]);
        int flag = 0;
        for(int ttt = 0; commands[jj][ttt] != '\0'; ttt++)
        {
            if (isspace(commands[jj][ttt]) != 1)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            // printf("skipped\n");
            continue;
        }
        // printf("commands[%d]: %s\n", jj, commands[jj]);
        char copy_input[100];
        strcpy(copy_input, commands[jj]);
        // char* prev_dir = malloc(sizeof(char) * 100);

        char *cwd = malloc(sizeof(char) * 100);
        getcwd(cwd, 100);
        // printf("command cwd: %s\n", cwd);

        char *token = strtok(copy_input, " \t\n");
        char *command = token;

        // if input string is warp then go to warp.c
        if (strcmp(command, "warp") == 0)
        {
            // printf("%s", prev_dir);
            // printf("here\n");
            warp(commands[jj], home, prev_dir);

            // printf("%s in command.c\n", prev_dir);
        }
        else if (strcmp(command, "peek") == 0)
        {
            peek(home, commands[jj]);
        }
        else if (strcmp(command, "proclore") == 0)
        {
            // printf("here");
            proclore(home, commands[jj]);
        }
        // if commands[jj] string is exit then exit the shell
        // else if (strcmp (commands[jj], "exit") == 0)
        // {
        //     exit(0);
        // }if(strcmp(commands[jj], "warp") == 0)
        else if (strcmp(command, "pastevents") == 0)
        {
            past_events(home, commands[jj], prev_dir);
        }
        else if (strcmp(command, "seek") == 0)
        {
            seek(home, commands[jj], prev_dir);
        }

        else
        {
            // printf("Command not found\n");
            int pid = runner(commands[jj], bg[jj], home, prev_dir);
        }
        // token2 = strtok(NULL, "&;");
        // if(token2 != NULL)
        //     {printf("token2: %s\n", token2);}
    }
}