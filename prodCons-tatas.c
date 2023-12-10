#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "test-and-set.h"

#define BUFFER_SIZE 8
#define MAX_ITEMS 8192

static int buffer[BUFFER_SIZE];
static int in = 0, out = 0;

static my_Sem empty;
static my_Sem full;
static my_Lock mutex;

static int num_producers;
static int num_consumers;

void* producer(void* arg) {
    int produced = 0;

    while (produced < MAX_ITEMS / num_producers) {
        int item = rand() % 100;
        for (int i = 0; i < 10000; i++);

        sem_wait_function(&empty);
        test_and_test_and_set_lock_function(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        produced++;

        unlock_function(&mutex);
        sem_post_function(&full);
        printf("Produced: %d\n", item);
        printf("Produit: %d\n", produced);
    }

    return NULL;
}

void* consumer(void* arg) {
    int consumed = 0;

    while (consumed < MAX_ITEMS / num_consumers) {
        for (int i = 0; i < 10000; i++);
        sem_wait_function(&full);
        test_and_test_and_set_lock_function(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        consumed++;

        unlock_function(&mutex);
        sem_post_function(&empty);

        // Modify the way items are consumed
        printf("Consumed: %d\n", item);
        printf("Consomme: %d\n", consumed);
    }

    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Insufficient arguments\n");
        return -1;
    }

    num_producers = atoi(argv[1]);
    num_consumers = atoi(argv[2]);

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Initialize producer-consumer components
    if (init_function(&mutex) != 0) {
        perror("Error creating the mutex\n");
    }

    if (sem_init_function(&empty, BUFFER_SIZE) != 0) {
        perror("Error creating the semaphore\n");
    }

    if (sem_init_function(&full, 0) != 0) {
        perror("sem_init full");
    }

    // Create producer and consumer threads
    for (int i = 0; i < num_producers; i++) {
        if (pthread_create(&producer_threads[i], NULL, &producer, NULL) != 0) {
            perror("Error creating producer thread");
        }
    }

    for (int i = 0; i < num_consumers; i++) {
        if (pthread_create(&consumer_threads[i], NULL, &consumer, NULL) != 0) {
            perror("pthread_create consumer");
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < num_producers; i++) {
        if (pthread_join(producer_threads[i], NULL) != 0) {
            perror("pthread_join producer");
        }
    }

    for (int i = 0; i < num_consumers; i++) {
        if (pthread_join(consumer_threads[i], NULL) != 0) {
            perror("pthread_join consumer");
        }
    }

    // Cleanup
    if (detroy_function(&mutex) != 0) {
        perror("mutex destroy");
    }

    if (sem_destroy_function(&full) != 0) {
        perror("sem full destroy");
    }

    if (sem_destroy_function(&empty) != 0) {
        perror("sem empty destroy");
    }

    return EXIT_SUCCESS;
}


