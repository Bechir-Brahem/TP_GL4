#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;
struct metro{
    int id;
    int start;
};

pthread_mutex_t mutex[5];
/*
 * nodes
 * a=>0 * b=>1 * c=>2
 * e=>3 * f=>4 * d=>5
 *
 * edges
 * ab=>0 * bc=>1 * cd=>2
 * eb=>3 * fa=>4
 */
int succ(int position){
    if(position==2)return 5;
    if(position==4)return 0;
    if(position==3)return 1;
    
    return position+1;
}
char node(int position){
    switch (position) {
        case 0:return 'a';
        case 1:return 'b';
        case 2:return 'c';
        case 3:return 'e';
        case 4:return 'f';
        case 5:return 'd';
    }
}

void* fonc(void* arg){
    metro m =*((metro*) arg);
    int position=m.start;
    int old_position;
    printf("train:%d starting from:%c\n",m.id,node(position));
    while (position!=5) {
        pthread_mutex_lock(&mutex[position]);
        usleep(500000);
        old_position=position; 
        position=succ(position);
        printf("train:%d is in station:%c\n",m.id,node(position));
        pthread_mutex_unlock(&mutex[old_position]);
    }
    printf("train:%d arrived at its destination:%c\n",m.id,node(position));
}


int main(void)
{
    ios_base::sync_with_stdio(false);
    cout.tie(nullptr);

    pthread_t tasks[10];
    for (int i=0;i<5;i++){
        pthread_mutex_init(&mutex[i], NULL);
    }
    srand(time(NULL));
    for (int i=0;i<10;i++) {
        metro m;
        m.id=i;
        m.start=rand()%6;
        pthread_create(&tasks[i], NULL, fonc, (void*) &m);
    }
    for (int i=0;i<10;i++) {
        pthread_join(tasks[i], NULL);
    }

    return 0; 
}
