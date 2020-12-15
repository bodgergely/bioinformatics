/*
 * rdtsc.cpp


 *
 *  Created on: Mar 28, 2017
 *      Author: geri
 */


#include "rdtsc.h"


using namespace std;

#ifdef __gnu_linux__
#include <unistd.h>
#elif _WIN32
#include "Windows.h"
#endif

unsigned long long estimate_tsc_per_sec()
{
	unsigned long long s = __rdtsc();
#ifdef __gnu_linux__
	usleep(1000000);
#elif _WIN32
	Sleep(1000);
#endif
	unsigned long long e = __rdtsc();
	return e - s;
}

double avg_cycles_per_microsec(int iterCount)
{
	vector<double> s;
	for(int i=0;i<iterCount;i++)
	{
		double cycles_per_micro_sec = estimate_tsc_per_sec() / (double)1000000;
		s.push_back(cycles_per_micro_sec);
	}
	return accumulate(s.begin(), s.end(), 0.0) / s.size();
}



