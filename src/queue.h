#pragma once

typedef struct Queue
{
  int (*comparator)(const void *, const void *);

  int min_capacity;
  int capacity;

  void **heap;
  int length;

  void (*enqueue)(struct Queue *self, void *element);
  void *(*dequeue)(struct Queue *self);
} Queue;

Queue *create_queue(const int size, int (*comparator)(const void *, const void *));
void destroy_queue(Queue *self);

void enqueue(Queue *self, void *item);
void *dequeue(Queue *self);