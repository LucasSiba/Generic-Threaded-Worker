#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "example-functions.h"

static void *
worker_function(void *thread_data)
{
    (void)thread_data;
    struct gtw_data data;
    while (gtw_next(&data)) {
        gtw_run(&data); 
    }
    pthread_exit(0);
}


int
main(int argc, char ** argv)
{
    int x;

    int worker_count = gtw_prepare(argc, argv);
    pthread_t workers[worker_count];

    for (x = 0; x < worker_count; x++) {
        pthread_create(&workers[x], NULL, &worker_function, NULL); 
    }

    for (x = 0; x < worker_count; x++) {
        pthread_join(workers[x], NULL);
    }

    return 0;
}
