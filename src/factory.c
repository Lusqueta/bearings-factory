#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "factory.h"
#include "machinery.h"
#include "queue.h"

#include "constants.h"

Factory *create_factory(double simulation_time)
{
  Factory *factory = (Factory *)malloc(sizeof(Factory));

  factory->simulate = &simulate;
  factory->pass_bearing_to_next_machine = &pass_bearing_to_next_machine;

  factory->machines = (Machine **)malloc(sizeof(Machine *) * 3);
  factory->arrivals = (double *)malloc(sizeof(double) * 3);
  factory->done_bearings_quantity = (int *)malloc(sizeof(int) * 4);
  factory->done_bearings_time = (double *)malloc(sizeof(double) * 4);

  factory->machines[0] = create_machine(TORNO, factory);
  factory->machines[1] = create_machine(FRESA, factory);
  factory->machines[2] = create_machine(MANDRIL, factory);

  factory->arrivals[0] = 0.0;
  factory->arrivals[1] = 0.0;
  factory->arrivals[2] = 0.0;

  factory->done_bearings_quantity[0] = 0;
  factory->done_bearings_quantity[1] = 0;
  factory->done_bearings_quantity[2] = 0;
  factory->done_bearings_quantity[3] = 0;

  factory->done_bearings_time[0] = 0.0;
  factory->done_bearings_time[1] = 0.0;
  factory->done_bearings_time[2] = 0.0;
  factory->done_bearings_time[3] = 0.0;

  factory->time_left = simulation_time;

  return factory;
}

int compare(const void *a, const void *b)
{
  double _a = *((double *)a);
  double _b = *((double *)b);

  if (_a > _b)
    return 1;
  else if (_a < _b)
    return -1;
  return 0;
}

double get_next_step(Factory *self)
{
  Machine **machines = self->machines;
  double *arrivals = self->arrivals;

  double times[7] = {0.0};
  int i = 0;

  for (; i < 3; i++)
  {
    Machine *machine = machines[i];
    if (machine->bearing != NULL)
    {
      times[i] = machine->time_left;
    }
  }

  for (; i < 7; i++)
  {
    times[i] = arrivals[i - 3];
  }

  qsort(times, 7, sizeof(double), compare);

  for (i = 0; i < 7; i++)
  {
    if (times[i] > 0.00000001)
    {
      return times[i];
    }
  }

  return times[0];
}

Bearing *pass_bearing_to_next_machine(Factory *self, Bearing *bearing)
{
  Machine **machines = self->machines;

  if (bearing->machines->size == 0)
    return bearing;

  EMachineType next_machine_type = (EMachineType)bearing->machines->pop(bearing->machines);

  Queue *queue = machines[next_machine_type]->left_bearings;

  // Push the bearing to the next machine
  queue->enqueue(queue, bearing);

  return NULL;
}

float generate_exponential(float avg)
{
  float u = 0;
  do
    u = ((float)rand()) / ((float)RAND_MAX);
  while ((u == 0) || (u == 1));
  return (-avg * log(u));
}

double refresh_arrival(EBearingType type)
{
  switch (type)
  {
  case CILINDRICO:
    return generate_exponential(chegada_media_cilindrico);
  case CONICO:
    return generate_exponential(chegada_media_conico);
  case ACO:
  case TITANIO:
    return generate_exponential(chegada_media_esferico);
  }

  return 0.0;
}

void manage_arrivals(Factory *self, double step)
{
  for (int type = 0; type < 3; type++)
  {
    self->arrivals[type] -= step;

    if (self->arrivals[type] <= 0.0)
    {

      if (type == ACO)
      {
        float random_number = ((float)rand()) / ((float)RAND_MAX);
        if (random_number < 0.1)
        {
          type = TITANIO;
        }
      }

      self->arrivals[type] = refresh_arrival((EBearingType)type);
      self->pass_bearing_to_next_machine(self, create_bearing(type));
    }
  }
}

// Step will never be bigger than self->time_left
void run_machines(Factory *self, double step)
{
  for (int i = 0; i < 3; i++)
  {
    Machine *machine = self->machines[i];
    machine->run(machine, step);
  }
}

void simulate(Factory *self)
{
  while (self->time_left > 0.0)
  {
    double step = fmin(get_next_step(self), self->time_left);

    run_machines(self, step);
    manage_arrivals(self, step);

    self->time_left -= step;
  }
}
