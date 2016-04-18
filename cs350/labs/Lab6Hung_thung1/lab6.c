#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

float market_value;
int NUM_THREADS;
int MARKET_THRESHOLD;
pthread_mutex_t mutexsum;

void *stock() {
    float price = 100;
    while(true) {
        float fluc = (2 * (float)rand()/(float)RAND_MAX) - 1;
        price += fluc;
        pthread_mutex_lock (&mutexsum);
        market_value += fluc;
        pthread_mutex_unlock (&mutexsum);
    }
    pthread_exit(NULL);
}

void *down_watcher() {
    while(true) {
        pthread_mutex_lock (&mutexsum);
        if(market_value < NUM_THREADS * (100-MARKET_THRESHOLD)) {
            printf("Market down to $%3.2f\n", market_value);
            exit(0);
        }
        pthread_mutex_unlock (&mutexsum);
    }
    pthread_exit(NULL);
}

void *up_watcher() {
    while(true) {
        pthread_mutex_lock (&mutexsum);
        if(market_value > NUM_THREADS * (100+MARKET_THRESHOLD)) {
            printf("Market up to $%3.2f\n", market_value);
            exit(0);
        }
        pthread_mutex_unlock (&mutexsum);
    }
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    if(argc != 3) exit(0);

    NUM_THREADS = atoi(argv[1]);
    MARKET_THRESHOLD = atoi(argv[2]);
    pthread_t stocks[NUM_THREADS];
    pthread_t watchers[2];
    int rc;
    time_t t;
    srand((unsigned) time(&t));

    market_value = NUM_THREADS * 100;
    printf("Initial market value: $%.2f\n", market_value);
    
    //  Create two watcher threads
    rc = pthread_create(&watchers[0], NULL, up_watcher, NULL);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&watchers[1], NULL, down_watcher, NULL);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    for(long t=2; t<NUM_THREADS; t++){
        //printf("In main: creating stock thread %ld\n", (t - 2));
        rc = pthread_create(&stocks[t], NULL, stock, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}
