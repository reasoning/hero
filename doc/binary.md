#Binary

Binary provides `Bits`, `Binary`, `Hashing` and `BinaryStream`


#Usage

`Bits` is a simple interface for manipulating integer bit masks.


`Binary` provides static wrappers for typical bit scan routines like `ctlz` and
 `ctz` along with power of two calculations for bit strings.

In addition it provides static methods for Perl style pack/unpack routines.

It also provides endian detection and a general purpose bitset/bitarray type along
with bits to string and string to bits conversions.

`BinaryStream` provides a binary stream interface cfor reading and writing binary 
data and serializations.

#Implementation

```cpp
class Bits
{
public:

	// Return the number of blocks required for N bits, where block size is 
	// specified in bytes
	static inline int Count(int bits, int size=sizeof(int))

	// Return the block that the given bit (0 indexed) occurs in (0 indexed) for 
	// a given block size
	static inline int Index(int bit, int size=sizeof(int))

	// Return a mask for the given bit within its byte		
	// For example bit 33 will flow through 4 bytes, and occur as the first bit
	// in the 5th byte - for which this function will return a mask.
	static inline int Mask(int bits, int index=0, int size=sizeof(int))

	static int Append(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int fromScheme=0, int size=sizeof(int));
	static int Prepend(int & to, int from, int bits, int toIndex=-1, int fromIndex=0, int fromScheme=0, int size=sizeof(int));

	// So to cant be a reference and take negative indexes
	//	Insert(a,b,5,-3,0)
	// We return the number of bits inserted.	
	static int Insert(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int toScheme=0, int fromScheme=0, int size=sizeof(int));

	// This is an alias for Insert
	static int Twiddle(int & to, int from, int bits, int toIndex=0, int fromIndex=0, int toScheme=0, int fromScheme=0, int size=sizeof(int))

	
	// Move a block of bits to the left or right, with care not to disturb the byte
	// ordering.  By subdividing the blocks into the largest chunks and using shift
	// operations, this shouldn't be to heinous.
	// Source and destination cannot overlap.
	static inline void Move(void * to, void * from, int bits)


	// Copy n bits from source to destination.
	// Optimised to copy by 32 bit blocks where possible like memcpy()
	// To and from cannot overlap.
	static inline void Copy(void * to, void * from, int bits)

};
```


`Hasing` includes common hash algoritms.

```cpp
class Hashing
{
public:

	// https://en.wikipedia.org/wiki/Jenkins_hash_function
	// http://www.burtleburtle.net/bob/hash/doobs.html
	
	// A collection of hashing algorithms which can be applied to binary or string data.

	class OneAtATime
	{
		static unsigned int Hash(unsigned char *key, int length);
	};

	class Lookup3Hash
	{
		static unsigned int Hashword(const unsigned int * key, int length, unsigned int primary);
		static void Hashword (const unsigned int * key, int length, unsigned int * primary, unsigned int * secondary);		
	};
	
	
	// Based on an original suggestion on Robert Jenkin's part in 1997, I have done some research for a 
	// version of the integer hash function. This is my latest version as of January 2007. The specific 
	// value of the bit shifts are obtained from running the accompanied search program.
	
	class IntegerHash
	{
		
		// Some interesting ideas here, particularly that an open addressing table can still take advantage
		// of a power of 2 table size (negating the need for slow prime lookup and modulus) if it hashes the
		// already hashed key a second time using one of these integer functions.
		// int index = hash(key) & (tablesize -1);
		
		// Also some notes on how multiple shift operations can be done in parallel on some hardware.
	
		static unsigned int Hash32Shift(unsigned int key);

		// Robert Jenkins' 32 bit integer hash function
		static unsigned int HashJenkins32(unsigned int key);
		static unsigned int Hash32ShiftMult(unsigned int key);
		
		// 64 bit Mix Functions
		static unsigned long long Hash64Shift(unsigned long long key)
		static unsigned int Hash6432Shift(unsigned long long key)

	};

	
	class SuperFastHash
	{
		static unsigned int Hash (const char * data, int length) 
	};


	class FnvHash
	{
		static unsigned int Hash(unsigned char * key, unsigned int length);
	};

	class Djb2Hash
	{	
		static unsigned int Hash(unsigned char *key, unsigned int length, unsigned int seed=0);		
		static unsigned int Rotate(unsigned int hash, unsigned int key);	
	};
	
	// This is its more common name on the internets
	typedef Djb2Hash BernsteinHash;

};
``


`BinaryStream` provides a wrapper for stream with overloads for reading and writing 
primitive types.

```cpp

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


	// Write in big or little endian mode (used by pack/unpack)
	int Write(char * data, int size, int mode);
	
	int Write8(const char & bits)					{return Write((char*)&bits,1);}
	int Write8(const unsigned char & bits)			{return Write((char*)&bits,1);}
	int Write16(const short & bits)					{return Write((char*)&bits,2);}
	int Write16(const unsigned short & bits)		{return Write((char*)&bits,2);}
	int Write32(const int & bits)					{return Write((char*)&bits,4);}
	int Write32(const unsigned int & bits)			{return Write((char*)&bits,4);}
	
	int Write64(const long long & bits)				{return Write((char*)&bits,8);}
	int Write64(const unsigned long long & bits)	{return Write((char*)&bits,8);}
	
	int WriteNull8(int amount)					{unsigned char data=0;int size=0;while(amount-- > 0) size += Write8(data);return size;}
	int WriteNull16(int amount)					{unsigned short data=0;int size=0;while(amount-- > 0) size += Write16(data);return size;}
	int WriteNull32(int amount)					{unsigned int data=0;int size=0;while(amount-- > 0) size += Write32(data);return size;}
	int WriteNull64(int amount)					{unsigned long long data=0;int size=0;while(amount-- > 0) size += Write64(data);return size;}
	

...
};

```

