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
#include <sys/signal.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "lab3.h"

//fprintf(stdout, "ALIVE:\t\tLevel 0 process with pid=%d, child of ppid=%d.\n", getpid(), getppid());

void signal_handler(int sig) {
	//printf("Child reached signal handler: %d, signal: %d\n", getpid(), sig);
    fprintf(stdout, "EXITING:\t\tProcess with pid=%d, child of ppid=%d.\n", getpid(), getppid());
	exit(0);
}

void print_arr(int* arr, int size) {
    for(int j = 0; j < size; j++) {
        printf("pid: %d\n", arr[j]);
    }
}

int main(int argc, char **argv) {

    int num_procs = atoi(argv[1]);
    if(num_procs < 1 || num_procs > 32) {
        fprintf(stderr, "<num-procs> out of bounds; must be between 1 and 32 inclusive.\n\n");
        return 0;
    }

    int* pid_list = malloc(sizeof(int) * num_procs);
    pid_list[0] = getpid();

    int buf = num_procs;

    fprintf(stdout, "ALIVE:\t\tLevel %d process with pid=%d, child of ppid=%d.\n", buf, getpid(), getppid());
    
    for(int i = 0; i < num_procs; i++) {
        pid_t pid; /* for a child process */
        int pfds[2];

        if(pipe(pfds) == -1) {
            fprintf(stderr, "Pipe error");
            return 0;
        }

        if((pid = fork()) < 0) { /* error */
            fprintf(stderr, "Fork failed.");
            return 0;
        }

        if(pid == 0) { /* child process */
            signal(SIGUSR1, signal_handler);
            fprintf(stdout, "ALIVE:\t\tLevel %d process with pid=%d, child of ppid=%d.\n", buf - 1, getpid(), getppid());
            close(pfds[1]);
            if(read(pfds[0], &buf, sizeof(int)) <= 0) {
                fprintf(stderr, "Pipe error in child");
                return 0;
            }

            //printf("Child: read \"%d\"\n", buf);
            pid_list[num_procs - buf] = getpid();

            if(buf <= 1) {  //at leaf node
                //printf("At leaf node, reading value of %d from parent's pipe.\n", buf);

                int fd;
                char * myfifo = "/tmp/myfifo";

                /* create the FIFO (named pipe) */
                mkfifo(myfifo, 0666);
                /* write "Hi" to the FIFO */
                fd = open(myfifo, O_WRONLY);
                write(fd, "Hi", sizeof("Hi"));
                close(fd);

                return 0;
            }

        }

        else { /* parent process */
            close(pfds[0]);
            buf = buf - 1;
            if(write(pfds[1], &buf, sizeof(int)) <= 0) {
                fprintf(stderr, "Pipe error in parent");
                return 0;
            }

            if(getpid() == pid_list[0]) {
                sleep(1);

                int fd;
                char * myfifo = "/tmp/myfifo";
                char named_buf[30];

                /* open, read, and display the message from the FIFO */
                fd = open(myfifo, O_RDONLY);
                read(fd, named_buf, 30);
                //printf("Received: %s\n", named_buf);
                close(fd);
                /* remove the FIFO */
                unlink(myfifo);
                if(strcmp("Hi", named_buf) == 0) {

                print_arr(pid_list, num_procs);

                kill(pid, SIGUSR1);
                    
                }
                return 0;
            }
            pause();
        }
    }
    return 0;
}
