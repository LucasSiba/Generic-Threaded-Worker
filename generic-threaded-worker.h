#ifndef __GENERIC_THREADED_WORKER_H__
#define __GENERIC_THREADED_WORKER_H__

int   gtw_prepare(int argc, char ** argv);
void* gtw_next(void);
void  gtw_run(void* run_data);

#endif
