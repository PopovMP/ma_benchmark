#include <strings.h>
#include <math.h>

void std_dev(
          double * restrict devs,
    const double * const    vals,
    const double * const    mas,
    const int               bars,
    const int               period
) {
    for (int bar = 0; bar < period - 1; ++bar)
        devs[bar] = 0.0;

    const double p_inv = 1.0 / period;
    double sum_sq = 0.0;
    for (int i = 0; i < period; ++i) {
        double valsI = vals[i];
        sum_sq += valsI * valsI;
    }
    double mean     = mas[period - 1];
    double mean_sq  = mean * mean;
    double variance = sum_sq * p_inv - mean_sq;
    devs[period - 1] = variance > 0.0 ? sqrt(variance) : 0.0;
    for (int i = period; i < bars; ++i) {
        double vals_i  = vals[i];
        double vals_ip = vals[i - period];
        sum_sq  += vals_i * vals_i - vals_ip * vals_ip;
        mean     = mas[i];
        mean_sq  = mean * mean;
        variance = sum_sq * p_inv - mean * mean;
        devs[i]  = variance > 0.0 ? sqrt(variance) : 0.0;
    }
}
