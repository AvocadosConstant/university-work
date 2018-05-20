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

#include "lab2.h"

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
    static int num_children = 1;    //  [1,3]
    static int sleep_time = 1;      //  Time in seconds
    bool ps_check = false;          //  True if both -p and -s flags were specified
    bool will_pause = false;        //  True if -p flag was specified

    extern char *optarg;
    int opt = 0;
	while((opt = getopt(argc, argv, "uN:M:ps:")) != -1) {
		switch(opt) {
			case 'u':{      //  prints the usage string
                print_usage_string();
            }
			case 'N':
                if((num_levels = atoi(optarg)) < 0 || num_levels > 4) {
                    fprintf(stderr, "Number of levels (%d) out of bounds.\n", num_levels);   
                    print_usage_string();
                }
				break;
			case 'M':
                if((num_children = atoi(optarg)) < 1 || num_children > 3) {
                    fprintf(stderr, "Number of children out of bounds.\n");   
                    print_usage_string();
                }
				break;
			case 'p':
                if(ps_check) {
                    fprintf(stderr, "Cannot specify both -p and -s flags.\n");
                    print_usage_string();
                }
                ps_check = true;
                will_pause = true;
				break;
			case 's':
                if(ps_check) {
                    fprintf(stderr, "Cannot specify both -p and -s flags.\n");
                    print_usage_string();
                }
                ps_check = true;
                if((sleep_time = atoi(optarg)) < 0) {
                    fprintf(stderr, "Sleep time cannot be negative.\n");
                    print_usage_string();
                }
				break;
			default:
                print_usage_string();
		}
	}

    num_levels--;   //  The root node of an n-level tree will be at level n-1, so num_levels must be decremented

    if(num_levels <= 0) {   //  Leaf node
        if(num_levels == 1) num_levels--;
        fprintf(stdout, "ALIVE:\t\tLevel 0 process with pid=%d, child of ppid=%d.\n", getpid(), getppid());
        if(will_pause) pause();
        else {
            sleep(sleep_time);
        }
        fprintf(stdout, "EXITING:\tLevel 0 process with pid=%d, child of ppid=%d.\n", getpid(), getppid());
        return 0;
    }

    fprintf(stdout, "ALIVE:\t\tLevel %d process with pid=%d, child of ppid=%d.\n", num_levels, getpid(), getppid());

    pid_t pid; /* for a child process */
    for(int i = 0; i < num_children; i++) {
        pid = fork();
        if(pid < 0) { /* error */
            fprintf(stderr, "Fork failed.");
            return 0;
        }
        if(pid == 0) { /* child process */
            char n_str[sizeof(int)];
            sprintf(n_str, "%d", num_levels);
            char m_str[sizeof(int)];
            sprintf(m_str, "%d", num_children);
            char s_str[sizeof(int)];
            sprintf(s_str, "%d", sleep_time);

            if(will_pause) execv("./lab2", (char *[]) {"./lab2", "-N", n_str, "-M", m_str, "-p", NULL});
            else execv("./lab2", (char *[]) {"./lab2", "-N", n_str, "-M", m_str, "-s", s_str, NULL});
        }
        else { /* parent process */
            wait(NULL);
        }
    }
    fprintf(stdout, "EXITING:\tLevel %d process with pid=%d, child of ppid=%d.\n", num_levels, getpid(), getppid());
    return 0;
}

