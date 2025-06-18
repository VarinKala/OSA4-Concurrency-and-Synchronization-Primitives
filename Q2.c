#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int total_passengers, car_capacity;
sem_t passenger_sem_create, car_sem;

void load() {
    printf("Loading Passengers\n");
}

void unload() {
    printf("Unloading Passengers\n");
}

void board(int passenger_no) {}

void offboard(int passenger_no) {}

void* car(void* args) {
    printf("Car is Ready");
    while (1) {
        load();

        for (int i = 0; i < car_capacity; i++) {
            sem_post(&passenger_sem_create);
        }
    }
}

void* passenger(void* args) {
    sem_wait(&passenger_sem_create);
    board();
}

int main() {
    printf("Enter the Total Number of Passengers: ");
    scanf("%d", &total_passengers);

    printf("Enter the Capacity of Car: ");
    scanf("%d", &car_capacity);

    printf("\n");

    if (car_capacity >= total_passengers) {
        perror("Car capacity should be less than the total number of passengers.\n");
    }

    for (int i = 0; i < total_passengers; i++) {
        sem_init(&passenger_sem_create, 0, 0);
    }

    sem_init(%car_sem, 0, 1);

    return 0;
}