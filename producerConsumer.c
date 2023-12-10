#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFFER_SIZE 8
#define MAX_ITEMS 8192

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int produced = 0, consumed = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    while (produced < MAX_ITEMS) {
        int item = rand() % 100;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        produced++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    return NULL;
}

void* consumer(void* arg) {
    while (consumed < MAX_ITEMS) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        consumed++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // Modify the way items are consumed
        printf("Consumed: %d\n", item);
    }

    return NULL;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        print("Insufficient arguments\n");
        return -1;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Initialize producer-consumer components
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Error creating the mutex\n");
    }

    if (sem_init(&empty, 0 , BUFFER_SIZE) != 0) {
        perror("Error creating the semaphore\n");
    }

    if (sem_init(&full, 0 , 0) != 0) {
        perror("sem_init full");
    }

    // Create producer and consumer threads
    for (int i = 0; i < num_producers; i++) {
        if (pthread_create(&producer_threads[i], NULL, producer, NULL) != 0) {
            perror("Error creating producer thread");
        }
    }

    for (int i = 0; i < num_consumers; i++) {
        if (pthread_create(&consumer_threads[i], NULL, consumer, NULL) != 0) {
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
        }
    }

    // Cleanup
    if (pthread_mutex_destroy(&mutex) != 0) {
        perror("mutex destroy");
    }

    if (sem_destroy(&full) != 0) {
        perror("sem full destroy");
    }

    if (sem_destroy(&empty) != 0) {
        perror("sem empty destroy");
    }

    return 0;
}

