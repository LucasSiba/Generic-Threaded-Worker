#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "generic-threaded-worker.h"

static pthread_mutex_t gtw_next_lock;

static void *
worker_function(void *thread_data)
{
    (void)thread_data;

    while (1) {
        void* run_data;
        pthread_mutex_lock(&gtw_next_lock);
        run_data = gtw_next();
        pthread_mutex_unlock(&gtw_next_lock);

        if (run_data == NULL) {
            break;
        } else {
            gtw_run(run_data);
        }
    }

    pthread_exit(0);
}

int
main(int argc, char ** argv)
{
    int x;
    int worker_count = gtw_prepare(argc, argv);
    pthread_t workers[worker_count];

    pthread_mutex_init(&gtw_next_lock, NULL); 

    for (x = 0; x < worker_count; x++) {
        pthread_create(&workers[x], NULL, &worker_function, NULL); 
    }

    for (x = 0; x < worker_count; x++) {
        pthread_join(workers[x], NULL);
    }
    
    pthread_mutex_destroy(&gtw_next_lock); 

    return 0;
}

