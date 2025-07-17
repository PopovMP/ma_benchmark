"use strict";

const dataSet = require("./data/EURUSD_M15.json");

main();

function main() {
    const state = {
        numA: new Array(dataSet.bars).fill(0.0),
        numB: new Array(dataSet.bars).fill(0.0),
        numC: new Array(dataSet.bars).fill(0.0),
        numD: new Array(dataSet.bars).fill(0.0),
    };

    setTimeout(benchMA,      0, state, 10);
    setTimeout(benchMA,      2, state, 10);
    setTimeout(benchRSI,     4, state, 10);
    setTimeout(benchRSI,     6, state, 10);
    setTimeout(benchStdDev,  8, state, 10);
    setTimeout(benchStdDev, 10, state, 10);
}

function simpleMA(maRef, price, period, shift) {
    let sum, bar, prev, temp, len, periodInv;
    maRef.fill(0.0, 0, period + shift - 1);

    sum = 0.0;
    bar = 0;
    while (bar < period) {
        sum += price[bar];
        ++bar;
    }

    prev = sum / period;
    maRef[period + shift - 1] = prev;

    bar = period;
    len = price.length - shift
    periodInv = 1.0 / period;
    while (bar < len) {
        temp  = price[bar];
        temp -= price[bar - period];
        temp *= periodInv;
        prev += temp;
        maRef[bar + shift] = prev;
        ++bar;
    }
}

// Before optimization: 6.88 seconds
function rsi(rsi_ref, price, period) {
    let bars, per1, perInv, bar, diff, temp, pos, neg;
    bars   = price.length;
    per1   = period - 1;
    perInv = 1.0 / period;

    rsi_ref[0] = 0.0;
    bar = 1;
    pos = 0.0;
    neg = 0.0;
    while (bar < bars) {
        diff = price[bar] - price[bar - 1];

        temp = pos * per1;
        if (diff > 0.0) {
            temp += diff;
        }
        pos = temp * perInv;

        temp = neg * per1;
        if (diff < 0.0) {
            temp -= diff;
        }
        neg = temp * perInv;

        rsi_ref[bar] = 100.0 - 100.0 * neg / (pos + neg);
        ++bar;
    }
}

function stdDev(stdDevs, vals, mas, period) {
  let len, perInv, sumSq, i, mean, meanSq, variance, valsI, valsIP;
  len    = vals.length;
  perInv = 1.0 / period;
  stdDevs.fill(0.0, 0, period - 1);

  sumSq = 0.0;
  i     = 0;
  while (i < period) {
    valsI  = vals[i];
    sumSq += valsI * valsI;
    ++i;
  }

  mean     = mas[period - 1];
  meanSq   = mean * mean;
  variance = sumSq * perInv - meanSq;
  stdDevs[period - 1] = variance > 0 ? Math.sqrt(variance) : 0;

  i = period;
  while (i < len) {
    valsI    = vals[i];
    valsIP   = vals[i - period];
    sumSq   += valsI * valsI - valsIP * valsIP;
    mean     = mas[i];
    meanSq   = mean * mean;
    variance = sumSq * perInv - mean * mean;
    stdDevs[i] = variance > 0 ? Math.sqrt(variance) : 0;
    ++i;
  }
}

function calcMA(dataSet, sharedState, runs) {
    const bars  = dataSet.bars;
    const close = dataSet.close;
    const res   = sharedState.numA;
    let sum   = 0.0;

    for (let i = 0; i < runs; ++i) {
        for (let period = 1; period < 200; ++period) {
            simpleMA(res, close, period, 0);
            sum += res[bars-1];
        }
    }

    return sum;
}

function calcRSI(dataSet, sharedState, runs) {
    const bars  = dataSet.bars;
    const close = dataSet.close;
    const res   = sharedState.numA;
    let sum   = 0.0;

    for (let i = 0; i < runs; ++i) {
        for (let period = 1; period < 200; ++period) {
            rsi(res, close, period);
            sum += res[bars-1];
        }
    }

    return sum;
}

function calcStdDev(dataSet, sharedState, runs) {
    const bars      = dataSet.bars;
    const close     = dataSet.close;
    const closeMa   = sharedState.numA;
    const stdDevRef = sharedState.numB;

    let sum   = 0.0;

    for (let i = 0; i < runs; ++i) {
        for (let period = 1; period < 200; ++period) {
            simpleMA(closeMa, close, period, 0);
            stdDev(stdDevRef, close, closeMa, period)
            sum += stdDevRef[bars-1];
        }
    }

    return sum;
}

function benchMA(state, runs) {
    const begin = Date.now();

    const actualSum = calcMA(dataSet, state, runs);

    const end     = Date.now();
    const timeSec = (end - begin) / 1000;

    console.log(`MA    : ${timeSec.toFixed(2)} seconds, sum: `, actualSum.toFixed(5));
}


function benchRSI(state, runs) {
    const begin = Date.now();

    const actualSum = calcRSI(dataSet, state, runs);

    const end     = Date.now();
    const timeSec = (end - begin) / 1000;

    console.log(`RSI   : ${timeSec.toFixed(2)} seconds, sum:`, actualSum.toFixed(5));
}

function benchStdDev(state, runs) {
    const begin = Date.now();

    const actualSum = calcStdDev(dataSet, state, runs);

    const end     = Date.now();
    const timeSec = (end - begin) / 1000;

    console.log(`StdDev: ${timeSec.toFixed(2)} seconds, sum:    `, actualSum.toFixed(5));
}
