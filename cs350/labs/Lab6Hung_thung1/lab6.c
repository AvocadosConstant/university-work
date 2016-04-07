#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_THREADS     5

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);

    int price = 100;

    for(int i = 0; i < 90; i++) {
        float fluc = (2 * (float)rand()/(float)RAND_MAX) - 1;
        for(long j = 0; j < tid; j++) {
            printf("\t");
        }
        printf("#%ld: %f\n", tid, fluc);
    }
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    sleep(5);
    /* Last thing that main() should do */
    pthread_exit(NULL);
}
