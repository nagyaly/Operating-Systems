#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define WORKERS 8
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
  printer(*label, pages);
  //------------------------------
  return NULL;
}

int main() {
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