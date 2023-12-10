#include "test-and-set.h"

static my_Lock this_lock;
static int THREADNUM;

static void *IDK(void *arg){
	for (int i = 0; i < 6400/THREADNUM; i++){
		lock_function(&this_lock);
		for (int j = 0; j < 10000; j++){
			//wait
		}
		unlock_function(&this_lock);
	}
	
	return NULL;
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Cette fonction prend un argument\n");
		return EXIT_FAILURE;
	}

	THREADNUM = atoi(argv[1]);
	pthread_t thread[THREADNUM];

	init_function(&this_lock);
	for (int i = 0; i < THREADNUM; i++){
		pthread_create(&thread[i], NULL, &IDK, NULL);
	}
	for (int i = 0; i < THREADNUM; i++){
		pthread_join(thread[i], NULL);
	}
	detroy_function(&this_lock);

	return EXIT_SUCCESS;
}
