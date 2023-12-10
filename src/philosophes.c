
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

int PHILOSOPHES;

pthread_t *phil;
pthread_mutex_t *baguette;

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
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else
        {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        mange(*id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
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
        pthread_mutex_init(&(baguette[i]), NULL);
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
        pthread_mutex_destroy(&(baguette[i]));
    }

    free(phil);
    free(baguette);
    return (EXIT_SUCCESS);
}
