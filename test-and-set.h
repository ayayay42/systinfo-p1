#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct{
	int *lock;
} my_Lock;

// 2.1

int test_and_set(my_Lock my_lock){
	int result;
	asm(
			"movl $1, %%eax;"
			"xchg %%eax, (%1);"
			"movl %%eax, %0;"
			:"=&r"(result)
			:"r"(my_lock.lock)
			:"%eax");
	return result;
}

void test_and_test_and_set_lock_function(my_Lock* my_lock){
	while (test_and_set(*my_lock)){
		while (*(my_lock->lock)){
			//wait
		}
	}
	
}

void init_function(my_Lock *my_lock_ptr){
	my_lock_ptr->lock = (int *)malloc(sizeof(int));
	*(my_lock_ptr->lock) = 0;
}

void lock_function(my_Lock *my_lock_ptr){
	while (test_and_set(*my_lock_ptr)){
		// wait
	}
}

void unlock_function(my_Lock *my_lock_ptr){
	asm(
		"movl $0, %%eax;"
		"xchg %%eax, (%0);"
		:
		: "r"(my_lock_ptr->lock)
		: "%eax");
}

void detroy_function(my_Lock *my_lock_ptr){
	free(my_lock_ptr->lock);
}

// 2.4

typedef struct{
	my_Lock *lock_ptr;
	int *pointer;
} my_Sem;

void sem_init_function(my_Sem *my_sem_ptr, int value){
	my_sem_ptr->pointer = (int *)malloc(sizeof(int));
	*(my_sem_ptr->pointer) = value;

	my_sem_ptr->lock_ptr = (my_Lock *)malloc(sizeof(my_Lock));
}

void sem_wait_function(my_Sem *my_sem_ptr){
	bool loop = true;
	while (loop)
	{
		lock_function(my_sem_ptr->lock_ptr);
		if (*(my_sem_ptr->pointer) > 0)
		{
			loop = false;
			*(my_sem_ptr->pointer) -= 1;
		}
		unlock_function(my_sem_ptr->lock_ptr);
	}
}

void sem_post_function(my_Sem *my_sem_ptr){
	lock_function(my_sem_ptr->lock_ptr);
	*(my_sem_ptr->pointer) += 1;
	unlock_function(my_sem_ptr->lock_ptr);
}

void sem_destroy_function(my_Sem *my_sem_ptr){
	detroy_function(my_sem_ptr->lock_ptr);
	free(my_sem_ptr->lock_ptr);
	free(my_sem_ptr->pointer);
}
