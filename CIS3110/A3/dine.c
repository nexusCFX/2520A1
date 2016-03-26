#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

// Need to cap # of philosophers to something
#define MAX 1000

typedef enum { THINK, HUNGRY, EAT } state;

sem_t S[1000];
sem_t lock;

void *philospher(void *num);
int pickup(int, int total);
void putdown(int, int total);
void test(int, int total);

int currState[MAX];

typedef struct PhilInfo {
    int num;
    int numEat;
    int total;
} PhilInfo;

int main(int argc, char *argv[]) {
    int i;

    int numPhil = atoi(argv[1]);
    int numEat = atoi(argv[2]);

    pthread_t thread_id[numPhil];
    sem_init(&lock, 0, 1);
    for (i = 0; i < numPhil; i++) {
        sem_init(&S[i], 0, 0);
    }
    for (i = 0; i < numPhil; i++) {
        PhilInfo *info = malloc(sizeof(*info));
        info->numEat = numEat;
        info->num = i;
        info->total = numPhil;

        pthread_create(&thread_id[i], NULL, philospher, info);
    }
    for (i = 0; i < numPhil; i++) {
        pthread_join(thread_id[i], NULL);
    }
}

void *philospher(void *stuff) {
    PhilInfo *info = (PhilInfo *)stuff;
    for (int i = 0; i < info->numEat; i++) {
        pickup(info->num, info->total);
        putdown(info->num, info->total);
        sleep(1);
    }
    free(info);
}

int pickup(int num, int total) {
    sem_wait(&lock);
    currState[num] = HUNGRY;
    test(num, total);
    sem_post(&lock);
    sem_wait(&S[num]);
    sleep(1);
}

void test(int num, int total) {
    if (currState[(num + total - 1) % total] != EAT &&
        currState[(num + 1) % total] != EAT) {
        currState[num] = EAT;
        sleep(1);
        printf("Philosopher %d is Eating\n", num + 1);
        sem_post(&S[num]);
    }
}

void putdown(int num, int total) {
    sem_wait(&lock);
    currState[num] = THINK;
    printf("Philosopher %d is thinking\n", num + 1);
    test((num + total - 1) % total, total);
    test((num + 1) % total, total);
    sem_post(&lock);
    sleep(1);
}
