/*
Copyright (c) 2023 Emerson Clarke <ZeroToHero>

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/platform.h"

#include <sys/timeb.h>
#include <time.h>

#pragma warning(disable:4996)

#ifdef HERO_PLATFORM_WINDOWS
#include "winsock2.h"

#endif

#ifdef HERO_PLATFORM_POSIX
#include <stdio.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace Hero {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Timers
{
public:

	static const int HoursPerDay					= 24;
	static const int HoursPerWeek					= 24*7;

	static const int MinutesPerHour					= 60;
	static const int MinutesPerDay					= 60*24;
	static const int MinutesPerWeek					= 60*24*7;

	static const int SecondsPerMinute				= 60;
	static const int SecondsPerHour					= 60*60;
	static const int SecondsPerDay					= 60*60*24;
	static const int SecondsPerWeek					= 60*60*24*7;

	static const int MillisecondsPerSecond			= 1000;
	static const int MillisecondsPerMinute			= 1000*60;
	static const int MillisecondsPerHour			= 1000*60*60;
	static const int MillisecondsPerDay				= 1000*60*60*24;
	static const int MillisecondsPerWeek			= 1000*60*60*24*7;

	static const long long MicrosecondsPerMillisecond	= 1000LL;
	static const long long MicrosecondsPerSecond		= 1000LL*1000;
	static const long long MicrosecondsPerMinute		= 1000LL*1000*60;
	static const long long MicrosecondsPerHour			= 1000LL*1000*60*60;
	static const long long MicrosecondsPerDay			= 1000LL*1000*60*60*24;
	static const long long MicrosecondsPerWeek			= 1000LL*1000*60*60*24*7;

	static const long long  NanosecondsPerMicrosecond	= 1000LL;
	static const long long  NanosecondsPerMillisecond	= 1000LL*1000;
	static const long long  NanosecondsPerSecond		= 1000LL*1000*1000;
	static const long long  NanosecondsPerMinute		= 1000LL*1000*1000*60;
	static const long long  NanosecondsPerHour			= 1000LL*1000*1000*60*60;
	static const long long  NanosecondssPerDay			= 1000LL*1000*1000*60*60*24;
	static const long long  NanosecondsPerWeek			= 1000LL*1000*1000*60*60*24*7;

	static const int DaysPerWeek		= 7;
	static const int DaysPerYear		= 365;
	static const int DaysPerLeapYear	= 366;

	static const int WeeksPerYear		= 52;
	static const int MonthsPerYear		= 12;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Quantum
{

};

class Duration
{

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Timer : public Timers
{
public:

	#ifdef HERO_PLATFORM_POSIX

	static struct timespec Timespec(long long nanoseconds)
	{

		struct timespec ts;
		ts.tv_sec = nanoseconds/Timers::NanosecondsPerSecond;
		ts.tv_nsec = nanoseconds-(ts.tv_sec*Timers::NanosecondsPerSecond);
		return ts;
	}

	static long long Timespec(struct timespec & ts)
	{
		long long nanoseconds = ts.tv_sec*Timers::NanosecondsPerSecond;
		nanoseconds += ts.tv_nsec;
		return nanoseconds;
	}

	static struct timeval Timeval(long long nanoseconds)
	{

		struct timeval tv;
		tv.tv_sec = nanoseconds/Timers::NanosecondsPerSecond;
		tv.tv_usec = (nanoseconds-(tv.tv_sec*Timers::NanosecondsPerSecond))/Timers::NanosecondsPerMicrosecond;
		return tv;
	}

	static long long Timeval(struct timeval & tv)
	{
		long long nanoseconds = tv.tv_sec*Timers::NanosecondsPerSecond;
		nanoseconds += tv.tv_usec * Timers::NanosecondsPerMicrosecond;
		return nanoseconds;
	}

	#endif

	#ifdef HERO_PLATFORM_WINDOWS
	long long Started;
	long long Stopped;
	static long long Frequency;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	#ifdef HERO_PLATFORM_APPLE
	struct timeval Started;
	struct timeval Stopped;
	#else
	struct timespec Started;
	struct timespec Stopped;
	#endif
	#endif

	Timer();
	~Timer();

	void Start();

	void Stop();

	void Step();

	void Reset();

	bool IsStarted();
	bool IsStopped();

	double ElapsedSeconds();
	double ElapsedMilliseconds();
	double ElapsedMicroseconds();
	double ElapsedNanoseconds();

	static double Seconds();	
	static double Milliseconds();
	static double Microseconds();

	static double Nanoseconds();

	double StartedSeconds();
	double StartedMilliseconds();
	double StartedMicroseconds();
	double StartedNanoseconds();

	double StoppedSeconds();
	double StoppedMilliseconds();
	double StoppedMicroseconds();
	double StoppedNanoseconds();

};

class Ticker
{
public:

	struct timeb Started;	
	struct timeb Stopped;	

	Ticker();
	~Ticker();

	void Start();
	void Stop();
	void Step();

	void Reset();

	bool IsStarted();
	bool IsStopped();

	long ElapsedMilliseconds();
	double ElapsedSeconds();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Timing
{
public:

	int Count;
	double Total;
	double Minimum;
	double Maximum;

	Timing():Count(0),Total(0),Minimum(0),Maximum(0)
	{
	}

	~Timing()
	{
	}

	void Sample(Timer & timer)
	{
		double elapsed = timer.ElapsedSeconds();

		if (elapsed < Minimum)
			Minimum = elapsed;

		if (elapsed > Maximum)
			Maximum = elapsed;

		++Count;
		Total+=elapsed;
	}

	double Average()
	{
		return Total/Count;
	}

	void Reset()
	{
		Count=0;
		Total=0;
		Minimum=0;
		Maximum=0;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

