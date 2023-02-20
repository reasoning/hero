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
#include "hero/sync.h"
#include "hero/time.h"
#include "hero/platform.h"
#include "hero/atomic.h"
#include "hero/thread.h"

#include "hero/structure.h"

#include <stdio.h>
#include <assert.h>

#ifdef HERO_PLATFORM_POSIX
#include <unistd.h>
#endif

#ifdef HERO_PLATFORM_MINGW
#undef HERO_PLATFORM_WINDOWS
#endif

#ifdef HERO_PLATFORM_WINDOWS
#include <process.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long ThreadId()
{
	return Thread::Identify();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Critical::Critical()
{
	memset(&Handle,0,sizeof(Handle));
	Create();
}
Critical::~Critical()
{
	Delete();
}

bool Critical::Enter()
{

	#ifdef HERO_PLATFORM_WINDOWS

	__try
	{
		EnterCriticalSection((LPCRITICAL_SECTION)&Handle);

		return true;
	}
	__except (EXCEPTION_CONTINUE_EXECUTION) 
	{

		return false;
	}

	#endif 

	#ifdef HERO_PLATFORM_POSIX

	if (pthread_mutex_lock((pthread_mutex_t*)&Handle) == 0)
	{
		return true;
	}
	#endif

	return false;

}

bool Critical::Leave()
{

	#ifdef HERO_PLATFORM_WINDOWS
	__try
	{		
		LeaveCriticalSection((LPCRITICAL_SECTION)&Handle);

		return true;
	}
	__except (EXCEPTION_CONTINUE_EXECUTION) 
	{

		return false;
	}

	#endif

	#ifdef HERO_PLATFORM_POSIX
	if (pthread_mutex_unlock((pthread_mutex_t*)&Handle) == 0)
	{

		return true;
	}

	#endif

	return false;
}

bool Critical::Try()
{

	#ifdef HERO_PLATFORM_WINDOWS

	if (TryEnterCriticalSection(&Handle) != 0)
	{

		return true;
	}

	#endif

	#ifdef HERO_PLATFORM_POSIX

	if (pthread_mutex_trylock((pthread_mutex_t*)&Handle) == 0)
	{

		return true;
	}	

	#endif

	return false;
}

bool Critical::Wait()
{

	if (Try())
	{
		Leave();
		return false;
	}

	return true;
}

bool Critical::Owned()
{

	#ifdef HERO_PLATFORM_WINDOWS

	return (unsigned int)((LPCRITICAL_SECTION)&Handle)->OwningThread == GetCurrentThreadId();
	#endif

	#ifdef HERO_PLATFORM_POSIX

	struct timespec ts = Timer::Timespec(1*Timer::NanosecondsPerMillisecond);
	int result = pthread_mutex_timedlock((pthread_mutex_t*)&Handle,&ts);

	if (result == EDEADLK)
	{

		return true;
	}
	else
	if (result == 0)
	{

		Leave();
		return false;
	}
	else
	{

		return false;
	}
	#endif

}

void Critical::Create()
{

	if (*(int*)(&Handle) == 0)
	{
	#ifdef HERO_PLATFORM_WINDOWS
	InitializeCriticalSection(&Handle);
	#endif

	#ifdef HERO_PLATFORM_POSIX

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init((pthread_mutex_t*)&Handle,&attr);
	pthread_mutexattr_destroy(&attr);

	#endif
	}
}

void Critical::Delete()
{
	if (*(int*)(&Handle) != 0)
	{
	#ifdef HERO_PLATFORM_WINDOWS
	DeleteCriticalSection(&Handle);
	#endif

	#ifdef HERO_PLATFORM_POSIX
	pthread_mutex_destroy((pthread_mutex_t*)&Handle);
	#endif
	}

	memset(&Handle,0,sizeof(Handle));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Mutex::Lock(int timeout)
{
	#ifdef HERO_PLATFORM_WINDOWS
	return WaitForSingleObject(Handle,timeout) == 0;
	#endif 

	#ifdef HERO_PLATFORM_POSIX

	if (timeout != TimeoutIndefinate)
	{
		struct timespec ts = Timer::Timespec(timeout*Timer::NanosecondsPerMillisecond);
		return pthread_mutex_timedlock((pthread_mutex_t*)&Handle,&ts);
	}

	return pthread_mutex_lock((pthread_mutex_t*)&Handle) == 0;
	#endif

}

bool Mutex::Unlock()
{

	#ifdef HERO_PLATFORM_WINDOWS
	return ReleaseMutex(Handle);
	#endif

	#ifdef HERO_PLATFORM_POSIX
	return pthread_mutex_unlock((pthread_mutex_t*)&Handle) == 0;
	#endif
}

bool Mutex::Try()
{
	#ifdef HERO_PLATFORM_WINDOWS

	return WaitForSingleObject(Handle,TimeoutImmediate) == 0;
	#endif

	#ifdef HERO_PLATFORM_POSIX

	return pthread_mutex_trylock((pthread_mutex_t*)&Handle) == 0;
	#endif
}

bool Mutex::Wait(int timeout)
{

	if (Lock(timeout))
	{
		Unlock();
		return false;
	}

	return true;
}

void Mutex::Create()
{

	#ifdef HERO_PLATFORM_WINDOWS
	Handle = CreateMutex(0,false,0);
	Assert(Handle != INVALID_HANDLE_VALUE);
	#endif

	#ifdef HERO_PLATFORM_POSIX
	memset(&Handle,0,sizeof(Handle));

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init((pthread_mutex_t*)&Handle,&attr);
	pthread_mutexattr_destroy(&attr);

	#endif
}

void Mutex::Delete()
{
	#ifdef HERO_PLATFORM_WINDOWS
	CloseHandle(Handle);
	Handle = INVALID_HANDLE_VALUE;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	pthread_mutex_destroy((pthread_mutex_t*)&Handle);
	memset(&Handle,0,sizeof(Handle));
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Condition::Condition():Wakeup(0)
{
	#ifdef HERO_PLATFORM_POSIX

		pthread_cond_init(&Handle,0);
	#endif

	#ifdef HERO_PLATFORM_WINDOWS
		#if HERO_PLATFORM_WINVER >= 0x0600
		InitializeConditionVariable(&Handle);
		#endif

	#endif

}

Condition::~Condition()
{
	#ifdef HERO_PLATFORM_POSIX
		pthread_cond_destroy(&Handle);
	#endif

	#ifdef HERO_PLATFORM_WINDOWS
	#endif
}

bool Condition::Wait(Critical & critical, unsigned int timeout)
{

	#ifdef HERO_PLATFORM_POSIX

	Timer timer;
	int time = timeout;

	do
	{
		int wakeup = Wakeup;

		struct timespec ts = Timer::Timespec(time*Timer::NanosecondsPerMillisecond);	
		int result = (timeout==TimeoutIndefinate)?pthread_cond_wait(&Handle,&critical.Handle):pthread_cond_timedwait(&Handle,&critical.Handle,&ts);

		if (result == 0)
		{

			return false;
		}
		else
		if (result == ETIMEDOUT)
		{

			return true;
		}

		if (Barrier::LoadAcquire((volatile int*)&Wakeup) != wakeup)
			return false;

		time -= timer.ElapsedMilliseconds();
	}
	while(time > 0 && timer.ElapsedMilliseconds() < timeout);

	return true;

	#endif

	#ifdef HERO_PLATFORM_WINDOWS

	#if HERO_PLATFORM_WINVER >= 0x0600

	Timer timer;
	int time = timeout;

	do
	{
		int wakeup = Wakeup;

		int result = SleepConditionVariableCS(&Handle,&critical.Handle,time);
		if (result == 0)
		{
			int error = GetLastError();
			Assert(error == ERROR_TIMEOUT);
			return true;
		}

		if (Barrier::LoadAcquire((volatile int*)&Wakeup) != wakeup)
			return false;

		time -= timer.ElapsedMilliseconds();
	}
	while(time > 0 && timer.ElapsedMilliseconds() < timeout);

	return true;

	#else

	Timer timer;

	int tick = 0;
	int time = timeout;
	int result = 0;

	Critical::Auto leave(critical,Critical::Auto::MODE_LEAVE);

	do 
	{
		int wakeup = Wakeup;

		if (Atomic::Cas((volatile int*)&Handle.Signal,1,0) == 1)	
			return false;

		tick = (time>100)?100:time;			
		result = WaitForMultipleObjects(2,Handle.Events,false,tick);
		if (result < 2)
			return false;

		if (Atomic::Cas((volatile int*)&Handle.Signal,1,0) == 1)	
			return false;

		time -= tick;

		if (Barrier::LoadAcquire((volatile int*)&Wakeup) != wakeup)
			return false;

		int error = GetLastError();

		Assert(result == WAIT_TIMEOUT && error == ERROR_SUCCESS);

	}
	while(time > 0 && timer.ElapsedMilliseconds() < timeout);

	if (result >= 2)
		return true;

	#endif	
	#endif	

	return false;
}

bool Condition::Signal()
{
	Atomic::Inc((volatile int*)&Wakeup);

	#ifdef HERO_PLATFORM_POSIX

	pthread_cond_signal(&Handle);

	#endif

	#ifdef HERO_PLATFORM_WINDOWS
	#if HERO_PLATFORM_WINVER >= 0x0600
	WakeConditionVariable(&Handle);
	#else	

	Sleep(0);

	while (Atomic::Cas((volatile int*)&Handle.Signal,0,1) != 0)
		Sleep(0);

	SetEvent((HANDLE)Handle.Events[ConditionObject::SIGNAL]);

	#endif
	#endif	

	return true;
}

bool Condition::Broadcast()
{
	Atomic::Inc((volatile int*)&Wakeup);

	#ifdef HERO_PLATFORM_POSIX

	pthread_cond_broadcast(&Handle);

	#endif

	#ifdef HERO_PLATFORM_WINDOWS
	#if HERO_PLATFORM_WINVER >= 0x0600
	WakeAllConditionVariable(&Handle);

	#else

	Sleep(0);
	PulseEvent((HANDLE)Handle.Events[ConditionObject::BROADCAST]);
	#endif	
	#endif	

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Event::EventWait
{
	Critical Critical;
	Condition Condition;

	int Index;
	int Status;

	int References;
	bool All;
	bool Waiting;

	EventWait(bool all, bool waiting=false):Index(-1),Status(0),References(0),All(all),Waiting(waiting) {}	

	void Decrement()
	{
		--References;
		Assert(References >= 0);	
	}

	void Increment()
	{
		Assert(References >= 0);	
		++References;
	}
};

struct Event::EventObject
{
	Critical Critical;
	Condition Condition;
	bool Manual;
	bool Set;

	Vector<EventWait*> Waiters;

	EventObject(bool manual=false, bool set=false):Manual(manual),Set(set) 
	{

	}

	~EventObject()
	{

		for (int w=0;w<Waiters.Length();++w)
		{			

			EventWait * wait = Waiters[w];

			wait->Critical.Enter();

			wait->Decrement();
			if (wait->References == 0)
				delete wait;		

		}

		Waiters.Release();
	}
};

Event::Event(bool manual, bool set)
{

	Handle = (void*) new EventObject(manual,set);

}

Event::~Event()
{

	EventObject * object = (EventObject*)Handle;
	if (object)
		delete object;

}

bool Event::Reset() 
{
	Mutex::Auto mutex(Mutex);

	EventObject * object = (EventObject*)Handle;
	object->Set = false;

	return true;
}

bool Event::Set()
{

	Mutex::Auto mutex(Mutex);

	EventObject * object = (EventObject*)Handle;
	object->Set = true;

	for (int w=0;w<object->Waiters.Length();)
	{

		EventWait * wait = object->Waiters[w];

		wait->Critical.Enter();

		wait->Decrement();

		if (!wait->Waiting)
		{	
			wait->Critical.Leave();	

			if (wait->References == 0)
				delete wait;

			if (!object->Manual)
			{

				object->Waiters.Remove(wait,w);
			}
			else
			{

				++w;
			}

			continue;
		}		

		if (!object->Manual)
			object->Set = false;

		if (wait->All)
		{
			--wait->Status;
			Assert(wait->Status >= 0);
		}
		else
		{
			wait->Status = wait->Index;
			wait->Waiting = false;
		}

		wait->Critical.Leave();
		wait->Condition.Signal();

		if (!object->Manual)
		{

			object->Waiters.Remove(wait,w);

			return true;
		}
		else
		{

			++w;
		}
	}

	if (!object->Manual)
	{
		if (object->Set)
		{	

			object->Condition.Signal();
		}
	}
	else
	{

		object->Waiters.Release();			

		object->Condition.Broadcast();		
	}

	return true;

}

bool Event::Pulse()
{
	Mutex::Auto mutex(Mutex);

	if (Set())
		return Reset();

	return false;
}

bool Event::Wait(int timeout)
{

	if (timeout == Event::TimeoutImmediate)
	{
		if (!Mutex.Try())
			return true;
	}
	else
	{

		if (!Mutex.Lock(timeout))
			return true;
	}

	bool wait = Event::WaitForEvent(this,timeout);

	Mutex.Unlock();

	return wait;
}

int Event::WaitForSingleEvent(Event * event, int timeout)
{

	if (!event || event->Wait(timeout)) 
		return -1;

	return 0;	
}

int Event::WaitForMultipleEvents(int count, Event * events, bool all, int timeout)
{
	EventWait * wfme = new EventWait(all,true);

	wfme->References = 1;
	wfme->Status = (all)?count:-1;

	wfme->Critical.Enter();

	bool found = false;
	int index = -1;

	for (int i=0;i<count;++i)
	{
		wfme->Index = i;

		events[i].Mutex.Lock();

		EventObject * object = (EventObject*)events[i].Handle;

		for (int w=0;w<object->Waiters.Length();)
		{

			EventWait * wait = object->Waiters[w];

			if (wait->Critical.Try())
			{
				if (!wait->Waiting)
				{
					wait->Decrement();
					wait->Critical.Leave();

					if (wait->References == 0)
						delete wait;						

					object->Waiters.Remove(wait,w);

					continue;
				}

				wait->Critical.Leave();
			}

			++w;
		}

		if (!Event::WaitForEvent(&events[i],timeout))
		{

			events[i].Mutex.Unlock();
			if (all)
			{
				--wfme->Status;
				Assert(wfme->Status >= 0);
			}
			else
			{
				wfme->Status = i;
				found = true;
				break;
			}		
		}
		else
		{
			++wfme->References;
			object->Waiters.Append(wfme);				

			events[i].Mutex.Unlock();
		}
	}

	while (!found)
	{

		found = (all && wfme->Status == 0) || (!all && wfme->Status != -1);

		if (!found)
		{

			if (timeout == Event::TimeoutIndefinate)
			{
				wfme->Condition.Wait(wfme->Critical,timeout);
			}
			else
			{
				Timer timer;

				if (wfme->Condition.Wait(wfme->Critical,timeout))
					timeout -= timer.ElapsedMilliseconds();				

				if (timeout <= 0)
					break;
			}			
		}	
	}

	if (found)
		index = wfme->Status;

	wfme->Waiting = false;

	bool referenced = wfme->References > 1;

	wfme->Decrement();
	if (wfme->References == 0)
	{

		delete wfme;
	}
	else
	{
		wfme->Critical.Leave();

		if (false)		

		{
			for (int i=0;i<count;++i)
			{

				events[i].Mutex.Lock();			

				EventObject * object = (EventObject*)events[i].Handle;

				for (int w=0;w<object->Waiters.Length();)
				{

					EventWait * wait = object->Waiters[w];	

					if (wait->Critical.Try())
					{						
						if (!wait->Waiting)
						{
							wait->Decrement();
							wait->Critical.Leave();

							if (wait->References == 0)
								delete wait;

							object->Waiters.Remove(wait,w);

							continue;
						}

						wait->Critical.Leave();
					}

					++w;
				}	

				events[i].Mutex.Unlock();
			}		

		}	
	}

	return index;
}

bool Event::WaitForEvent(Event * event, int timeout)
{

	EventObject * object = (EventObject*)event->Handle;
	if (!object->Set)
	{
		if (timeout == Event::TimeoutImmediate)
			return true;

		object->Critical.Enter();
		object->Condition.Wait(object->Critical,timeout);
		object->Critical.Leave();

	}

	if (object->Set)
	{	
		if (!object->Manual)
			object->Set = false;

		return false;
	}

	return true;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Semaphore::Semaphore(int count)
{
#ifdef HERO_PLATFORM_WINDOWS

	Handle = CreateSemaphore(0,0,count,0);
	if (Handle == 0)
	{
		Raise("Semaphore::Semaphore - Error %d, Could not create semaphore\n",GetLastError());
	}

#endif
#ifdef HERO_PLATFORM_POSIX

	if (sem_init(Handle,0,count) == -1)
	{
		Raise("Semaphore::Semaphore - Error %d, Could not create semaphore\n",errno);
	}

#endif
}

Semaphore::~Semaphore()
{
#ifdef HERO_PLATFORM_WINDOWS
	CloseHandle(Handle);
#endif
#ifdef HERO_PLATFORM_POSIX

#endif

}

bool Semaphore::Enter()
{
#ifdef HERO_PLATFORM_WINDOWS

	WaitForSingleObject(Handle, INFINITE);
#endif
#ifdef HERO_PLATFORM_POSIX
	sem_post(Handle);
#endif

	return false;
}

bool Semaphore::Leave()
{
#ifdef HERO_PLATFORM_WINDOWS	
	ReleaseSemaphore(Handle,1,0);
#endif
#ifdef HERO_PLATFORM_POSIX
	sem_wait(Handle);
#endif

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Monitor::Wait(unsigned int timeout)
{

	Atomic::Inc((volatile int *)&Waiters);

	Critical::Auto critical(Critical);

	int wakeups = Wakeups;

	bool awake = false;		

	while (Waiting() && !awake)
	{

		bool wait = false;
		if (timeout != TimeoutIndefinate)
			wait = Condition.Wait(Critical,timeout);				
		else
			wait = Condition.Wait(Critical);

		if (wait && Waiting())
		{

			Atomic::Dec((volatile int*)&Waiters);
			return true;
		}

		awake = wakeups != Wakeups && !Waiting();
	}

	int waiters = Atomic::Dec((volatile int*)&Waiters);

	if (Events.Signal > 0)
	{
		if (Events.Reset)
			Reset();

		--Events.Signal;		
	}
	else
	if (Events.Broadcast > 0)
	{

		if (waiters == 0)
		{
			if (Events.Reset)
				Reset();

			--Events.Broadcast;								
		}
	}

	return false;
}

bool Monitor::Waiting()
{

	if (Events.Reset)
		return !Events.Signal && !Events.Broadcast;
	else
		return !Events.Set;

}

void Monitor::Set()
{
	Critical::Auto critical(Critical);
	Events.Set = true;
	Events.Reset = false;
	Broadcast();
}

void Monitor::Reset()
{
	Critical::Auto critical(Critical);
	Events.Set = false;
	Events.Reset = true;	
}

void Monitor::Signal()
{
	Critical::Auto critical(Critical);
	++Wakeups;
	++Events.Signal;		
	Condition.Signal();
}

void Monitor::Broadcast()
{

	Critical::Auto critical(Critical);
	++Wakeups;

	Atomic::Cas((volatile int *)&Events.Broadcast,0,1);
	Condition.Broadcast();	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ThreadLocalCleanupTuple : public Object
{
	unsigned long Id;
	int Type;
	int Index;
	ThreadLocalCleanup::CleanupFunc Func;

	ThreadLocalCleanupTuple(): 
		Id(0),Type(0),Index(0),Func(0)
	{		
	}

	ThreadLocalCleanupTuple(unsigned long id, int type, int index, ThreadLocalCleanup::CleanupFunc func):
		Id(id),Type(type),Index(index),Func(func)
	{		
	}

	int Compare(Object * object, int comparitor=Comparable::COMPARE_GENERAL)
	{
		ThreadLocalCleanupTuple * tuple = (ThreadLocalCleanupTuple*)object;	
		return Index-tuple->Index;
	}
};

typedef Set<ThreadLocalCleanupTuple> ThreadLocalCleanupTuples;

class ThreadLocalCleanupImpl
{
public:	

	SpinLock Lock;
	Map<unsigned long,ThreadLocalCleanupTuples> Cleaner;

	ThreadLocalCleanupImpl()
	{
	}

	~ThreadLocalCleanupImpl()
	{
		Vector<ThreadLocalCleanupTuple> tuples;

		{
			SpinLock::WriteLock write(Lock);
			Iterand<unsigned long> cleaner = Cleaner.Keys.Forward();
			while(cleaner)
			{
				unsigned long id = cleaner();

				Iterand<ThreadLocalCleanupTuple> tuple = Cleaner.Values[cleaner.Index].Forward();
				while(tuple)
				{				
					tuples.Append(tuple());

					++tuple;
				}			
				++cleaner;
			}

			Cleaner.Release();
		}

		for (int t=0;t<tuples.Size;++t)
		{
			ThreadLocalCleanupTuple & tuple = tuples[t];
			ThreadLocalCleanup::CleanupFunc func = tuple.Func;
			if (func) func(tuple.Id,tuple.Type,tuple.Index);
		}			
	}

};

void ThreadLocalCleanup::Delete(unsigned long id)
{
	Vector<ThreadLocalCleanupTuple> tuples;

	Assert(Impl);
	{
		SpinLock::WriteLock write(Impl->Lock);
		Result<bool,int> cleaner = Impl->Cleaner.Remove(id);
		if (cleaner)
		{
			Iterand<ThreadLocalCleanupTuple> tuple = Impl->Cleaner.At(cleaner.Index).Forward();
			while(tuple)
			{				
				tuples.Append(tuple());

				++tuple;
			}
		}	
	}

	for (int t=0;t<tuples.Size;++t)
	{
		ThreadLocalCleanupTuple & tuple = tuples[t];
		ThreadLocalCleanup::CleanupFunc func = tuple.Func;
		if (func) func(tuple.Id,tuple.Type,tuple.Index);
	}
}

void ThreadLocalCleanup::Remove(unsigned long id, int index)
{

	Assert(Impl);
	SpinLock::WriteLock write(Impl->Lock);

	ThreadLocalCleanupTuple tuple(id,0,index,0);
	Result<bool,int> cleaner = Impl->Cleaner.Select(id);
	if (cleaner)
	{
		Impl->Cleaner.At(cleaner.Index).Remove(tuple);
	}
}

void ThreadLocalCleanup::Remove(unsigned long id)
{
	Assert(Impl);
	SpinLock::WriteLock write(Impl->Lock);

	Impl->Cleaner.Remove(id);

}

void ThreadLocalCleanup::Update(unsigned long id, int type, int index, CleanupFunc func)
{
	Assert(Impl);
	SpinLock::WriteLock write(Impl->Lock);
	ThreadLocalCleanupTuple tuple(id,type,index,func);
	Result<bool,int> cleaner = Impl->Cleaner.Update(id);
	if (cleaner)
	{
		Impl->Cleaner.At(cleaner.Index).Update(tuple);
	}
}

ThreadLocalCleanup & ThreadLocalCleanup::Singleton()
{	
	static ThreadLocalCleanup cleanup;
	return cleanup;
}

ThreadLocalCleanup::ThreadLocalCleanup():Impl(0)
{
	Impl = new ThreadLocalCleanupImpl();
}

ThreadLocalCleanup::~ThreadLocalCleanup()
{
	if (Impl)
		delete Impl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX

ThreadLocalStorage::ThreadLocalStorageInit ThreadLocalStorage::Init;
#endif

ThreadLocalStorage::ThreadLocalStorage()
{	
	#ifdef HERO_PLATFORM_WINDOWS
	Index = TlsAlloc();
	#endif

	#ifdef HERO_PLATFORM_POSIX
	pthread_key_create(&Index,0);
	#endif

}

ThreadLocalStorage::~ThreadLocalStorage()
{

	#ifdef HERO_PLATFORM_WINDOWS
	TlsFree(Index);
	#endif	

	#ifdef HERO_PLATFORM_POSIX
	pthread_key_delete(Index);
	#endif	
}

bool ThreadLocalStorage::Set(void * data)
{	
	return ThreadLocalStorage::Set(Index,data);	
}

void * ThreadLocalStorage::Get()
{
	return ThreadLocalStorage::Get(Index);
}

bool ThreadLocalStorage::Set(int index, void * data)
{

	#ifdef HERO_PLATFORM_WINDOWS
	return TlsSetValue(index,data) != 0;
	#endif

	#ifdef HERO_PLATFORM_POSIX
	return pthread_setspecific(index,data) == 0;
	#endif		
}

void * ThreadLocalStorage::Get(int index)
{
	#ifdef HERO_PLATFORM_WINDOWS
	void * data = TlsGetValue(index);
	#endif	

	#ifdef HERO_PLATFORM_POSIX
	void * data = pthread_getspecific(index);
	#endif		

	return data;
}

bool ThreadLocalStorage::Del(int index)
{
	#ifdef HERO_PLATFORM_WINDOWS
	return TlsFree(index);
	#endif	

	#ifdef HERO_PLATFORM_POSIX
	return pthread_key_delete(index) == 0;
	#endif		
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_I386
typedef int RWBytes;
#endif

#ifdef HERO_PLATFORM_X86_64
typedef long long RWBytes;
#endif

bool SpinLock::LockRead(bool yield)
{
    while(true)
    {

		Assert((Bytes&ReaderBytes) < (ReaderBytes-1));

        unsigned long spin=1;
        while((Bytes&WriterBytes) != 0)
        {        

            if (!yield) return false;
				Thread::Yield(spin);
			spin = (spin)?spin<<1:1; 
        }

        Assert(Bytes < WriterBytes);

        if((Atomic::Inc((volatile RWBytes*)&Bytes)&WriterBytes) == 0)
            return true;

        Atomic::Dec((volatile RWBytes*)&Bytes);
    }
}

bool SpinLock::UnlockRead()
{
	Atomic::Dec((volatile RWBytes*)&Bytes);
	return true;
}

bool SpinLock::LockWrite(bool yield)
{
    while(true)
    {

		Assert((Bytes&WriterBytes) < (WriterBytes-1));

        unsigned long spin = 1;
        while((Bytes&WriterBytes) != 0)
        {

            if (!yield) return false;
			Thread::Yield(spin);
			spin = (spin)?spin<<1:1; 
        }

 		if((Atomic::Add((volatile RWBytes*)&Bytes,WriterBit)&WriterBytes) == 0)
        {

            unsigned long spin = 1;
            while((Bytes&ReaderBytes) != 0)
            {

                Thread::Yield(spin);
                spin = (spin)?spin<<1:1;
            }

            return true;
        }

        Atomic::Sub((volatile RWBytes*)&Bytes,WriterBit);

    }
}

bool SpinLock::UnlockWrite()
{
	Atomic::Sub((volatile RWBytes*)&Bytes, WriterBit);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ReadWriteLock::LockRead(bool yield)
{
	if (!Writer)
	{

		if (SpinLock::LockRead(yield))
		{
			unsigned long id = Thread::Identify();
			Reader.Set(id);	
		}

		return true;	
	}

	return false;
}

bool ReadWriteLock::UnlockRead()
{
	unsigned long reader = Reader.Get();
	if (reader)
	{
		SpinLock::UnlockRead();
		Reader.Set(0);
		return true;
	}

	return false;
}

bool ReadWriteLock::LockWrite(bool yield)
{

	unsigned long id = Thread::Identify();	
	if (Writer != id)
	{
		unsigned long reader = Reader.Get();		
		if (reader == id)
		{

			SpinLock::UnlockRead();
			Reader.Set(0);
		}

		if (SpinLock::LockWrite(yield))	
		{
			Writer = id;
			return true;
		}
	}

	return false;
}

bool ReadWriteLock::UnlockWrite()
{

	Writer = 0;
	SpinLock::UnlockWrite();
	return true;
}

bool ReadWriteLock::IsReader()
{
	unsigned long id = Thread::Identify();
	unsigned long & reader = Reader.Get();
	return (reader == id);
}	

bool ReadWriteLock::IsWriter()
{
	unsigned long id = Thread::Identify();
	return (Writer == id);
}

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

