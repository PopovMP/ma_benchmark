#include <strings.h>

void init_ma(
          double * restrict ma_ref,
    const double * const    price_ref,
    const int               period,
    const int               shift
) {
    bzero(ma_ref, (period + shift - 1) * sizeof(double));

    double sum = 0.0;
    for (int bar = 0; bar < period; ++bar) {
        sum += price_ref[bar];
    }

    ma_ref[period + shift - 1] = sum / period;
}

void simple_ma(
          double * restrict ma_ref,
    const double * const    price_ref,
    const int               bars,
    const int               period,
    const int               shift
) {
    init_ma(ma_ref, price_ref, period, shift);

    for (int bar = period; bar < bars - shift; ++bar) {
        ma_ref[bar + shift] = ma_ref[bar + shift - 1] +
                              (price_ref[bar] - price_ref[bar - period]) / period;
    }
}
