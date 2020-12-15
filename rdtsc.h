/*
 * rdtsc.h
 *
 *  Created on: Mar 28, 2017
 *      Author: geri
 */

#ifndef RDTSC_H_
#define RDTSC_H_

#include <vector>
#include <numeric>

typedef unsigned long long ull;

#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

// static unsigned long long rdtsc(void)
// {
//     unsigned hi, lo;
//     __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
//     return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
// }

unsigned long long estimate_tsc_per_sec();
double avg_cycles_per_microsec(int iterCount);

#endif /* RDTSC_H_ */
