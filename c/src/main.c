#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../lib/data-set.h"
#include "../lib/shared-state.h"
#include "../lib/moving-average.h"
#include "../lib/rsi.h"
#include "../lib/std-dev.h"

void print_bar(DataSet *dataSet, int bar) {
    printf("Bar: %d, %ld, %.5f, %.5f, %.5f, %.5f, %d, %d\n", bar + 1,
           dataSet->time   [bar],
           dataSet->open   [bar],
           dataSet->high   [bar],
           dataSet->low    [bar],
           dataSet->close  [bar],
           dataSet->volume [bar],
           dataSet->spreads[bar]);
}

double calc_ma(const DataSet *const data, SharedState *restrict state, const int runs) {
    const int               bars      = data->bars;
    const double * const    close_ref = data->close;
          double * restrict ma_ref    = state->numA;

    double sum = 0.0;

    for (int run = 0; run < runs; ++run) {
        for (int period = 1; period < 200; ++period) {
            simple_ma(ma_ref, close_ref, bars, period, 0);
            sum += ma_ref[bars - 1];
        }
    }

    return sum;
}

double calc_rsi(const DataSet *const data, SharedState *restrict state, const int runs) {
    const int               bars      = data->bars;
    const double * const    close_ref = data->close;
          double * restrict rsi_ref   = state->numA;

    double sum = 0.0;
    for (int run = 0; run < runs; ++run) {
        for (int period = 1; period < 200; ++period) {
            rsi(rsi_ref, close_ref, bars, period);
            sum += rsi_ref[bars - 1];
        }
    }

    return sum;
}

double calc_std_dev(const DataSet *const data, SharedState *restrict state, const int runs) {
    const int               bars         = data->bars;
    const double * const    close_ref    = data->close;
          double * restrict close_ma_ref = state->numA;
          double * restrict std_dev_ref  = state->numB;

    double sum = 0.0;
    for (int run = 0; run < runs; ++run) {
        for (int period = 1; period < 200; ++period) {
            simple_ma(close_ma_ref, close_ref, bars, period, 0);
            std_dev(std_dev_ref, close_ref, close_ma_ref, bars, period);
            sum += std_dev_ref[bars - 1];
        }
    }

    return sum;
}

void bench_ma(const DataSet *const data, SharedState *restrict state, const int runs) {
    const clock_t begin = clock();

    const double sum = calc_ma(data, state, runs);

    const clock_t end = clock();

    printf("MA    : %.2f seconds, sum:  %.5f\n", (end - begin) / 1000000.0, sum);
}

void bench_rsi(const DataSet *const data, SharedState *restrict state, const int runs) {
    const clock_t begin = clock();

    const double sum = calc_rsi(data, state, runs);

    const clock_t end = clock();

    printf("RSI   : %.2f seconds, sum: %.5f\n", (end - begin) / 1000000.0, sum);
}

void bench_std_dev(const DataSet *const data, SharedState *restrict state, const int runs) {
    const clock_t begin = clock();

    const double sum = calc_std_dev(data, state, runs);

    const clock_t end = clock();

    printf("StdDev: %.2f seconds, sum:     %.5f\n", (end - begin) / 1000000.0, sum);
}

int main(void) {
    const char *file_path = "./data/EURUSD15.lb";

    DataSet *data = read_data_set(file_path, 5);
    if (data == NULL) {
        return EXIT_FAILURE;
    }

    print_bar(data, data->bars - 1);

    SharedState *state = alloc_shared_state(data->bars);

    if (state == NULL) {
        printf("Cannot allocate the SharedSate\n");
        free_data_set(data);
        data = NULL;
        return EXIT_FAILURE;
    }

    bench_ma     (data, state, 10);
    bench_ma     (data, state, 10);
    bench_rsi    (data, state, 10);
    bench_rsi    (data, state, 10);
    bench_std_dev(data, state, 10);
    bench_std_dev(data, state, 10);

    free_shared_state(state);
    free_data_set(data);

    return EXIT_SUCCESS;
}
