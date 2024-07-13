#include "headers.h"

int print_flag = 0;

void set_path(char* root, char* cwd, char* path) {
  for (int i = 0;; i++) {
    if (root[i] == '\0') {
      path[0] = '~';
      path[1] = '\0';
      break;
    } else if (cwd[i] == '\0' || cwd[i] != root[i]) {
      break;
    }
  }
  if (path[0] == '~')
    strcat(path, cwd + strlen(root));
  else
    strcpy(path, cwd);
}

int dflag, fflag, fileflag;
char file_name[PATH_MAX];
char dir_path[PATH_MAX];
char temp[PATH_MAX];
char cwd[PATH_MAX];
char _root[PATH_MAX];

void discover_search(char* path) {
  DIR* dir;
  if (!(dir = opendir(path))) return;

  struct dirent* entry;

  char path2[PATH_MAX];
  char temp_path[PATH_MAX];
  while ((entry = readdir(dir)) != NULL) {
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
      continue;
    }
    sprintf(temp_path, "%s/%s", path, entry->d_name);
    set_path(cwd, temp_path, path2);
    if (path2[0] == '~') {
      path2[0] = '.';
    }

    struct stat st2;
    stat(path2, &st2);

    if (S_ISDIR(st2.st_mode)) {
      if (fileflag == 0 && dflag){
        printf("%s\n", path2);
        print_flag=1;
      }
      discover_search(path2);
      continue;
    }

    if (fflag == 0) continue;
    if (fileflag == 0) {
      printf("%s\n", path2);
      print_flag=1;
      continue;
    }

    int len1 = strlen(path2);
    int len2 = strlen(file_name);

    int tempflag = 0;
    for (int i = len1 - 1, j = len2 - 1; i && j; i--, j--) {
      if (path2[i] != file_name[j]) {
        tempflag = 1;
        break;
      }
    }
    if (tempflag) continue;

    printf("%s\n", path2);
    print_flag=1;

  }
}

// void discover(char* root, char** commands, int nargs) {
void seek(char *root, char *input, char *prev_dir) {
  
  print_flag = 0;

  char *token = strtok(input, " \t\n");
  char *commands[100];
  commands[0] = token;
  commands[1] = NULL;
  int i;
  for (i = 0; token != NULL; i++)
  {
    token = strtok(NULL, " \t\n");
    commands[i + 1] = token;
    commands[i + 2] = NULL;
  }

  int nargs = i;

  // printf("commands: \n");
  // for (int i = 0; i < nargs; i++) {
  //   printf("--%s--", commands[i]);
  // }
  // printf("\n");
  
  strcpy(_root, root);
  dflag = 0, fflag = 0, fileflag = 0;

  for (int j = 1; j < nargs; j++) {
    if (!strcmp(commands[j], "-d")) {
      dflag = 1;
      continue;
    }
    if (!strcmp(commands[j], "-f")) {
      fflag = 1;
      continue;
    }
    if (commands[j][0] == '-') {
      fprintf(stderr, ": discover: invalid option --  \'%s\'\n", commands[j] + 1);
      return;
    }
  }

  if (dflag == 0 && fflag == 0){
    dflag = fflag = 1;
  }

  // printf("dflag: %d, fflag: %d\n", dflag, fflag);

  file_name[0] = '\0';
  dir_path[0] = '\0';
  char default_dir_path[PATH_MAX];
  strcpy(default_dir_path, root);

  getcwd(cwd, sizeof(cwd));
  for (int i = 1; i < nargs; i++) {
    // printf("commands[i]: %s\n", commands[i]);
    int len = strlen(commands[i]);
    strcpy(dir_path, default_dir_path);
    strcpy(temp, dir_path);
    if (commands[i][0] == '-') {
      continue;
    }
    else if (commands[i][0] != '.' && commands[i][0] != '~') {
      // printf("hi\n");
      fileflag = 1;
      strcpy(file_name, commands[i]);
      // printf("file_name: %s\n", file_name);
      continue;
    } 
    else if (commands[i][0] == '.'){
      sprintf(dir_path, "%s%s", cwd, commands[i] + 1);
    }
    else if (commands[i][0] == '~') {
      sprintf(dir_path, "%s%s", root, commands[i] + 1);
    }
    else {
      fprintf(stderr, ": discover: invalid option --  \'%s\'\n", commands[i]);
      return;
    }

    if (dir_path[0] == '~') {
      sprintf(dir_path, "%s%s", root, temp + 1);
    }
    else if (dir_path[0] == '.') {
      sprintf(dir_path, "%s%s", cwd, temp + 1);
    }


    chdir(dir_path);
    // printf("dir_path: %s\n", dir_path);
    if (chdir(dir_path) < 0) {
      strcpy(file_name, dir_path);
      fprintf(stderr, ": discover: Target directory -- \'%s\' does not exist\n", temp);
      chdir(cwd);
      continue;
    }
  }

  // printf("file_name: %s\n", file_name);

  char cwd2[PATH_MAX];
  getcwd(cwd2, sizeof(cwd2));

  if (dflag == 1 && fileflag == 0) {
    printf(".\n");
  }

  discover_search(cwd2);

  if (print_flag == 0){
    printf("No Match Found!\n");
  }

  chdir(cwd);
}