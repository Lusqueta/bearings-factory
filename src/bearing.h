#pragma once

#include "stack.h"

typedef enum EBearing
{
  CILINDRICO,
  CONICO,
  ACO,
  TITANIO
} EBearingType;

typedef struct Bearing
{
  EBearingType type;
  Stack *machines;
  double time_consumed;
} Bearing;

Bearing *create_bearing(EBearingType type);
void destroy_bearing(Bearing *bearing);