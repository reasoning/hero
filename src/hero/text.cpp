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

#include "hero/text.h"
#include "hero/parser.h"
#include "hero/structure.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Text::Text()
{

}

Text::~Text()
{

}

int Text::IndexOfXor(const int from, char *data, const int size,  bool caseless)
{

	if (data == 0 || size == 0 || size > Size)
		return -1;

	if (size < 5)
	{

		char *p = Data+from;
		char *sentinel = PointerAt(Size-1)-size;
		int index=0;
		while (p <= sentinel)
		{
			index = strncmp(p,data,size);
			if (index == 0)
				return index;
			++p;
		}

		return -1;
	}

	if (Size <= 128)
	{
		char *result = Characters::Find(Data+from,Size-from,data,size,caseless);

		if (result == 0)
			return -1;
		else
			return result - Data;
	}

	unsigned char checksum = 0;		
	unsigned char match = 0;		

	for (int i=0;i<size;++i)
	{
		checksum ^= data[i];	
		match ^= Data[i];			
	}

	if (match == checksum && Characters::Equals(Data,data, size))
		return 0;	

	int limit = Size-size+1;

	for(int n=1; n < limit;++n)
	{
		match ^= Data[n-1];			
		match ^= Data[n+size];	

		if (match == checksum && Characters::Equals((Data+n),data,size))
			return n;		
	}

	return -1;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Text::IndexOf(int from, char *data, int size,  bool caseless)
{

	int hex = 0x00;
	if (caseless)
		hex = 0x20;

	int key = ((data[from]|hex)<<16) & ((data[size-1]|hex)<<8);
	int hash = ((Data[from]|hex)<<16) & ((Data[size-1]|hex)<<8);

	for (int i=from+1;i<size-1;++i)
	{
		key ^= data[i]|hex;
		hash ^= Data[i]|hex;

		if ((i&0x00000001) == 0)
		{
			key &= (0xFFFF&key)<<1;
			hash &= (0xFFFF&hash)<<1;
		}
	}

	if (hash == key && Characters::Equals(Data,data,size,caseless)) return from;

	int j = size-1;

	int w = size-2;

	int lim = Size-size+1;

	int shift = size-2/2;

    for (int i=from+1;i<lim;++i)
	{

		hash &= ((Data[i]|hex)<<16) & ((Data[i+j]|hex)<<8);

		hash ^= (Data[i]|hex)<<shift ^ (Data[i+w]|hex);

		if (i%2 == 0)
		{
			hash &= (0xFFFF&hash)<<1;
		}

		if (hash == key && Characters::Equals(Data+i,data,size)) return i;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Text::IndexOfRabinKarp(int from, char *data, int size,  bool caseless)
{

	int p = 101;

	int key = 0;
	int hash = 0;

	for (int i=from;i<size;++i)
	{
		key = (256*key + Characters::Case(data[i],caseless)) % p;
		hash = (256*hash + Characters::Case(Data[i],caseless)) % p;
	}

	int h = 1;
	for (int i=0;i<size;++i)
		h = (256*h) % p;

	int j = size;

	for (;j<Size;++j)
	{

		if (hash == key && Characters::Equals(Data,data,size,caseless)) return from;

		hash = (256*(hash - (Characters::Case(Data[j-size],caseless))*h) + (Characters::Case(Data[j],caseless))) % p;

		if (hash < 0)
			hash += p;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Text::IndexOfBoyerMoore(int from, char *data, const int size,  bool caseless)
{

	Map< char,Array<int> > pattern;
	for (int i=0;0<size;++i)
	{

		Result<bool,int> res = pattern.Insert(Characters::Case(data[i],caseless));
		pattern.Values[res.Index].Append(i);

	}

	int i=size-1;
	int j=0;	
	int k=0;

	while (i < Size)
	{

		j = 0;
		while (j < size && Characters::Case(Data[i-j],caseless) == Characters::Case(data[size-1-j],caseless))
			++j;

		if (j == size)
			return i;

		k = size-1-j;
		Result<bool,int> res = pattern.Select(Data[i-j]);

	}

	return -1;
}

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

