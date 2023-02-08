# Hero


Hero is a superluminal object oriented framework written in a pragmatic subset of C++ called C-- 

Based on a larger framwork called Reason that was written 2 decades ago, it has no dependencies, a flat namespace, and will build without libstdc++ if you like.

- It runs on Linux and Windows.

- By default no RTTI, and no exceptions.

- Suitable for embedded code, games development, and anything where you value clean simple code and an API like no other.

Leave the C++ standard library in the dust.

Above all it is readable, maintainable code that you study, understand, customise and remix in an infinite number of ways.

This is an export from a private repository, so no comments for now.  Sorry.

## Building

Build with cmake.

cd build && cmake ../
make

Customise CMakeLists.txt for build options and compiler settings.

By default Hero will build libhero.so libhero.a, and a hero binary.

## Features

Out of the box you will have access to powerful features for systems programming.

### Binary and Strings

- Binary, and Bits for handling binary data.
- String formatting with Stream, and Perl style Pack/Unpack.
- Polymorphic string views by default with Substring
- Data processing using nested blocks with Path, and Segment.
- Argument and option parsing with Args, Options, Section, and Configuration for config files.
- Regular expression support integrated into String with Regex

### Time and Date
Time operations using high frequency Timer class, convert between any date/time format using Epoch and Calendar.  Calculate elapsed time using simple stopwatch behavior.

- Time, Date, Year, Month, Day, Calendar, Epoch
- Interval
- Timer


### Files and Folders
Using an abstraction over storage, its possible to have a File or Folder refer to contents of an archive or FTP/SSH server through the same interface.

List, create, delete, read and write files and folders.

- File 
- Folder


### Streams

A full set of streaming objects, with integration into other types in the type system.
- Stream, StreamBuffer, StreamFilter, FileStream, StringStream, FormattingStream
- StreamParser so that the Parser framework reads directly from any stream

### Reference Counting
- Weak, Strong, and Auto pointers that can point to handles or pointers with full lifecycle management and owned vs shared semantics.

- Optional and Variable for delayed creation of type.

### Multithreading

- Thread + Fibre for light weight multithreading
- SpinLock, ReadWriteLock (a recursive SpinLock)
- ThreadLocalStorage
- Event notification, Condition variables, Critical sections and Hazard pointers.
- Atomic operations and Barrier's.

### Numerics
- Big integer classes, Signed, Unsigned using the latest Ryu algorithm for fast double/string conversion.
- A generic Number object

### Data Structures

A data structure library build around high performance cache efficient arrays.  

Multi-index containers can be leveraged by using ArrayIndex, and HashIndex.

All data structures share a common interface and are sortable/unique, and have upper and lower bound semantics.

- Array, ArrayIndex, ArraySet, ArrayMap
- Vector
- HashIndex, HashSet, HashMap
- Stack, List, Hashtable
- Map, Set (by default ArrayMap/ArraySet)
 

