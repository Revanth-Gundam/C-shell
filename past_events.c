#include "headers.h"

void past_events(char *home, char *input, char *prev_dir)
{
    char path[1024];
    long unsigned size = 1000;
    strcpy(path, home);
    strcat(path, "/.pastevents");
    int no_commands = 0;
    char *commands[20];
    commands[0] = NULL;

    FILE *file = fopen(path, "r");
    if (file)
    {
        for (int i = 0; i < 15; i++)
        {
            commands[i + 1] = NULL;
            if (getline(&commands[i], &size, file) > 0)
            {
                no_commands++;
            }
            else
            {
                break;
            }
        }
        fclose(file);
    }
    char *token = strtok(input, " \t\n");

    int i = 0;
    while (token != NULL)
    {
        token = strtok(NULL, " \t\n");
        if (token == NULL)
        {
            if (i == 0)
            {
                // do the exact pastevents function and print 15 last commands
                for (int j = no_commands - 1; j >= 0; j--)
                {
                    printf("%s", commands[j]);
                }
                break;
            }
            break;
        }
        else if (strcmp(token, "purge") == 0)
        {
            // delete all the past events
            FILE *file = fopen(path, "w");
            fclose(file);
        }
        else if (strcmp(token, "execute") == 0)
        {
            token = strtok(NULL, " \t\n");
            // Execute the command at position in pastevents (ordered from most recent to oldest)
            int position = atoi(token);
            if (position > no_commands)
            {
                printf("Invalid position\n");
                break;
            }
            else
            {
                // do later
                // printf("position = %d\n",position);
                char *comm = commands[position - 1];
                // printf("command: %s", command);
                char copy[1024];
                strcpy(copy, comm);
                char *token = strtok(copy, " \t\n");
                char *command_name = token;
                command(comm, home, prev_dir, 0);
                add_past(home, comm);
                // printf("command_name: %s\n", command_name);

                //     if (strcmp (command_name, "warp") == 0)
                //     {
                //         // printf("command: %s", command);
                //         warp(command, home, prev_dir);
                //     }
                //     else if (strcmp (command_name, "peek") == 0)
                //     {
                //         peek(home, command);
                //     }
                //     else if (strcmp (command_name, "proclore") == 0)
                //     {
                //         proclore(home, command);
                //     }
                //     // else if (strcmp (command_name, "pastevents") == 0)
                //     // {
                //     //     past_events(home, command);
                //     // }
                //     else
                //     {
                //         printf("Command not found\n");
                //     }
            }
        }
        i++;
    }
}

void add_past(char *home, char *input)
{
    // if pastevents is a command itself then do not save it into the file, even in multiple commands together
    char copy_input[1000];
    int boole = 0;
    // char *commands[100];
    char new_command[1000];
    char copy_input_commands[1000];
    strcpy(copy_input_commands, input);
    char *token2 = strtok(copy_input_commands, "&;");
    while (token2 != NULL)
    {
        sscanf(token2, "%s", new_command);
        if((strcmp(new_command, "pastevents") == 0))
        {
            boole = 1;
            return;
        }
        // commands[no_commands] = token2;
        token2 = strtok(NULL, "&;");
    }

    strcpy(copy_input, input);
    char *token = strtok(copy_input, " \t\n");
    if (strcmp(token, "pastevents") == 0)
    {
        return;
    }



    char path[1024];
    long unsigned size = 1000;
    strcpy(path, home);
    strcat(path, "/.pastevents");
    int no_commands = 0;
    char *commands[20];
    commands[0] = NULL;
    // printf("%s\n", path);

    FILE *file = fopen(path, "r");
    if (file)
    {
        for (int i = 0; i < 15; i++)
        {
            commands[i + 1] = NULL;
            if (getline(&commands[i], &size, file) > 0)
            {
                no_commands++;
            }
            else
            {
                break;
            }
        }
        fclose(file);
    }
    // printf("file created\n");
    // printf("%d\n", no_commands);
    // printf("commands: \n");
    if (file != NULL)
    {
        fclose(file);
    }

    if ((no_commands > 0) && (strcmp(input, commands[0]) == 0))
    {
        // printf("repeated command\n");
        // fclose(file);
        return;
    }
    file = fopen(path, "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    if (no_commands >= 15)
    {
        fprintf(file, "%s", input);
        for (int i = 0; i < 14; i++)
        {
            fprintf(file, "%s", commands[i]);
        }
    }
    else
    {
        int charwrit = fprintf(file, "%s", input);
        if (charwrit <= 0)
        {
            printf("Error writing to file\n");
            return;
        }
        // printf("%d\n", charwrit);
        // printf("%s", input);

        for (int i = 0; i < no_commands; i++)
        {
            fprintf(file, "%s", commands[i]);
        }
    }
    fclose(file);
}
