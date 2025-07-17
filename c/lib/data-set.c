#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "data-set.h"

DataSet *alloc_data_set(const int bars) {
    DataSet * restrict data = malloc(sizeof(DataSet));
    if (data == NULL) return NULL;

    data->bars = bars;

    if ((data->time    = malloc(sizeof(time_t) * bars)) == NULL) return NULL;
    if ((data->open    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((data->high    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((data->low     = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((data->close   = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((data->volume  = malloc(sizeof(int   ) * bars)) == NULL) return NULL;
    if ((data->spreads = malloc(sizeof(int   ) * bars)) == NULL) return NULL;

    return data;
}

void free_data_set(DataSet * restrict data) {
    if (data == NULL) return;

    if (data->time != NULL) {
        free(data->time);
        data->time = NULL;
    }

    if (data->open != NULL) {
        free(data->open);
        data->open = NULL;
    }

    if (data->high != NULL) {
        free(data->high);
        data->high = NULL;
    }

    if (data->low != NULL) {
        free(data->low);
        data->low = NULL;
    }

    if (data->close != NULL) {
        free(data->close);
        data->close = NULL;
    }

    if (data->volume != NULL) {
        free(data->volume);
        data->volume = NULL;
    }

    if (data->spreads != NULL) {
        free(data->spreads);
        data->spreads = NULL;
    }

    free(data);

    data = NULL;
}

long get_file_size(FILE * restrict file) {
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

time_t get_millennium(void) {
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
DataSet *read_data_set(const char * const file_path, const int digits) {
    FILE * restrict file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return NULL;
    }

    const int bars = (int) (get_file_size(file) / 28);

    DataSet * restrict data = alloc_data_set(bars);
    if (data == NULL) {
        fprintf(stderr, "Failed to allocate DataSet.\n");
        fclose(file);
        return NULL;
    }

    const time_t millennium = get_millennium();
    const double divider    = pow(10, digits);

    uint32_t buffer[7];
    for (int bar = 0; bar < bars; ++bar) {
        const size_t chunks_read = fread(&buffer, 4, 7, file);
        if (chunks_read != 7) {
            fprintf(stderr, "Failed to read data bar.\n");
            fclose(file);
            return NULL;
        }

        data->time   [bar] = (time_t) (buffer[0] * 60 + millennium);
        data->open   [bar] = (double) (buffer[1] / divider);
        data->high   [bar] = (double) (buffer[2] / divider);
        data->low    [bar] = (double) (buffer[3] / divider);
        data->close  [bar] = (double) (buffer[4] / divider);
        data->volume [bar] = (int   )  buffer[5];
        data->spreads[bar] = (int   )  buffer[6];
    }

    fclose(file);

    return data;
}
