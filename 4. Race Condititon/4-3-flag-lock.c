#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define TCOUNT 3
int TICKETS = 10;
int flag = 0;
//------------------------------------------------------------
void* routine(void* raw_args) {
  char* label = (char*)raw_args;
  while (TICKETS > 0) {
    sleep(1);                             // fake delay
    while(flag == 1);                     //wait for flag to turn 0, notice the ;
    // sleep(1);                             // this fake delay force 2 thrads entering the critical section 
    flag = 1;                             //lock
    //==============================> CRITICAL SECTION
    if (TICKETS > 0) {                    // GUARD
      sleep(1);                           // fake delay
      TICKETS = TICKETS - 1;
      printf("Thread %c sold 1 ticket, %d left\n", *label, TICKETS);
    }
    //==============================> CRITICAL SECTION
    // if(*label == 'C') break;              //kill Thread C on purpose, this produce deadlock !!
    flag = 0;                             //unlock
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