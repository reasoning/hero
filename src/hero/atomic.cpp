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

#include "hero/hero.h"
#include "hero/atomic.h"

#include <malloc.h>
#ifdef HERO_PLATFORM_POSIX
#include <mm_malloc.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






namespace Hero {




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Atomic::Cas(volatile int * dest, int prev, int with)
{
    #ifdef __wasm__
    int value = *dest;
    if (value == prev)
        *dest = with;
    return value;
    #endif

	#ifdef HERO_PLATFORM_WINDOWS
	
	
	return InterlockedCompareExchange((long*)((int*)dest),with,prev);
	#endif
	
	#ifdef HERO_PLATFORM_POSIX
	
	#ifdef HERO_PLATFORM_APPLE
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	int value = prev;
	while (!OSAtomicCompareAndSwap32(prev,with,dest))
		if ((value = *dest) != prev) break;
	return value;
	
	#else

	#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
	
	
	return __sync_val_compare_and_swap (dest,prev,with);
	
	#else

	
	
	
	
	
	
	
	

	asm volatile("lock;cmpxchgl %1,%2":"=a"(prev):"q"(with),"m"(*dest),"0"(prev):"memory");
	return prev;	
	
	
	
	
	
	#if defined(__i386__)
	
	#endif
	
	#if defined(__x86_64__)
	
	#endif
	
	
	#endif
	
	#endif 
	
	#endif 
	
	return 0;
}

int Atomic::Swp(volatile int * dest, int with)
{
    #ifdef __wasm__
    int value = *dest;
    *dest = with;
    return value;
    #endif

	#ifdef HERO_PLATFORM_WINDOWS
	return InterlockedExchange((long*)((int*)dest),with);
	#endif

	#ifdef HERO_PLATFORM_POSIX
	
	#ifdef HERO_PLATFORM_APPLE
	
	
	
	
	
	
	int value = *dest;
	while (!OSAtomicCompareAndSwap32(value, with, dest))
		value = *dest;
	return value;
	
	#else

	
	#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
	
	
	
	
	
	
	
	
	
	
	
	
	
	return __sync_lock_test_and_set (dest,with);
	
	#else
	
	
	
	
	
	
	
	
	
	
	int value;
	asm volatile("lock;xchgl %1,%2":"=r"(value):"m"(*dest),"0"(with):"memory");
	return value;	
	
	#endif
	
	#endif 
	
	#endif 

	return 0;
}

int Atomic::Inc(volatile int * dest)
{
    #ifdef __wasm__
    *dest += 1;
    return *dest;
    #endif


	#ifdef HERO_PLATFORM_WINDOWS
	return InterlockedIncrement((long*)((int*)dest));
	#endif


	#ifdef HERO_PLATFORM_POSIX
	
	#ifdef HERO_PLATFORM_APPLE
	
	
	

 	
	
	
	
	
	
	
	
	
	

	return OSAtomicIncrement32Barrier(dest);
	
	#else

	
	#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
	
	
	return __sync_add_and_fetch(dest,1);
	
	#else
	
	
		
	
	
	

 	
 	
 	
 	

	int value;
	asm volatile("lock;incl %1;movl %1,%0":"=r"(value):"m"(*dest):"memory");
	return value;
	
	#endif
	
	#endif 
	
	#endif 

	return 0;	
	
}

int Atomic::Dec(volatile int * dest)
{
    #ifdef __wasm__
    *dest -= 1;
    return *dest;
    #endif

	#ifdef HERO_PLATFORM_WINDOWS
	return InterlockedDecrement((long*)((int*)dest));
	#endif	
	
	#ifdef HERO_PLATFORM_POSIX
	
	#ifdef HERO_PLATFORM_APPLE
	
	
	

	return OSAtomicDecrement32Barrier(dest);
	
	#else

	
	#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
	
	
	return __sync_sub_and_fetch(dest,1);
	
	#else
	
	
	
	
	
	
	
	
	int value;
	asm volatile("lock;decl %1;movl %1,%0":"=r"(value):"m"(*dest):"memory");
	return value;	

	
	#endif
	
	#endif 
	
	#endif 

	return 0;	
}

int Atomic::Add(volatile int * dest, int by)
{   
    #ifdef __wasm__
    int value = *dest;
    *dest += by;
    return value;    
    #endif

	#ifdef HERO_PLATFORM_WINDOWS
	return InterlockedExchangeAdd((long*)((int*)dest),by);
	#endif


	#ifdef HERO_PLATFORM_POSIX
	
	
	
	
	
	
	
	
	
	

	int value;
	asm volatile("lock; xaddl %2,%1":"=a"(value):"m"(*dest),"0"(by):"memory");
	return value;		
	
	
	
	#endif 

	return 0;	
}

int Atomic::Sub(volatile int * dest, int by)
{
    #ifdef __wasm__
    int value = *dest;
    *dest -= by;
    return value;
    #endif  

	#ifdef HERO_PLATFORM_WINDOWS
	return InterlockedExchangeAdd((long*)((int*)dest),-by);
	#endif


	#ifdef HERO_PLATFORM_POSIX
	
	

	

	

	
	
	
	int value;
	asm volatile("lock; xaddl %2,%1":"=a"(value):"m"(*dest),"0"(-by):"memory");
	return value;
	
	
	
	#endif 

	return 0;	
	
}

int Atomic::Mul(volatile int * dest, int by)
{
	int initial = 0;
	int result = 0;
	do 
	{
		
		
		
		
		

		if (initial != *dest)
		{
			
			
			
			initial = *dest;
			result = initial * by; 
		}
	}
	while(Cas(dest,initial,result) != initial);
	
	return result;
}

int Atomic::Div(volatile int * dest, int by)
{
	int initial = 0;
	int result = 0;
	do 
	{
		if (initial != *dest)
		{
			initial = *dest;
			result = initial / by; 
		}
	}
	while(Cas(dest,initial,result) != initial);
	
	return result;
}

int Atomic::Mod(volatile int * dest, int by)
{
	int initial = 0;
	int result = 0;
	do 
	{
		if (initial != *dest)
		{
			
			
			initial = *dest;
			result = initial % by; 
		}
	}
	while(Cas(dest,initial,result) != initial);
	
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * Atomic::Cas(volatile void ** dest, void * prev, void * with)
{
    #ifdef HERO_PLATFORM_X86_64
    return (void*)Atomic::Cas((long long *)dest,(long long)prev,(long long)with);
    #else
    return (void*)Atomic::Cas((int *)dest,(int)prev,(int)with);
    #endif
}

void * Atomic::Swp(volatile void ** dest, void * with)
{
	#ifdef HERO_PLATFORM_X86_64
    return (void*)Atomic::Swp((long long *)dest,(long long)with);
	#else
    return (void*)Atomic::Swp((int *)dest,(int)with);
	#endif    
}

void * Atomic::Add(volatile void ** dest, void * by)
{
	#ifdef HERO_PLATFORM_X86_64
    return (void*)Atomic::Add((long long *)dest,(long long)by);
	#else
    return (void*)Atomic::Add((int *)dest,(int)by);
	#endif
}

void * Atomic::Sub(volatile void ** dest, void * by)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Sub((long long *)dest,(long long)by);
	#else
	return (void*)Atomic::Sub((int *)dest,(int)by);
	#endif    
}

void * Atomic::Inc(volatile void ** dest)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Inc((long long *)dest);
	#else
	return (void*)Atomic::Inc((int *)dest);
	#endif   
}

void * Atomic::Dec(volatile void ** dest)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Dec((long long *)dest);
	#else
	return (void*)Atomic::Dec((int *)dest);
	#endif    
}

void * Atomic::Mul(volatile void ** dest, void * by)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Mul((long long *)dest,(long long)by);
	#else
	return (void*)Atomic::Mul((int *)dest,(int)by);
	#endif       
}

void * Atomic::Div(volatile void ** dest, void * by)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Div((long long *)dest,(long long)by);
	#else
	return (void*)Atomic::Div((int *)dest,(int)by);
	#endif    
}

void * Atomic::Mod(volatile void ** dest, void * by)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void*)Atomic::Mod((long long *)dest,(long long)by);
	#else
	return (void*)Atomic::Mod((int *)dest,(int)by);
	#endif     
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Atomic::Load(volatile int * dest) 
{
	return Barrier::LoadAcquire(dest);
}

void * Atomic::Load(volatile void **  dest)
{
	return Barrier::LoadAcquire(dest);
}


void Atomic::Store(volatile int * dest, int with)
{
	return Barrier::StoreRelease(dest,with);
}

void Atomic::Store(volatile void ** dest, void * with)
{
	return Barrier::StoreRelease(dest,with);
}
	
#ifdef HERO_PLATFORM_X86_64

long long Atomic::Load(volatile long long * dest)
{
	return Barrier::LoadAcquire(dest);
}

void Atomic::Store(volatile long long * dest, long long with)
{
	return Barrier::StoreRelease(dest,with);
}

#endif
	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_X86_64

long long Atomic::Cas(volatile long long * dest, long long prev, long long with)
{
    #ifdef __wasm__
    int value = *dest;
    if (value == prev)
        *dest = with;
    return value;
    #endif

    #ifdef HERO_PLATFORM_WINDOWS
    
    
    return InterlockedCompareExchange64((long long*)((long long*)dest),with,prev);
    #endif
    
    #ifdef HERO_PLATFORM_POSIX
    
    
    #ifdef HERO_PLATFORM_APPLE

    long long value = prev;
    while (!OSAtomicCompareAndSwap64(prev,with,dest))
        if ((value = *dest) != prev) break;
    return value;
    
    #else
    
    #if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0))    
    
    
    return __sync_val_compare_and_swap(dest,prev,with);
    #else
    

	asm volatile("lock; cmpxchgq %1,%2":"=a" (prev):"q"(with),"m"(*dest),"0"(prev):"memory");
	return prev;

    
    #endif 
    
    #endif 
    

    #endif
    return 0;
}

long long Atomic::Swp(volatile long long * dest, long long with)
{
    #ifdef __wasm__
    int value = *dest;
    *dest = with;
    return value;
    #endif


    #ifdef HERO_PLATFORM_WINDOWS
    return InterlockedExchange64((long long*)((long long*)dest),with);
    #endif
    
    
    #ifdef HERO_PLATFORM_POSIX
    
    
    #ifdef HERO_PLATFORM_APPLE
    
    
    
    
    int value = *dest;
    while (!OSAtomicCompareAndSwap64(value, with, dest))
        value = *dest;
    return value;
    
    #else
    
    
    #if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
    
    
    
    return __sync_lock_test_and_set (dest,with);
    #else
    

	int value;
	asm volatile("lock;xchgq %1,%2":"=r"(value):"m"(*dest),"0"(with):"memory");
	return value;	    
    

    
    #endif 
    
    #endif 
    
    
    #endif
    
    return 0;
}

long long Atomic::Inc(volatile long long * dest)
{
    #ifdef __wasm__
    *dest += 1;
    return *dest;
    #endif


    #ifdef HERO_PLATFORM_WINDOWS
    return InterlockedIncrement64((long long*)((long long*)dest));
    #endif
    
    
    #ifdef HERO_PLATFORM_POSIX
    
    
    #ifdef HERO_PLATFORM_APPLE

    return OSAtomicIncrement64Barrier(dest);
    
    #else
    
    
    #if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
    
    
    return __sync_add_and_fetch(dest,1);
    #else
    
    
	int value;
	asm volatile("lock;incq %1;movl %1,%0":"=r"(value):"m"(*dest):"memory");
	return value;	    
    
    
    #endif 
    
    #endif 
       
    #endif
    
    return 0;	
    
}

long long Atomic::Dec(volatile long long * dest)
{

    #ifdef __wasm__
    *dest -= 1;
    return *dest;
    #endif

    #ifdef HERO_PLATFORM_WINDOWS
    return InterlockedDecrement64((long long*)((long long*)dest));
    #endif	
    
    #ifdef HERO_PLATFORM_POSIX
    
    
    #ifdef HERO_PLATFORM_APPLE
    
    
    
    
    return OSAtomicDecrement64Barrier(dest);
    
    #else
    
    #if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)) 
    
    
    return __sync_sub_and_fetch(dest,1);
    #else
    
	
	int value;
	asm volatile("lock;decq %1;movl %1,%0":"=r"(value):"m"(*dest):"memory");
	return value;	    
    
  
    #endif 
    
    #endif 
    
    
    #endif
    
    return 0;	
}

long long Atomic::Add(volatile long long * dest, long long by)
{

    #ifdef __wasm__
    int value = *dest;
    *dest += by;
    return value;    
    #endif


    #ifdef HERO_PLATFORM_WINDOWS
    return InterlockedExchangeAdd64((long long*)((long long*)dest),by);
    #endif
    
    
    #ifdef HERO_PLATFORM_POSIX

 	#ifdef HERO_PLATFORM_CPP11
    
    
    
    

    
    

    
    

	
	
    #endif
    
    
    
	int value;
	asm volatile("lock; xaddq %2,%1":"=a"(value):"m"(*dest),"0"(by):"memory");
	
	
	
	
	return value;    
    
    
    
    #endif
    return 0;	
}

long long Atomic::Sub(volatile long long * dest, long long by)
{
    #ifdef __wasm__
    int value = *dest;
    *dest -= by;
    return value;
    #endif

    #ifdef HERO_PLATFORM_WINDOWS
    return InterlockedExchangeAdd64((long long*)((long long*)dest),-by);
    #endif
    
    
    #ifdef HERO_PLATFORM_POSIX   
    
    
    
	int value;
	asm volatile("lock; xaddq %2,%1":"=a"(value):"m"(*dest),"0"(-by):"memory");
	return value;    
    
    
     
    #endif
    
    return 0;	
    
}

long long Atomic::Mul(volatile long long * dest, long long by)
{
    long long initial = 0;
    long long result = 0;
    do 
    {
        
        
        
        
        
        
        if (initial != *dest)
        {
            
            
            
            initial = *dest;
            result = initial * by; 
        }
    }
    while(Cas(dest,initial,result) != initial);
    
    return result;
}

long long Atomic::Div(volatile long long * dest, long long by)
{
    long long initial = 0;
    long long result = 0;
    do 
    {
        if (initial != *dest)
        {
            initial = *dest;
            result = initial / by; 
        }
    }
    while(Cas(dest,initial,result) != initial);
    
    return result;
}

long long Atomic::Mod(volatile long long * dest, long long by)
{
    long long initial = 0;
    long long result = 0;
    do 
    {
        if (initial != *dest)
        {
            
            
            initial = *dest;
            result = initial % by; 
        }
    }
    while(Cas(dest,initial,result) != initial);
    
    return result;
}


#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Barrier::StoreAcquire(volatile int * dest, int with)
{
	
	
	Atomic::Swp(dest,with);
}


void Barrier::StoreRelease(volatile int * dest, int with)
{
	
	
	AtomicCompilerBarrier();
	*dest = with;
}

int Barrier::LoadAcquire(volatile int * dest)
{
	
	int value = *dest;
	AtomicCompilerBarrier();
	return value;
}


int Barrier::LoadRelease(volatile int * dest)
{
	AtomicMemoryBarrier();
	return *dest;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Barrier::StoreAcquire(volatile void ** dest, void * with)
{
	#ifdef HERO_PLATFORM_X86_64
	StoreAcquire((volatile long long *)dest,(long long)with);
	#else
	StoreAcquire((volatile int *)dest,(int)with);
	#endif
}


void Barrier::StoreRelease(volatile void ** dest, void * with)
{
	#ifdef HERO_PLATFORM_X86_64
	StoreRelease((volatile long long *)dest,(long long)with);
	#else
	StoreRelease((volatile int *)dest,(int)with);
	#endif
}

void * Barrier::LoadAcquire(volatile void ** dest)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void *) LoadAcquire((volatile long long *)dest);
	#else
	return (void *) LoadAcquire((volatile int *)dest);
	#endif
}


void * Barrier::LoadRelease(volatile void ** dest)
{
	#ifdef HERO_PLATFORM_X86_64
	return (void *) LoadRelease((volatile long long *)dest);
	#else
	return (void *) LoadRelease((volatile int *)dest);
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_X86_64

void Barrier::StoreAcquire(volatile long long * dest, long long with)
{
	Atomic::Swp((volatile long long *)dest,(long long)with);
}


void Barrier::StoreRelease(volatile long long * dest, long long with)
{
	AtomicCompilerBarrier();
	*dest = with;
}

long long Barrier::LoadAcquire(volatile long long * dest)
{
	long long value = *dest;
	AtomicCompilerBarrier();
	return value;
}


long long Barrier::LoadRelease(volatile long long * dest)
{
	AtomicMemoryBarrier();
	return *dest;
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_WINDOWS



#endif


#ifdef HERO_PLATFORM_POSIX


void AtomicCompilerBarrier()
{
	asm volatile("":::"memory");
}

void AtomicMemoryBarrier()
{
	#ifdef HERO_PLATFORM_I386
	
	
	volatile int value = 0;
	Atomic::Swp(&value,1);
	#endif
	
	#ifdef HERO_PLATFORM_X86_64
	
	asm volatile("mfence":::"memory");
	#endif
}

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////