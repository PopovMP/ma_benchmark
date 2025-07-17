#ifndef STD_DEV_H
#define STD_DEV_H

void
std_dev(
          double * restrict std_dev_ref,
    const double * const    values,
    const double * const    values_ma,
    const int               bars,
    const int               period
);

#endif
