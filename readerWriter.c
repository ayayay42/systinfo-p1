#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_WRITERS 640
#define NUM_READERS 2560

int wc = 0, rc = 0; // initializing writer and reader count
int read_n;
int write_n;

pthread_mutex_t mwc; // mwc is mutex writer count
pthread_mutex_t mrc; // mrc is mutex reader count
pthread_mutex_t z;

sem_t wsem, rsem; // writer and reader semaphore

void *writer(void *arg) {
    for (int i = 0; i < write_n; i++) {
        pthread_mutex_lock(&mwc);
        wc++;
        if (wc == 1) {
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mwc);

        sem_wait(&wsem);
        // Write to the database
        for (int i = 0; i < 10000; i++);
        sem_post(&wsem);

        pthread_mutex_lock(&mwc);
        wc--;
        if (wc == 0) {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mwc);
    }
}

void *reader(void *read) {
    for (int i = 0; i < read_n; i++) {
        pthread_mutex_lock(&z);
        sem_wait(&rsem);
        pthread_mutex_lock(&mrc);
        rc++;
        if (rc == 1) {
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mrc);
        sem_post(&rsem);
        pthread_mutex_unlock(&z);
        for (int i = 0; i < 10000; i++);
        pthread_mutex_lock(&mrc);
        rc--;
        if (rc == 0) {
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mrc);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int read_n_th, write_n_th;

    if (argc != 2) {
        printf("Insufficient arguments\n");
        return -1;
    }

    int nbr_threads = atoi(argv[1]);
    read_n_th = nbr_threads / 2;
    write_n_th = nbr_threads - read_n_th;

    pthread_t thread_readers[read_n_th], thread_writers[write_n_th];

    // Initialize mutex and semaphores
    if (pthread_mutex_init(&mrc, NULL) != 0) {printf("Error creating the reader mutex\n");}
    if (pthread_mutex_init(&mwc, NULL) != 0) {printf("Error creating the writer mutex\n");}

    if (sem_init(&rsem, 0, 1) != 0) {printf("Error creating the reader semaphore\n");}
    if (sem_init(&wsem, 0, 1) != 0) {printf("Error creating the writer semaphore\n");}

    // Create threads for readers
    read_n = NUM_READERS/read_n_th;
    for (int i = 0; i < read_n; i++) {
        if (pthread_create(&thread_readers[i], NULL, reader, NULL) != 0) {
            printf("Error creating reader thread\n");
        }
    }

    // Create threads for writers
    write_n = NUM_WRITERS/write_n_th;
    for (int i = 0; i < write_n; i++) {
        if (pthread_create(&thread_writers[i], NULL, writer, NULL) != 0) {
            printf("Error creating writer thread\n");
        }
    }

    // Join threads
    for (int i = 0; i < read_n; i++) {
        if (pthread_join(thread_readers[i], NULL) != 0) {
            printf("Error joining reader thread\n");
        }
    }

    for (int i = 0; i < write_n; i++) {
        if (pthread_join(thread_writers[i], NULL) != 0) {
            printf("Error joining writer thread\n");
        }
    }

    // Destroy mutex and semaphores
    if (pthread_mutex_destroy(&mrc) != 0) {printf("Error destroying the reader mutex\n");}
    if (pthread_mutex_destroy(&mwc) != 0) {printf("Error destroying the writer mutex\n");}

    if (sem_destroy(&rsem) != 0) {printf("Error destroying the reader semaphore\n");}
    if (sem_destroy(&wsem) != 0) {printf("Error destroying the writer semaphore\n");}

    return (EXIT_SUCCESS);
}
