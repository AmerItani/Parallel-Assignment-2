#include <stdio.h>
#include <stdlib.h>

#define MAX 10000 // Maximum integer value in the array
#define N_BUCKETS 10 // Number of buckets
#define ARRAY_SIZE 100 // Size of the array to sort
#define RANGE (MAX / N_BUCKETS) // Range of values in each bucket

int array[ARRAY_SIZE]; // Array to sort

typedef struct {
    int *bucket;
    int size;
} Bucket;

// Comparison function for qsort
int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Main function
int main() {
    Bucket buckets[N_BUCKETS]; // Array of buckets
    int i, j;

    // Initialize array with random numbers
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX;
    }

    // Initialize buckets
    for (i = 0; i < N_BUCKETS; i++) {
        buckets[i].bucket = (int *)malloc(ARRAY_SIZE * sizeof(int)); // Allocate memory for bucket
        buckets[i].size = 0;
    }

    // Distribute the array elements into buckets
    for (i = 0; i < ARRAY_SIZE; i++) {
        int index = array[i] / RANGE;
        if (index >= N_BUCKETS) index = N_BUCKETS - 1; // Ensure the last bucket catches everything
        buckets[index].bucket[buckets[index].size++] = array[i];
    }

    // Sort each bucket
    for (i = 0; i < N_BUCKETS; i++) {
        qsort(buckets[i].bucket, buckets[i].size, sizeof(int), compare_ints);
    }

    // Merge the buckets back into the original array
    int pos = 0;
    for (i = 0; i < N_BUCKETS; i++) {
        for (j = 0; j < buckets[i].size; j++) {
            array[pos++] = buckets[i].bucket[j];
        }
        free(buckets[i].bucket); // Free the memory allocated for the bucket
    }

    // Print the sorted array
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
