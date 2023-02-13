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

#include "hero/signed.h"
#include "hero/unsigned.h"
#include "hero/integer.h"
#include "hero/error.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Signed::operator =(const Signed &x) 
{

	if (this == &x)
		return;

	Sign = x.Sign;

	Magnitude = x.Magnitude;
}

Signed::Signed(const Block *data, int size, int sign) : Magnitude(data, size) 
{

	switch (sign)
	{
	case SIGN_ZERO:
		if (!Magnitude.IsZero())
			Raise("Signed::Signed: Cannot use a sign of zero with a nonzero magnitude");
		Sign = SIGN_ZERO;
		break;
	case SIGN_POSITIVE:
	case SIGN_NEGATIVE:

		Sign = Magnitude.IsZero() ? SIGN_ZERO : sign;
		break;
	default:

		Raise("Signed::Signed: Invalid sign");
	}
}

Signed::Signed(const Unsigned &x, int sign) : Magnitude(x)
{
	switch (sign) 
	{
	case SIGN_ZERO:
		if (!Magnitude.IsZero())
			Raise("Signed::Signed: Cannot use a sign of zero with a nonzero magnitude");
		Sign = SIGN_ZERO;
		break;
	case SIGN_POSITIVE:
	case SIGN_NEGATIVE:

		Sign = Magnitude.IsZero() ? SIGN_ZERO : sign;
		break;
	default:

		Raise("Signed::Signed: Invalid sign");
	}
}

Signed::Signed(const Substring& str)
{
	Signed res = Integer::StringToSigned(str);
	Move(res);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Signed::Signed(unsigned long long x)	: Magnitude(x) { Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE; }
Signed::Signed(unsigned long  x)		: Magnitude(x) { Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE; }
Signed::Signed(unsigned int   x)		: Magnitude(x) { Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE; }
Signed::Signed(unsigned short x)		: Magnitude(x) { Sign = Magnitude.IsZero() ? SIGN_ZERO : SIGN_POSITIVE; }

namespace 
{
	template <class X, class UX>
	Block MagOf(X x) 
	{

		return Block(x < 0 ? UX(-x) : x);
	}

	template <class X>
	int SignOf(X x) 
	{
		return (x == 0) ? Signed::SIGN_ZERO
			: (x > 0) ? Signed::SIGN_POSITIVE
			: Signed::SIGN_NEGATIVE;
	}
}

Signed::Signed(signed long long x)	: Sign(SignOf(x)), Magnitude(MagOf<signed long long, unsigned long long>(x)) {}
Signed::Signed(signed long x)		: Sign(SignOf(x)), Magnitude(MagOf<signed long, unsigned long>(x)) {}
Signed::Signed(signed int x)		: Sign(SignOf(x)), Magnitude(MagOf<signed int, unsigned int>(x)) {}
Signed::Signed(signed short x)		: Sign(SignOf(x)), Magnitude(MagOf<signed short, unsigned short>(x)) {}

Signed::~Signed()
{

}

String Signed::Str()
{
	return Integer::SignedToString(*this);
}

unsigned long long	Signed::UnsignedLongLong()	{ return ConvertToUnsignedPrimitive<unsigned long long>	(); }
signed long long	Signed::SignedLongLong()	{ return ConvertToSignedPrimitive<signed long long, unsigned long long>	(); }

unsigned long		Signed::UnsignedLong ()		{ return ConvertToUnsignedPrimitive<unsigned long>				(); }
unsigned int		Signed::UnsignedInt  ()		{ return ConvertToUnsignedPrimitive<unsigned int>				(); }
unsigned short		Signed::UnsignedShort()		{ return ConvertToUnsignedPrimitive<unsigned short>				(); }

signed long			Signed::SignedLong()		{ return ConvertToSignedPrimitive<signed long, unsigned long>	(); }
signed int			Signed::SignedInt()			{ return ConvertToSignedPrimitive<signed int, unsigned int>		(); }
signed short		Signed::SignedShort()		{ return ConvertToSignedPrimitive<signed short, unsigned short>	(); }

Signed Signed::operator +(const Signed &x) 
{
	Signed ans;
	ans.Add(*this, x);
	return ans;
}

Signed Signed::operator -(const Signed &x) 
{
	Signed ans;
	ans.Subtract(*this, x);
	return ans;
}

Signed Signed::operator *(const Signed &x) 
{
	Signed ans;
	ans.Multiply(*this, x);
	return ans;
}

Signed Signed::operator /(const Signed &x) 
{
	if (x.IsZero())
	{
		Raise("Signed::operator /: division by zero");
		return Signed();
	}

	Signed q, r;
	r = *this;
	r.DivideWithRemainder(x, q);
	return q;
}

Signed Signed::operator %(const Signed &x) 
{
	if (x.IsZero())
	{
		Raise("Signed::operator %: division by zero");
		return Signed();
	}

	Signed q, r;
	r = *this;
	r.DivideWithRemainder(x, q);
	return r;
}

Signed Signed::operator -() 
{
	Signed ans;
	ans.Negate(*this);
	return ans;
}

Signed & Signed::operator += (const Signed &x) 
{
	Add(*this, x);
	return *this;
}

Signed & Signed::operator -= (const Signed &x) 
{
	Subtract(*this, x);
	return *this;
}

Signed & Signed::operator *= (const Signed &x) 
{
	Multiply(*this, x);
	return *this;
}

Signed & Signed::operator /= (const Signed &x) 
{
	if (x.IsZero()) 
	{	
		Raise("Signed::operator /=: division by zero");
		return *this;
	}

	Signed q;
	DivideWithRemainder(x, q);

	*this = q;

	return *this;
}

Signed & Signed::operator %= (const Signed &x) 
{
	if (x.IsZero()) 
	{
		Raise("Signed::operator %=: division by zero");
		return *this;
	}

	Signed q;

	DivideWithRemainder(x, q);
	return *this;
}

Signed & Signed::Flip() 
{
	Sign = int(-Sign);
	return *this;
}

int Signed::Compare(const Signed &x) 
{

	if (Sign < x.Sign)
	{
		return -1;
	}
	else 
	if (Sign > x.Sign)
	{
		return 1;
	}
	else
	{
		switch (Sign) 
		{

		case Signs::SIGN_ZERO:
			return 0; 
		case Signs::SIGN_POSITIVE:

			return Magnitude.Compare(x.Magnitude);
		case Signs::SIGN_NEGATIVE:

			return (int)(-Magnitude.Compare(x.Magnitude));
		default:
			Raise("Signed::Compare: Signed internal error");
		}
	}

	return 0;
}

Signed& Signed::Add(const Signed &a, const Signed &b) 
{
	if (this == &a || this == &b)
	{
		Move(Signed().Add(a, b)); 
		return *this;
	}

	if (a.Sign == SIGN_ZERO)
	{
		operator =(b);
	}
	else 
	if (b.Sign == SIGN_ZERO)
	{
		operator =(a);

	}
	else 
	if (a.Sign == b.Sign) 
	{
		Sign = a.Sign;
		Magnitude.Add(a.Magnitude, b.Magnitude);
	} 
	else 
	{

		switch (((Signed&)a).Magnitude.Compare(((Signed&)b).Magnitude)) 
		{
		case 0:

			Magnitude = 0;
			Sign = SIGN_ZERO;
			break;

		case 1:
			Sign = a.Sign;
			Magnitude.Subtract(a.Magnitude, b.Magnitude);
			break;
		case -1:
			Sign = b.Sign;
			Magnitude.Subtract(b.Magnitude, a.Magnitude);
			break;
		}
	}

	return *this;
}

Signed& Signed::Subtract(const Signed &a, const Signed &b) 
{

	if (this == &a || this == &b)
	{
		Move(Signed().Subtract(a, b)); 
		return *this;
	}

	if (a.Sign == SIGN_ZERO) 
	{
		Magnitude = b.Magnitude;

		Sign = int(-b.Sign);
	} 
	else 
	if (b.Sign == SIGN_ZERO)
	{
		operator =(a);

	}
	else 
	if (a.Sign != b.Sign) 
	{
		Sign = a.Sign;
		Magnitude.Add(a.Magnitude, b.Magnitude);
	}
	else 
	{

		switch (((Signed&)a).Magnitude.Compare(((Signed&)b).Magnitude)) 
		{

		case 0:
			Magnitude = 0;
			Sign = Signs::SIGN_ZERO;
			break;

		case 1:
			Sign = a.Sign;
			Magnitude.Subtract(a.Magnitude, b.Magnitude);
			break;

		case -1:
			Sign = int(-b.Sign);
			Magnitude.Subtract(b.Magnitude, a.Magnitude);
			break;
		}
	}

	return *this;
}

Signed& Signed::Multiply(const Signed &a, const Signed &b) 
{
	if (this == &a || this == &b)
	{
		Move(Signed().Multiply(a, b));
		return *this;
	}

	if (a.Sign == SIGN_ZERO || b.Sign == SIGN_ZERO) 
	{
		Sign = SIGN_ZERO;
		Magnitude = 0;
		return *this;
	}

	Sign = (a.Sign == b.Sign) ? SIGN_POSITIVE : SIGN_NEGATIVE;

	Magnitude.Multiply(a.Magnitude, b.Magnitude);

	return *this;
}

Signed & Signed::Divide(const Signed &a, const Signed &b) 
{
	if (this == &a || this == &b)
	{
		Move(Signed().Divide(a, b));
		return *this;
	}

	Sign = (a.Sign == b.Sign) ? SIGN_POSITIVE : SIGN_NEGATIVE;

	Magnitude.Divide(a.Magnitude,b.Magnitude);

	return *this;
}

Signed & Signed::Pow(const Signed &x)
{
	Signed & _x = (Signed&)x;

	if (x == 1) return *this;

	if (x == 0)
	{

		operator = (1);
	}
	else
	{

		Signed pow(*this);
		while(_x > 1)
		{
			pow *= *this;
			--_x;
		}

		Move(pow);
	}

	return *this;
}

Signed & Signed::Mod(const Signed &x)
{
	Signed q;
	DivideWithRemainder(x,q);
	return *this;
}

void Signed::DivideWithRemainder(const Signed &b, Signed &q) {

	if (this == &q)
	{
		Raise("Signed::DivideWithRemainder: Cannot write quotient and remainder into the same variable");
		return;
	}

	if (this == &b || &q == &b) 
	{
		Signed tmpB(b);
		DivideWithRemainder(tmpB, q);
		return;
	}

	if (b.Sign == SIGN_ZERO) 
	{
		q.Magnitude = 0;
		q.Sign = SIGN_ZERO;
		return;
	}

	if (Sign == SIGN_ZERO) 
	{
		q.Magnitude = 0;
		q.Sign = SIGN_ZERO;
		return;
	}

	if (Sign == b.Sign) 
	{

		q.Sign = SIGN_POSITIVE;
	} 
	else 
	{

		q.Sign = SIGN_NEGATIVE;

		Magnitude--;

	}

	Magnitude.DivideWithRemainder(b.Magnitude, q.Magnitude);

	if (Sign != b.Sign) 
	{

		q.Magnitude++;

		Magnitude.Subtract(b.Magnitude, Magnitude);
		Magnitude--;
	}

	Sign = b.Sign;

	if (Magnitude.IsZero())
		Sign = SIGN_ZERO;
	if (q.Magnitude.IsZero())
		q.Sign = SIGN_ZERO;

}

Signed& Signed::Negate(const Signed &a) 
{
	if (this == &a)
	{
		Move(Signed().Negate(a));
		return *this;
	}

	Magnitude = a.Magnitude;

	Sign = int(-a.Sign);
	return *this;
}

Signed& Signed::operator ++() 
{
	if (Sign == SIGN_NEGATIVE) 
	{
		Magnitude--;
		if (Magnitude == 0)
			Sign = SIGN_ZERO;
	} 
	else 
	{
		Magnitude++;
		Sign = SIGN_POSITIVE; 
	}

	return *this;
}

Signed Signed::operator ++(int) 
{
	Signed prev = *this;
	operator ++();
	return prev;
}

Signed & Signed::operator --() 
{
	if (Sign == SIGN_POSITIVE) 
	{
		Magnitude--;
		if (Magnitude == 0)
			Sign = SIGN_ZERO;
	} 
	else 
	{
		Magnitude++;
		Sign = SIGN_NEGATIVE;
	}

	return *this;
}

Signed Signed::operator --(int) 
{
	Signed prev = *this;
	operator --();
	return prev;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

