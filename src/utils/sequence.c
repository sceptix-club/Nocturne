#include "utils/sequence.h"

int* GenerateRandomSequence(unsigned int count, int min, int max) {
    if (max - min + 1 < count) {
        printf("Error: Range is too small for the number of unique values requested.\n");
        return NULL;
    }

    int* sequence = (int*)malloc(count * sizeof(int));
    if (!sequence) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Initialize the array with values
    for (int i = 0; i < count; i++) {
        sequence[i] = min + i;
    }

    // Shuffle the array
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = sequence[i];
        sequence[i] = sequence[j];
        sequence[j] = temp;
    }

    return sequence;
}