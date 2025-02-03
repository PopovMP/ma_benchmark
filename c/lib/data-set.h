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

DataSet *readDataSet(const char *const filePath, const int digits);

void freeDataSet(DataSet *dataSet);

#endif
