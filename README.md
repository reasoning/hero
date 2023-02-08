# Hero


Hero is a superluminal object oriented framework written in a pragmatic subset of `C++` called `C--`

Based on a larger framework called Reason that was written 2 decades ago, it has no dependencies, a flat namespace, and will build without libstdc++ if you like.

- It runs on Linux and Windows.

- By default no RTTI, and no exceptions.

- Suitable for embedded code, games development, and anything where you value clean simple code and an API like no other.

Leave the C++ standard library in the dust.

Above all it is readable, maintainable code that you study, understand, customise and remix in an infinite number of ways.

This is an export from a private repository, so source comments have been stripped for now.

## Building

Build with cmake.

```
cd build && cmake ../
make
```

Customise CMakeLists.txt for build options and compiler settings.

By default Hero will build libhero.so libhero.a, and a hero binary.

## Features

Out of the box you will have access to powerful features for systems programming.

### Binary and Strings

- `Binary`, and `Bits` for handling binary data.
- `String` formatting with `Stream`, `Formatter`, and Perl style `Pack/Unpack` for streams.
- Polymorphic string views by default with `Substring`
- Unlock incredibly powerful in-place data processing using nested blocks with `Path`, and `Segment`.
- Argument and option parsing with `Args, Options, Section`, and `Configuration` for config files.
- Regular expression support integrated into `String` with `Regex`

### Time and Date
Time operations using high frequency `Timer` class, convert between any date/time format using `Epoch` and `Calendar`.  Calculate elapsed time using simple stopwatch behavior.

- `Time, Date, Year, Month, Day, Calendar, Epoch`
- `Interval`
- `Timer`

### Callbacks

Near zero cost callbacks, no heap allocation for non functor or lambda types, an actual generic interface `Callback<void,int,int>` can point to an object, a function, a lambda, a functor, or an object delayed this.

- `Callback`

### Files and Folders
Using an abstraction over storage, its possible to have a File or Folder refer to contents of an archive or FTP/SSH server through the same interface.

List, create, delete, read and write files and folders.

- `File` 
- `Folder`


### Streams

A full set of streaming objects, with integration into other types in the type system.
- `Reader, Writer, Stream, StreamBuffer, StreamFilter, FileStream, StringStream, FormattingStream`
- `StreamParser` so that the `Parser` framework reads directly from any stream

### Reference Counting
- `Weak, Strong`, and `Auto` pointers that can point to handles or pointers with full lifecycle management and owned vs shared semantics.

- `Optional` and `Variable` for delayed creation of type.

### Multithreading

- `Thread/Fibre` for light weight multithreading
- `SpinLock, ReadWriteLock` (a recursive `SpinLock`)
- `ThreadLocalStorage`
- `Event` notification, `Condition` variables, `Critical` sections and `Hazard` pointers.
- `Atomic` operations and `Barrier`'s.

### Numerics
- Big integer classes, `Signed`, `Unsigned`, and `Integer` using the latest Ryu algorithm for fast double/string conversion.
- A generic `Number` object

### Error Handling

Handle errors like `C` (at the source) but with a structured take on `errno/GetLastError()`

- Raise deep errors without exceptions using scoped `Try/Catch/Raise`
- Common inbuild types for error categories via `RaiseError<ConversionError>("Message")`
- Verify and test function results using `Verify()/Verified()`


### Data Structures

A data structure library built around high performance cache efficient arrays.  

Limitless multi-index containers can be leveraged by using `ArrayIndex`, and `HashIndex`.

All data structures share a common interface and are sortable/unique, and have upper and lower bound semantics.

True generic interface for iteration, algorithms, and simple while/for loops using `Iterand`, `Iterator`, and the `Iterable` interface.  No type traits or viral templates here.

All map/set types use arrays for key/values with items available in insertion order, leveraging the index types.

Iteration speed is 0..N in a contiguous `Array` for every type but `List`

- `Slice`
- `Iterand, Iterator, Iterable`
- `Array, ArrayIndex, ArraySet, ArrayMap`
- `Vector`
- `HashIndex, HashSet, HashMap`
- `Stack, List, Hashtable`
- `Map, Set` (by default backed with `ArrayMap/ArraySet`)
 

