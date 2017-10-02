/*************************************************************************
 *
 * EVERT Library
 * -------------
 *
 * (C) 2004-2005 Samuli Laine
 * All Rights Reserved.
 *
 ************************************************************************/

#include "elTimer.hpp"

#include <sys/time.h>

#include <GL/glu.h>

using namespace EL;

//------------------------------------------------------------------------

Timer::Timer(void)
{
	clear();
}

void Timer::clear(void)
{
	timespec ts;
   	clock_gettime(CLOCK_MONOTONIC, &ts);
	m_timer = ts.tv_sec * (uint64_t)1000000000L + ts.tv_nsec;
}

double Timer::get(void) const
{
	timespec ts;
   	clock_gettime(CLOCK_MONOTONIC, &ts);
	uint64_t nsec = ts.tv_sec * (uint64_t)1000000000L + ts.tv_nsec;
	nsec -= m_timer;
	return nsec;
}

//------------------------------------------------------------------------

double CPUTimer::s_msec = 0.001;

int64_t CPUTimer::rdtsc(void)
{
	timespec ts;
   	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return ts.tv_sec * (uint64_t)1000000000L + ts.tv_nsec;
}

void CPUTimer::calibrate(const Timer& referenceTimer)
{
//FIXME: calibration needed?
/*
	double  t1 = referenceTimer.get();
	int64_t t2 = getI();
	s_msec = t1/(double)t2;
*/
}
