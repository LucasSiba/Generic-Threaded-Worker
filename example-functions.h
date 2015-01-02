#ifndef __EXAMPLE_FUNCTIONS_H__
#define __EXAMPLE_FUNCTIONS_H__

struct gtw_data {
    int some_data;
};

int  gtw_prepare(int argc, char ** argv);
int  gtw_next(struct gtw_data* run_data);
void gtw_run(struct gtw_data* run_data);

#endif
