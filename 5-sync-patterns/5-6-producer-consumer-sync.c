#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS 10

char buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;           // counts free slots (starts at BUFFER_SIZE)
sem_t full;            // counts filled slots (starts at 0)
pthread_mutex_t lock;  // protects buffer, in, out, and printing

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
    sem_wait(&empty);           // sleep if buffer is full
    pthread_mutex_lock(&lock);  // lock buffer

    buffer[in] = item;
    printf("Produced %c -> ", item);
    print_buffer();
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&lock);
    sem_post(&full);            // wake consumer: 1 more filled slot
    sleep(1);
  }
  return NULL;
}

void* consumer(void* arg) {
  for (int i = 0; i < ITEMS; i++) {
    sem_wait(&full);            // sleep if buffer is empty
    pthread_mutex_lock(&lock);  // lock buffer

    char item = buffer[out];
    buffer[out] = '\0';
    printf("Consumed %c -> ", item);
    print_buffer();
    out = (out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&lock);
    sem_post(&empty);           // wake producer: 1 more empty slot
    sleep(4);  // slower consumer so buffer fills up
  }
  return NULL;
}

int main() {
  pthread_t prod, cons;

  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);
  pthread_mutex_init(&lock, NULL);

  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);

  pthread_join(prod, NULL);
  pthread_join(cons, NULL);

  sem_destroy(&empty);
  sem_destroy(&full);
  pthread_mutex_destroy(&lock);
  return 0;
}
