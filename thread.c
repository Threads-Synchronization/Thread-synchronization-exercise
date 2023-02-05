#include "thread.h"
void init_all_threads(p_parameters parameters)
{
    parameters->threads=(p_all_threads)calloc(1,sizeof(all_threads));
    assert(parameters->threads);
    //priority:
    pthread_attr_t attr;
    struct sched_param param;
    int init_valid, get_valid;
    init_valid=pthread_attr_init (&attr);
    get_valid=pthread_attr_getschedparam (&attr, &param);
    if(init_valid<0 || get_valid<0)
    {
        printf("ERROR: setting thread priority failed");
        //create thread with default priority
        create_thread_with_default_priority(parameters->threads->trd4,(void*)parameters,trd4);
        create_thread_with_default_priority(parameters->threads->trd3,(void*)parameters,trd3);
        create_thread_with_default_priority(parameters->threads->trd2,(void*)parameters,trd2);
        create_thread_with_default_priority(parameters->threads->trd1,(void*)parameters,trd1);
    }
    else
    {
        //set thread 1
        create_thread(parameters->threads->trd4, attr,(void*)parameters,param,trd4);
        //set thread 2
        create_thread(parameters->threads->trd3, attr,(void*)parameters,param,trd3);
        //set thread 3
        create_thread(parameters->threads->trd2, attr,(void*)parameters,param,trd2);
        //set thread 4
        create_thread(parameters->threads->trd1, attr,(void*)parameters,param,trd1);
    }
}
create_thread_with_default_priority(pthread_t th,void* parameters,void*(func)(p_parameters))
{
    int create_valid=pthread_create(&(th),NULL,func,parameters);
    if(create_valid<0)
    {
        printf("init: ERROR: *pthread_create* failed");
        exit(1);
    }
}
create_thread(pthread_t th,pthread_attr_t attr,void* parameters,struct sched_param param,void*(func)(p_parameters))
{
    int create_valid,set_valid;
    (param.sched_priority)++;
    set_valid=pthread_attr_setschedparam (&attr, &param);
    if(set_valid<0)
    {
        create_valid=pthread_create(&(th),NULL,func,parameters);
        if(create_valid<0)
        {
            printf("init: ERROR: *pthread_create* failed");
            exit(1);
        }
    }
    else
    {
        create_valid=pthread_create(&(th),&attr,func,parameters);
        if(create_valid<0)
        {
            printf("init: ERROR: *pthread_create* failed");
            exit(1);
        }
    }
}
void init_parameters(p_parameters params)
{
    int is_valid,i;
    params->status=1;
    params->log_file=fopen("threads.log", "w");
    assert(params->log_file);
    is_valid=sem_init(&(params->sema_arr),0,1);
    if(is_valid<0)
    {
        printf("init: ERROR: *sem_init* failed");
        exit(1);
    }
    init_all_threads(params);
}
void free_parameters(p_parameters parameters)
{
    free(parameters-> threads);
    if(parameters)
    {
        free(parameters);
    }
}

void* trd4(p_parameters params)
{
    int msgid2, msgid3, msgsnd_valid, random;
    message message2, message3;
    key_t key2, key3;

    srand(time(NULL));

    // ftok to generate unique key
    key2 = ftok("progfile2", KEY_TRD2);
    assert(key2);

    key3 = ftok("progfile3", KEY_TRD3);
    assert(key3);

    // msgget creates a message queue and returns identifier
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    if(msgid2<0)
    {
        printf("ERROR: *msgget* failed");
        exit(1);
    }
    message2.mesg_type = CLOCK_MSG;

    msgid3 = msgget(key3, 0666 | IPC_CREAT);
    if(msgid3<0)
    {
        printf("ERROR: *msgget* failed");
        exit(1);
    }
    message3.mesg_type = CLOCK_MSG;

    while(1)
    {
        printf("thread4\n");
        params->clock=clock()%100;
        message2.mesg_text[0]=params->clock;
        message3.mesg_text[0]=params->clock;
        msgsnd_valid=msgsnd(msgid2,&message2,sizeof(message), IPC_NOWAIT);
        if(msgsnd_valid<0)
        {
            printf("WARNING: *%s* in *msgsnd*\tthread4\tmessage: %d \n",strerror(errno),message2.mesg_text[0]);
        }
        msgsnd_valid=msgsnd(msgid3,&message3,sizeof(message),IPC_NOWAIT);
        if(msgsnd_valid<0)
        {
            printf("WARNING: *%s* in *msgsnd*\tthread4\tmessage: %d \n",strerror(errno),message3.mesg_text[0]);
        }
        random=rand()%10+1;
        usleep(random*1000);
    }
}

void* trd3(p_parameters params)
{
    int msgid,index,snprintf_valid,msgrcv_valid;
    message msg;
    key_t key;

    // ftok to generate unique key
    key = ftok("progfile3", KEY_TRD3);
    assert(key);
    // msgget creates a message queue and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid<0)
    {
        printf("ERROR: *msgget* failed");
        exit(1);
    }
    while(1)
    {
        printf("thread3\n");
        msgrcv_valid=msgrcv(msgid,&msg, sizeof(msg),CLOCK_MSG,0&IPC_NOWAIT);
        if(msgrcv_valid<0)
        {
            printf("thread3: WARNING: *%s* in *msgrcv*\tmessage: %d \n",strerror(errno),msg.mesg_text[0]);
        }
        index=(int)msg.mesg_text[0];
        sem_wait(&(params->sema_arr));
        snprintf_valid= snprintf(params->result_arr+(index*SIZE_OF_DATA),SIZE_OF_DATA,"tH33333333\tindex: %d\tline number: %d\n",index, __LINE__);
        if(snprintf_valid<0)
        {
            printf("thread 3: ERROR: %s *snprintf* snprintf_valid= %d failed\n",strerror(errno),snprintf_valid);
        }
        sem_post(&(params->sema_arr));
    }
}

void* trd2(p_parameters params)
{
    key_t key;
    int index, msgid, msgrcv_valid,snprintf_valid;
    message msg;

    // ftok to generate unique key
    key = ftok("progfile2", KEY_TRD2);
    assert(key);
    // msgget creates a message queue and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid<0)
    {
        printf("thread 2: ERROR: *msgget* failed");
        exit(1);
    }

    while(1)
    {
        printf("thread2\n");
        msgrcv_valid=msgrcv(msgid,&msg, sizeof(message),CLOCK_MSG,0&IPC_NOWAIT);
        if(msgrcv_valid<0)
        {
            printf("thread2: WARNING: *%s* in *msgrcv*\tmessage: %d \n",strerror(errno),msg.mesg_text[0]);
        }
        index=(int)msg.mesg_text[0];
        sem_wait(&(params->sema_arr));
        snprintf_valid= snprintf( params->result_arr+(index*SIZE_OF_DATA),SIZE_OF_DATA,"Th222222222\tindex: %d\tline number: %d\n",index, __LINE__);
        if(snprintf_valid<0)
        {
            printf("thread 2: ERROR: %s *snprintf* snprintf_valid= %d failed\n",strerror(errno),snprintf_valid);
        }
        sem_post(&(params->sema_arr));
    }
}

void* trd1(p_parameters params)
{
    int i,*arr;
    while(params->size_of_file<MB)
    {
        printf("thread1\n");
        for(i=0; i<ARR_LENGTH;i++)
        {
            arr=(params->result_arr)+(i*SIZE_OF_DATA);
            if(*arr)
            {
                fputs(arr,params->log_file);
                fseek(params->log_file,0L,SEEK_END);
                params->size_of_file=ftell(params->log_file);
                fseek(params->log_file,0L,SEEK_CUR);
            }
        }
        fputs("-----------------------------\n",params->log_file);
        usleep(50000);
    }
    if(params->log_file)
    {
        fclose(params->log_file);
    }
    params->status=0;
}
