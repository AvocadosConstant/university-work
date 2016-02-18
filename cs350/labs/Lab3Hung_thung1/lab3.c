#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "lab3.h"

void print_usage_string(void) {
    char c;
    FILE *file = fopen("res/usage.txt", "rt");
    while((c = getc(file)) != EOF) {
        printf("%c",c);   
    }
    fclose(file);
    exit(0);
}

int main(int argc, char **argv) {

    static int num_levels = 0;      //  [0,4]

    num_levels--;   //  The root node of an n-level tree will be at level n-1, so num_levels must be decremented

    if(num_levels <= 0) {   //  Leaf node
        if(num_levels == 1) num_levels--;
        fprintf(stdout, "ALIVE:\t\tLevel 0 process with pid=%d, child of ppid=%d.\n", getpid(), getppid());
        
        //Do leaf stuff

        fprintf(stdout, "EXITING:\tLevel 0 process with pid=%d, child of ppid=%d.\n", getpid(), getppid());
        return 0;
    }

    fprintf(stdout, "ALIVE:\t\tLevel %d process with pid=%d, child of ppid=%d.\n", num_levels, getpid(), getppid());

    pid_t pid; /* for a child process */
        pid = fork();
        if(pid < 0) { /* error */
            fprintf(stderr, "Fork failed.");
            return 0;
        }
        if(pid == 0) { /* child process */
            execv("./lab2", (char *[]) {"./lab2", NULL});
        }
        else { /* parent process */
            wait(NULL);
        }
    fprintf(stdout, "EXITING:\tLevel %d process with pid=%d, child of ppid=%d.\n", num_levels, getpid(), getppid());
    return 0;
}

