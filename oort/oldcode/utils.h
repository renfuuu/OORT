#ifndef utils_h_
#define utils_h_

#include <ctime>
#include <cstdlib>

struct Timer
{
	bool hasStarted;
	clock_t startTime;
	
	clock_t testTime;
	clock_t timePassed;
	double secondsPassed;

	Timer()
	{
		hasStarted = false;
	}

	void start()
	{
		startTime = clock();
		hasStarted = true;
	}

	double getRealTime()
	{
		if(!hasStarted)
		{
			return std::numeric_limits<double>::infinity();
		}

		testTime = clock();
		timePassed = testTime - startTime;
		secondsPassed = timePassed/(double)CLOCKS_PER_SEC;

		return secondsPassed;
	}

	int getIntTime()
	{
		return (int)getRealTime();
	}

};

#endif