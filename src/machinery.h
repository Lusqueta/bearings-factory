#pragma once

#include "bearing.h"
#include "factory.h"

typedef enum EMachine
{
  TORNO,
  FRESA,
  MANDRIL
} EMachineType;

typedef struct Machine
{
  EMachineType type;
  struct Factory *factory;
  Queue *left_bearings;
  Bearing *bearing;

  double time_left;
  double time_required;

  void (*run)(struct Machine *self, double step);
  void (*get_next_bearing)(struct Machine *self);
} Machine;

Machine *create_machine(EMachineType type, struct Factory *factory);

void run(Machine *self, double step);
void get_next_bearing(Machine *self);
double get_run_time(Machine *machine, Bearing *bearing);
