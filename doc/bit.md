# Bit

The heavily used `Flags<>` template which has
specialisations for 8, 16, 32, and 64 bit sizes and has static 
and non static methods for bit manipulation.

The interface is defined by a macro and provides the following bit
and flag (bitmask) operations for clean semantics.

```cpp
Type Bits;
inline bool Is(Type bits)				{ return(Bits & bits)!=0||Bits==bits;}
inline bool Equals(Type bits)			{ return(Bits == bits);}
inline void Xor(Type bits)				{ Bits ^= bits; }
inline void On(Type bits)				{ Bits |= bits; }
inline void On()						{ Bits |= ~0; }
inline void Off(Type bits)				{ Bits &= ~bits; }
inline void Off()						{ Bits &= 0; }
inline void Flip()						{ Bits = ~Bits; }
inline void Assign(Type bits)			{ Bits = bits; }
inline Type Union(Type bits)			{ return Bits | bits; }
inline Type Difference(Type bits)		{ return Bits & ~bits; }
inline Type Intersection(Type bits)		{ return Bits & bits; }
inline operator Type ()					{ return Bits;}
inline Type & operator = (Type bits)	{ Assign(bits);	return Bits; }
inline Type & operator |= (Type bits)	{ On(bits);	return Bits; }
inline Type & operator &= (Type bits)	{ Bits &= bits; return Bits; }
inline void Clear()						{ Bits = 0; }
inline Type operator [](int i)			{ return (Bits & ((Type)(1)<<i)); }
inline int Count(Type bits=0)           { return Bit::Count((bits==0)?Bits:bits);}

```


The `Bit` class has methods for counting bit population in various sized unsigned primitive types.

Bit also contains short definitions for unsigned and signed integer types.

```cpp
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef char		i8;
typedef short		i16;
typedef int			i32;
typedef long long	i64;
```

These are preferred to standard types like unit64_t both due to simplicity and also
because these types are not defined when not using the C++ standard
library, which is one of the ways Hero can operate.

Internally Hero only uses basic primitive types and almost always signed integers.

Due to conversion to larger type and overflow errors unsigned is avoided for all but special handle or bit/binary values.

## Usage 

The `Bit` class instance can point to an external char * pointer value (the byte) and a bit index.  From this it will construct a mask for that bit, it then has two methods to turn the but on and off.

Similar behavior can be had from the `Flag<>` template.


```cpp
char byte = 0x7F;

// Bit is dead simple, and probably never used
Bit bit;
bit.Set(&byte,8);

bit.On();
Assert(bit.Get() && byte == 0xFF);

// Flags is used everywhere as the generic bitmask type for
// enum values and options in Hero
Flags<8> flags;
flags.On(byte);
flags.Flip();
Assert(flags.Is(0x80));

```

There is also a `Bitset` class which provides yet another
growable bit array with methods to get/set and fill bits.

`Bitset` also provides a method to count a range of bits.

```cpp

Bitset set;

// Fill does not resize, this is a basic class
set.Resize(16,0);
set.Fill(4,12,true);

```

## Implementation

There are many ways to count the bit population such as [Hamming weight](https://en.wikipedia.org/wiki/Hamming_weight)


The fastest is with a 65536 entry table of half words (16bit) which is a trade off of memory vs performance but requires 65Kb.  The next best is a smaller table of bytes (8bit) and then its the original bit twiddling (Hackers Delight) solution below and finally the CPU popcnt 
instruction or compiler intrinsic like GCC's `__builtin_popcount`.


```cpp
static int Count(unsigned char bits)
{
    #ifdef HERO_BIT_COUNT_TABLE
    return Counts[bits];
    #else
    bits -= (bits & 0xaa) >> 1;
    bits =  (bits & 0x33) + ((bits >> 2) & 0x33);
    bits =  (bits + (bits >> 4)) & 0x0f;
    return (int)(bits) & 0xff;
    #endif
}
```