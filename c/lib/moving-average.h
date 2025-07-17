#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

void simple_ma(
          double * restrict ma_ref,
    const double * const    price_ref,
    const int               bars,
    const int               period,
    const int               shift
);

#endif
