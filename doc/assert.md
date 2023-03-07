# Assert

Assert provides macros and utilities to assert behavior at runtime.

It uses debugger detection to automatically create breakpoints when an assert fails
during a debug session.

It can also be used to instrument code and force a breakpoint either directly or 
with a condition.

It can be enabled in either release or debug but is by default disabled in release unless
`HERO_ASSERT_RELEASE`is defined.

Unlike traditional assert, Hero assert never causes any stack unwind, and you can always
continue stepping through the code in the debugger once a breakpoint is hit.

## Usage

Simple usage examples.

```cpp

// Assert a basic condition
int index=0;
Assert(index >=0);

// Force a breakpoint directly
if (index == 0)
	AssertBreak();

```

Assert can also be used to raise an AssertError using the stack based Try/Catch/Raise
mechanism in Hero.  This provides deep error handling like exceptions but without stack
unwind overhead, and still allowing for correct local error handling like C.

```cpp
AssertRaise();
```

## Implementation

Internalyl the Assert macro uses the `int 3` assembly instruction to create a breakpoint
and the `__debugbreak()` intrinsic on Win64.

```cpp
inline bool AssertBreak()
{
		#ifdef HERO_PLATFORM_WINDOWS
    if (IsDebuggerPresent())
    {
	    #ifdef HERO_PLATFORM_X86_64
	    __debugbreak();
	    #else
	    __asm{int 3};
	    #endif
    }
		#endif

    #ifdef HERO_PLATFORM_POSIX
    if (IsDebuggerPresent())
    {
        asm("int $3");
    }
    #endif
    return true;
}
```

It also uses an `IsDebuggerPresent()` call which is available natively on Windows but
implemented using the following filesystem trick on Linux.

```cpp
int IsDebuggerPresent()
{
    // https://stackoverflow.com/questions/3596781/how-to-detect-if-the-current-process-is-being-run-by-gdb
    // GDB apparently opens FD(s) 3,4,5 (whereas a typical prog uses only stdin=0, stdout=1,stderr=2)

    int rc = 0;
    FILE *fd = fopen("/tmp", "r");
    if (fileno(fd) > 5)
        rc = 1;

    fclose(fd);
    return rc;
}
```