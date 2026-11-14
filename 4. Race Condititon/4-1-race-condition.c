#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define TCOUNT 3
int TICKETS = 10;
//------------------------------------------------------------
void* routine(void* raw_args) {
  char* label = (char*)raw_args;
  while (TICKETS > 0) {
    sleep(1);                             // fake delay
    //==============================> CRITICAL SECTION
    if (TICKETS > 0) {                    // GUARD
      sleep(1);                           // fake delay
      TICKETS = TICKETS - 1;
      printf("Thread %c sold 1 ticket, %d left\n", *label, TICKETS);
    }
    //==============================> CRITICAL SECTION
  }
  return NULL;
}
//---------------------------------------------------------
int main() {
  pthread_t tids[TCOUNT];
  char labels[TCOUNT];
  for (int i = 0; i < TCOUNT; i++) {
    labels[i] = 'A' + i;
    pthread_create(&tids[i], NULL, routine, &labels[i]);
  }
  for (int i = 0; i < TCOUNT; i++) {
    pthread_join(tids[i], NULL);
  }
  printf("TICKETS LEFT: %d\n", TICKETS);
  return 0;
}