#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 10000 // Maximum value in the array
#define N_BUCKETS 10 // Number of buckets
#define ARRAY_SIZE 100 // Size of the array to sort
#define RANGE (MAX / N_BUCKETS) // Range of each bucket

int array[ARRAY_SIZE]; // Global array to sort

typedef struct {
    int* bucket;
    int size;
    int capacity;
} Bucket;

Bucket buckets[N_BUCKETS]; // Array of buckets

// Function to compare integers, used by qsort
int compare_ints(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Function to initialize buckets
void init_buckets() {
    for (int i = 0; i < N_BUCKETS; i++) {
        buckets[i].bucket = (int*)malloc(ARRAY_SIZE * sizeof(int));
        buckets[i].size = 0;
        buckets[i].capacity = ARRAY_SIZE;
    }
}

// Function to distribute elements into buckets
void distribute_elements() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int index = array[i] / RANGE;
        if (index >= N_BUCKETS) index = N_BUCKETS - 1; // Ensure the last bucket catches everything
        buckets[index].bucket[buckets[index].size++] = array[i];
    }
}

// Main function
int main() {
    // Initialize array with random values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX;
    }

    init_buckets();
    distribute_elements();

    // Parallel sorting of buckets
    #pragma omp parallel for
    for (int i = 0; i < N_BUCKETS; i++) {
        qsort(buckets[i].bucket, buckets[i].size, sizeof(int), compare_ints);
    }

    // Merging sorted buckets
    int array_index = 0;
    for (int i = 0; i < N_BUCKETS; i++) {
        for (int j = 0; j < buckets[i].size; j++) {
            array[array_index++] = buckets[i].bucket[j];
        }
        free(buckets[i].bucket); // Don't forget to free memory!
    }

    // Print sorted array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
