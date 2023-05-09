#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define trucks	5
#define cars	5
#define vehicules  (trucks+cars)
#define en_attente 1
#define rien 2
#define traverser 3

pthread_mutex_t sc;
sem_t sempriv[vehicules];
int etat[vehicules];
int nbr_camions_bloques=0;
int seuil=0;



void attendre()
{
    struct timespec delai;
    delai.tv_sec=1;
    delai.tv_nsec=0;
    nanosleep(&delai,NULL);
}

void* voiture(void* arg)
{
    attendre();
    entrer(5, *((int*)arg));
    printf(" Car %d : enters\n",*((int*)arg)-trucks);
    attendre();
    printf(" Car %d : exits\n",*((int*)arg)-trucks);
    sortir(5, *((int*)arg));
    pthread_exit(NULL);
}

void* camion(void* arg)
{
    attendre();
    entrer(15,*((int*)arg));
    printf(" truck %d : enters\n",*((int*)arg));
    attendre();
    printf(" truck %d : exits\n",*((int*)arg));
    sortir(15,*((int*)arg));
    pthread_exit(NULL);
}

void entrer(int tonnes, int id)
{
    pthread_mutex_lock(&sc);

    if(seuil+tonnes <= 15)
    {
        seuil=seuil+tonnes;
        etat[id]=traverser;
        sem_post(&sempriv[id]);
    }
    else
    {
        etat[id]=en_attente;
        if(tonnes==15)
            nbr_camions_bloques++;
    }

    pthread_mutex_unlock(&sc);
    sem_wait(&sempriv[id]);
}



void sortir(int tonnes, int pid)
{
    int i;

    pthread_mutex_lock(&sc);

    etat[pid]=rien;
    seuil=seuil-tonnes;

    for(i=0; i<trucks; i++)
        if ( (etat[i]==en_attente) && (seuil==0) )
        {
            sem_post(&sempriv[i]);
            seuil=15;
            nbr_camions_bloques--;
        }

    for(i=trucks; i<vehicules; i++)
        if ( (seuil<15) && (nbr_camions_bloques==0)
                && (etat[i]==en_attente) )
        {
            seuil=seuil+5;
            sem_post(&sempriv[i]);
        }

    pthread_mutex_unlock(&sc);
}






int main(int argc, char* argv[])
{
    int i;
    pthread_t id;


    for(i=0; i<vehicules; i++)
    {
        etat[i]=rien;
        sem_init(&sempriv[i],0,0);
    }

    pthread_mutex_init(&sc,0);

    for(i=0; i<vehicules; i++)
    {
        int* j=(int*)malloc(sizeof(int));
        *j=i;
        if (i<trucks)
            pthread_create(&id,NULL,camion,j);
        else if(i==trucks){
            attendre();
        	pthread_create(&id,NULL,voiture,j);
    }
        else {
        	pthread_create(&id,NULL,voiture,j);
        
        }
    }


    pthread_exit(NULL);
};


