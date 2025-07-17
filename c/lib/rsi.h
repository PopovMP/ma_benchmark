#ifndef RSI_H
#define RSI_H

void rsi(
          double * restrict rsi_ref,
    const double * const    price,
    const          int      bars,
    const          int      period
);

#endif
