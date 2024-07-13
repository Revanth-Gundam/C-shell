#include "headers.h"

int main()
{
    // Keep accepting commands
    // get current working directory and call it home directory
    char home[1024];
    getcwd(home, 1024);
    char curr_dir[1024];
    getcwd(curr_dir, 1024);
    char prev_dir[1024];
    
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(home);
        char input[4096];
        fgets(input, 4096, stdin);
        command(input, home, prev_dir, 1);

    }
}
