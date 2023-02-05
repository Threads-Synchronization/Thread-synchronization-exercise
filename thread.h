#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include<semaphore.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

#define ARR_LENGTH 100
#define SIZE_OF_DATA 100
#define SIZE_OF_MESSAGE 1
#define THREADS_AMMOUNT 4
#define KEY_TRD2 62
#define KEY_TRD3 63
#define MB 1048576
#define CLOCK_MSG 1

typedef struct all_threads{
    pthread_t trd1;
    pthread_t trd2;
    pthread_t trd3;
    pthread_t trd4;
}all_threads, *p_all_threads;

typedef struct mesg_buffer {
    long mesg_type;
    char mesg_text[SIZE_OF_MESSAGE];
} message;

typedef struct parameters{
    p_all_threads threads;
    char result_arr[ARR_LENGTH*SIZE_OF_DATA];
    FILE *log_file;
    long size_of_file;
    clock_t clock;
    int status;
    sem_t sema_arr;
    message message_queue;
}parameters, *p_parameters;

void init_all_threads(p_parameters);
void init_parameters(p_parameters);
create_thread_with_default_priority(pthread_t th,void* parameters,void*(func)(p_parameters));
create_thread(pthread_t th,pthread_attr_t attr,void* parameters,struct sched_param param,void*(func)(p_parameters));

void free_parameters(p_parameters);
void* trd4(p_parameters);
void* trd3(p_parameters);
void* trd2(p_parameters);
void* trd1(p_parameters);
