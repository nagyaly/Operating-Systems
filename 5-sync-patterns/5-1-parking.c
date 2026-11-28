#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define TOTAL_CARS 8
void* routine(void* raw_args) {
  char* label = (char *)raw_args;
  //------------------------------ criticl section
  printf("Car %c: PARKED\n", *label);
  sleep(2);               //parking
  printf("Car %c: LEFT\n", *label);
  //------------------------------
  return NULL;
}

int main() {
  pthread_t threads[TOTAL_CARS];
  char labels[TOTAL_CARS];
  for (int i = 0; i < TOTAL_CARS; i++) {
    labels[i] = 'A' + i;
    pthread_create(&threads[i], NULL, routine, &labels[i]);
  }
  for (int i = 0; i < TOTAL_CARS; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}