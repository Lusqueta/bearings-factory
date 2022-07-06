#include <stdio.h>
#include <stdlib.h>

#include "bearing.h"
#include "machinery.h"
#include "stack.h"

Bearing *create_bearing(EBearingType type)
{
  Bearing *bearing = (Bearing *)malloc(sizeof(Bearing));

  Stack *stack = create_stack(sizeof(EMachineType *), 5);

  bearing->type = type;
  bearing->machines = stack;
  bearing->time_consumed = 0.0;

  switch (type)
  {
  case CILINDRICO:
    stack->push(stack, (EMachineType *)MANDRIL);
    stack->push(stack, (EMachineType *)TORNO);
    stack->push(stack, (EMachineType *)FRESA);
    stack->push(stack, (EMachineType *)TORNO);
    break;
  case CONICO:
    stack->push(stack, (EMachineType *)TORNO);
    stack->push(stack, (EMachineType *)MANDRIL);
    stack->push(stack, (EMachineType *)TORNO);
    break;
  case ACO:
    stack->push(stack, (EMachineType *)TORNO);
    stack->push(stack, (EMachineType *)MANDRIL);
    stack->push(stack, (EMachineType *)FRESA);
    break;
  case TITANIO:
    stack->push(stack, (EMachineType *)TORNO);
    stack->push(stack, (EMachineType *)FRESA);
    stack->push(stack, (EMachineType *)TORNO);
    stack->push(stack, (EMachineType *)MANDRIL);
    stack->push(stack, (EMachineType *)FRESA);
    break;
  }

  return bearing;
}

void destroy_bearing(Bearing *bearing)
{
  if (bearing == NULL)
    return;

  destroy_stack(bearing->machines);
  free(bearing);
}
