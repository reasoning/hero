#Atomic

Atomic provides atomic operations and barriers (load/store semantics) for 32 and 64 bit
primitive types.

## Usage

Atomic features static methods which can be called as in this example with a Hazard 
pointer type.

```cpp
class Hazard
{
public:

	void * Bytes;

	Hazard():Bytes(0) {}

	bool Set(bool set)
	{
		if (set)
		{
			if (Atomic::Inc((volatile void **)&Bytes) == (void*)1)

				return true;

			Atomic::Dec((volatile void **)&Bytes);

			return false;		
		}
		else
		{
			Atomic::Dec((volatile void **)&Bytes);

			return true;
		}
	}

...
};

```


## Implementation


The semantics of the various methods follow the Windows primitives most closely i.e. 
`InterlockedExchangeAdd`.  

This is because in some cases like `Atomic::Add()` there is 
a choice between two different semantics like `__sync_fetch_and_sub` vs 
`__sync_sub_and_fetch`.  

The difference is that one version returns the value before modification, and the
other after. Hero uses the one which allows for consistent cross platform behavior
and also the most pragmatic for implementing lock free algorithms
based on personal experience.

Some methods also fallback to assembly for the implementation

```cpp
int value;
	asm volatile("lock; xaddl %2,%1":"=a"(value):"m"(*dest),"0"(-by):"memory");
	return value;
```

The full interface for Atomic is as follows.

```cpp
class Atomic
{
public:

	// These are all 32 bit operations, so if an integer isnt 32 bits on your platform
	// forget about it...

	static void And();
	static void Or();
	static void Xor();
	
	//static bool Exchange();	
	//static bool CompareExchange(void ** dest, void * with, void * exch);	

	// Atomic exchange dest and with, only if the previous value is prev. Returns the 
	// previous value of dest, which should match prev if successful.
	static int Cas(volatile int * dest, int prev, int with);
	
	// Atomic exchange dest and with, returns the previous value of dest.
	static int Swp(volatile int * dest, int with);

	// Atomic add/sub dest and return the initial value.
	static int Add(volatile int * dest, int by);
	static int Sub(volatile int * dest, int by);

	// Atomic inc/dec dest and return the resulting value
	static int Inc(volatile int * dest);
	static int Dec(volatile int * dest);

	static int Mul(volatile int * dest, int by);
	static int Div(volatile int * dest, int by);
	static int Mod(volatile int * dest, int by);

	//static void Increment();
	//static void Decrement();

	static void * Cas(volatile void ** dest, void * prev, void * with);
	static void * Swp(volatile void ** dest, void * with);
    

	static void * Add(volatile void ** dest, void * by);
	static void * Sub(volatile void ** dest, void * by);
    
	static void * Inc(volatile void ** dest);
	static void * Dec(volatile void ** dest);
    
	static void * Mul(volatile void ** dest, void * by);
	static void * Div(volatile void ** dest, void * by);
	static void * Mod(volatile void ** dest, void * by);
...
};
```


Barrier provides load store according to the principles outlined here
[Acquire Release Semantics](http://preshing.com/20120913/acquire-and-release-semantics/)

```cpp
class Barrier
{
public:

	template<typename _Kind_>
	static _Kind_* Load(_Kind_ ** dest, _Kind_  * value)
	{
		// Passing anything through volatile on recent versions visual studio should
		// force a memory barrier.
			
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
...
};
```

