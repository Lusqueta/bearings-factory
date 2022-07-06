#pragma once

#include "bearing.h"
#include "queue.h"
#include "machinery.h"

typedef struct Factory
{
  struct Machine **machines;

  double *arrivals;

  int *done_bearings_quantity;
  double *done_bearings_time;

  double time_left;

  void (*simulate)(struct Factory *self);
  Bearing *(*pass_bearing_to_next_machine)(struct Factory *self, Bearing *bearing);
} Factory;

Factory *create_factory(double simulation_time);

void simulate(Factory *self);
Bearing *pass_bearing_to_next_machine(Factory *self, Bearing *bearing);
