#include <malloc.h>
#include "shared-state.h"

SharedState *allocSharedState(int bars) {
    SharedState *sharedState = malloc(sizeof(SharedState));
    if (sharedState == NULL) return NULL;

    sharedState->firstBar = 0;

    if ((sharedState->numA    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((sharedState->numB    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((sharedState->numC    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((sharedState->numD    = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((sharedState->balance = malloc(sizeof(double) * bars)) == NULL) return NULL;
    if ((sharedState->equity  = malloc(sizeof(double) * bars)) == NULL) return NULL;

    return sharedState;
}

void freeSharedState(SharedState *sharedState) {
    if (sharedState == NULL) return;

    if (sharedState->numA != NULL) {
        free(sharedState->numA);
        sharedState->numA = NULL;
    }
    if (sharedState->numB != NULL) {
        free(sharedState->numB);
        sharedState->numB = NULL;
    }
    if (sharedState->numC != NULL) {
        free(sharedState->numC);
        sharedState->numC = NULL;
    }
    if (sharedState->numD != NULL) {
        free(sharedState->numD);
        sharedState->numD = NULL;
    }
    if (sharedState->balance != NULL) {
        free(sharedState->balance);
        sharedState->balance = NULL;
    }
    if (sharedState->equity != NULL) {
        free(sharedState->equity);
        sharedState->equity = NULL;
    }

    free(sharedState);
    sharedState = NULL;
}
