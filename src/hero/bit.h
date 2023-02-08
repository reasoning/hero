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


#include "hero/assert.h"
#include "hero/indices.h"
#include "hero/platform.h"







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#ifdef HERO_PLATFORM_WINDOWS

	typedef unsigned __int8		u8;
	typedef unsigned __int16	u16;
	typedef unsigned __int32	u32;
	typedef unsigned __int64	u64;

	typedef __int8		i8;
	typedef __int16		i16;
	typedef __int32		i32;
	typedef __int64		i64;

#endif


#ifdef HERO_PLATFORM_POSIX

	typedef unsigned char		u8;
	typedef unsigned short		u16;
	typedef unsigned int		u32;
	typedef unsigned long long	u64;

	typedef char		i8;
	typedef short		i16;
	typedef int			i32;
	typedef long long	i64;

#endif



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define HERO_BIT_COUNT_TABLE

class Bit
{
public:

	static char Counts[];

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

	static int Count(unsigned short bits)
	{
		#ifdef HERO_BIT_COUNT_TABLE
		return Counts[bits&0xFF] + Counts[bits>>8];
		#else
		bits -= (bits & 0xaaaa) >> 1;
		bits =  (bits & 0x3333) + ((bits >> 2) & 0x3333);
		bits =  (bits + (bits >> 4)) & 0x0f0f;
		bits += bits >> 8;     
		return (int)(bits) & 0xff;
		#endif
	}


	static int Count(unsigned int bits)
	{
		#ifdef HERO_BIT_COUNT_TABLE
		return Counts[bits&0xFF] + Counts[bits>>8] + Counts[bits>>16] + Counts[bits>>24];
		#else	
		bits -= (bits & 0xaaaaaaaa) >> 1;
		bits =  (bits & 0x33333333) + ((bits >> 2) & 0x33333333);
		bits =  (bits + (bits >> 4)) & 0x0f0f0f0f;
		bits += bits >> 8;     
		bits += bits >> 16;    
		return (int)(bits) & 0xff;
		#endif
	}

	static int Count(unsigned long long bits)
	{
		#ifdef HERO_BIT_COUNT_TABLE
		return Count(bits&0xFFFFFFFF) + Count(bits>>32);
		#else			
		bits -= (bits & 0xaaaaaaaaaaaaaaaaLL) >> 1;
		bits =  (bits & 0x3333333333333333LL) + ((bits >> 2) & 0x3333333333333333LL);
		bits =  (bits + (bits >> 4)) & 0x0f0f0f0f0f0f0f0fLL;
		bits += bits >> 8;     
		bits += bits >> 16;    
		return ((int)(bits) + (int)(bits >> 32)) & 0xff;
		#endif
	}


public:

	unsigned char * Byte;	
	short Index;			
	short Mask;				

	Bit(unsigned char *byte, int index)
	{
		Set(byte,index);
	}

	void Set(unsigned char *byte, int index)
	{
		Byte = byte;
		Index = index;
		Mask = 1 << Index;
	}
	
	bool Get()
	{
		return (*Byte&Mask) != 0;
	}

	void On()
	{
		*Byte |= Mask;
	}

	void Off()
	{
		*Byte &= ~Mask;
	}

	operator unsigned char () 
	{
		return *Byte & Mask;
	}
	
	Bit & operator = (Bit bit)
	{
		(bit==0)?Off():On();
		return *this;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define FLAGS_GENERATOR(Type) \
Type Bits;\
inline bool Is(Type bits)				{ return(Bits & bits)!=0||Bits==bits;}\
inline bool Equals(Type bits)			{ return(Bits == bits);}\
inline void Xor(Type bits)				{ Bits ^= bits; }\
inline void On(Type bits)				{ Bits |= bits; }\
inline void On()						{ Bits |= ~0; }\
inline void Off(Type bits)				{ Bits &= ~bits; }\
inline void Off()						{ Bits &= 0; }\
inline void Flip()						{ Bits = ~Bits; }\
inline void Assign(Type bits)			{ Bits = bits; }\
inline Type Union(Type bits)			{ return Bits | bits; }\
inline Type Difference(Type bits)		{ return Bits & ~bits; }\
inline Type Intersection(Type bits)		{ return Bits & bits; }\
inline operator Type ()					{ return Bits;}\
inline Type & operator = (Type bits)	{ Assign(bits);	return Bits; }\
inline Type & operator |= (Type bits)	{ On(bits);	return Bits; }\
inline Type & operator &= (Type bits)	{ Bits &= bits; return Bits; }\
inline void Clear()						{ Bits = 0; }\
inline Type operator [](int i)			{ return (Bits & ((Type)(1)<<i)); }\
inline int Count(Type bits=0) { return Bit::Count((bits==0)?Bits:bits);}

#define FLAGS_GENERATOR_STATIC(Type) \
static bool Is(Type bitmask, Type bits)				{ return(bitmask & bits)!=0||bitmask==bits; };\
static bool Equals(Type bitmask, Type bits)			{ return(bitmask == bits); };\
static void Xor(Type &bitmask, Type bits)			{ bitmask ^= bits; };\
static void On(Type &bitmask, Type bits)			{ bitmask |= bits; };\
static void On(Type &bitmask)						{ bitmask |= ~0; };\
static void Off(Type &bitmask, Type bits)			{ bitmask &= ~bits; };\
static void Off(Type &bitmask)						{ bitmask &= 0; };\
static void Flip(Type &bitmask)						{ bitmask = ~bitmask; };\
static void Assign(Type &bitmask, Type bits)			{ bitmask = bits; };\
static Type Union(Type bitmask, Type bits)			{ return bitmask | bits; };\
static Type Difference(Type bitmask, Type bits)		{ return bitmask & ~bits; };\
static Type Intersection(Type bitmask, Type bits)	{ return bitmask & bits; };\
static void Clear(Type bitmask, Type bits)			{ bitmask = 0; };\
static Type Bit(Type bitmask, int i)				{ return (bitmask & ((Type)(1)<<i)); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
class Mask
{
public:

	FLAGS_GENERATOR_STATIC(unsigned char)
	FLAGS_GENERATOR_STATIC(unsigned short)
	FLAGS_GENERATOR_STATIC(unsigned int)
	FLAGS_GENERATOR_STATIC(unsigned long long)
	
};



template<int _Size_=8>
class Flags : public Mask
{
public:

	
	

	Flags():Bits(0){};
	Flags(unsigned char bits):Bits(bits){}
	FLAGS_GENERATOR(unsigned char);

};	

template<>
class Flags<16> : public Mask
{
public:
	Flags():Bits(0){};
	Flags(unsigned short bits):Bits(bits){};
	FLAGS_GENERATOR(unsigned short);

};	

template<>
class Flags<32> : public Mask
{
public:
	Flags():Bits(0){};
	Flags(unsigned int bits):Bits(bits){};
	FLAGS_GENERATOR(unsigned int);

};

template<>
class Flags<64> : public Mask
{
public:
	Flags<64>():Bits(0){};
	Flags<64>(unsigned long long bits):Bits(bits){};
	FLAGS_GENERATOR(unsigned long long);

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bitset
{
public:

	#ifdef HERO_PLATFORM_I386
	static const unsigned int BitsetFill = 0xFFFFFFFFU;
	static const unsigned int BitsetSize = 32;
	static const unsigned int BitsetMask = 31;
	static const unsigned int BitsetLog2 = 5;
	
	typedef unsigned int Kind;

	#endif

	#ifdef HERO_PLATFORM_X86_64
	static const unsigned long long BitsetFill = 0xFFFFFFFFFFFFFFFFUL;
	static const unsigned long long BitsetSize = 64;
	static const unsigned long long BitsetMask = 63;
	static const unsigned long long BitsetLog2 = 6;
	
	typedef unsigned long long Kind;

	#endif
	
	

	int Allocated;
	int Size;
	Kind * Data;

	#define BitsetSlots(size) ((size+BitsetMask)/BitsetSize)
	#define BitsetAllocated() (Allocated*BitsetSize)



	Bitset(int size, bool value):
		Allocated(BitsetSlots(size)),Size(size),Data(0)
	{
		
		
		Data = (Kind*) new Kind(Allocated);
		Fill(value);
	}


	Bitset():
		Allocated(0),Size(0),Data(0)
	{
	}

	~Bitset()
	{	
		delete Data;
	}

	Kind & Slot(int index)
	{
		int slots = BitsetSlots(Size);
		Indices::Normalise(index,slots);
		Assert(index < slots);

		
		Kind & slot = Data[index/BitsetSize];
		return slot;
	}

	bool Get(int index)
	{
		Kind & slot = Slot(index);

		
		return ((1 & (slot >> (index&BitsetMask))) == 1);
	}

	void Set(int index, bool value)
	{
		if (value)
			On(index);
		else
			Off(index);
	}

	void On(int index)
	{
		
		
		Kind & slot = Slot(index);
		Kind bit = ((Kind)1 << (index&BitsetMask));
		slot |= bit;	
	}


	void Off(int index)
	{
		Kind & slot = Slot(index);
		Kind bit = ((Kind)1 << (index&BitsetMask));
		slot &= ~bit;
	}


	void Resize(int size, bool value)
	{
		int allocated = BitsetSlots(size);
		if (allocated > Allocated)
		{
			Kind * slots = (Kind*) new Kind(allocated);
			if (Data)
			{
				for (int i=0;i<Allocated;++i)
					slots[i] = Data[i];
					
				delete Data;		
			}
			
			Data = slots;
			Allocated = allocated;		
		}
		
		
		if (size > Size)
			Fill(Size,size,value);

		Size = size;
		

	}
	
	void Fill(bool value)
	{
		Kind bytes = (value)?(~0):0;
		int slots = BitsetSlots(Size);
		for (int i=0;i<slots;++i)
			Data[i] = bytes;	
	}

	
	
	
	
	
	void Fill(int from, int to, bool value)
	{
		Indices::Normalise(from,Size);
		Indices::Normalise(to,Size);
		Assert(from <= to);
		Assert(to < BitsetAllocated());
		
		
		int fromSlot = (from/BitsetSize), toSlot = (to/BitsetSize);
		int fromBit = (from&BitsetMask), toBit = (to&BitsetMask);
		
		Kind lowMask = (BitsetFill<<fromBit);
		Kind highMask = (BitsetFill>>(BitsetMask-toBit));
		
		
		if (fromSlot == toSlot)
		{
			Kind mask = (highMask & lowMask);
			if (value)
				Data[fromSlot] |= mask;
			else
				Data[fromSlot] &= (~mask);				
		}
		else
		{
			
			if (value)
			{
				Data[fromSlot] |= lowMask;
				Data[toSlot] |= highMask;
			}
			else
			{
				Data[fromSlot] &= (~lowMask);
				Data[toSlot] &= (~highMask);
			}	
			
			
			Kind bytes = (value)?(~0):0;
			for (int i=fromSlot+1;i<toSlot;++i)
				Data[i] = bytes;
		}
	}
	
	int Count(int from, int to)
	{
		Indices::Normalise(from,Size);
		Indices::Normalise(to,Size);
		Assert(from <= to);
		Assert(to < BitsetAllocated());
		
		
		int fromSlot = (from/BitsetSize), toSlot = (to/BitsetSize);
		int fromBit = (from&BitsetMask), toBit = (to&BitsetMask);
		
		Kind lowMask = (BitsetFill<<fromBit);
		Kind highMask = (BitsetFill>>(BitsetMask-toBit));
		
		
		if (fromSlot == toSlot)
		{			
			Kind mask = (highMask & lowMask);
			return Bit::Count(Data[fromSlot]&mask);
		}
		else
		{
			
			int count = 0;
			count += Bit::Count(Data[fromSlot]&lowMask);
			count += Bit::Count(Data[toSlot]&highMask);
			
			
			for (int i=fromSlot+1;i<toSlot;++i)
				count += Bit::Count(Data[i]);
				
			return count;
		}		
	}

		

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


