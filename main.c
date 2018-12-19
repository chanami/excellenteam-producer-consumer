# include <stdio.h>
# include <pthread.h>
#include <stdlib.h>
# define BUFFER_SIZE 6
# define PRODUCER_CAPACITY 40
# define CONSUMER_CAPACITY 30
# define NUM_OF_PRODUCERS 3
# define NUM_OF_CONSUMERS 4
void *run_producer();
void *run_consumer();

int BufferIndex = 0;
char *BUFFER;

pthread_cond_t Buffer_Not_Full=PTHREAD_COND_INITIALIZER;
pthread_cond_t Buffer_Not_Empty=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;

int main()
{
    pthread_t ptid,ctid;

    BUFFER=(char *) malloc(sizeof(unsigned long) * BUFFER_SIZE);

    for (i=0;i<NUM_OF_PRODUCERS;++i)
    {
        pthread_create(&ptid,NULL,run_producer,NULL);

    }
    for (i=0;i<NUM_OF_PRODUCERS;++i)
    {
        pthread_join(ptid,NULL);

    }
    for (i=0;i<NUM_OF_CONSUMERS;++i)
    {
        pthread_create(&ctid,NULL,run_consumer,NULL);

    }
    for (i=0;i<NUM_OF_CONSUMERS;++i)
    {
        pthread_join(ctid,NULL);

    }

    pthread_join(ptid,NULL);
    pthread_join(ctid,NULL);


    return 0;
}

unsigned long get_product_number()
{
    return (rand() % 1000000);
}

void *run_producer()
{
    int i;
    for( i=0 ; i < PRODUCER_CAPACITY ; ++i)
    {
        pthread_mutex_lock(&mVar);
        if(BufferIndex==BUFFER_SIZE)
        {
            pthread_cond_wait(&Buffer_Not_Full,&mVar);
        }
        BUFFER[BufferIndex++]=get_product_number();
        printf("Produce : %d  Product : %lu\n",BufferIndex,BUFFER[BufferIndex]);
        pthread_mutex_unlock(&mVar);
        pthread_cond_signal(&Buffer_Not_Empty);
    }
    return 0;

}

void *run_consumer()
{
    int i;
    for(i =0 ; i < CONSUMER_CAPACITY ; ++i)
    {
        pthread_mutex_lock(&mVar);
        if(BufferIndex==-1)
        {
            pthread_cond_wait(&Buffer_Not_Empty,&mVar);
        }
        printf("Consume Product : %lu\n",BUFFER[BufferIndex--]);
        pthread_mutex_unlock(&mVar);
        pthread_cond_signal(&Buffer_Not_Full);
    }
    return 0;
}