#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#define WORKERS 8
pthread_mutex_t lock;
void printer(char worker, int pages){
    printf("Worker %c printing %d pages\n", worker, pages);
    FILE *file = fopen("printer.txt", "a");
    setbuf(file, NULL);
    for(int i=1;i<=pages;i++){
        printf("Worker %c Page %d/%d\n", worker, i, pages);
        fprintf(file, "Woker %c Page %d/%d\n", worker, i, pages);
        sleep(1);
    }
    fprintf(file, "-----------------------------\n");
    fclose(file);
    printf("Worker %c finished printing %d pages\n", worker, pages);

}
void* routine(void* raw_args) {
  char* label = (char *) raw_args;
  int pages = 1 + rand() % 6;
  //------------------------------ criticl section
  int status = pthread_mutex_lock(&lock);
  if(status == EOWNERDEAD){
    pthread_mutex_consistent(&lock);
  }
  printer(*label, pages);

  pthread_mutex_unlock(&lock);
  //------------------------------ 
  return NULL;
}

int main() {
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&lock, &attr);
  fclose(fopen("printer.txt", "w"));  // clear file at startup
  srand(time(0));
  pthread_t tids[WORKERS];
  char labels[WORKERS];
  for (int i = 0; i < WORKERS; i++) {
    labels[i] = 'A' + i;
    pthread_create(&tids[i], NULL, routine, &labels[i]);
  }
  for (int i = 0; i < WORKERS; i++) {
    pthread_join(tids[i], NULL);
  }
  return 0;
}