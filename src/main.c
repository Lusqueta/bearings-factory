#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "factory.h"
#include "bearing.h"

int main(int argc, char *argv[])
{

  srand(time(NULL));
  if (argv[1] == NULL)
  {
    printf("Usage: ./main <seconds_left>\n");
    return 1;
  }

  Factory *factory = create_factory(atof(argv[1]));
  factory->simulate(factory);

  char *labels[] = {"Cilindrico", "Conico", "Aco", "Titanio"};
  for (int i = 0; i < 4; i++)
  {
    int quantity = factory->done_bearings_quantity[i];
    double time_consumed = factory->done_bearings_time[i];

    printf("%s   : %7.2f \t %d \n", labels[i], time_consumed / (double)quantity, quantity);
  }

  return 0;
}
