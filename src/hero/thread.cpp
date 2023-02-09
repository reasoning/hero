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
#include "hero/thread.h"
#include "hero/atomic.h"
#include "hero/error.h"
#include "hero/string.h"
#include "hero/sync.h"
#include "hero/timer.h"

#ifdef HERO_PLATFORM_POSIX
    #include <sched.h>

    #ifndef __wasm__

        #include <assert.h>
        #include <pthread.h>
        #include <unistd.h>
        #include <xmmintrin.h>

    #endif

#endif

#ifdef HERO_PLATFORM_POSIX

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wunused-variable"
    #pragma clang diagnostic ignored "-Wunused-parameter"
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wreturn-stack-address"
    #pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

#endif

#ifdef HERO_PLATFORM_WINDOWS
    #include <process.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero
{

Thread::Thread(class Runnable* runnable, void* context)
    : Runnable(runnable), Context(context), Handle(0), Id(0)
{
}

Thread::Thread(class Runnable& runnable, void* context)
    : Runnable(&runnable), Context(context), Handle(0), Id(0)
{
}

Thread::Thread(const Thread& thread)
    : Runnable(thread.Runnable), Context(thread.Context), Handle(thread.Handle), Id(thread.Id)
{
}

Thread::Thread()
    : Runnable(0), Context(0), Handle(0), Id(0)
{
}

Thread::~Thread() {}

Thread& Thread::operator=(const Thread& thread)
{
    Runnable = thread.Runnable;
    Context = thread.Context;
    Handle = thread.Handle;
    Id = thread.Id;
    return *this;
}

void Thread::Sleep(unsigned int milliseconds)
{
#ifdef HERO_PLATFORM_POSIX

    #ifdef HERO_PLATFORM_APPLE

    struct timespec ts;
    ts.tv_sec = milliseconds / Timer::MillisecondsPerSecond;
    ts.tv_nsec = (milliseconds - (ts.tv_sec * Timer::MillisecondsPerSecond)) / Timer::NanosecondsPerMillisecond;
    nanosleep(&ts, 0);

    #else

    if ((milliseconds * Timer::MicrosecondsPerMillisecond) >= Timer::MicrosecondsPerSecond)
    {
        sleep((milliseconds + (Timer::MillisecondsPerSecond - 1)) / 1000);
    }
    else
    {
        usleep(milliseconds * Timer::MicrosecondsPerMillisecond);
    }

    #endif

#endif
#ifdef HERO_PLATFORM_WINDOWS
    ::Sleep(milliseconds);
#endif
}

void Thread::Yield(unsigned int spin)
{
    Assert(spin < (Timer::NanosecondsPerSecond * 10));

#ifdef HERO_PLATFORM_POSIX

    if (spin < Timer::NanosecondsPerMicrosecond)
    {
    #ifdef HERO_PLATFORM_CLANG
        _mm_pause();
    #else
        sched_yield();
    #endif
    }
    else if (spin < Timer::NanosecondsPerMillisecond)
    {
        sched_yield();
    }
    else
    {
        unsigned long micro = spin / Timer::NanosecondsPerMicrosecond;
        usleep(micro);
    }

#endif

#ifdef HERO_PLATFORM_WINDOWS

    if (spin < Timer::NanosecondsPerMicrosecond)
    {
        YieldProcessor();
    }
    else if (spin < Timer::NanosecondsPerMillisecond)
    {
        SwitchToThread();
    }
    else
    {
        Sleep(spin / Timer::NanosecondsPerMillisecond);
    }

#endif
}

void Thread::Pause()
{
#ifdef HERO_PLATFORM_POSIX
    sched_yield();
#endif
#ifdef HERO_PLATFORM_WINDOWS
    SleepEx(0, 0);
#endif
}

unsigned long Thread::Identify()
{
#ifdef HERO_PLATFORM_POSIX

    return (unsigned long)pthread_self();

#endif
#ifdef HERO_PLATFORM_WINDOWS

    return (unsigned long)GetCurrentThreadId();
#endif
}

void Thread::Name(const Substring& name)
{
#ifdef HERO_PLATFORM_WINDOWS

    const DWORD MS_VC_EXCEPTION = 0x406D1388;
    #pragma pack(push, 8)

    typedef struct tagTHREADNAME_INFO
    {
        DWORD dwType;
        LPCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags;
    } THREADNAME_INFO;

    #pragma pack(pop)
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name.Data;
    info.dwThreadID = Id;
    info.dwFlags = 0;
    #pragma warning(push)
    #pragma warning(disable : 6320 6322)
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
    #pragma warning(pop)
#endif

#ifdef HERO_PLATFORM_LINUX
    pthread_setname_np(Id, name.Data);
#endif
}

Thread Thread::Start(class Runnable* runnable, void* context)
{
    Thread thread(runnable, context);
    thread.Start();
    return thread;
}

void Thread::Start()
{
    unsigned long id = 0;
    void* handle = Handle;

    if ((long long)handle > 0)
        return;

    Id = 0;
    Handle = 0;

    Thread* thread = new Thread(*this);

#ifdef HERO_PLATFORM_WINDOWS

    #ifdef HERO_PLATFORM_WINDOWS
    unsigned int t = (unsigned int)id;
    handle = (void*)_beginthreadex(0, 0, Spawn, thread, 0, &t);
    id = t;
    #else
    handle = (void*)_beginthreadex(0, 0, Spawn, thread, 0, &id);
    #endif

    if (handle == 0)
    {
        Raise("Thread::Start - Could not create thread.\n");
    }

    if ((long long)Atomic::Cas((volatile void**)&Handle, 0, handle) != 0)
    {
        CloseHandle((void*)handle);
    }

#endif

#ifdef HERO_PLATFORM_POSIX

    pthread_attr_t attributes;
    pthread_attr_init(&attributes);

    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);

    #ifdef HERO_PLATFORM_LINUX

    pthread_t t;
    handle = (void*)pthread_create(&t, 0, Spawn, thread);
    id = t;
    Print("Thread::Start - pthread_t: %08lX, id: %ld\n", t, id);
    #else
    handle = (void*)pthread_create((pthread_t*)((void*)&id), &attributes, Spawn, thread);

    #endif

    if (handle == 0)
    {
    #ifdef HERO_PLATFORM_I386
        Atomic::Cas((volatile int*)&Handle, 0, 1);
    #else
        Atomic::Cas((volatile long long*)&Handle, 0, 1);
    #endif
    }
    else
    {
        Raise("Thread::Start - Could not create thread.\n");
    }

    pthread_attr_destroy(&attributes);

#endif

    Id = id;
}

void Thread::Stop()
{
    if (Thread::Identify() != Id)
        return;

#ifdef HERO_PLATFORM_WINDOWS

    void* handle = Handle;
    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        _endthreadex(0);
        CloseHandle((void*)handle);
    }
#endif

#ifdef HERO_PLATFORM_POSIX

    void* handle = Handle;
    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        pthread_cancel((pthread_t)Id);
    }

#endif
}

void Thread::Kill()
{
#ifdef HERO_PLATFORM_WINDOWS

    void* handle = Handle;
    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        unsigned long exit = 0;
        GetExitCodeThread((void*)handle, &exit);
        TerminateThread((void*)handle, exit);
        CloseHandle((void*)handle);
    }

#endif

#ifdef HERO_PLATFORM_POSIX

    void* handle = Handle;
    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        pthread_cancel((pthread_t)Id);
    }
#endif
}

void Thread::Join()
{
    void* handle = Handle;

    if ((long long)handle <= 0)
        return;

#ifdef HERO_PLATFORM_WINDOWS

    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        WaitForSingleObject((void*)handle, INFINITE);
        CloseHandle((void*)handle);
    }

#endif

#ifdef HERO_PLATFORM_POSIX

    if ((long long)Atomic::Cas((volatile void**)&Handle, handle, (void*)-1) > 0)
    {
        Print("Thread::Join - Joining thread %ld...\n", Id);
        Print("Thread::Join - Handle of thread %ld is %ld\n", Id, (void*)Handle);

        int error = pthread_join((pthread_t)Id, 0);
        if (error)
        {
            Print("Thread::Join - Could not join thread %ld, %s\n", Id, strerror(error));
        }
        else
        {
            Print("Thread::Join - Joined thread %ld\n", Id);
        }
    }

#endif
}

#ifdef HERO_PLATFORM_WINDOWS
unsigned int Thread::Spawn(void* spawn)
{
    Thread* thread = (Thread*)spawn;

    if (!thread->Handle)
    {
        void* process = (void*)GetCurrentProcess();
        void* pseudo = (void*)GetCurrentThread();
        void* handle = 0;
        DuplicateHandle((void*)process, (void*)pseudo, (void*)process, (void**)&handle, 0, false, DUPLICATE_SAME_ACCESS);

        if ((long long)Atomic::Cas((volatile void**)&thread->Handle, 0, handle) > 0)
        {
            CloseHandle((void*)handle);
        }
    }

    thread->Id = GetCurrentThreadId();

    if (thread->Runnable)
        thread->Runnable->Run(thread);

    void* handle = thread->Handle;
    if ((long long)Atomic::Cas((volatile void**)&thread->Handle, handle, (void*)-1) > 0)
    {
        CloseHandle((void*)handle);
    }

    unsigned long id = thread->Id;
    ThreadLocalCleanup::Singleton().Delete(id);

    delete thread;

    return 0;
}
#endif

#ifdef HERO_PLATFORM_POSIX
void* Thread::Spawn(void* spawn)
{
    Thread* thread = (Thread*)spawn;

    Atomic::Cas((volatile void**)&thread->Handle, (void*)0, (void*)1);

    thread->Id = (unsigned long)pthread_self();

    if (thread->Runnable)
        thread->Runnable->Run(thread);

    void* handle = thread->Handle;
    if ((long long)Atomic::Cas((volatile void**)&thread->Handle, handle, (void*)-1) > 0)
    {
        int error = pthread_detach((pthread_t)thread->Id);
    }

    Print("Thread::Spawn - Exiting thread %d\n", thread->Id);

    unsigned long id = thread->Id;
    ThreadLocalCleanup::Singleton().Delete(id);

    delete thread;

    return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace Hero

#ifdef __wasm__

    #ifdef HERO_PLATFORM_POSIX

        #pragma clang diagnostic pop
    #endif

#endif