#include "headers.h"

void warp(char* input, char* home, char* prev_dir) {
  char* token = strtok(input, " \t\n");
  // printf("%s\n", token);
  int i = 0;
  while (token != NULL) {
    token = strtok(NULL, " \t\n");
    if (token == NULL) {
      if (i == 0) {
        getcwd(prev_dir, 1024);
        chdir(home);
      }
      break;
    }
    if (strcmp(token, "~") == 0) {
      getcwd(prev_dir, 100);
      chdir(home);

    } else if (strcmp(token, "..") == 0) {
      getcwd(prev_dir, 100);
      chdir("..");

    } else if (strcmp(token, ".") == 0) {
      getcwd(prev_dir, 100);
      continue;
    } else if (strcmp(token, "-") == 0) {
      // go to previous directory which we had been working on before the previous command
      // printf("%s\n", prev_dir);
      // chdir(prev_dir);
      // getcwd(prev_dir, 100);
      char temp[500];
      strcpy(temp, prev_dir);
      chdir(prev_dir);
      strcpy(prev_dir, temp);
    } else {
      // prev_dir = getcwd(prev_dir, 100);
      getcwd(prev_dir, 1024);
      // printf("before cwd: %s\n", prev_dir);
      // printf("%s\n", token);
      char temp[500];
      strcpy(temp, prev_dir);
      strcat(temp, "/");
      strcat(temp, token);
      // printf("temp = %s\n", temp);
      if (chdir(temp)) {
        printf("No such file or directory\n");
        return;
      }
      // getcwd(temp, 1024);
      // printf("after cwd: %s\n", temp);
      // printf("%s\n", prev_dir);
    }
    // break;
    i++;
  }
}