#include <malloc.h>
#include "shared-state.h"

SharedState *alloc_shared_state(int bars) {
    SharedState *state = malloc(sizeof(SharedState));
    if (state == NULL) return NULL;

    state->firstBar = 0;

    if ((state->numA    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((state->numB    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((state->numC    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((state->numD    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((state->balance = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((state->equity  = malloc(sizeof(double) * bars)) == NULL) return NULL;

    return state;
}

void free_shared_state(SharedState *state) {
    if (state == NULL) return;

    if (state->numA != NULL) {
        free(state->numA);
        state->numA = NULL;
    }
    if (state->numB != NULL) {
        free(state->numB);
        state->numB = NULL;
    }
    if (state->numC != NULL) {
        free(state->numC);
        state->numC = NULL;
    }
    if (state->numD != NULL) {
        free(state->numD);
        state->numD = NULL;
    }
    if (state->balance != NULL) {
        free(state->balance);
        state->balance = NULL;
    }
    if (state->equity != NULL) {
        free(state->equity);
        state->equity = NULL;
    }

    free(state);
    state = NULL;
}
