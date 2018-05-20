#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

pthread_mutex_t market_lock;
float market_value;
int NUM_THREADS;
int MARKET_THRESHOLD;
pthread_t up_watcher_thread;
pthread_t down_watcher_thread;

void stock() {
    while(true) {
        // random float between -1 and 1
        pthread_mutex_lock (&market_lock);
            float fluc = (2 * (float)rand()/(float)RAND_MAX) - 1;
            market_value += fluc;
        pthread_mutex_unlock (&market_lock);
    }
}

void down_watcher() {
    while(true) {
        pthread_mutex_lock (&market_lock);
            if(market_value < NUM_THREADS * (100-MARKET_THRESHOLD)) {
                fprintf(stdout, "Market down to $%3.2f\n", market_value);
                pthread_kill(up_watcher_thread, SIGINT);
                pthread_mutex_unlock (&market_lock);
                pthread_exit(0);
            }
        pthread_mutex_unlock (&market_lock);
    }
}

void up_watcher() {
    while(true) {
        pthread_mutex_lock (&market_lock);
            if(market_value > NUM_THREADS * (100+MARKET_THRESHOLD)) {
                fprintf(stdout, "Market up to $%3.2f\n", market_value);
                pthread_kill(down_watcher_thread, SIGINT);
                pthread_mutex_unlock (&market_lock);
                pthread_exit(0);
            }
        pthread_mutex_unlock (&market_lock);
    }
}

int main (int argc, char *argv[]) {
    if(argc != 3) exit(0);

    NUM_THREADS = atoi(argv[1]);
    MARKET_THRESHOLD = atoi(argv[2]);

    pthread_t stocks[NUM_THREADS];
    int rc;
    time_t t;
    srand((unsigned) time(&t));

    market_value = NUM_THREADS * 100;
    printf("Initial market value: $%.2f\n", market_value);
    
    //  Create two watcher threads
    rc = pthread_create(&up_watcher_thread, NULL, (void *)up_watcher, NULL);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    rc = pthread_create(&down_watcher_thread, NULL, (void *)down_watcher, NULL);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    for(long t=2; t<NUM_THREADS; t++){
        //printf("In main: creating stock thread %ld\n", (t - 2));
        rc = pthread_create(&stocks[t], NULL, (void *)stock, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_join(up_watcher_thread, NULL);
    pthread_join(down_watcher_thread, NULL);

    // Kill the stocks
    for(long t=2; t < NUM_THREADS; t++) {
        pthread_kill(stocks[t], SIGINT);
    }

    /* Last thing that main() should do */
    pthread_exit(0);
}
