#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t var_lock;
pthread_mutex_t forks[5];
pthread_mutex_t bowls[2];
int bowl_available[2];
pthread_cond_t fork_available[5];
int done[5];

void eating(int philosopher_no) {
    printf("Philosopher %d is eating\n", philosopher_no + 1);
    sleep(2);
}

void thinking(int philosopher_no) {
    printf("Philosopher %d is thinking\n", philosopher_no + 1);
    sleep(1);
}

void* philosopher(void* args) {
    int philosopher_no = (*(int*)args);
    
    while (1) {
        thinking(philosopher_no);

        while (done[(philosopher_no) % 5] == 0 || done[(philosopher_no + 1) % 5] == 0)
            pthread_cond_wait(&fork_available[(philosopher_no) % 5], &forks[(philosopher_no + 1) % 5]);

        done[(philosopher_no + 1) % 5] = 0;
        done[(philosopher_no) % 5] = 0;

        pthread_mutex_lock(&forks[(philosopher_no + 1) % 5]);
        pthread_mutex_lock(&forks[(philosopher_no) % 5]);

        int bowl_used;

        if (bowl_available[0] == 1) {
            bowl_used = 0;
            bowl_available[0] = 0;
        }
        else {
            bowl_used = 1;
            bowl_available[1] = 0;
        }

        printf("Philosopher %d takes fork %d and %d\n", philosopher_no + 1, ((philosopher_no) % 5) + 1, ((philosopher_no + 1) % 5) + 1);

        pthread_mutex_lock(&var_lock);
        pthread_mutex_lock(&bowls[bowl_used]);
        printf("Philosopher %d takes bowl\n", philosopher_no + 1);
        pthread_mutex_unlock(&var_lock);

        eating(philosopher_no);

        pthread_mutex_lock(&var_lock);
        pthread_mutex_unlock(&bowls[bowl_used]);
        printf("Philosopher %d puts back bowl\n", philosopher_no + 1);
        pthread_mutex_unlock(&var_lock);

        bowl_available[bowl_used] = 1;

        done[(philosopher_no + 1) % 5] = 1;
        done[(philosopher_no) % 5] = 1;

        pthread_cond_signal(&fork_available[(philosopher_no + 1) % 5]);
        pthread_cond_signal(&fork_available[(philosopher_no) % 5]);

        pthread_mutex_unlock(&forks[(philosopher_no + 1) % 5]);
        pthread_mutex_unlock(&forks[(philosopher_no) % 5]);
        printf("Philosopher %d puts back fork %d and %d\n", philosopher_no + 1, ((philosopher_no) % 5) + 1, ((philosopher_no + 1) % 5) + 1);

    }
}

int main() {
    printf("Number of Philosophers: %d\n", 5);
    printf("Number of Forks: %d\n", 5);
    printf("Number of Bowls: %d\n", 2);

    pthread_t threads[5];

    pthread_mutex_init(&var_lock, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_cond_init(&fork_available[i], NULL);
        done[i] = 1;
    }

    pthread_mutex_init(&bowls[0], NULL);
    pthread_mutex_init(&bowls[1], NULL);
    bowl_available[0] = 1;
    bowl_available[1] = 1;

    int arr[5] = {0, 1, 2, 3, 4};

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, philosopher, &arr[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
