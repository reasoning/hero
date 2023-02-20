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

#include "hero/hero.h"
#include "hero/string.h"
#include "hero/number.h"
#include "hero/stream.h"

#include "hero/bit.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef HERO_PLATFORM_WASM

#ifdef HERO_PLATFORM_LINUX
#include <x86intrin.h>
#define HERO_BIT_EXPECT
#endif

#ifdef HERO_PLATFORM_WINDOWS
#include <intrin.h>
#include <vadefs.h>
#endif

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

typedef unsigned char Byte;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Bits
{
public:

	enum BitDirection
	{
		DIRECTION_NONE		=0,
		DIRECTION_LEFT		=-1,		
		DIRECTION_RIGHT		=1,			
	};

	enum BitScheme
	{
		SCHEME_NONE			=(0),

		SCHEME_INDEX_LEFT	=(0),	
		SCHEME_INDEX_RIGHT	=(1)<<1,
		SCHEME_INDEX = SCHEME_INDEX_LEFT|SCHEME_INDEX_RIGHT,

		SCHEME_BITS_LEFT	=(1)<<2,
		SCHEME_BITS_RIGHT	=(0),	
		SCHEME_BITS = SCHEME_BITS_LEFT|SCHEME_BITS_RIGHT
	};

	static inline int Count(int bits, int size=sizeof(int))
	{

		size = size<<3;

		return (bits + size-1)/size;
	}

	static inline int Index(int bit, int size=sizeof(int))
	{
		size = size<<3;

		return bit/size;
	}

	static inline int Mask(int bits, int index=0, int size=sizeof(int))
	{
		size = size<<3;

		int mask = (int)((1LL<<bits)-1)<<((size-bits)-index);

		return mask;
	}

	static int Append(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int fromScheme=0, int size=sizeof(int));
	static int Prepend(int & to, int from, int bits, int toIndex=-1, int fromIndex=0, int fromScheme=0, int size=sizeof(int));

	static int Insert(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int toScheme=0, int fromScheme=0, int size=sizeof(int));

	static int Twiddle(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int toScheme=0, int fromScheme=0, int size=sizeof(int))
	{
		return Insert(to,from,bits,toIndex,fromIndex,toScheme,fromScheme,size);
	}

	static inline void Move(void * to, void * from, int bits)
	{

	}

	static inline void Copy(void * to, void * from, int bits)
	{
		int bytes = bits+7/8;

        Assert(&from != &to);
		Assert((&from+bytes < &to) || (&to+bytes < &from));

		const int size = sizeof(int);

		for (;bytes>size;bytes-=size,from=(int*)from+1,to=(int*)to+1)
		{
			*(int*)to = *(int*)from;
		}

		for ( ; bytes%size != 0; --bytes,from=(char*)from+1,to=(char*)to+1 )
		{
			*(char*)to = *(char*)from;
		}

		char mask = (char) Mask(bits%8,0,sizeof(char));
		*(char*)to = *(char*)from & mask;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Hashing
{
public:

	class OneAtATime
	{
	public:

		static unsigned int Hash(unsigned char *key, int length)
		{
			unsigned int hash = 0;
			int i;

			for (i = 0; i < length; i++) {
				hash += key[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
			}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			return hash;
		}
	};

	class Lookup3Hash
	{
	public:

		#define Hashsize(n) ((unsigned int)1<<(n))
		#define Hashmask(n) (Hashsize(n)-1)
		#define Rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

		#define Mix(a,b,c) \
		{ \
			a -= c;  a ^= Rot(c, 4);  c += b; \
			b -= a;  b ^= Rot(a, 6);  a += c; \
			c -= b;  c ^= Rot(b, 8);  b += a; \
			a -= c;  a ^= Rot(c,16);  c += b; \
			b -= a;  b ^= Rot(a,19);  a += c; \
			c -= b;  c ^= Rot(b, 4);  b += a; \
		}

		#define Final(a,b,c) \
		{ \
			c ^= b; c -= Rot(b,14); \
			a ^= c; a -= Rot(c,11); \
			b ^= a; b -= Rot(a,25); \
			c ^= b; c -= Rot(b,16); \
			a ^= c; a -= Rot(c,4);  \
			b ^= a; b -= Rot(a,14); \
			c ^= b; c -= Rot(b,24); \
		}

		static unsigned int Hashword(const unsigned int * key, int length, unsigned int primary)
		{
			unsigned int a,b,c;

			a = b = c = 0xdeadbeef + (((unsigned int)length)<<2) + primary;

			while (length > 3)
			{
				a += key[0];
				b += key[1];
				c += key[2];
				Mix(a,b,c);
				length -= 3;
				key += 3;
			}

			switch(length)
			{ 
			case 3 : c+=key[2];
			case 2 : b+=key[1];
			case 1 : a+=key[0];
				Final(a,b,c);
			case 0:     
				break;
			}

			return c;
		}

		static void Hashword (const unsigned int * key, int length, unsigned int * primary, unsigned int * secondary)
		{
			unsigned int a,b,c;

			a = b = c = 0xdeadbeef + ((unsigned int)(length<<2)) + *primary;
			c += *secondary;

			while (length > 3)
			{
				a += key[0];
				b += key[1];
				c += key[2];
				Mix(a,b,c);
				length -= 3;
				key += 3;
			}

			switch(length)
			{ 
			case 3 : c+=key[2];
			case 2 : b+=key[1];
			case 1 : a+=key[0];
				Final(a,b,c);
			case 0:     
				break;
			}

			*primary=c; *secondary=b;
		}
	};

	class IntegerHash
	{
	public:

		static unsigned int Hash32Shift(unsigned int key)
		{
		  key = ~key + (key << 15); 
		  key = key ^ (key >> 12);
		  key = key + (key << 2);
		  key = key ^ (key >> 4);
		  key = key * 2057; 
		  key = key ^ (key >> 16);
		  return key;
		}

		static unsigned int HashJenkins32(unsigned int key)
		{
			key = (key+0x7ed55d16) + (key<<12);
			key = (key^0xc761c23c) ^ (key>>19);
			key = (key+0x165667b1) + (key<<5);
			key = (key+0xd3a2646c) ^ (key<<9);
			key = (key+0xfd7046c5) + (key<<3);
			key = (key^0xb55a4f09) ^ (key>>16);
			return key;
		}

		static unsigned int Hash32ShiftMult(unsigned int key)
		{
			int c2=0x27d4eb2d; 
			key = (key ^ 61) ^ (key >> 16);
			key = key + (key << 3);
			key = key ^ (key >> 4);
			key = key * c2;
			key = key ^ (key >> 15);
			return key;
		}	

		static unsigned long long Hash64Shift(unsigned long long key)
		{
			key = (~key) + (key << 21); 
			key = key ^ (key >> 24);
			key = (key + (key << 3)) + (key << 8); 
			key = key ^ (key >> 14);
			key = (key + (key << 2)) + (key << 4); 
			key = key ^ (key >> 28);
			key = key + (key << 31);
			return key;
		}		

		static unsigned int Hash6432Shift(unsigned long long key)
		{
			key = (~key) + (key << 18); 
			key = key ^ (key >> 31);
			key = key * 21; 
			key = key ^ (key >> 11);
			key = key + (key << 6);
			key = key ^ (key >> 22);
			return (int) key;
		}

	};

	class SuperFastHash
	{
	public:

		#define Get16Bits(d) (*((const unsigned short *) (d)))

		static unsigned int Hash (const char * data, int length) 
		{
			unsigned int hash = length, tmp;
			int rem;

			if (length <= 0 || data == NULL) return 0;

			rem = length & 3;
			length >>= 2;

			for (;length > 0; length--) {
				hash  += Get16Bits (data);
				tmp    = (Get16Bits (data+2) << 11) ^ hash;
				hash   = (hash << 16) ^ tmp;
				data  += 2*sizeof (unsigned short);
				hash  += hash >> 11;
			}

			switch (rem) {
				case 3: hash += Get16Bits (data);
						hash ^= hash << 16;
						hash ^= data[sizeof (unsigned short)] << 18;
						hash += hash >> 11;
						break;
				case 2: hash += Get16Bits (data);
						hash ^= hash << 11;
						hash += hash >> 17;
						break;
				case 1: hash += *data;
						hash ^= hash << 10;
						hash += hash >> 1;
			}

			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;

			return hash;
		}

	};

	class FnvHash
	{
	public:

		static unsigned int Hash(unsigned char * key, unsigned int length)
		{

			static unsigned int prime = 16777619;
			static unsigned int offset = 2166136261U;

			unsigned int hash = offset;
			for (unsigned int i=0; i < length; ++i)
			{
				hash ^= key[i];
				hash *= prime;
			}

			return hash;
		}
	};

	class Djb2Hash
	{
	public:

		static const int Ascii[];

		static unsigned int Hash(unsigned char *key, unsigned int length, unsigned int seed=0)
		{
			unsigned int hash = seed?seed:5381;
			for (unsigned int i=0; i < length; ++i) 

				hash = ((hash << 5) + hash) + Ascii[key[i]]; 
			return hash;
		}

		static unsigned int Rotate(unsigned int hash, unsigned int key)
		{

			return ((hash << 5) + hash) + key;
		}

	};

	typedef Djb2Hash BernsteinHash;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Binary
{
public:

	static inline int PowerOfTwo(unsigned int key)
	{

		#if defined(HERO_PLATFORM_WINDOWS) && !defined(HERO_PLATFORM_MINGW)

		unsigned long index;
		_BitScanReverse(&index,key);
		return index; 
		#else

		key |= key >> 1;
		key |= key >> 2;
		key |= key >> 4;
		key |= key >> 8;
		key |= key >> 16;

		return (key+1)>>1;	
		#endif

	}

	static unsigned int BitIndex(unsigned int bit)
	{
		#ifndef HERO_PLATFORM_WASM

		#ifdef HERO_PLATFORM_LINUX
		return (unsigned int)__builtin_clz(bit) + 1;
		#endif
		#ifdef HERO_PLATFORM_WINDOWS

		return (unsigned int)__lzcnt((int)bit) + 1;
		#endif

		#else

		int index = 0;
		while ((bit & 0x80000000) == 0 && ++index)
			bit <<= 1;
		return index;

		#endif
	}

	static unsigned long long BitIndex(unsigned long long bit)
	{
		#if !defined(HERO_PLATFORM_32BIT) && !defined(HERO_PLATFORM_WASM)

		#ifdef HERO_PLATFORM_LINUX		
		return (unsigned long long)__builtin_clzll(bit) + 1;
		#endif

		#ifdef HERO_PLATFORM_WINDOWS

		return (unsigned long long)__lzcnt64((long long)bit) + 1;
		#endif				

		#else

		int index = 0;
		while ((bit & 0x80000000LL) == 0 && ++index)
			bit <<= 1;
		return index;

		#endif
	}

	static bool BitIndexReverse(unsigned int& index, unsigned int bit)
	{

		#ifdef HERO_BIT_EXPECT
		if (__builtin_expect((bit == 0),0))
			return false;
		#else
		if (bit == 0)
			return false;
		#endif

		#ifndef HERO_PLATFORM_WASM

		#ifdef HERO_PLATFORM_LINUX
		index = (unsigned int)__builtin_clz(bit) + 1;
		#endif
		#ifdef HERO_PLATFORM_WINDOWS
		unsigned long pos=0;
		index = (unsigned int)_BitScanReverse(&pos,bit);
		#endif

		#else

		index = BitIndex(bit);

		#endif

		return true;
	}

	static bool BitIndexReverse(unsigned long long& index, unsigned long long bit)
	{

		#ifdef HERO_BIT_EXPECT
		if (__builtin_expect((bit == 0),0))
			return false;
		#else
		if (bit == 0)
			return false;
		#endif

		#if !defined(HERO_PLATFORM_32BIT) && !defined(HERO_PLATFORM_WASM)

		#ifdef HERO_PLATFORM_LINUX
		index = (unsigned long long)__builtin_clzll(bit) + 1;
		#endif
		#ifdef HERO_PLATFORM_WINDOWS
		unsigned long pos=0;
		index = (unsigned long long)_BitScanReverse64(&pos,bit);
		#endif

		#else

		index = BitIndex(bit);

		#endif
		return true;
	}

	static bool BitIndexForward(unsigned int& index, unsigned int bit)
	{

		#ifdef HERO_BIT_EXPECT
		if (__builtin_expect((bit == 0),0))
			return false;
		#else
		if (bit == 0)
			return false;
		#endif

		#ifndef HERO_PLATFORM_WASM

		#ifdef HERO_PLATFORM_LINUX
		index = (unsigned int)__builtin_ctz(bit) + 1;
		#endif
		#ifdef HERO_PLATFORM_WINDOWS
		unsigned long pos=0;
		index = (unsigned int)_BitScanForward(&pos,bit);
		#endif

		#else

		index = 0;
		while ((bit & 0x00000001) == 0 && ++index)
			bit >>= 1;

		#endif
		return true;
	}

	static bool BitIndexForward(unsigned long long& index, unsigned long long bit)
	{
		#ifdef HERO_BIT_EXPECT
		if (__builtin_expect((bit == 0),0))
			return false;
		#else
		if (bit == 0)
			return false;
		#endif

		#if !defined(HERO_PLATFORM_32BIT) && !defined(HERO_PLATFORM_WASM)

		#ifdef HERO_PLATFORM_LINUX
		index = (unsigned long long)__builtin_ctzll(bit) + 1;
		#endif
		#ifdef HERO_PLATFORM_WINDOWS
		unsigned long pos=0;
		index = (unsigned long long)_BitScanForward64(&pos,bit);
		#endif

		#else

		index = 0;
		while ((bit & 0x00000001LL) == 0 && ++index)
			bit >>= 1;

		#endif
		return true;
	}

	static unsigned int NextPowerOfTwo(unsigned int power)
	{
		unsigned int index = 0;
		BitIndexReverse(index, power);
		return 1 << (index + 1);
	}

	static unsigned long long NextPowerOfTwo(unsigned long long power)
	{
		unsigned long long index = 0;
		BitIndexReverse(index, power);
		return 1ULL << (index + 1ULL);
	}

public:

	unsigned char * Data;		
	int Size;

	int Count;					
	int Index;					

	enum BinaryModes
	{
		ENDIAN_NONE		=(0),		
		ENDIAN_LITTLE 	=(1)<<1,	
		ENDIAN_BIG 		=(1)<<2,	

		ORDER_ASCENDING = ENDIAN_LITTLE,		
		ORDER_DESCENDING = ENDIAN_BIG,
	};

	int Mode;	

	void Endianness()
	{
		Mode = Endian();

		if (!Mode)
		{
			Raise(new SystemError("Binary::Endianness() - Cannot determine system byte order.\n"));

		}
	}

	static int Endian()
	{
		int mode = 0;

		int thirtytwo = 0x01020304L;
		char eight;

		memcpy(&eight,&thirtytwo,1);

		switch (eight)
		{
		case 0x01: mode = ENDIAN_BIG; break;	
		case 0x04: mode = ENDIAN_LITTLE; break;	
		}

		return mode;
	}	

	static bool IsLittleEndian()
	{
		int thirtytwo = 0x01020304L;
		char eight = *((char*)&thirtytwo);		
		return eight == 0x04;
	}

	static bool IsBigEndian()
	{
		int thirtytwo = 0x01020304L;
		char eight = *((char*)&thirtytwo);
		return eight == 0x01;
	}

	Binary()
	{
		Data=0;
		Size=0;
		Count=0;
		Index=0;

		Endianness();
	}

	~Binary()
	{
		if (Data)
			delete [] Data;
	}

	int Remaining() 
	{
		return 8*Size-Count;
	}

	Binary & Allocate(int bits=0);

	Binary & Resize(int bits);
	Binary & Reserve(int bits);

	Binary & Left(int bits);
	Binary & Right(int bits);

	void Copy(Binary &bits)
	{
		if (Data == 0)
		{

			Allocate(bits.Count);
			Count = bits.Count;
		}
		else
		{
			Resize(bits.Count - Count);
		}

		memcpy(Data,bits.Data,Bytes());
	}

	void Append(Binary &bits)
	{
		if (bits.Data == 0) 
			return;

		Assert(Data != bits.Data);

		int offset = Count;
		Resize(bits.Count);
		memcpy(Data+offset,bits.Data,bits.Bytes());	
	}

	void Prepend(Binary &bits)
	{		
		Assert(bits.Data != Data);

		if (bits.Data == 0) 
			return;

		Resize(bits.Count);
		Left(bits.Count);

		memcpy(Data,bits.Data,Bytes());
	}

	int Bytes()			
	{
		return (Count+7)/8;

	}

	int Mask()
	{	
		return 2^Count-1;
	}

	bool Is(Binary &bits)
	{ 

		int mask = Mask() & bits.Mask();
		int data = *(int*)Data & mask;
		int comp = *(int*)bits.Data & mask;

		return (data&comp)!=0||data==comp;
	}

	bool Equals(Binary &bits)
	{

	}

	void Clear()
	{

		memset(Data,0,Bytes());
	}

	Bit operator[](int bit) 
	{

		return Bit(&Data[Bits::Index(bit)],bit & 7);
	}

	static Superstring Pack(const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		Superstring superstring;
		superstring.Allocate(String::Length(format)*1.25);	
		PackVa(superstring,format,va);
		va_end(va);
		return superstring;
	}

	static Superstring PackVa(const char * format, va_list va)
	{
		Superstring superstring;
		superstring.Allocate(String::Length(format)*1.25);	
		PackVa(superstring,format,va);
		return superstring;
	}

	static void Pack(String & string, const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		PackVa(string,format,va);
		va_end(va);
	}

	static void PackVa(String & string, const char * format, va_list va);

	static void Pack(String & string, const char * format, String & args);

	static void Unpack(Substring & sequence, const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		UnpackVa(sequence,format,va);
		va_end(va);
	}

	static void UnpackVa(Substring & sequence, const char * format, va_list va);

	static void Unpack(Substring & sequence, const char * format, String & args);

	void Terminate() 
	{

	}

	template<typename _Kind_>
	static String BitsToString(_Kind_ kind, int endian = Binary::ENDIAN_LITTLE)
	{

		int size = sizeof(_Kind_)*8;
		String str;
		str.Reserve(size);

		if (endian == 0) endian = Binary::Endian();

		if (endian == Binary::ENDIAN_BIG)
		{

			while (size-- > 0)
			{
				str += (kind&(1))?1:0;
				kind >>= 1;
			}
		}
		else
		{

			while (size-- > 0)
			{
				str += ((kind>>size)&1)?1:0;
			}
		}

		return str;
	}

	template<typename _Kind_>
	static _Kind_ StringToBits(const String & str, int endian = Binary::ENDIAN_LITTLE)
	{

		_Kind_ kind();

		int size=str.Size;

		if (endian == 0) endian = Binary::Endian();

		if (endian == Binary::ENDIAN_BIG)
		{

			while (size-- > 0)
			{
				int offset = ((str.Size-1)-size);
				int index = offset;
				kind |= (str.Data[index]=='1')?(1<<offset):0;
			}
		}
		else
		{

			while (size-- > 0)
			{
				int offset = ((str.Size-1)-size);
				int index = size;
				kind |= (str.Data[index]=='1')?(1<<offset):0;
			}
		}

		return kind;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BinaryStream : public StreamFilter
 {
public:

	BinaryStream(class Stream & stream):StreamFilter(&stream) {}
	BinaryStream(class Stream * stream):StreamFilter(stream) {}
	BinaryStream() {}
	~BinaryStream() {}

	using StreamFilter::Write;
	int Write(const bool & data)				{return Write((char&)data);}
	int Write(const char & data)				{return Write((char *) &data,sizeof(char));}
	int Write(const signed char & data)			{return Write((char *) &data,sizeof(signed char));}
	int Write(const unsigned char & data)		{return Write((char *) &data,sizeof(unsigned char));}
	int Write(const short & data)				{return Write((char *) &data,sizeof(short));}
	int Write(const unsigned short &data)		{return Write((char *) &data,sizeof(unsigned short));}
	int Write(const int & data)					{return Write((char *) &data,sizeof(int));}
	int Write(const unsigned int & data)		{return Write((char *) &data,sizeof(unsigned int));}
	int Write(const long & data)				{return Write((char *) &data,sizeof(long));}
	int Write(const unsigned long & data)		{return Write((char *) &data,sizeof(unsigned long));}
	int Write(const long long & data)			{return Write((char *) &data,sizeof(long long));}
	int Write(const unsigned long long & data)	{return Write((char *) &data,sizeof(unsigned long long));}
	int Write(const float & data)				{return Write((char *) &data,sizeof(float));}
	int Write(const double & data)				{return Write((char *) &data,sizeof(double));}
	int Write(const void * & data)				{return Write((char*) &data,sizeof(void*));}

	int Write(char * data, int size, int mode);

	int Write8(const char & bits)					{return Write((char*)&bits,1);}
	int Write8(const unsigned char & bits)			{return Write((char*)&bits,1);}
	int Write16(const short & bits)					{return Write((char*)&bits,2);}
	int Write16(const unsigned short & bits)		{return Write((char*)&bits,2);}
	int Write32(const int & bits)					{return Write((char*)&bits,4);}
	int Write32(const unsigned int & bits)			{return Write((char*)&bits,4);}

	#ifdef HERO_PLATFORM_WINDOWS
	int Write32(const long & bits)					{return Write((char*)&bits,4);}
	int Write32(const unsigned long & bits)			{return Write((char*)&bits,4);}
	#else
	int Write64(const long & bits)					{return Write((char*)&bits,8);}
	int Write64(const unsigned long & bits)			{return Write((char*)&bits,8);}
	#endif	

	int Write64(const long long & bits)				{return Write((char*)&bits,8);}
	int Write64(const unsigned long long & bits)	{return Write((char*)&bits,8);}

	int WriteNull8(int amount)					{unsigned char data=0;int size=0;while(amount-- > 0) size += Write8(data);return size;}
	int WriteNull16(int amount)					{unsigned short data=0;int size=0;while(amount-- > 0) size += Write16(data);return size;}
	int WriteNull32(int amount)					{unsigned int data=0;int size=0;while(amount-- > 0) size += Write32(data);return size;}
	int WriteNull64(int amount)					{unsigned long long data=0;int size=0;while(amount-- > 0) size += Write64(data);return size;}

	using StreamFilter::Read;
	int Read(bool & data)						{return Read((char&)data);}
	int Read(char & data)						{return Read((char *) &data,sizeof(char));}
	int Read(signed char & data)				{return Read((char *) &data,sizeof(signed char));}
	int Read(unsigned char & data)				{return Read((char *) &data,sizeof(unsigned char));}
	int Read(short & data)						{return Read((char *) &data,sizeof(short));}
	int Read(unsigned short &data)				{return Read((char *) &data,sizeof(unsigned short));}
	int Read(int & data)						{return Read((char *) &data,sizeof(int));}
	int Read(unsigned int & data)				{return Read((char *) &data,sizeof(unsigned int));}
	int Read(long & data)						{return Read((char *) &data,sizeof(long));}
	int Read(unsigned long & data)				{return Read((char *) &data,sizeof(unsigned long));}
	int Read(long long & data)					{return Read((char *) &data,sizeof(long long));}
	int Read(unsigned long long & data)			{return Read((char *) &data,sizeof(unsigned long long));}
	int Read(float & data)						{return Read((char *) &data,sizeof(float));}
	int Read(double & data)						{return Read((char *) &data,sizeof(double));}
	int Read(void * & data)						{return Read((char*) &data,sizeof(void*));}

	int Read8(char & bits)						{return Read((char*)&bits,1);}
	int Read8(unsigned char & bits) 			{return Read((char*)&bits,1);}
	int Read16(short & bits)					{return Read((char*)&bits,2);}
	int Read16(unsigned short & bits)			{return Read((char*)&bits,2);}
	int Read32(int & bits)						{return Read((char*)&bits,4);}
	int Read32(unsigned int & bits)				{return Read((char*)&bits,4);}

	#ifdef HERO_PLATFORM_WINDOWS
	int Read32(const long & bits)				{return Read((char*)&bits,4);}
	int Read32(const unsigned long & bits)		{return Read((char*)&bits,4);}
	#else
	int Read64(const long & bits)				{return Read((char*)&bits,8);}
	int Read64(const unsigned long & bits)		{return Read((char*)&bits,8);}
	#endif	

	int Read64(long long & bits)				{return Read((char*)&bits,8);}
	int Read64(unsigned long long & bits)		{return Read((char*)&bits,8);}

	unsigned char Read8() {unsigned char bits;Read8(bits);return bits;}
	unsigned short Read16() {unsigned short bits;Read16(bits);return bits;}
	unsigned int Read32() {unsigned int bits;Read32(bits);return bits;}
	unsigned long long Read64() {unsigned long long bits;Read64(bits);return bits;}

	int ReadNull8(int amount)					{unsigned char data=0;int size=0;while(amount-- > 0) size += Read8(data);return size;}
	int ReadNull16(int amount)					{unsigned short data=0;int size=0;while(amount-- > 0) size += Read16(data);return size;}
	int ReadNull32(int amount)					{unsigned int data=0;int size=0;while(amount-- > 0) size += Read32(data);return size;}
	int ReadNull64(int amount)					{unsigned long long data=0;int size=0;while(amount-- > 0) size += Read64(data);return size;}

};

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

