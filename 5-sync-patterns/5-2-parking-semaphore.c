#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define ALLOWED_CARS 3
#define TOTAL_CARS 8
sem_t sem;         // Semaphore
void* car(raw_args) {
  char* label = (char *) raw_args;
  sem_wait(&sem);                 // lock
  //------------------------------ criticl section
  printf("Car %c: PARKED\n", *label);
  sleep(2);               //parking
  printf("Car %c: LEFT\n", *label);
  //------------------------------
  sem_post(&sem);                 // unlock
  return NULL;
}

int main() {
  pthread_t threads[TOTAL_CARS];
  char labels[TOTAL_CARS];
  sem_init(&sem, 0, ALLOWED_CARS);
  for (int i = 0; i < TOTAL_CARS; i++) {
    labels[i] = 'A' + i;
    pthread_create(&threads[i], NULL, car, &labels[i]);
  }
  for (int i = 0; i < TOTAL_CARS; i++) {
    pthread_join(threads[i], NULL);
  }
  sem_destroy(&sem);  // Destroy the semaphore
  return 0;
}