#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define TCOUNT 3
int TICKETS = 10;
sem_t sem;              //semaphore
//------------------------------------------------------------
void* routine(void* raw_args) {
  char* label = (char*)raw_args;
  while (TICKETS > 0) {
    sleep(1);                             // fake delay
    sem_wait(&sem);                       //decrement the semaphore (LOCK)
    //==============================> CRITICAL SECTION
    if (TICKETS > 0) {                    // GUARD
      sleep(1);                           // fake delay
      TICKETS = TICKETS - 1;
      printf("Thread %c sold 1 ticket, %d left\n", *label, TICKETS);
    }
    //==============================> CRITICAL SECTION
    sem_post(&sem);                       //increment the semaphore (UNLOCK)
  }
  return NULL;
}
//---------------------------------------------------------
int main() {
  pthread_t tids[TCOUNT];
  char labels[TCOUNT];
  sem_init(&sem, 0, 1);           //initialize binary semaphore (only 1 thread)
  // First Parameter (type)    0: threads, 1: processes
  // Second Parameter, number of allowed threads to enter the critical section  
  for (int i = 0; i < TCOUNT; i++) {
    labels[i] = 'A' + i;
    pthread_create(&tids[i], NULL, routine, &labels[i]);
  }
  for (int i = 0; i < TCOUNT; i++) {
    pthread_join(tids[i], NULL);
  }
  printf("TICKETS LEFT: %d\n", TICKETS);
  sem_destroy(&sem);              //destroy the semaphore
  return 0;
}