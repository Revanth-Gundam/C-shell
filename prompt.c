#include "headers.h"
#include "prompt.h"

void prompt(char *home) {
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");    
    // On every line when waiting for user input, a shell prompt of the following form must appear along with it.
    // get the username using getlogin
    char *username = getlogin();
    // get the hostname using gethostname
    char hostname[1024];
    gethostname(hostname, 1024);
    // get the current working directory using getcwd
    char cwd[1024];
    getcwd(cwd, 1024);
    // printf("prompt cwd: %s\n", cwd);

    // get the home directory from main file
    // char *home = home;
    // replace the home directory with ~
    if (strstr(cwd, home) != NULL) {
        char *temp = strstr(cwd, home);
        char *new_cwd = (char *)malloc(1024 * sizeof(char));
        strcpy(new_cwd, "~");
        strcat(new_cwd, temp + strlen(home));
        strcpy(cwd, new_cwd);
    }
    // print the prompt using a colour


    printf("<"ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "@" ANSI_COLOR_RED "%s" ANSI_COLOR_RESET ":" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "> ", username, hostname, cwd);
    // printf("%s, %s, %s", username, hostname, cwd);

}
