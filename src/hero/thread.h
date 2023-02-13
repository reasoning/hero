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

#include "hero/generic.h"
#include "hero/traits.h"
#include "hero/assert.h"
#include "hero/callback.h"

#ifdef HERO_PLATFORM_WINDOWS

	#include <process.h>    

#endif

#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wreturn-stack-address"

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Thread;

class Runnable
{
public:

	virtual void Run(Thread * thread)
	{
	}

	virtual void Run(void *)
	{	
	}

};

class Thread
{
public:

	class Runnable * Runnable;
	void * Context;

  Strong<void *,Shared> Handle;	

  unsigned long Id;

	Thread(class Runnable * runnable, void * context=0);
	Thread(class Runnable & runnable, void * context=0);
	Thread(const Thread & thread);
	Thread();
	~Thread();

	Thread & operator = (const Thread & thread);

	void Start();		
	void Stop();
	void Join();
	void Kill();

	static void Sleep(unsigned int milliseconds = 1000);

	static unsigned long Identify();

	void Name(const Substring & name);

    #if defined(HERO_PLATFORM_WINDOWS) || defined(HERO_PLATFORM_MINGW)

    #undef Yield
    #endif

    static void Yield(unsigned int spin=0);    

	static void Pause();

	static Thread Start(class Runnable * runnable, void * pointer);

protected:
	#ifdef HERO_PLATFORM_WINDOWS
		static unsigned int __stdcall Spawn(void * spawn);
	#endif

	#ifdef HERO_PLATFORM_POSIX
		static void * Spawn(void * spawn);
	#endif

};

class Fibre
{
public:

	class Thread Thread;
	Hero::Callback<void, void *> Callback;
	void * Context;

	class Runner : public Runnable
	{
	public:

		Runner()
		{

		}

		void Run(class Thread * thread)
		{
			Fibre * fibre = (Fibre*)thread->Context;

			fibre->Thread = *thread;
			fibre->Callback((void*)fibre);
			delete fibre;
		}
	};

	static Runner Run;

	Fibre():Thread(0),Context(0)
	{

	}

	Fibre(const Fibre & fibre):
		Thread(fibre.Thread),Callback(fibre.Callback),Context(fibre.Context)
	{		
	}

	Fibre(const Hero::Callback<void, void *> callback, void * context=0):
		Thread(0),Callback(callback),Context(context)
	{
	}

	Fibre & operator = (const Fibre & fibre)
	{
		Thread = ((Fibre&)fibre).Thread;
		Callback = fibre.Callback;
		Context = fibre.Context;
		return *this;
	}	

	~Fibre()
	{
	}

	Fibre & Start();
	void Join();
	void Stop();

	static Fibre Start(Hero::Callback<void, void *> callback, void * context=0);
	static void Join(Fibre & fibre);
	static void Stop(Fibre & fibre);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic pop

#endif