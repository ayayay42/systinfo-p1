#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "test-and-set.h"

#define NUM_WRITERS 640
#define NUM_READERS 2560

//2.5 readerWriter 

static int wc = 0, rc = 0; // initializing writer and reader count
static int read_n;
static int write_n;

static my_Lock mwc; // mwc is mutex writer count
static my_Lock mrc; // mrc is mutex reader count
static my_Lock z;

static my_Sem wsem, rsem; // writer and reader semaphore

void *writer(void *arg) {
    for (int i = 0; i < write_n; i++) {
        lock_function(&z);
		sem_wait_function(&wsem);
		lock_function(&mwc);
        wc++;
        if (wc == 1) {
            sem_wait_function(&rsem);
        }
        unlock_function(&mwc);
        sem_post_function(&wsem);
        unlock_function(&z);
        // Write to the database
        for (int i = 0; i < 1000000; i++); //1 000 000 steps

        lock_function(&mwc);
        wc--;
        if (wc == 0) {
            sem_post_function(&rsem);
        }
        unlock_function(&mwc);
    }
    return NULL;
}


void *reader(void *read) {
    for (int i = 0; i < read_n; i++) {
        lock_function(&z);
        sem_wait_function(&rsem);
        lock_function(&mrc);
        rc++;
        if (rc == 1) {
            sem_wait_function(&wsem);
        }
        unlock_function(&mrc);
        sem_post_function(&rsem);
        unlock_function(&z);
        for (int i = 0; i < 1000000; i++); //1 000 000 steps
        lock_function(&mrc);
        rc--;
        if (rc == 0) {
            sem_post_function(&wsem);
        }
        unlock_function(&mrc);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int read_n_th, write_n_th;

    if (argc != 3) {
        printf("Insufficient arguments\n");
        return -1;
    }

    // TODO check arguments AUB

    //int nbr_threads = atoi(argv[1]);

	//vérifier si les arguments sont des nombres valides
	for (int i = 1; i < argc; i++)
	{
		char *endptr;
		long val = strtol(argv[i], &endptr, 10);
		if (endptr == argv[i] || *endptr != '\0' || val < 0)
		{
			printf("Argument %d is not a valid number\n", i);
			return EXIT_FAILURE;
		}
	}
	
    write_n_th = atoi(argv[1]);
    read_n_th = atoi(argv[2]);

    pthread_t* const thread_readers = calloc(read_n_th, sizeof *thread_readers);
    pthread_t* const thread_writers = calloc(write_n_th, sizeof *thread_writers);

    // Initialize mutex and semaphores
    if (init_function(&mrc) != 0) {printf("Error creating the reader mutex\n");}
    if (init_function(&mwc) != 0) {printf("Error creating the writer mutex\n");}

    if (sem_init_function(&rsem,1) != 0) {printf("Error creating the reader semaphore\n");}
    if (sem_init_function(&wsem,1) != 0) {printf("Error creating the writer semaphore\n");}

    // Create threads for readers
    read_n = NUM_READERS/read_n_th;
    for (int i = 0; i < read_n_th; i++) {
        if (pthread_create(&thread_readers[i], NULL, reader, NULL) != 0) {
            printf("Error creating reader thread\n");
        }
    }
    // Create threads for writers
    write_n = NUM_WRITERS/write_n_th;
    for (int i = 0; i < write_n_th; i++) {
        if (pthread_create(&thread_writers[i], NULL, writer, NULL) != 0) {
            printf("Error creating writer thread\n");
        }
    }

    // Join threads
    for (int i = 0; i < read_n_th; i++) {
        if (pthread_join(thread_readers[i], NULL) != 0) {
            printf("Error joining reader thread\n");
        }
    }
    for (int i = 0; i < write_n_th; i++) {
        if (pthread_join(thread_writers[i], NULL) != 0) {
            printf("Error joining writer thread\n");
        }
    }
    // Destroy mutex and semaphores
    if (detroy_function(&mrc) != 0) {printf("Error destroying the reader mutex\n");}
    if (detroy_function(&mwc) != 0) {printf("Error destroying the writer mutex\n");}

    if (sem_destroy_function(&rsem) != 0) {printf("Error destroying the reader semaphore\n");}
    if (sem_destroy_function(&wsem) != 0) {printf("Error destroying the writer semaphore\n");}

    free(thread_readers);
    free(thread_writers);

    return (EXIT_SUCCESS);
}
