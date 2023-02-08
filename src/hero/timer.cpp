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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4244)

#include "hero/timer.h"
#include "hero/error.h"


#include <memory.h>


#ifdef HERO_PLATFORM_MINGW


#undef HERO_PLATFORM_POSIX

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef HERO_PLATFORM_WINDOWS
long long Timer::Frequency = 0;
#endif


Timer::Timer()
{
	
	Reset();
	Start();
}

Timer::~Timer()
{
	
}

void Timer::Start()
{
	
	

	#ifdef HERO_PLATFORM_WINDOWS
	LARGE_INTEGER integer;
	QueryPerformanceCounter(&integer);
	Started = (long long)integer.QuadPart;
	Stopped = 0;
	#endif

	#ifdef HERO_PLATFORM_POSIX

	#ifdef HERO_PLATFORM_APPLE
	if (gettimeofday(&Started,0) != 0)
	#else
    if (clock_gettime(CLOCK_REALTIME,&Started) != 0)
	#endif
    {
		Raise("Timer::Start - Failed to get time for high resolution timer\n");        
    }

	memset(&Stopped,0,sizeof(struct timespec));

	#endif

	#ifdef HERO_PLATFORM_APPLE
	
	
	#endif
	

}

void Timer::Step()
{
	
	
	
	

	#ifdef HERO_PLATFORM_WINDOWS
	if (IsStarted())
		Stopped = 0;
	else 
		Start();
	#endif


	#ifdef HERO_PLATFORM_POSIX
	if (IsStarted())
		#ifdef HERO_PLATFORM_APPLE
		memset(&Stopped,0,sizeof(struct timeval));
		#else
		memset(&Stopped,0,sizeof(struct timespec));
		#endif
	else
		Start();
	#endif

}

void Timer::Stop()
{
	
	

	#ifdef HERO_PLATFORM_WINDOWS
	LARGE_INTEGER integer;
	QueryPerformanceCounter(&integer);
	Stopped = (long long)integer.QuadPart;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	
    
	
	#ifdef HERO_PLATFORM_APPLE
	if (gettimeofday(&Stopped,0) != 0)
	#else
    if (clock_gettime(CLOCK_REALTIME,&Stopped) != 0)
	#endif
    {
		Raise("Timer::Stop - Failed to get time for high resolution timer\n");        
    }
	#endif

}

void Timer::Reset()
{
	
	

	#ifdef HERO_PLATFORM_WINDOWS
	Started = 0;
	Stopped = 0;
	if (!Frequency)
	{
		LARGE_INTEGER integer;
		QueryPerformanceFrequency(&integer);
		Frequency = (long long)integer.QuadPart;
	}
	
	#endif


	#ifdef HERO_PLATFORM_POSIX
	#ifdef HERO_PLATFORM_APPLE
	memset(&Started,0,sizeof(struct timeval));
	memset(&Stopped,0,sizeof(struct timeval));	
	#else
	memset(&Started,0,sizeof(struct timespec));
	memset(&Stopped,0,sizeof(struct timespec));
	
	#endif
	#endif
}

bool Timer::IsStarted()
{
	
	
	#ifdef HERO_PLATFORM_WINDOWS
	return Started != 0;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	#ifdef HERO_PLATFORM_APPLE
	return Started.tv_sec != 0 || Started.tv_usec != 0;
	#else
	return Started.tv_sec != 0 || Started.tv_nsec != 0;
	#endif
	#endif

}
bool Timer::IsStopped()
{
	

	#ifdef HERO_PLATFORM_WINDOWS
	return Stopped != 0;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	#ifdef HERO_PLATFORM_APPLE
	return Stopped.tv_sec != 0 || Stopped.tv_usec != 0;
	#else
	return Stopped.tv_sec != 0 || Stopped.tv_nsec != 0;
	#endif
	#endif

}


double Timer::Seconds()
{
	return (double)Timer::Nanoseconds() / Timers::NanosecondsPerSecond;
}

double Timer::Milliseconds()
{
	return (double)Timer::Nanoseconds() / Timers::NanosecondsPerMillisecond;
}

double Timer::Microseconds()
{
	return (double)Timer::Nanoseconds() / Timers::NanosecondsPerMicrosecond;
}

double Timer::Nanoseconds()
{
	
	
	
	
	
	
	
	
	
	Timer timer;

	#ifdef HERO_PLATFORM_WINDOWS
	
	
	
	
	
	
	
	
	double nanoseconds = (double)(timer.Started) / ((double)timer.Frequency / Timers::NanosecondsPerSecond);
	
	
	return nanoseconds;
	
	
	
	
	#endif

	#ifdef HERO_PLATFORM_POSIX

	
	
	
	
	long long secondsToNanoseconds = timer.Started.tv_sec * Timers::NanosecondsPerSecond;
	
	#ifdef HERO_PLATFORM_APPLE
	return  secondsToNanoseconds + (timer.Started.tv_usec * Timers::NanosecondsPerMicrosecond);
	#else
	return  secondsToNanoseconds + timer.Started.tv_nsec;
	#endif

	#endif
}


double Timer::ElapsedSeconds()
{
	

	return (double)ElapsedNanoseconds() / Timers::NanosecondsPerSecond;
}


double Timer::ElapsedMilliseconds()
{
	

	return (double)ElapsedNanoseconds() / Timers::NanosecondsPerMillisecond;
}

double Timer::ElapsedMicroseconds()
{
	return (double)ElapsedNanoseconds() / Timers::NanosecondsPerMicrosecond;
}

double Timer::ElapsedNanoseconds()
{

	#ifdef HERO_PLATFORM_WINDOWS
	if (IsStopped())
	{
		return  (double)(Stopped-Started) / ((double)Frequency / Timers::NanosecondsPerSecond);
	}
	else
	if (IsStarted())
	{
		LARGE_INTEGER integer;
		QueryPerformanceCounter(&integer);
		long long now = (long long)integer.QuadPart;		

		return  (double)(now-Started) / ((double)Frequency / Timers::NanosecondsPerSecond);
	}
	else
	{
		return 0;
	}
	#endif

	#ifdef HERO_PLATFORM_POSIX
	
	
	
	
	if (IsStopped())
	{
		long long secondsToNanoseconds = (Stopped.tv_sec - Started.tv_sec) * Timers::NanosecondsPerSecond;
		#ifdef HERO_PLATFORM_APPLE
		return secondsToNanoseconds + ((Stopped.tv_usec - Started.tv_usec) * Timers::NanosecondsPerMicrosecond);
		#else	
		return  secondsToNanoseconds + (Stopped.tv_nsec - Started.tv_nsec);
		#endif
	}
	else
	if (IsStarted())
	{
		#ifdef HERO_PLATFORM_APPLE
		struct timeval now;
		if (gettimeofday(&now,0) != 0)
		#else
		struct timespec now;
		if (clock_gettime(CLOCK_REALTIME,&now) != 0) 
		#endif
		{
			Raise("Timer::Start - Failed to get time for high resolution timer\n");        
			return 0;
		}

		long long secondsToNanoseconds = (now.tv_sec - Started.tv_sec) * Timers::NanosecondsPerSecond;
		#ifdef HERO_PLATFORM_APPLE
		return  secondsToNanoseconds + ((now.tv_usec - Started.tv_usec) * Timers::NanosecondsPerMicrosecond);
		#else
		return  secondsToNanoseconds + (now.tv_nsec - Started.tv_nsec);
		#endif
	}
	else
	{
		return 0;
	}
	#endif

}



double Timer::StartedSeconds()
{
	return (double)StartedNanoseconds() / Timers::NanosecondsPerSecond;
}

double Timer::StartedMilliseconds()
{
	return (double)StartedNanoseconds() / Timers::NanosecondsPerMillisecond;
}

double Timer::StartedMicroseconds()
{
	return (double)StartedNanoseconds() / Timers::NanosecondsPerMicrosecond;
}

double Timer::StartedNanoseconds()
{
	if (IsStarted())
	{
		#ifdef HERO_PLATFORM_WINDOWS
		double nanoseconds = (double)(Started) / ((double)Frequency / Timers::NanosecondsPerSecond);
		return nanoseconds;
		#endif

		#ifdef HERO_PLATFORM_POSIX
		
		long long secondsToNanoseconds = Started.tv_sec * Timers::NanosecondsPerSecond;	
		#ifdef HERO_PLATFORM_APPLE
		return  secondsToNanoseconds + (Started.tv_usec * Timers::NanosecondsPerMicrosecond);
		#else
		return  secondsToNanoseconds + Started.tv_nsec;
		#endif
		
		#endif
	}
	
	return 0.0;
}


double Timer::StoppedSeconds()
{
	return (double)StoppedNanoseconds() / Timers::NanosecondsPerSecond;
}

double Timer::StoppedMilliseconds()
{
	return (double)StoppedNanoseconds() / Timers::NanosecondsPerMillisecond;
}

double Timer::StoppedMicroseconds()
{
	return (double)StoppedNanoseconds() / Timers::NanosecondsPerMicrosecond;
}

double Timer::StoppedNanoseconds()
{
	if (IsStopped())
	{
		#ifdef HERO_PLATFORM_WINDOWS
		double nanoseconds = (double)(Stopped) / ((double)Frequency / Timers::NanosecondsPerSecond);
		return nanoseconds;
		#endif

		#ifdef HERO_PLATFORM_POSIX
		
		long long secondsToNanoseconds = Stopped.tv_sec * Timers::NanosecondsPerSecond;	
		#ifdef HERO_PLATFORM_APPLE
		return  secondsToNanoseconds + (Stopped.tv_usec * Timers::NanosecondsPerMicrosecond);
		#else
		return  secondsToNanoseconds + Stopped.tv_nsec;
		#endif
		
		#endif
	}
	
	return 0.0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Ticker::Ticker()
{
	
	Reset();
	Start();
}

Ticker::~Ticker()
{
	
}

void Ticker::Start()
{
	memset(&Stopped,0,sizeof(struct timeb));
	ftime(&Started);
}

void Ticker::Step()
{
	if (IsStarted())
		memset(&Stopped,0,sizeof(struct timeb));
	else
		Start();
}

void Ticker::Stop()
{
	if (IsStarted())
		ftime(&Stopped);
}

void Ticker::Reset()
{
	memset(&Started,0,sizeof(struct timeb));
	memset(&Stopped,0,sizeof(struct timeb));
}

bool Ticker::IsStarted()
{
	return Started.time != 0;
}
bool Ticker::IsStopped()
{
	return Stopped.time != 0;
}

long Ticker::ElapsedMilliseconds()
{
	if (IsStopped())
	{
		long secondsToMilliseconds = (Stopped.time - Started.time) * 1000;
		return  secondsToMilliseconds + (Stopped.millitm - Started.millitm);
	}
	else
	if (IsStarted())
	{
		struct timeb now;
		ftime(&now);
		
		long secondsToMilliseconds = (now.time - Started.time) * 1000;
		return  secondsToMilliseconds  + (now.millitm - Started.millitm);
	}
	else
	{
		return 0;
	}
}

double Ticker::ElapsedSeconds()
{
	if (IsStopped())
	{
		return (Stopped.time - Started.time) + ((double)(Stopped.millitm - Started.millitm) / 1000);
	}
	else
	if (IsStarted())
	{
		
		struct timeb now;
		ftime(&now);
		return (now.time - Started.time) + ((double)(now.millitm - Started.millitm) / 1000);
	}
	else
	{
		return 0;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////