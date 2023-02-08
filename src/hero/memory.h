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

#include "hero/traits.h"
#include "hero/sync.h"

#include <memory.h>
#include <memory>
#include <stdlib.h>

#ifdef HERO_USING_TRACY
#include <tracy/Tracy.hpp>
#endif






#define HERO_ALIGNED_ALLOC_SLAB
#define HERO_ALIGNED_ALLOC_SLAB_TRACY











#ifdef HERO_ALIGNED_ALLOC
#define HERO_ALIGNED(size) alignas(size)
#else
#define HERO_ALIGNED(size)
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Hero {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








struct LeakDetectorCounter
{
    struct LeakDetectorCounterImpl;
    LeakDetectorCounterImpl * Impl;

    LeakDetectorCounter(const char * tag);
    ~LeakDetectorCounter();

    void Inc();
    void Dec();
};

template<typename _Kind_, const char * _Tag_>
class LeakDetector
{
public:
    

    static LeakDetectorCounter Counter;

    LeakDetector()  
    {
        Counter.Inc();
    }

    ~LeakDetector() 
    {
        Counter.Dec();
    }


};

template<typename _Kind_, const char * _Tag_>
LeakDetectorCounter LeakDetector<_Kind_,_Tag_>::Counter(_Tag_);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LeakTrackerTable
{
    struct LeakTrackerTableImpl;
    LeakTrackerTableImpl * Impl;
    
    LeakTrackerTable(const char *);
    ~LeakTrackerTable();

    void Add(void * addr);
    void Del(void * addr);
};

template<typename _Kind_, const char * _Tag_>
class LeakTracker : LeakDetector<_Kind_,_Tag_>
{
public:

    static LeakTrackerTable Table;

    LeakTracker()
    {
        Table.Add(this);
    }

    ~LeakTracker()
    {
        Table.Del(this);
    }
};


template<typename _Kind_, const char * _Tag_>
LeakTrackerTable LeakTracker<_Kind_,_Tag_>::Table(_Tag_);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



struct SlabEntry {
    
    

    SlabEntry* Next;
    SlabEntry* Prev;

#ifndef __wasm__
    
    long long Bytes;
    
    
    long long Offset;
#else
    int Bytes;
    int Offset;
#endif 

    
    
    char Data[4];

    SlabEntry();
    ~SlabEntry();
};

struct SlabHandle {
    
    
    
    
    

    Hazard Haz;
    SlabEntry* Entry;

    SlabHandle()
        : Entry(0)
    {}
    ~SlabHandle() { Entry = 0; }
};

struct SlabList {
    SlabList();
    ~SlabList();

    
    static const int Power = 6;
    static const long long Capacity[Power];
    static SlabEntry* Head[Power];
    static int Count[Power];
    static Hazard Haz[Power];
    static long long Hits;

    static SlabEntry* Get(long long size);
    static bool Put(SlabEntry* storage);

    
};

struct SlabAllocator {
    
    
    

    
    

    
    SpinLock Lock;

    SlabAllocator(){};
    ~SlabAllocator(){};

    static SlabAllocator& Singleton();

    void* Allocate(SlabHandle& handle, long long amount, long long size);
    void* Reallocate(SlabHandle& handle, long long amount, long long size);
};

struct MemoryStorage {
    
    int Bytes;
    
    int Capacity;
    
    char Data[4];
};



struct MemoryPoolData {
    MemoryPoolData* Next;
    MemoryPoolData()
        : Next(0)
    {}
};

struct MemoryPool {
    MemoryPool();
    ~MemoryPool();

    static const int Power = 18;
    static const int Capacity[Power];
    static MemoryPoolData* Head[Power];
    static int Count[Power];
    static Hazard Haz[Power];
    static long long Hits;

    static MemoryStorage* Get(int size);
    static bool Put(MemoryStorage* storage);

    static void Clear();
};


struct MemoryAllocator {

    SpinLock Lock;
    

    MemoryAllocator(){};
    ~MemoryAllocator(){};

    static MemoryAllocator& Singleton();

    MemoryStorage* Allocate(int amount, int size);
    MemoryStorage* Reallocate(MemoryStorage* storage, int amount, int size);
};












#if defined(__linux__) || defined(__wasm__)
#ifdef HERO_ALIGNED_ALLOC

void* hero_aligned_alloc(size_t alignment, size_t size);
#else

void* hero_aligned_alloc(size_t, size_t size);
#endif







void hero_aligned_free(void* mem);










#define hero_alloc hero_aligned_alloc
#define hero_free hero_aligned_free

#endif




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

