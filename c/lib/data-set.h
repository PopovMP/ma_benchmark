#ifndef DATA_SET_H
#define DATA_SET_H

#include <time.h>

typedef struct {
    int     bars;
    time_t *time;
    double *open;
    double *high;
    double *low;
    double *close;
    int    *volume;
    int    *spreads;
} DataSet;

DataSet *read_data_set(const char * const file_path, const int digits);

void free_data_set(DataSet * restrict data);

#endif
