#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "example-functions.h"

// Called before anything else
// Return the number of thread workers to start
int
gtw_prepare(int argc, char ** argv)
{
    if (argc == 1) {
        return 1;
    } else {
        return atoi(argv[1]);
    }
}

// Prepare the next peice of data for work
// Return zero means stop/done
int
gtw_next(struct gtw_data* run_data)
{
    memset(run_data, 0, sizeof(struct gtw_data));

    static int x = 1000;
    run_data->some_data = x;
    x--;
    return (x > 0);
}

// Do the work on the data
void
gtw_run(struct gtw_data* run_data)
{
    printf("%d\n", run_data->some_data);
}

