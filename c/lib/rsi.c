void rsi(double* restrict rsi_ref, const double* const price,
         const int bars, const int period) {
    rsi_ref[0] = 0.0;
    const int per1 = period - 1;
    double pos = 0.0, neg = 0.0;
    for (int bar = 1; bar < bars; ++bar) {
        register double diff, temp;
        diff  = price[bar];
        diff -= price[bar - 1];

        // Pos
        temp  = pos;
        temp *= per1;
        if (diff > 0.0) {
            temp += diff;
        }
        temp /= period;
        pos   = temp;

        // Neg
        temp  = neg;
        temp *= per1;
        if (diff < 0.0) {
            temp -= diff;
        }
        temp /= period;
        neg   = temp;

        rsi_ref[bar] = 100.0 - 100.0 / (1.0 + pos / neg);
    }
}
