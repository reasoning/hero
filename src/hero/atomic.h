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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/hero.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

#ifdef __wasm__
#define HERO_PLATFORM_64BIT
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Atomic
{
public:

	static void And();
	static void Or();
	static void Xor();

	static int Cas(volatile int * dest, int prev, int with);

	static int Swp(volatile int * dest, int with);

	static int Add(volatile int * dest, int by);
	static int Sub(volatile int * dest, int by);

	static int Inc(volatile int * dest);
	static int Dec(volatile int * dest);

	static int Mul(volatile int * dest, int by);
	static int Div(volatile int * dest, int by);
	static int Mod(volatile int * dest, int by);

	static void * Cas(volatile void ** dest, void * prev, void * with);
	static void * Swp(volatile void ** dest, void * with);

	static void * Add(volatile void ** dest, void * by);
	static void * Sub(volatile void ** dest, void * by);

	static void * Inc(volatile void ** dest);
	static void * Dec(volatile void ** dest);

	static void * Mul(volatile void ** dest, void * by);
	static void * Div(volatile void ** dest, void * by);
	static void * Mod(volatile void ** dest, void * by);    

	#ifdef HERO_PLATFORM_X86_64

	static long long Cas(volatile long long * dest, long long prev, long long with);
	static long long Swp(volatile long long * dest, long long with);

	static long long Add(volatile long long * dest, long long by);
	static long long Sub(volatile long long * dest, long long by);

	static long long Inc(volatile long long * dest);
	static long long Dec(volatile long long * dest);

	static long long Mul(volatile long long * dest, long long by);
	static long long Div(volatile long long * dest, long long by);
	static long long Mod(volatile long long * dest, long long by);

	#endif

	static int Load(volatile int * dest);
	static void * Load(volatile void **  dest);

	static void Store(volatile int * dest, int with);
	static void Store(volatile void ** dest, void * with);

	#ifdef HERO_PLATFORM_X86_64

	static long long Load(volatile long long * dest);
	static void Store(volatile long long * dest, long long with);

	#endif

};

class Barrier
{
public:

	template<typename _Kind_>
	static _Kind_* Load(_Kind_ ** dest, _Kind_  * value)
	{

		volatile _Kind_ * barrier = (volatile _Kind_ *)value;
		*dest = (_Kind_*)barrier;
		return value;
	}

	template<typename _Kind_>
	static _Kind_* Store(_Kind_ ** dest, _Kind_  * value)
	{
		volatile _Kind_ ** barrier = (volatile _Kind_**)dest;
		*barrier = value; 
		return value;
	}

	static int LoadAcquire(volatile int * dest);
	static int LoadRelease(volatile int * dest);

	static void StoreAcquire(volatile int  * dest, int with);
	static void StoreRelease(volatile int * dest, int with);

	static void * LoadAcquire(volatile void ** dest);
	static void * LoadRelease(volatile void ** dest);

	static void StoreAcquire(volatile void ** dest, void * with);
	static void StoreRelease(volatile void ** dest, void * with);

	#ifdef HERO_PLATFORM_X86_64

	static long long LoadAcquire(volatile long long * dest);
	static long long LoadRelease(volatile long long * dest);

	static void StoreAcquire(volatile long long * dest, long long with);
	static void StoreRelease(volatile long long * dest, long long with);

	#endif
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_WINDOWS

#define AtomicCompilerBarrier _ReadWriteBarrier
#define AtomicMemoryBarrier MemoryBarrier

#endif

#ifdef HERO_PLATFORM_POSIX

void AtomicCompilerBarrier();

void AtomicMemoryBarrier();

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////