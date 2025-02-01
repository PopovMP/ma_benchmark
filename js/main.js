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

    setTimeout(benchMA,   0, state,  10);
    setTimeout(benchMA,   3, state,  10);
    setTimeout(benchRSI,  9, state,  10);
    setTimeout(benchRSI, 15, state,  10);
}

function simpleMA(maRef, price, period, shift) {
    maRef.fill(0.0, 0, period + shift - 1);

    let sum = 0.0;
    for (let bar = 0; bar < period; ++bar) {
        sum += price[bar];
    }

    let prev = sum / period;
    maRef[period + shift - 1] = prev;

    for (let bar = period, len = price.length - shift; bar < len; ++bar) {
        let temp;
        temp  = price[bar];
        temp -= price[bar - period];
        temp /= period;
        prev += temp;
        maRef[bar + shift] = prev;
    }
}

// Before optimization: 6.88 seconds
function rsi(rsi_ref, price, period) {
    const bars = price.length;
    const per1 = period - 1;
    let pos  = 0.0;
    let neg  = 0.0;

    rsi_ref[0] = 0.0;

    let bar = 1;
    let diff, temp;
    while (bar < bars) {
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
        ++bar;
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

function benchMA(state, runs) {
    const begin = Date.now();

    const actualSum = calcMA(dataSet, state, runs);

    const end     = Date.now();
    const timeSec = (end - begin) / 1000;

    console.log(`MA time: ${timeSec.toFixed(2)} seconds`);
    console.log(`MA sum : `, actualSum.toFixed(5));
}


function benchRSI(state, runs) {
    const begin = Date.now();

    const actualSum = calcRSI(dataSet, state, runs);

    const end     = Date.now();
    const timeSec = (end - begin) / 1000;

    console.log(`RSI time: ${timeSec.toFixed(2)} seconds`);
    console.log(`RSI sum : `, actualSum.toFixed(5));
}
