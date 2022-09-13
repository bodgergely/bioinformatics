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

int avg_cycles_per_microsec(int iterCount)
{
    vector<double> s;
    for (int i = 0; i < iterCount; i++) {
        auto cycles_per_micro_sec = estimate_tsc_per_sec() / 1000000;
        s.push_back(cycles_per_micro_sec);
    }
    return accumulate(s.begin(), s.end(), 0) / s.size();
}

Timer::Timer() : _counter(0), _cyclesPerMicroSec(0) {}

Timer::Timer(ull cyclesPerMicroSec)
    : _counter(0), _cyclesPerMicroSec(cyclesPerMicroSec)
{
}

void Timer::start()
{
    _counter = __rdtsc();
}

ull Timer::stop()
{
    return __rdtsc() - _counter;
}

ull Timer::microsecs(ull cycles)
{
    if (_cyclesPerMicroSec == 0) {
        _cyclesPerMicroSec = avg_cycles_per_microsec(1);
    }
    return cycles / _cyclesPerMicroSec;
}
