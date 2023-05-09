#include <bits/pthreadtypes.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

sem_t sem_compteur;
pthread_mutex_t camion_sc;

#define trucks	5
#define cars	5
#define vehicules  (trucks+cars)

void attendre()
{
    struct timespec delai;
    delai.tv_sec=1;
    delai.tv_nsec=0;
    nanosleep(&delai,NULL);
}

void* car(void* arg)
{
    attendre();
    entrer(5);
    printf(" car %d : enters\n",*((int*)arg)-trucks);
    attendre();
    printf(" car %d : exits\n",*((int*)arg)-trucks);
    sortir(5);
    pthread_exit(NULL);
}

void* truck(void* arg)
{
    attendre();
    entrer(15);
    printf(" truck %d : enters\n",*((int*)arg));
    attendre();
    printf(" truck %d : exits\n",*((int*)arg));
    sortir(15);
    pthread_exit(NULL);
}

void entrer(int tonnes)
{
    if (tonnes==15)
    {
        pthread_mutex_lock(&camion_sc);
        sem_wait(&sem_compteur);
        sem_wait(&sem_compteur);
        sem_wait(&sem_compteur);
        pthread_mutex_unlock(&camion_sc);
    }
    else  sem_wait(&sem_compteur);
}


void sortir(int tonnes)
{
    while(tonnes>0)
    {
        sem_post(&sem_compteur);
        tonnes=tonnes-5;
    };
}


int main(int argc, char* argv[])
{
    int i;
    pthread_t id;
    pthread_t tasks[vehicules];

    pthread_attr_t attr;
    struct sched_param param;
    pthread_attr_init(&attr);
    param.sched_priority = 12;
    pthread_setschedparam (pthread_self(),SCHED_FIFO, &param);
    pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);





    sem_init(&sem_compteur,0,3);

    for(i=0; i<vehicules; i++)
    {

        int* j=(int*)malloc(sizeof(int));
        *j=i;
        if (i<trucks){
            pthread_attr_t attr;
            struct sched_param param;
            pthread_attr_init(&attr);
            param.sched_priority = 1;
            pthread_setschedparam (&tasks[i],SCHED_RR, &param);
            pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            pthread_create(&tasks[i],&attr,truck,j);}
        else{
            pthread_attr_t attr;
            struct sched_param param;
            pthread_attr_init(&attr);
            param.sched_priority = 2;
            pthread_setschedparam (&tasks[i],SCHED_FIFO, &param);
            pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            pthread_create(&tasks[i],&attr,car,j);
    }}
    pthread_exit(NULL);
};


