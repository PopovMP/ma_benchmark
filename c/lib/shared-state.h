#ifndef SHARED_STATE_H
#define SHARED_STATE_H

typedef struct {
    int     firstBar;
    double* numA;
    double* numB;
    double* numC;
    double* numD;
    double* balance;
    double* equity;
} SharedState;

SharedState* allocSharedState(int bars);

void freeSharedState(SharedState* sharedState);

#endif
