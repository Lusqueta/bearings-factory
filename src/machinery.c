#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "bearing.h"
#include "machinery.h"
#include "queue.h"
#include "factory.h"

#include "constants.h"

int min(int a, int b)
{
  return a < b ? a : b;
}

int comparator(const void *bearing_a, const void *bearing_b)
{
  EBearingType a = min(((Bearing *)bearing_a)->type, ACO);
  EBearingType b = min(((Bearing *)bearing_b)->type, ACO);

  if (a > b)
    return 1;
  if (a < b)
    return -1;
  return 0;
}

Machine *create_machine(EMachineType type, Factory *factory)
{
  Machine *machine = (Machine *)malloc(sizeof(Machine));
  machine->left_bearings = create_queue(12, comparator);

  machine->type = type;
  machine->factory = factory;
  machine->bearing = NULL;
  machine->time_left = 0.0;
  machine->time_required = 0.0;

  machine->run = &run;
  machine->get_next_bearing = &get_next_bearing;

  return machine;
}

float tempo(float Estadia_Equipamento_Rolamento)
{
  float u = ((float)rand()) / ((float)RAND_MAX);
  return 2.0 * Estadia_Equipamento_Rolamento * u;
}

double get_run_time(Machine *machine, Bearing *bearing)
{
  double timings[3][4] = {
      // CILINDRO, CONICO, ACO, TITANIO
      {estadia_torno_cilindrico, estadia_torno_conico, estadia_torno_esferico_aco, estadia_torno_esferico_titanio},        // TORNO
      {estadia_fresa_cilindrico, 0.0, estadia_fresa_esferico_aco, estadia_fresa_esferico_titanio},                         // FRESA
      {estadia_mandril_cilindrico, estadia_mandril_conico, estadia_mandril_esferico_aco, estadia_mandril_esferico_titanio} // MANDRIL
  };

  if (bearing == NULL)
    return 0.0;

  return tempo(timings[machine->type][bearing->type]);
}

void get_next_bearing(Machine *self)
{
  self->bearing = NULL;

  if (self->left_bearings->length == 0)
    return;

  self->bearing = self->left_bearings->dequeue(self->left_bearings);
  self->time_left = get_run_time(self, self->bearing);
  self->time_required = self->time_left;
}

// step | R e step > 0.0
void run(Machine *self, double step)
{
  if (self->bearing == NULL)
  {
    return get_next_bearing(self);
  }

  self->time_left -= step;
  self->bearing->time_consumed += step;

  if (self->time_left <= 0.0)
  {
    // returns done when there's no more machines to pass through
    Bearing *done = self->factory->pass_bearing_to_next_machine(self->factory, self->bearing);

    if (done != NULL)
    {
      self->factory->done_bearings_quantity[done->type]++;
      self->factory->done_bearings_time[done->type] += done->time_consumed;
      destroy_bearing(done);
    }

    get_next_bearing(self);
  }
}