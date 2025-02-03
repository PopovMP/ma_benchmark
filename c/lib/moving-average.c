#include <strings.h>

void initMA(double *restrict ma, const double *const price,
            const int period, const int shift) {
    bzero(ma, (period + shift - 1) * sizeof(double));

    double sum = 0.0;
    for (int bar = 0; bar < period; ++bar) {
        sum += price[bar];
    }

    ma[period + shift - 1] = sum / period;
}

void simpleMA(double *restrict ma, const double *const price,
              const int bars, const int period, const int shift) {
    initMA(ma, price, period, shift);

    for (int bar = period; bar < bars - shift; ++bar) {
        ma[bar + shift] = ma[bar + shift - 1] +
                          (price[bar] - price[bar - period]) / period;
    }
}
