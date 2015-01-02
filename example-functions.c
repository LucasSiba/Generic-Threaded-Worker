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

    char line_buf[1024];

    if (fgets(line_buf, sizeof(line_buf), stdin)) {
        int x;
        for (x = 0; line_buf[x] != '\n' && line_buf[x] != '\0'; x++) {
            run_data->buf[x] = line_buf[x];
        }
        run_data->buf[x] = '\0';
        return 1;
    }

    return 0;
}


// Do the work on the data
void
gtw_run(struct gtw_data* run_data)
{
    FILE         * fp;
    unsigned int   bytes_read    = 0;
    unsigned int   bytes_written = 0;
    char           cmd[1024];
    char           result[1024];

    snprintf(cmd, sizeof(cmd), "dig +short %s", run_data->buf);

    fp = popen(cmd, "r");
    if (fp == NULL) { 
        // assert?
        return;
    }

    while ((bytes_read = fread(result + bytes_written, sizeof(char), (sizeof(result) - bytes_written - 1), fp)) != 0) {
        bytes_written += bytes_read;
    }

    result[bytes_written] = '\0';
    pclose(fp);

    printf("%s\n%s\n", run_data->buf, result);
}

