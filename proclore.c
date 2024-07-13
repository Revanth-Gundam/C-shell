#include "headers.h"

void print_process_info(int pid);

int is_foreground_process(int pid) {
    pid_t terminal_pgid = tcgetpgrp(STDIN_FILENO);
    if (terminal_pgid == -1) {
        perror("tcgetpgrp");
        return -1;
    }

    return (pid_t)pid == terminal_pgid;
}

void proclore(char* home, char* input)
{
    char* token = strtok(input, " \t\n");
    int i = 0;
    while (token != NULL) 
    {
        token = strtok(NULL, " \t\n");
        if(token == NULL)
        {
            if(i == 0)
            {
                // only proclore is given
                pid_t pid = getpid();
                print_process_info(pid);
                break;
            }   
            break;
        }
        else
        {
            int pid = atoi(token);
            print_process_info(pid);
        }
        i++;
    }

}

void print_process_info(int pid) {
    struct kinfo_proc process_info;
    size_t length = sizeof(process_info);
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, pid };

    if (sysctl(mib, 4, &process_info, &length, NULL, 0) == -1) {
        perror("sysctl");
        return;
    }

    char command[100]; // Command to run ps for the given PID
    snprintf(command, sizeof(command), "ps -o stat= -p %d", pid);

    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return;
    }

    char process_status;
    if (fscanf(fp, " %c", &process_status) != 1) {
        perror("fscanf");
        pclose(fp);
        return;
    }

    pclose(fp);

    // printf("pid : %d\n", pid);
    // printf("process status : %c\n", process_info.kp_proc.p_stat);
    printf("pid : %d\n", pid);
    if(is_foreground_process(pid))
    {
        printf("process status : %c+\n", process_status);
    }
    else
    {
        printf("process status : %c\n", process_status);
    }
    printf("Process Group : %d\n", process_info.kp_eproc.e_pgid);

    // printf("Virtual memory : %lu\n", process_info.kp_eproc.e_vm.vd_rss);
    // printf("Virtual memory : %lld\n", (long long)process_info.kp_eproc.e_vmexts.vme_rss);
    // printf("Virtual memory : %llu\n", (unsigned long long)process_info.kp_eproc.e_vm_map_size);
    
    char executable_path[PATH_MAX];
    if (proc_pidpath(pid, executable_path, sizeof(executable_path)) > 0) {
        printf("executable path : %s\n", executable_path);
    } else {
        perror("proc_pidpath");
    }
}