#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

static int imax(int a, int b)
{
  return a > b ? a : b;
}

Queue *create_queue(const int capacity, int (*comparator)(const void *, const void *))
{
  Queue *self = NULL;

  self = (Queue *)malloc(sizeof(Queue));

  if (self == NULL)
  {
    printf("\nPriority queue memory allocation failed.\n");
    return NULL;
  }

  self->comparator = comparator;

  self->min_capacity = capacity;
  self->capacity = capacity;

  self->length = 0;
  self->heap = malloc(capacity * sizeof(void *));

  self->enqueue = &enqueue;
  self->dequeue = &dequeue;

  // initialize heap
  for (int i = 0; i < capacity; i++)
  {
    self->heap[i] = NULL;
  }

  return self;
}

void destroy_queue(Queue *self)
{
  if (self == NULL)
  {
    return;
  }

  free(self->heap);
  free(self);
}

void swap(Queue *self, const int a, const int b)
{
  void *temp = NULL;
  temp = self->heap[a];
  self->heap[a] = self->heap[b];
  self->heap[b] = temp;
}

void heapify_top(Queue *self, int key, int length)
{
  int j;

  while (2 * key <= length)
  {
    j = 2 * key;

    if (j < length && self->comparator(self->heap[j], self->heap[j + 1]) == 1)
    {
      j++;
    }

    if (self->comparator(self->heap[key], self->heap[j]) == -1)
    {
      break;
    }

    swap(self, key, j);
    key = j;
  }
}

void heapify_bottom(Queue *self, int key)
{
  while (key > 1 && self->comparator(self->heap[key / 2], self->heap[key]) == 1)
  {
    swap(self, key, key / 2);
    key = key / 2;
  }
}

int check_capacity(Queue *self, const int min_capacity)
{
  int old_capacity, new_capacity;
  void **new_heap_array;

  old_capacity = self->capacity;

  if (min_capacity > old_capacity)
  {
    /* New capacity formula took over Java Arraylist */
    new_capacity = (old_capacity * 3) / 2 + 1;
    if (new_capacity < min_capacity)
    {
      new_capacity = min_capacity;
    }

    new_heap_array = (void **)realloc((void *)self->heap, new_capacity * sizeof(void *));

    if (new_heap_array == NULL)
    {
      return 0;
    }

    self->heap = new_heap_array;
    self->capacity = new_capacity;
  }
  return 1;
}

void enqueue(Queue *self, void *item)
{
  if (self == NULL || item == NULL)
  {
    return;
  }

  if (check_capacity(self, self->length + 2) == 0)
  {
    return;
  }

  self->length += 1;
  self->heap[self->length] = item;

  heapify_bottom(self, self->length);
}

void *dequeue(Queue *self)
{
  if (self == NULL)
  {
    return NULL;
  }

  if (self->length != 0)
  {
    swap(self, 1, self->length);
    heapify_top(self, 1, self->length - 1);

    self->length--;

    return self->heap[self->length + 1];
  }
  return NULL;
}
