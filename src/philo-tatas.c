#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "test-and-set.h"


int PHILOSOPHES;

pthread_t *phil;
my_Lock *baguette;

void mange(int id)
{
    //Philosophe mange
}

//1.1

void *philosophe(void *arg)
{

    int *id = (int *)arg;
    int left = *id;
    int right = (left + 1) % PHILOSOPHES;

    for (int i = 0; i < 100000; i++){
        // philosophe pense
        if (left < right)
        {
            test_and_test_and_set_lock_function(&baguette[left]);
            test_and_test_and_set_lock_function(&baguette[right]);
        }
        else
        {
            test_and_test_and_set_lock_function(&baguette[right]);
            test_and_test_and_set_lock_function(&baguette[left]);
        }
        mange(*id);
        unlock_function(&baguette[left]);
        unlock_function(&baguette[right]);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Cette fonction prend un argument\n");
        return (EXIT_FAILURE);
    }

    PHILOSOPHES = atoi(argv[1]); //1 car 0 est le nom du programme
    if (PHILOSOPHES <= 1){
        return EXIT_SUCCESS;
    }
    
    phil = malloc(sizeof(pthread_t) * PHILOSOPHES);
    baguette = malloc(sizeof(pthread_mutex_t) * PHILOSOPHES);

    for (int i = 0; i < PHILOSOPHES; i++){
		init_function(&baguette[i]);
    }

    int id[PHILOSOPHES];
    for (int i = 0; i < PHILOSOPHES; i++){
        id[i] = i;
        pthread_create(&phil[i], NULL, &philosophe, &(id[i]));
    }

    for (int i = 0; i < PHILOSOPHES; i++){
        pthread_join(phil[i], NULL);
    }

    for (int i = 0; i < PHILOSOPHES; i++){
		detroy_function(&baguette[i]);
    }

    free(phil);
    free(baguette);
    return (EXIT_SUCCESS);
}
