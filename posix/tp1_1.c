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

    sem_init(&sem_compteur,0,3);

    for(i=0; i<vehicules; i++)
    {

        int* j=(int*)malloc(sizeof(int));
        *j=i;
        if (i<trucks)
            pthread_create(&id,NULL,truck,j);
        else
            pthread_create(&id,NULL,car,j);
    }
    pthread_exit(NULL);
};


