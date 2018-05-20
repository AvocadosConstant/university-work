#include "mytimer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval tv1, tv2;

void start_timer() {
    gettimeofday(&tv1, NULL);
}

void stop_timer() { 
    gettimeofday(&tv2, NULL);
}

void print_elapsed() {   
    fprintf(
        stderr, 
        "Run time elapsed: %f seconds.\n", 
        (double) (tv2.tv_usec - tv1.tv_usec) / 100000 + (double) (tv2.tv_sec - tv1.tv_sec)
    );
}
