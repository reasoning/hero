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

#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"

#endif

#ifdef __wasm__

#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/memory.h"
#include "hero/thread.h"
#include "hero/sync.h"
#include "hero/binary.h"
#include "hero/assert.h"

#ifdef HERO_PLATFORM_POSIX
#ifndef __wasm__
#include <x86intrin.h>
#endif
#endif

#ifdef HERO_USING_TRACY
#include <tracy/Tracy.hpp>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LeakDetectorCounter::LeakDetectorCounterImpl
{

    int Count;
    #ifdef HERO_PLATFORM_X86_64
    long long Peak; 
    #else
    int Peak;
    #endif

    String Tag;

    LeakDetectorCounterImpl(const char * tag):Count(0),Peak(0),Tag(tag) {}
    ~LeakDetectorCounterImpl() 
    {
        PrintLn("LeakDetector<%s> Peak: %lld, Count: %d",Tag.Data,Peak,Count);
        Assert(Count >= 0);
    }
};

LeakDetectorCounter::LeakDetectorCounter(const char * tag)
{
    Impl = new LeakDetectorCounterImpl(tag);
}

LeakDetectorCounter::~LeakDetectorCounter()
{
    if (Impl) delete Impl;    
}

void LeakDetectorCounter::Inc()
{
    Atomic::Inc((volatile int *)&Impl->Count);
    #ifdef HERO_PLATFORM_X86_64
    Atomic::Inc((volatile long long *)&Impl->Peak);
    #else
    Atomic::Inc((volatile int *)&Impl->Peak);
    #endif

}

void LeakDetectorCounter::Dec()
{
    Atomic::Dec((volatile int *)&Impl->Count);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LeakTrackerTable::LeakTrackerTableImpl
{
    SpinLock Lock;
    HashMap<void*,int> Address;
    String Tag;

    LeakTrackerTableImpl(const char * tag):Tag(tag) {}
    ~LeakTrackerTableImpl() {}

};

LeakTrackerTable::LeakTrackerTable(const char * tag)
{
    Impl = new LeakTrackerTableImpl(tag);
}

LeakTrackerTable::~LeakTrackerTable()
{
    delete Impl;    
}

void LeakTrackerTable::Add(void * addr)
{
    SpinLock::WriteLock write(Impl->Lock);

    Result<bool,int> map = Impl->Address.Insert(addr,1);
    if (!map)

        Impl->Address.At(map.Index)++;
}

void LeakTrackerTable::Del(void * addr)
{
    SpinLock::WriteLock write(Impl->Lock);

    Result<bool,int> map = Impl->Address.Select(addr);
    if (!map)
    {   
        PrintLn("LeakTracker<%s> Address not found [%08LX]",Impl->Tag.Data,addr);
        Assert(false);
    }

    int & value = Impl->Address.At(map.Index);

    if (value < 0)
    {

        PrintLn("LeakTracker<%s> Double delete detected [%08LX] (%d)",Impl->Tag.Data,addr,value);
        Assert(false);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* AlignedAlloc(size_t alignment, size_t size)
{
    #ifdef HERO_PLATFORM_WINDOWS
    return _aligned_malloc(alignment,size);
    #else
    return aligned_alloc(alignment,size);
    #endif
}

void AlignedFree(void* data)
{
    #ifdef HERO_PLATFORM_WINDOWS
    _aligned_free(data);
    #else
    free(data);
    #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SlabEntry::SlabEntry()
    : Next(0)
    , Prev(0)
    , Bytes(0)
    , Offset(0)
{

    memset(Data, 0, 4);

}

SlabEntry::~SlabEntry() {}

SlabEntry* SlabList::Get(long long size)
{

    if (size < Capacity[0])
        size = Capacity[0];
    int index = 0;
    while (size > Capacity[index] && index < Power)
        ++index;

    long long power = Capacity[index];
    if (power < size)
        power = size;

    Assert(power > Capacity[Power - 1] || (index >= 0 && index <= Power - 1));
    Assert(power > Capacity[Power - 1] || Capacity[index] == power);

    SlabEntry* entry = 0;

    if ((power > Capacity[Power - 1] && (power = size)) || Head[index] == 0) {

        char* data = new char[sizeof(SlabEntry) + power];
        entry = new (((char*)data)) SlabEntry();
    } else {

        SlabEntry* head = 0;
        SlabEntry* next = 0;

        do {
            head = Head[index];
            if (head) {
                Hazard::Setter haz(Haz[index]);
                if (!haz.Set) {
                    Thread::Yield();

                    break;
                }

                head = (SlabEntry*)Atomic::Load((volatile void**)&Head[index]);
                if (!head)
                    break;

                next = (SlabEntry*)Atomic::Load((volatile void**)&head->Next);

                Atomic::Swp((volatile void**)&next->Prev, 0);
                Atomic::Swp((volatile void**)&Head[index], next);

                SlabEntry* data = head;
                data->Prev = 0;
                data->Next = 0;

                entry = new ((char*)data) SlabEntry();
                Atomic::Dec((volatile int*)&Count[index]);

                break;
            }

        } while (head);

        if (!entry) {

            char* data = new char[sizeof(SlabEntry) + power];
            entry = new (((char*)data)) SlabEntry();
        }
    }

    entry->Bytes = power;

    Assert(power == entry->Bytes);

    return entry;
}

bool SlabList::Put(SlabEntry* entry)
{

    long long size = entry->Bytes;
    int index = 0;
    while (size > Capacity[index] && index < Power)
        ++index;

    long long power = Capacity[index];
    if (power < size)
        power = size;

    Assert(power > Capacity[Power - 1] || (index >= 0 && index <= Power - 1));
    Assert(power > Capacity[Power - 1] || Capacity[index] == power);
    Assert(power > Capacity[Power - 1] || power == entry->Bytes);

    if (power > Capacity[Power - 1] || Count[index] > 100) {
        entry->~SlabEntry();
        delete[](char*) entry;
    } else {
        SlabEntry* data = (SlabEntry*)entry;

        SlabEntry* head = 0;
        SlabEntry* next = 0;

        do {
            Hazard::Setter haz(Haz[index]);
            if (!haz.Set) {

                entry->~SlabEntry();
                delete[](char*) entry;

                break;
            }

            entry->~SlabEntry();
            head = (SlabEntry*)Atomic::Load((volatile void**)&Head[index]);

            data->Next = head;
            data->Prev = 0;
            Atomic::Swp((volatile void**)head->Prev, data);

            Atomic::Swp((volatile void**)&Head[index], data);
            Atomic::Inc((volatile int*)&Count[index]);

            break;

        } while (true);
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const long long SlabList::Capacity[] = { 0x100000, 0xA00000, 0x6400000, 0x40000000, 0x280000000, 0x500000000 };
SlabEntry* SlabList::Head[] = {};
int SlabList::Count[] = {};
Hazard SlabList::Haz[] = {};
long long SlabList::Hits = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SlabAllocator& SlabAllocator::Singleton()
{

    static SlabAllocator slab = SlabAllocator();
    return slab;
}

void* SlabAllocator::Allocate(SlabHandle& handle, long long amount, long long size)
{
    return Reallocate(handle, amount, size);
}

void* SlabAllocator::Reallocate(SlabHandle& handle, long long amount, long long size)
{
    long long bytes = amount * size;

    if (bytes <= 0)
        return 0;

    long long pad = 0x800;
    bytes += pad * 2;

    do 
    {

        Hazard::Setter haz(handle.Haz);
        if (!haz.Set) 
        {

            Thread::Yield();
            continue;
        }

        SlabEntry* entry = (SlabEntry*)Atomic::Load((volatile void**)&handle.Entry);
        long long offset = 0;

        if (entry) 
        {

            Assert(entry->Offset < entry->Bytes);

            Assert(bytes > 0);
            #ifndef __wasm__
            offset = Atomic::Load((volatile long long*)&(entry->Offset));
            #else
            offset = Atomic::Load((volatile int*)&(entry->Offset));
            #endif 

            if (bytes > entry->Bytes - offset) {

                long long next = (bytes > entry->Bytes) ? bytes : entry->Bytes;

                entry = SlabList::Get(next);

                #ifndef __wasm__
                if (entry)
                    offset = Atomic::Load((volatile long long*)&(entry->Offset));
                #else
                if (entry)
                    offset = Atomic::Load((volatile int*)&(entry->Offset));
                #endif 
            }

        } 
        else 
        {

            entry = SlabList::Get(bytes);

            #ifndef __wasm__
            if (entry)
                offset = Atomic::Load((volatile long long*)&(entry->Offset));
            #else
            if (entry)
                offset = Atomic::Load((volatile int*)&(entry->Offset));
            #endif 
        }

        if (entry) 
        {

            Assert(entry->Offset < entry->Bytes);

            char* data = entry->Data + offset + pad;

            (*(long long*)(data - sizeof(long long))) = bytes;

            if (*data != 0) 
            {
                Raise("SlabAllocator::Reallocate: Entry data was not null");

            }

            Assert(data >= entry->Data && data <= (entry->Data + entry->Bytes));

            if (handle.Entry == entry) 
            {

#ifndef __wasm__
                Atomic::Add((volatile long long*)&(entry->Offset), bytes);
#else
                Atomic::Add((volatile int*)&(entry->Offset), bytes);
#endif

                if (*(entry->Data + entry->Offset) != 0) 
                {
                    Raise("SlabAllocator::Reallocate: Entry data was not null");

                }

            } 
            else 
            {

                memset(entry->Data, 0, entry->Bytes);
            }

            if (handle.Entry != entry)
                Atomic::Swp((volatile void**)&(handle.Entry), (void*)entry);

            Assert(entry->Offset < entry->Bytes);

            return (void*)data;

        } 
        else 
        {

            Raise("SlabAllocator::Reallocate: Allocation failed ", bytes, " at ", (void*)entry);
            return 0;
        }
    }
    while (true);

    return 0;
}

MemoryPool::MemoryPool() {}

MemoryPool::~MemoryPool() {}

void MemoryPool::Clear()
{
    int index = 0;
    for (; index < Power; ++index) 
    {
        do 
        {
            Hazard::Setter haz(Haz[index]);
            if (!haz.Set) 
            {
                Thread::Yield();
                continue;
            }

            MemoryPoolData* head = 0;
            MemoryPoolData* prev = 0;

            head = Head[index];
            while (head) {
                prev = head;
                head = head->Next;

                AlignedFree((char*)prev);

            }

            Head[index] = 0;
            break;
        } 
        while (true);
    }
}

MemoryStorage* MemoryPool::Get(int size)
{

    if (size < 4)
        size = 4;
    int power = Binary::NextPowerOfTwo((unsigned int)size - 1);
    int index = Binary::BitIndex((unsigned int)power) - 2;

    Assert(power > Capacity[Power - 1] || (index >= 0 && index <= Power - 1));
    Assert(power > Capacity[Power - 1] || Capacity[index] == power);

    MemoryStorage* storage = 0;

    if ((power > Capacity[Power - 1] && (power = size)) || Head[index] == 0) 
    {

        char* data = (char*)AlignedAlloc(32, sizeof(MemoryStorage) + power);

        storage = new (((char*)data)) MemoryStorage();
    } 
    else 
    {

        MemoryPoolData* head = 0;
        MemoryPoolData* next = 0;

        do 
        {
            head = Head[index];
            if (head) 
            {
                Hazard::Setter haz(Haz[index]);
                if (!haz.Set) 
                {
                    Thread::Yield();
                    continue;

                    break;
                }

                head = (MemoryPoolData*)Atomic::Load((volatile void**)&Head[index]);
                if (!head)
                    break;

                next = (MemoryPoolData*)Atomic::Load((volatile void**)&head->Next);

                Atomic::Swp((volatile void**)&Head[index], next);

                MemoryPoolData* data = head;
                data->Next = 0;
                storage = new ((char*)data) MemoryStorage();
                Atomic::Dec((volatile int*)&Count[index]);

                break;
            }

        } 
        while (head);

        if (!storage) 
        {

            char* data = (char*)AlignedAlloc(32, sizeof(MemoryStorage) + power);

            storage = new (((char*)data)) MemoryStorage();
        }
    }

    storage->Bytes = power;

    Assert(power == storage->Bytes);

    return storage;
}

bool MemoryPool::Put(MemoryStorage* storage)
{

    int size = storage->Bytes;
    Assert(size >= 4);
    int power = Binary::NextPowerOfTwo((unsigned int)size - 1);
    int index = Binary::BitIndex((unsigned int)power) - 2;

    Assert(power > Capacity[Power - 1] || (index >= 0 && index <= Power - 1));
    Assert(power > Capacity[Power - 1] || Capacity[index] == power);
    Assert(power > Capacity[Power - 1] || power == storage->Bytes);

    if (power > Capacity[Power - 1] || Count[index] > 1000) 
    {
        storage->~MemoryStorage();

        AlignedFree((char*)storage);

    } 
    else 
    {
        MemoryPoolData* data = (MemoryPoolData*)storage;

        MemoryPoolData* head = 0;
        MemoryPoolData* next = 0;

        do 
        {
            Hazard::Setter haz(Haz[index]);
            if (!haz.Set) 
            {

                storage->~MemoryStorage();

                AlignedFree((char*)storage);

                break;
            }

            storage->~MemoryStorage();
            head = (MemoryPoolData*)Atomic::Load((volatile void**)&Head[index]);
            data->Next = head;

            Atomic::Swp((volatile void**)&Head[index], data);
            Atomic::Inc((volatile int*)&Count[index]);

            break;

        } 
        while (true);
    }

    return true;
}

const int MemoryPool::Capacity[] = { 4,    8,    16,   32,    64,    128,    256,    512,   1024,
                                  2048, 4096, 8192, 16384, 65536, 131072, 262144, 524288 };
MemoryPoolData* MemoryPool::Head[] = {};
int MemoryPool::Count[] = {};
Hazard MemoryPool::Haz[] = {};
long long MemoryPool::Hits = 0;

MemoryAllocator& MemoryAllocator::Singleton()
{

    static MemoryAllocator allocator = MemoryAllocator();
    return allocator;
}

MemoryStorage* MemoryAllocator::Allocate(int amount, int size)
{
    return Reallocate(0, amount, size);
}

MemoryStorage* MemoryAllocator::Reallocate(MemoryStorage* storage, int amount, int size)
{
    int bytes = amount * size;
    if (bytes > 0) 
    {
        if (storage) 
        {

            Assert(bytes > 0);

            storage->Capacity = bytes;

            if (bytes < storage->Bytes)
                return storage;
        }

        storage = MemoryPool::Get(bytes);
        Assert(storage != 0);
    }

    return storage;
}

struct AlignedAllocSlab {
    SlabHandle Handle;

    AlignedAllocSlab()
    {

        SlabAllocator::Singleton().Allocate(Handle, 0x280000000, 1);
    }

    static AlignedAllocSlab* Singleton()
    {
        static AlignedAllocSlab alloc;
        return &alloc;
    }
};

void* SlabAlignedAlloc(size_t size)
{
    #ifdef HERO_ALIGNED_ALLOC_SLAB_TRACY

    void* data = malloc(size);

    #ifdef HERO_USING_TRACY

    TracyAllocS(data, size, 16);
    #endif

    return data;
    #else

    SlabHandle & handle = AlignedAllocSlab::Singleton()->Handle;
    void * data = SlabAllocator::Singleton().Reallocate(handle, size, 1);

    if (size > 0x40000000)
    {
        Raise("SlabAlignedAlloc: Large alloc of ", size ," at ", data);
    }

    return data;

    #endif

}

static long long leaked = 0;

void SlabAlignedFree(void* data)
{
    #ifdef HERO_ALIGNED_ALLOC_SLAB_TRACY

    #ifdef HERO_USING_TRACY

    TracyFreeS(data, 16);
    #endif

    return free(data);

    #else

    long long bytes = (*(long long*)(((char*)data) - sizeof(long long)));

    if (bytes == 0 || bytes > 0x500000000) 
    {
        Raise("SlabAlignedFree: Large or zero deallocation detected, possible memory underflow/overflow");
    } 
    else 
    {
        leaked += bytes;
    }

    #endif

}

#ifdef HERO_ALIGNED_ALLOC
void* HeroAlignedAlloc(size_t alignment, size_t size)
#else
void* HeroAlignedAlloc(size_t, size_t size)
#endif
{

    void* data = 0;

#ifdef HERO_ALIGNED_ALLOC_SLAB

    #ifdef HERO_ALIGNED_ALLOC
    size += alignment - (size % alignment);
    #endif

    data = SlabAlignedAlloc(size);
    if (data)
        return data;

#endif

#ifdef HERO_ALIGNED_ALLOC
    size_t pad = size + alignment - (size % alignment);

    size += alignment - (size % alignment);
    data = aligned_alloc(alignment, size);
#else

    data = malloc(size);
#endif

    if (data == 0) 
    {
        Raise("HeroAlignedAlloc: Bad alloc of size: ", size);    
    }
    return data;
}

void HeroAlignedFree(void* mem)
{

    #ifdef HERO_ALIGNED_ALLOC_SLAB
    SlabAlignedFree(mem);
    #else
    free(mem);
    #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic pop

#endif