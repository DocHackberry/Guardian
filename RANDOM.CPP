

#include <time.h>
#include "random.h"


int randnum(int low, int high)			//returns a random number between high and low numbers
{
	static int init = 0;

	if (init == 0)
	{
		srand(time(0));
		rand();
		init = 1;
	}

	double rb = rand() / 32767.0;
	int	range = high - low + 1;

	int rnd = int(rb * range);

	return rnd + low;

}
