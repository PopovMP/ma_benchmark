#include <string.h>

void simpleMA(double* restrict ma_ref, const double* const price,
              const int bars, const int period, const int shift) {
    memset(ma_ref, 0.0, (period + shift - 1) * sizeof(double));

    double sum = 0.0;
    for (int bar = 0; bar < period; ++bar) {
        sum += price[bar];
    }

    double prev = sum / period;
    ma_ref[period + shift - 1] = prev;

    for (int bar = period, len = bars - shift; bar < len; ++bar) {
        register double temp;
        temp  = price[bar];
        temp -= price[bar - period];
        temp /= period;
        prev += temp;
        ma_ref[bar + shift] = prev;
    }
}
