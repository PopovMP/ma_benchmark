#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "data-set.h"

DataSet *allocDataSet(const int bars) {
    DataSet *dataSet = malloc(sizeof(DataSet));
    if (dataSet == NULL) return NULL;

    dataSet->bars = bars;

    if ((dataSet->time    = malloc(sizeof(time_t) * bars)) == NULL) return NULL;
    if ((dataSet->open    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((dataSet->high    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((dataSet->low     = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((dataSet->close   = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((dataSet->volume  = malloc(sizeof(int   ) * bars)) == NULL) return NULL;
    if ((dataSet->spreads = malloc(sizeof(int   ) * bars)) == NULL) return NULL;

    return dataSet;
}

void freeDataSet(DataSet *dataSet) {
    if (dataSet == NULL) return;

    if (dataSet->time != NULL) {
        free(dataSet->time);
        dataSet->time = NULL;
    }

    if (dataSet->open != NULL) {
        free(dataSet->open);
        dataSet->open = NULL;
    }

    if (dataSet->high != NULL) {
        free(dataSet->high);
        dataSet->high = NULL;
    }

    if (dataSet->low != NULL) {
        free(dataSet->low);
        dataSet->low = NULL;
    }

    if (dataSet->close != NULL) {
        free(dataSet->close);
        dataSet->close = NULL;
    }

    if (dataSet->volume != NULL) {
        free(dataSet->volume);
        dataSet->volume = NULL;
    }

    if (dataSet->spreads != NULL) {
        free(dataSet->spreads);
        dataSet->spreads = NULL;
    }

    free(dataSet);

    dataSet = NULL;
}

long getFileSize(FILE *file) {
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

time_t getMillennium(void) {
    struct tm date = {0};

    date.tm_year  = 2000 - 1900;
    date.tm_mon   = 0;
    date.tm_mday  = 1;
    date.tm_hour  = 0;
    date.tm_min   = 0;
    date.tm_sec   = 0;
    date.tm_isdst = 0;

    return mktime(&date);
}

/**
 * Reads a data file and returns a DataSet object.
 *
 * @param filePath The path of the data file to read.
 * @return The DataSet object containing the data read from the file, or NULL if an error occurred.
 */
DataSet *readDataSet(const char *filePath, const int digits) {
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return NULL;
    }

    const int bars = (int) (getFileSize(file) / 28);

    DataSet *dataSet = allocDataSet(bars);
    if (dataSet == NULL) {
        fprintf(stderr, "Failed to allocate DataSet.\n");
        fclose(file);
        return NULL;
    }

    const time_t millennium = getMillennium();
    const double multiplier = pow(10, digits);

    uint32_t temp[7];
    for (int i = 0; i < bars; ++i) {
        const size_t bytesRead = fread(&temp, 4, 7, file);
        if (bytesRead != 7) {
            fprintf(stderr, "Failed to read data bar.\n");
            fclose(file);
            return NULL;
        }

        dataSet->time   [i] = (time_t) (temp[0] * 60 + millennium);
        dataSet->open   [i] = (double) (temp[1] / multiplier);
        dataSet->high   [i] = (double) (temp[2] / multiplier);
        dataSet->low    [i] = (double) (temp[3] / multiplier);
        dataSet->close  [i] = (double) (temp[4] / multiplier);
        dataSet->volume [i] = (int   )  temp[5];
        dataSet->spreads[i] = (int   )  temp[6];
    }

    fclose(file);

    return dataSet;
}
