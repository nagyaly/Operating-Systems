#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS 10

char buffer[BUFFER_SIZE];
int in = 0, out = 0;

void print_buffer() {
  printf("[");
  for (int i = 0; i < BUFFER_SIZE; i++) {
    if (buffer[i] == '\0') {
      printf(" - ");
    } else {
      printf(" %c ", buffer[i]);
    }
  }
  printf("]\n");
}

void* producer(void* arg) {
  for (int i = 0; i < ITEMS; i++) {
    char item = 'A' + i;
    buffer[in] = item;
    printf("Produced %c -> ", item);
    print_buffer();
    in = (in + 1) % BUFFER_SIZE;
    sleep(1);
  }
  return NULL;
}

void* consumer(void* arg) {
  for (int i = 0; i < ITEMS; i++) {
    char item = buffer[out];
    buffer[out] = '\0';
    printf("Consumed %c -> ", item ? item : '-');
    print_buffer();
    out = (out + 1) % BUFFER_SIZE;
    sleep(4);  // slower consumer so buffer fills up
  }
  return NULL;
}

int main() {
  pthread_t prod, cons;

  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);

  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  return 0;
}
