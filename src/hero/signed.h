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

#include "hero/unsigned.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Signed 
{
public:

	enum Signs { SIGN_NEGATIVE = -1, SIGN_ZERO = 0, SIGN_POSITIVE = 1 };

	int Sign;
	Unsigned Magnitude;

	Signed() : Sign(SIGN_ZERO), Magnitude() {}
	Signed(const Signed &x) : Sign(x.Sign), Magnitude(x.Magnitude) {};	
	Signed(const Substring& str);

	Signed(const Block *data, int size, int sign);

	Signed(const Block *data, int size) : Magnitude(data, size) 
	{

		Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE;
	}

	Signed(const Unsigned &x, int sign);	
	Signed(const Unsigned &x) : Magnitude(x) 
	{

		Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE;
	}

	Signed(  signed long long x);
	Signed(unsigned long long x);
	Signed(  signed long x);
	Signed(unsigned long x);	
	Signed(  signed int x);
	Signed(unsigned int x);	
	Signed(  signed short x);
	Signed(unsigned short x);

	~Signed();

	String Str();

	void operator=(const Signed &x);

	Signed & Clone(Signed & x)
	{
		Sign = x.Sign;
		Magnitude.Clone(x.Magnitude);
		return *this;
	}

	Signed & Copy(Signed & x)
	{
		Sign = x.Sign;
		Magnitude.Copy(x.Magnitude);
		return *this;
	}

	Signed & Move(Signed & x)
	{
		Sign = x.Sign;
		Magnitude.Move(x.Magnitude);
		x.Sign = 0;
		return *this;
	}	

	signed short		SignedShort();
	signed int			SignedInt();
	signed long			SignedLong();
	signed long long	SignedLongLong();

	unsigned short		UnsignedShort();
	unsigned int		UnsignedInt();
	unsigned long		UnsignedLong();
	unsigned long long	UnsignedLongLong();

	template <class X> X ConvertToUnsignedPrimitive();
	template <class X, class UX> X ConvertToSignedPrimitive();

	bool IsZero() const { return Sign == SIGN_ZERO; } 

	bool IsPositive() const {return Sign == SIGN_POSITIVE;}
	bool IsNegative() const {return Sign == SIGN_NEGATIVE;}

	int Compare(const Signed &x);

	bool operator == (const Signed &x) const
	{
		return Sign == x.Sign && Magnitude == x.Magnitude;
	}

	bool operator != (const Signed &x) const 
	{ 
		return !operator ==(x); 
	}

	bool operator <  (const Signed &x) {return Compare(x) < 0;}
	bool operator <= (const Signed &x) {return Compare(x) <= 0;}
	bool operator >= (const Signed &x) {return Compare(x) >= 0;}
	bool operator >  (const Signed &x) {return Compare(x) > 0;}

	Signed & Add(const Signed &a, const Signed &b);
	Signed & Add(const Signed &x) {Add(*this,x);return *this;}

	Signed & Subtract(const Signed &a, const Signed &b);
	Signed & Subtract(const Signed &x) {Subtract(*this,x);return *this;}

	Signed & Multiply(const Signed &a, const Signed &b);
	Signed & Multiply(const Signed &x) {Multiply(*this,x);return *this;}

	Signed & Divide(const Signed &a, const Signed &b);
	Signed & Divide(const Signed &x) {Divide(*this,x);return *this;}

	Signed & Pow(const Signed &x);
	Signed & Mod(const Signed &x);

	void DivideWithRemainder(const Signed &b, Signed &q);

	Signed & Negate(const Signed &a);

	Signed operator +(const Signed &x);
	Signed operator -(const Signed &x);
	Signed operator *(const Signed &x);
	Signed operator /(const Signed &x);
	Signed operator %(const Signed &x);
	Signed operator -();

	Signed & operator +=(const Signed &x);
	Signed & operator -=(const Signed &x);
	Signed & operator *=(const Signed &x);
	Signed & operator /=(const Signed &x);
	Signed & operator %=(const Signed &x);

	Signed& Flip();

	Signed & operator ++();
	Signed & operator --();

	Signed operator ++(int);
	Signed operator --(int);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class X>
X ConvertUnsignedToPrimitiveAccess(const Unsigned &a) 
{
	return ((Unsigned&)a).ConvertToUnsignedPrimitive<X>();
}

template <class X>
X Signed::ConvertToUnsignedPrimitive() 
{

	if (Sign == SIGN_NEGATIVE)
	{
		Raise(new UnderflowError("Signed ConvertToUnsignedPrimitive: Cannot convert a negative integer to an unsigned type"));
		return 0;
	}

	return ConvertUnsignedToPrimitiveAccess<X>(Magnitude);
}

template <class X, class UX>
X Signed::ConvertToSignedPrimitive() 
{
	if (Sign == SIGN_ZERO)
	{
		return 0;
	}
	else
	if (Magnitude.Length() == 1) 
	{

		Block b = Magnitude.Get(0);
		if (Sign == SIGN_POSITIVE) 
		{
			X x = X(b);
			if (x >= 0 && Block(x) == b)
				return x;
		} 
		else 
		{
			X x = -X(b);

			if (x < 0 && Block(UX(-x)) == b)
				return x;
		}

	}

	Raise(new OverflowError("Signed::ConvertToSignedPrimitive<>: Value is too big to fit in the requested type"));
	return X();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
