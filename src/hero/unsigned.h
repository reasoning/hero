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

#include "hero/structure.h"
#include "hero/error.h"

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

class Integer;

typedef unsigned long long Block;

class Unsigned : public Vector<Block> 
{
public:

	static const int N = 8 * sizeof(Block);

	Unsigned() : Vector<Block>() {}
	Unsigned(const Unsigned & x) : Vector<Block>(x) {}
	Unsigned(const Integer & integer);
	Unsigned(const Substring & str);

	Unsigned(const Block *data, int size) : Vector<Block>((Block*)data,size) 
	{

		Trim();
	}

	Unsigned(int, int allocated) : Vector<Block>(allocated) 
	{

	}		

	Unsigned(unsigned long long x);
	Unsigned(  signed long long x);

	Unsigned(unsigned long  x);
	Unsigned(  signed long  x);
	Unsigned(unsigned int   x);
	Unsigned(  signed int   x);
	Unsigned(unsigned short x);
	Unsigned(  signed short x);

	Unsigned(float x);
	Unsigned(double x);

	~Unsigned() {}

	String Str();

	Unsigned & operator=(const Unsigned &x) 
	{
		Vector<Block>::operator =(x);
		return *this;
	}

	Unsigned & Clone(Unsigned & x)
	{
		Vector<Block>::Clone(x);
		return *this;
	}

	Unsigned & Copy(Unsigned & x)
	{
		Vector<Block>::Copy(x);
		return *this;
	}

	Unsigned & Move(Unsigned & x)
	{
		Vector<Block>::Move(x);
		return *this;
	}

	void Trim() { 
		while (Size > 0 && Data[Size - 1] == 0)
			Size--;
	}

	template <class X> void InitFromUnsignedPrimitive(X x);

	template <class X> void InitFromSignedPrimitive(X x);

	unsigned long long UnsignedLongLong();
	signed long long SignedLongLong();

	unsigned long UnsignedLong();
	signed long SignedLong();

	unsigned int UnsignedInt();
	signed int SignedInt();

	unsigned short UnsignedShort();
	signed short SignedShort();

	template <class X> X ConvertToSignedPrimitive();
	template <class X> X ConvertToUnsignedPrimitive();

	Block operator[] (int index) {Get(index);}

	Block Get(int index) 
	{
		return index >= Size ? 0: Data[index];
	}

	void Set(int index, Block block);

	bool IsZero() { return Vector<Block>::IsEmpty(); }

	int BitLength();

	bool Bit(int index) 
	{
		return (Get(index / N) & (Block(1) << (index % N))) != 0;
	}

	void Bit(int index, bool bit);

	bool Equals(const Unsigned &x) {return Compare(x) == 0;}

	int Compare(const Unsigned &x);

	bool operator == (const Unsigned &x) const 
	{
		if (Size != x.Size) return false;

		for (int i=0;i<Size;++i) 
			if (Data[i] != x.Data[i]) return false;

		return true;

	}

	bool operator !=(const Unsigned &x) const 
	{
		return !operator == (x);

	}

	bool operator <  (const Unsigned &x) {return Compare(x) == -1;}
	bool operator <= (const Unsigned &x) {return Compare(x) != 1;}
	bool operator >= (const Unsigned &x) {return Compare(x) != -1;}
	bool operator >  (const Unsigned &x) {return Compare(x) == 1;}

	Unsigned & Add(const Unsigned &a, const Unsigned &b);
	Unsigned & Add(const Unsigned &x) {Add(*this,x);return *this;}

	Unsigned & Subtract(const Unsigned &a, const Unsigned &b);
	Unsigned & Subtract(const Unsigned &x) {Subtract(*this,x);return *this;}

	Unsigned & Multiply(const Unsigned &a, const Unsigned &b);
	Unsigned & Multiply(const Unsigned &x) {Multiply(*this,x);return *this;}

	Unsigned & Divide(const Unsigned &a, const Unsigned &b);
	Unsigned & Divide(const Unsigned &x) {Divide(*this,x);return *this;}

	Unsigned & BitAnd(const Unsigned &a, const Unsigned &b);
	Unsigned & BitAnd(const Unsigned &x) {BitAnd(*this,x);return *this;}

	Unsigned & BitOr(const Unsigned &a, const Unsigned &b);
	Unsigned & BitOr(const Unsigned &x) {BitOr(*this,x);return *this;}

	Unsigned & BitXor(const Unsigned &a, const Unsigned &b);
	Unsigned & BitXor(const Unsigned &x) {BitXor(*this,x);return *this;}

	Unsigned & BitShiftLeft(const Unsigned &a, int b);
	Unsigned & BitShiftLeft(int b) {BitShiftLeft(*this,b);return *this;}

	Unsigned & BitShiftRight(const Unsigned &a, int b);
	Unsigned & BitShiftRight(int b) {BitShiftRight(*this,b);return *this;}

	void DivideWithRemainder(const Unsigned &b, Unsigned &q);

	Unsigned & Pow(const Unsigned &x);
	Unsigned & Mod(const Unsigned &x);

	Unsigned operator +(const Unsigned &x);
	Unsigned operator -(const Unsigned &x);
	Unsigned operator *(const Unsigned &x);
	Unsigned operator /(const Unsigned &x);
	Unsigned operator %(const Unsigned &x);

	Unsigned operator &(const Unsigned &x);
	Unsigned operator |(const Unsigned &x);
	Unsigned operator ^(const Unsigned &x);
	Unsigned operator <<(int b);
	Unsigned operator >>(int b);

	Unsigned & operator +=(const Unsigned &x);
	Unsigned & operator -=(const Unsigned &x);
	Unsigned & operator *=(const Unsigned &x);
	Unsigned & operator /=(const Unsigned &x);
	Unsigned & operator %=(const Unsigned &x);
	Unsigned & operator &=(const Unsigned &x);
	Unsigned & operator |=(const Unsigned &x);
	Unsigned & operator ^=(const Unsigned &x);
	Unsigned & operator <<=(int b);
	Unsigned & operator >>=(int b);

	Unsigned & operator ++(   );	
	Unsigned operator ++(int);	
	Unsigned & operator --(   );	
	Unsigned operator --(int);	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class X>
void Unsigned::InitFromUnsignedPrimitive(X x) 
{
	if (x == 0)
		; 
	else 
	{

		Insert(Block(x),0);
	}
}

template <class X>
void Unsigned::InitFromSignedPrimitive(X x) 
{
	if (x < 0)
	{
		Raise("Unsigned InitFromSignedPrimitive: Cannot construct a Unsigned from a negative number");
		return;
	}

	InitFromUnsignedPrimitive(x);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class X>
X Unsigned::ConvertToUnsignedPrimitive() 
{
	if (Size == 0)
	{

		return 0;
	}
	else 
	if (Size == 1) 
	{

		X x = X(Data[0]);

		if (Block(x) == Data[0])

			return x;

	}

	Raise("Unsigned ConvertToPrimitive: Value is too big to fit in the requested type");
	return 0;
}

template <class X>
X Unsigned::ConvertToSignedPrimitive()
{
	X x = ConvertToUnsignedPrimitive<X>();
	if (x < 0)
	{
		Raise("Unsigned ConvertToSignedPrimitive: Value is too big to fit in the requested type");
		return 0;
	}

	return x;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
