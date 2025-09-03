# Benchmark MA and RSI

This repo includes source ocd eof two indicators: Moving Average and RSI, written in three languages: Fortran, C, and JavaScript.

The purpose of the repo is to compare the performance.

Please see the "readme.d" files in the subdirs.

## Usage

Fortran and C

    make clean
    make
    ./main

JavaScript

    node ./main.js

## Results Intel 5 13500

**C**
```
Bar: 200000, 1721414700, 1.08824, 1.08836, 1.08779, 1.08779, 1682, 3
MA    : 0.17 seconds, sum:  2167.24726
MA    : 0.17 seconds, sum:  2167.24726
RSI   : 1.51 seconds, sum: 87365.03064
RSI   : 1.53 seconds, sum: 87365.03064
StdDev: 0.67 seconds, sum:     1.89292
StdDev: 0.68 seconds, sum:     1.89292
```

**Fortran**
```
Bar: 200000, 1721414700, 1.08824, 1.08836, 1.08779, 1.08779, 1682, 3
MA    :  0.17 seconds, sum: 2167.24726
MA    :  0.17 seconds, sum: 2167.24726
RSI   :  1.59 seconds, sum: 87365.03064
RSI   :  1.59 seconds, sum: 87365.03064
StdDev:  0.77 seconds, sum: 1.90622
StdDev:  0.77 seconds, sum: 1.90622
```

**JavaScript**
```
MA    : 0.40 seconds, sum:  2167.24726
MA    : 0.28 seconds, sum:  2167.24726
RSI   : 1.94 seconds, sum: 87365.03064
RSI   : 1.89 seconds, sum: 87365.03064
StdDev: 0.92 seconds, sum:     1.89292
StdDev: 0.80 seconds, sum:     1.89292
```

