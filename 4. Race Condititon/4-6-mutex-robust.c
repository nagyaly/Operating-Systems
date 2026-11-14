#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>      // Needed for EOWNERDEAD
#define TCOUNT 3
int TICKETS = 10;
pthread_mutex_t lock;              //mutex
//------------------------------------------------------------
void* routine(void* raw_args) {
  char* label = (char*)raw_args;
  while (TICKETS > 0) {
    sleep(1);                             // fake delay
    int status = pthread_mutex_lock(&lock); // LOCK (and check if previous owner died)
    if (status == EOWNERDEAD) {
      printf("%c recovered the mutex\n", *label);
      pthread_mutex_consistent(&lock);    // Mark the recovered mutex as healthy again
    }
      //==============================> CRITICAL SECTION
    if (TICKETS > 0) {                    // GUARD
      sleep(1);                           // fake delay
      TICKETS = TICKETS - 1;
      printf("Thread %c sold 1 ticket, %d left\n", *label, TICKETS);
    }
    //==============================> CRITICAL SECTION
    // if(*label == 'B') break;              //but semaphore still suffer from deadlock !!
    pthread_mutex_unlock(&lock);          // release the mutex (UNLOCK)
  }
  return NULL;
}
//---------------------------------------------------------
int main() {
  pthread_t tids[TCOUNT];
  char labels[TCOUNT];
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&lock, &attr);
  for (int i = 0; i < TCOUNT; i++) {
    labels[i] = 'A' + i;
    pthread_create(&tids[i], NULL, routine, &labels[i]);
  }
  for (int i = 0; i < TCOUNT; i++) {
    pthread_join(tids[i], NULL);
  }
  printf("TICKETS LEFT: %d\n", TICKETS);
  pthread_mutex_destroy(&lock);           // destroy the mutex
  return 0;
}