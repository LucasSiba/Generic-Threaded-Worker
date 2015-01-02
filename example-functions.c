#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "generic-threaded-worker.h"

#define INPUT_LINE_BUF_SIZE 4096

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
// Return NULL means stop
void*
gtw_next(void)
{
    char* line_buf = malloc(INPUT_LINE_BUF_SIZE);

    if (fgets(line_buf, INPUT_LINE_BUF_SIZE, stdin)) {
        int x;
        for (x = 0; line_buf[x] != '\n' && line_buf[x] != '\0'; x++);
        line_buf[x] = '\0';
        return line_buf;
    }

    return NULL;
}


// Do the work on the data
void
gtw_run(void* run_data)
{
    FILE         * fp;
    unsigned int   bytes_read    = 0;
    unsigned int   bytes_written = 0;
    char           cmd[1024];
    char           result[1024];

    if (run_data == NULL) {
        goto DONE;
    }

    snprintf(cmd, sizeof(cmd), "dig +short %s", (char*)run_data);

    fp = popen(cmd, "r");
    if (fp == NULL) { 
        // assert?
        goto DONE;
    }

    while ((bytes_read = fread(result + bytes_written, sizeof(char), (sizeof(result) - bytes_written - 1), fp)) != 0) {
        bytes_written += bytes_read;
    }

    result[bytes_written] = '\0';
    pclose(fp);

    printf("%s\n%s\n", (char*)run_data, result);

DONE:
    free(run_data);
    return;
}

