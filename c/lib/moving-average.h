#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

void simpleMA(double *restrict maRef, const double *const price,
              const int bars, const int period, const int shift);

#endif
