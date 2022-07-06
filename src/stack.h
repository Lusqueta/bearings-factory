#pragma once

#include <stdlib.h>

struct Stack
{
  int max_size;
  int size;
  int item_size;
  void **contents;

  void (*push)(struct Stack *stack, void *item);
  void *(*pop)(struct Stack *stack);
};

typedef struct Stack Stack;

Stack *create_stack(int item_size, int size);
void destroy_stack(Stack *stack);
void push(Stack *stack, void *item);
void *pop(Stack *stack);