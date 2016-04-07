#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_THREADS 50
float market_value;
pthread_mutex_t mutexsum;

void *stock(void *threadid)
{
    long tid;
    tid = (long)threadid;
    //printf("Hello World! It's me, thread #%ld!\n", tid);

    float price = 100;

    for(int i = 0; i < 1000; i++) {
        float fluc = (2 * (float)rand()/(float)RAND_MAX) - 1;
        price += fluc;
        pthread_mutex_lock (&mutexsum);
        market_value += fluc;
        pthread_mutex_unlock (&mutexsum);
        printf("Stock %ld:\tStock fluctuation: % .2f\tStock Price: $% 3.2f\tMarket Value: $% 3.2f\n", tid, fluc, price, market_value);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    
    market_value = NUM_THREADS * 100;
    printf("Initial market value: $%.2f\n", market_value);
    
    //  Create two watcher threads

    for(long t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, stock, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
