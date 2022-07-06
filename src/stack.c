#include <stdlib.h>

#include "stack.h"

Stack *create_stack(int item_size, int size)
{
  Stack *self = (Stack *)malloc(sizeof(Stack));

  if (self == NULL)
  {
    return NULL;
  }

  self->contents = (void **)calloc(item_size, size);

  if (self->contents == NULL)
  {
    free(self);
    return NULL;
  }

  self->max_size = size;
  self->size = 0;
  self->item_size = item_size;

  self->push = &push;
  self->pop = &pop;

  return self;
}

void destroy_stack(Stack *self)
{
  free(self->contents);
  free(self);
}

void push(Stack *self, void *item)
{

  if (self->size >= self->max_size)
    return;

  self->contents[self->size] = item;
  self->size += 1;
}

void *pop(Stack *self)
{

  if (self->size == 0)
    return NULL;

  void *item = self->contents[self->size - 1];
  self->contents[self->size - 1] = NULL;
  (self->size)--;
  return item;
}