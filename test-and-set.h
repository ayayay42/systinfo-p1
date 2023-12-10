#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct{
	int lock;
} my_Lock;

// 2.1

void test_and_set(my_Lock* my_lock){
	int flag = 1;
	while (flag){
		asm volatile(
			"xchg %0, %1"
			: "=r" (flag)
			: "m" (my_lock->lock), "0" (flag)
			: "memory");
	}
}

void test_and_test_and_set_lock_function(my_Lock* my_lock){
	// TODO
}

void* init_function(my_Lock *my_lock_ptr){
	my_lock_ptr->lock = 0;
	return NULL;
}

void lock_function(my_Lock *my_lock_ptr){
	test_and_set(my_lock_ptr);
}

void unlock_function(my_Lock *my_lock_ptr){
	my_lock_ptr->lock = 0;
}

void* detroy_function(my_Lock *my_lock_ptr){
	return NULL;
}

// 2.4

typedef struct{
	my_Lock lock_ptr;
	int value;
} my_Sem;

void* sem_init_function(my_Sem *my_sem_ptr, int value){
	my_sem_ptr->value = value;

	return NULL;
}

void sem_wait_function(my_Sem *my_sem_ptr){
	lock_function(&my_sem_ptr->lock_ptr);

	while (my_sem_ptr->value == 0){
		unlock_function(&my_sem_ptr->lock_ptr);
		lock_function(&my_sem_ptr->lock_ptr);
	}
	
	my_sem_ptr->value -= 1;
	unlock_function(&my_sem_ptr->lock_ptr);
}

void sem_post_function(my_Sem *my_sem_ptr){
	lock_function(&my_sem_ptr->lock_ptr);
	my_sem_ptr->value += 1;
	unlock_function(&my_sem_ptr->lock_ptr);
}

void* sem_destroy_function(my_Sem *my_sem_ptr){
	detroy_function(&my_sem_ptr->lock_ptr);
	return NULL;
}
