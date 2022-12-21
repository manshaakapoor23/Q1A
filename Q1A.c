//Dining philosophers with one sauce bowl each and 5 forks using semaphores

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define n 5
#define THINKING 1
#define EATING 0
#define left (np+4)%n
#define right (np+4)%n

int state[n];
int p[n] = {0,1,2,3,4};
sem_t mutex;
sem_t Sem[n];

void check(int np){
    if(state[np]==THINKING && state[left]!=EATING && state[right]!=EATING){
        state[np]=EATING;
        sleep(2);
        printf("Philosopher %d picked up %d fork\n",np,right+1);
        printf("Philosopher %d picked up %d fork\n",np,left+1);
        printf("Philosopher %d EATING\n",np+1);
        sem_post(&Sem[np]);
    }
}

void take_fork(int np){
    sem_wait(&mutex);
    state[np]=EATING;
    check(np); 
    sem_post(&mutex);
    sem_wait(&Sem[np]); 
}

void put_fork(int np){
    sem_wait(&mutex);
    state[np]=THINKING;
    printf("Philosopher %d puts down fork %d\n",np,right+1);
    printf("Philosopher %d puts down fork %d\n",np,left+1);
    printf("Philosopher %d THINKING\n",np+1);
    check(left);
    check(right);
    sem_post(&mutex);
}

void *philosopher(void* num){
    while(1){
        int* i = num;
        sleep(1);
        take_fork(*i);
        sleep(1);
        put_fork(*i);
    }
}

int main() {
    int i;
    pthread_t threads[n];
    sem_init(&mutex,0,1); // initialise semaphores
    for(i=0;i<n;i++){
        sem_init(&Sem[i],0,1); //creating philosophers processes
    }
    for(i=0;i<n;i++){
        pthread_create(&threads[i],NULL,&philosopher,&p[i]);
        printf("Philosopher %d THINKING\n",i+1);
    }
    for(i=0;i<n;i++){
        pthread_join(threads[i],NULL);
    }
    return 0;
}
