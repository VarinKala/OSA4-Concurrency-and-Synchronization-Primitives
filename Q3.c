#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CARS 5

sem_t bridge_mutex;
sem_t left_mutex;
sem_t right_mutex;
int cars_on_bridge = 0;

void passing(int dir, int car_number)
{
    if(car_number == 0)
    {
        perror("NO CAR PRESENT");
    }
    if (dir == 0)
    {
        printf("Left car no.%d is crossing the bridge.\n", car_number);
    }
    else
    {
        printf("Right car no.%d is crossing the bridge.\n", car_number);
    }
    sleep(1);
}

void *left(void *args)
{
    int car_number = *((int *)args);
    while (1)
    {
        sem_wait(&left_mutex);
        sem_wait(&bridge_mutex);

        if (cars_on_bridge < MAX_CARS)
        {
            cars_on_bridge++;
            passing(0, car_number);
            cars_on_bridge--;
        }
        sem_post(&bridge_mutex);
        sem_post(&left_mutex);
    }
}

void *right(void *args)
{
    int car_number = *((int *)args);
    while (1)
    {
        sem_wait(&right_mutex);
        sem_wait(&bridge_mutex);

        if (cars_on_bridge < MAX_CARS)
        {
            cars_on_bridge++;
            passing(1, car_number);
            cars_on_bridge--;
        }
        sem_post(&bridge_mutex);
        sem_post(&right_mutex);
    }
}

int main()
{
    int left_cars, right_cars;

    printf("Number of cars on the left side : ");
    scanf("%d", &left_cars);
    printf("Number of cars on the right side : ");
    scanf("%d", &right_cars);

    sem_init(&bridge_mutex, 0, 1);
    sem_init(&left_mutex, 0, 1);
    sem_init(&right_mutex, 0, 1);

    pthread_t left_threads[left_cars], right_threads[right_cars];
    int left_args[left_cars], right_args[right_cars];

    for (int i = 0; i < left_cars; i++)
    {
        left_args[i] = i + 1;
        pthread_create(&left_threads[i], NULL, left, (void *)&left_args[i]);
    }
    for (int i = 0; i < right_cars; i++)
    {
        right_args[i] = i + 1;
        pthread_create(&right_threads[i], NULL, right, (void *)&right_args[i]);
    }
    for (int i = 0; i < left_cars; i++)
    {
        pthread_join(left_threads[i], NULL);
    }
    for (int i = 0; i < right_cars; i++)
    {
        pthread_join(right_threads[i], NULL);
    }

    sem_destroy(&bridge_mutex);
    sem_destroy(&left_mutex);
    sem_destroy(&right_mutex);

    return 0;
}