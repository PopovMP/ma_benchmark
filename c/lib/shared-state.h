#ifndef SHARED_STATE_H
#define SHARED_STATE_H

typedef struct {
    int     firstBar;
    double *numA;
    double *numB;
    double *numC;
    double *numD;
    double *balance;
    double *equity;
} SharedState;

SharedState *alloc_shared_state(int bars);

void free_shared_state(SharedState *sharedState);

#endif
