# Concurrency-and-Synchronization-Primitives
[Documentation](https://docs.google.com/document/d/16SQZw-vdhjBKRVhMSvvBTxqU9JWolBUwcgyq_oi_rSg/edit?usp=sharing)
---

## Q1
Why can Deadlocks occur in th Problem Setup?
- In the above problem, let us consider the situation where forks 2 and 3 are in use. Now, if the philosopher close to forks 1 and 2 decides to eat, he will first pick up fork 1. But since fork 2 is already in use, he will wait for fork 2 while having locked fork 1. This may continue on with other philosophers holding one fork only, and thus lead to deadlock.

How does our proposed Solution avoid Deadlock?
- Our proposed solution involves using a Condition Variable. We propose that once a philosopher decides to start eating, he first checks if both the forks are available using 'pthread_cond_wait', and he is only able to eat if both the forks are already available, or if either is not available, and he gets a signal once the fork is no longer occupied.

Fairness of the Solution:
- According to our solution, All 5 of the Philosophers are able to eat at random time intervals. The order in which the Philosophers eat is random as well.

## Q3
Explanation of code logic and avoidance of concurrency bugs:
- `Semaphores`: We use semaphores to control access to the bridge and to manage the queues on both sides. The `mutex` semaphore ensures mutual exclusion when updating shared variables, and `leftQueue` and `rightQueue` semaphores are used to control the entry and exit of cars on each side.
- `Arrival and Departure`: Cars arriving at the bridge check the current count of cars on the bridge. If the limit is not reached, they proceed; otherwise, they wait in the corresponding queue. When a car departs, it updates the count and signals the waiting cars in the opposite queue, if any.
- `Sleep Function`: The `sleep(1)` function simulates the time it takes for a car to cross the bridge.
- `Dynamic Memory Allocation`: To pass the car number to the thread functions, dynamic memory allocation is used to avoid potential data races.
- `Joining Threads`: The `pthread_join` function is used to ensure that the main thread waits for all threads to finish before exiting.

This program should provide a basic implementation of the given problem with synchronization using semaphores and should avoid common concurrency issues.
