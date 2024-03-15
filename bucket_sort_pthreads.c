#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10000 // Maximum integer value in the array
#define N_BUCKETS 10 // Number of buckets
#define N_THREADS N_BUCKETS // One thread per bucket
#define ARRAY_SIZE 100 // Size of the array to sort
#define RANGE (MAX / N_BUCKETS) // Range of values in each bucket

int array[ARRAY_SIZE]; // Global array to sort

typedef struct {
    int *bucket;
    int size;
} Bucket;

Bucket buckets[N_BUCKETS]; // Array of buckets

// Thread function to sort a bucket
void *sort_bucket(void *arg) {
    Bucket *bucket = (Bucket *)arg;
    // Implement your preferred sorting algorithm here, e.g., qsort()
    qsort(bucket->bucket, bucket->size, sizeof(int), compare_ints);
    pthread_exit(NULL);
}

// Comparison function for qsort
int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Main function
int main() {
    pthread_t threads[N_THREADS];
    int i, j;

    // Initialize array with random numbers
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX;
    }

    // Initialize buckets
    for (i = 0; i < N_BUCKETS; i++) {
        buckets[i].bucket = (int *)malloc(ARRAY_SIZE * sizeof(int));
        buckets[i].size = 0;
    }

    // Distribute the array elements into buckets
    for (i = 0; i < ARRAY_SIZE; i++) {
        int index = array[i] / RANGE;
        if (index >= N_BUCKETS) index = N_BUCKETS - 1; // Ensure the last bucket catches everything
        buckets[index].bucket[buckets[index].size++] = array[i];
    }

    // Create threads to sort each bucket
    for (i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, sort_bucket, (void *)&buckets[i]);
    }

    // Wait for all threads to complete
    for (i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
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
