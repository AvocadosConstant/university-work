#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <stdbool.h>

#include "lab2.h"

int intcmp(const void * a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

void print_usage_string(void) {
    char c;
    FILE *file = fopen("res/usage.txt", "rt");
    while((c = getc(file)) != EOF) {
        printf("%c",c);   
    }
    fclose(file);
}

void child_process(int level, int num_children) {
        for(int i = 0; i < level; i++) fprintf(stderr, "\t");
        fprintf(stderr, "Level %d Child process created.\n", level);
        pid_t pid; /* for a child process */
        if(level > 0) {
            for(int i = 0; i < num_children; i++) {
                pid = fork();
                if(pid < 0) { /* error */
                    fprintf(stderr, "Fork failed.");
                    return;
                }
                if(pid == 0) { /* child process */
                    child_process(level - 1, num_children);
                }
                else { /* parent process */
                    parent_process(level, num_children);
                }
            }
        }
        else {  //  Leaf node
            
        }
        //for(int i = 0; i < level; i++) fprintf(stderr, "\t");
        execl("/bin/bash","c","echo \"\"",NULL);
}

void parent_process(int level, int num_children) {
        /* parent will wait for the child to complete */
        //fprintf(stderr, "Level %d Parent process created.\n", level);
        wait(NULL);
        //printf("Child Complete.\n");
        //fprintf(stderr, "Level %d Parent process completes.\n", level);
}

int main(int argc, char **argv) {
    
    static int num_levels = 0;      //  [0,4]
    static int num_children = 1;    //  [1,3]
    static int sleep_time = 1;      //  Time in seconds
    bool ps_check = false;

    extern char *optarg;
    int opt = 0;
	while((opt = getopt(argc, argv, "uN:M:ps:")) != -1) {
		switch(opt) {
			case 'u':{      //  prints the usage string
                print_usage_string();
                return 0;
            }
			case 'N':
                if((num_levels = atoi(optarg)) < 0 || num_levels > 4) {
                    fprintf(stderr, "Number of levels out of bounds.\n");   
                    return 0;
                }
				break;
			case 'M':
                if((num_children = atoi(optarg)) < 1 || num_children > 3) {
                    fprintf(stderr, "Number of children out of bounds.\n");   
                    return 0;
                }
				break;
			case 'p':
                if(ps_check) fprintf(stderr, "Cannot specify both -p and -s flags.\n");
                ps_check = true;
				break;
			case 's':
                if(ps_check) fprintf(stderr, "Cannot specify both -p and -s flags.\n");
                ps_check = true;
                if((sleep_time = atoi(optarg)) < 0) {
                    fprintf(stderr, "Sleep time cannot be negative.\n");
                }
				break;
			default:
                print_usage_string();
				return 0;		
		}
	}

    if(num_levels == 0) {
        return 0;
    }

    pid_t pid; /* for a child process */
    pid = fork();

    if(pid < 0) { /* error */
        fprintf(stderr, "Fork failed.");
        return 1;
    }
    if(pid == 0) { /* child process */
        child_process(num_levels - 1, num_children);
    }
    else { /* parent process */
        parent_process(num_levels, num_children);
    }
    return 0;
}

