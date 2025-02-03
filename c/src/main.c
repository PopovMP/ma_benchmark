#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../lib/data-set.h"
#include "../lib/shared-state.h"
#include "../lib/moving-average.h"
#include "../lib/rsi.h"

void printBar(DataSet *dataSet, int bar) {
    printf("Bar: %d, %ld, %.5f, %.5f, %.5f, %.5f, %d, %d\n", bar + 1,
           dataSet->time   [bar],
           dataSet->open   [bar],
           dataSet->high   [bar],
           dataSet->low    [bar],
           dataSet->close  [bar],
           dataSet->volume [bar],
           dataSet->spreads[bar]);
}

double calcMA(const DataSet *const dataSet, SharedState *restrict sharedState, const int runs) {
    const int           bars  = dataSet->bars;
    const double *const close = dataSet->close;
    double *restrict    res   = sharedState->numA;

    double sum = 0.0;

    for (int i = 0; i < runs; ++i) {
        for (int period = 1; period < 200; ++period) {
            simpleMA(res, close, bars, period, 0);
            sum += res[bars - 1];
        }
    }

    return sum;
}

double calcRSI(const DataSet *const dataSet, SharedState *restrict sharedState, const int runs) {
    const int           bars  = dataSet->bars;
    const double *const close = dataSet->close;
    double *restrict    res   = sharedState->numA;

    double sum = 0.0;
    for (int i = 0; i < runs; ++i) {
        for (int period = 1; period < 200; ++period) {
            rsi(res, close, bars, period);
            sum += res[bars - 1];
        }
    }

    return sum;
}

void benchMA(const DataSet *const dataSet, SharedState *restrict sharedState, const int runs) {
    const clock_t begin = clock();

    const double sumMA = calcMA(dataSet, sharedState, runs);

    const clock_t end = clock();

    printf("MA time: %.2f seconds\n", (end - begin)/1000000.0);
    printf("MA Sum %.5f\n", sumMA);
}

void benchRSI(const DataSet *const dataSet, SharedState *restrict sharedState, const int runs) {
    const clock_t begin = clock();

    const double sumRSI = calcRSI(dataSet, sharedState, runs);

    const clock_t end = clock();

    printf("RSI time: %.2f seconds\n", (end - begin)/1000000.0);
    printf("RSI Sum %.5f\n", sumRSI);
}

int main(void) {
    const char *filePath = "./data/EURUSD15.lb";

    DataSet *dataSet = readDataSet(filePath, 5);
    if (dataSet == NULL) {
        return EXIT_FAILURE;
    }

    printBar(dataSet, dataSet->bars-1);

    SharedState *sharedState = allocSharedState(dataSet->bars);

    if (sharedState == NULL) {
        printf("Cannot allocate the SharedSate\n");
        freeDataSet(dataSet);
        dataSet = NULL;
        return EXIT_FAILURE;
    }

    benchMA (dataSet, sharedState, 10);
    benchMA (dataSet, sharedState, 10);
    benchRSI(dataSet, sharedState, 10);
    benchRSI(dataSet, sharedState, 10);

    freeSharedState(sharedState);
    freeDataSet(dataSet);

    return EXIT_SUCCESS;
}
