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

#include "hero/string.h"
#include "hero/error.h"

#include "hero/unsigned.h"
#include "hero/signed.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FixedSchema
{
	enum FixedSchemaMode
	{
		MODE_NONE		=(0),
		MODE_FORMAT		=(1),
		MODE_DIGITS		=(1)<<1,

		MODE_SCIENTIFIC	=(1)<<2,
		MODE_PRECISION	=(1)<<3,

		MODE_ROUND_EVEN			=(1)<<4,
		MODE_ROUND_CONSERVATIVE	=(1)<<5,
	};

	Flags<8> Mode;

	int Bits;		
	int Exponent;	
	int Mantissa;	

	FixedSchema(int mode=MODE_NONE):
		Bits(0),Exponent(0),Mantissa(0),Mode(mode)
	{
	}

	FixedSchema(int bits, int exponent, int mantissa):
		Bits(bits),Exponent(exponent),Mantissa(mantissa),Mode(MODE_FORMAT)
	{
	}

	static FixedSchema Digits(int exponent, int mantissa, int mode=MODE_NONE)
	{
		mode |= MODE_DIGITS;
		FixedSchema schema(mode);

		schema.Exponent = exponent;
		schema.Mantissa = mantissa;
		return schema;
	}

	static FixedSchema Scientific(int bits, int mode=MODE_NONE)
	{
		mode |= MODE_SCIENTIFIC;
		FixedSchema schema(mode);
		schema.Bits = bits; 
		return schema;
	}

	static FixedSchema Precision(int bits, int mode=MODE_NONE)
	{

		mode |= MODE_PRECISION;
		FixedSchema schema(mode);
		schema.Bits = bits; 
		return schema;
	}

	int Bias() {return ((1<<(Exponent-1))-1);}
};

class Fixed
{
public:

	static FixedSchema Float16;
	static FixedSchema Float32;
	static FixedSchema Float64;
	static FixedSchema Float80;
	static FixedSchema Float128;
	static FixedSchema Float256;

	enum FixedStates 
	{	

		STATE_NONE		=(0), 
		STATE_ZERO		=(1), 
		STATE_POSITIVE	=(1)<<1,
		STATE_NEGATIVE	=(1)<<2, 

		STATE_NAN		=(1)<<3, 
		STATE_INFINITY	=(1)<<4,

		STATE_NEGATIVE_INFINITY		= STATE_NEGATIVE|STATE_INFINITY,
		STATE_POSITIVE_INFINITY		= STATE_POSITIVE|STATE_INFINITY,

		STATE_NEGATIVE_ZERO			= STATE_NEGATIVE|STATE_ZERO,
		STATE_POSITIVE_ZERO			= STATE_POSITIVE|STATE_ZERO,
	};

	using Unsigned = Hero::Unsigned;
	using Signed = Hero::Signed;

	Flags<8> State;

	int Scale;

	Signed Value;

	int Precision;

	Fixed():State(0),Scale(0),Precision(10)
	{
	}

	Fixed(int value):State(STATE_NONE),Value(value),Scale(0),Precision(10) {}
	Fixed(long long value):State(STATE_NONE),Value(value),Scale(0),Precision(10) {}

	Fixed(double value):State(0),Scale(0),Precision(10)
	{

		Fixed fixed = ConvertDoubleToFixed(value);
		Move(fixed);
	}

	Fixed(const String & value)
		:State(0),Scale(0),Precision(10)
	{
		Fixed fixed = ConvertStringToFixed(value);
		Move(fixed);
	}

	Fixed(int scale, const Signed & value, int state=STATE_NONE):
		State(state),Scale(scale),Value(value),Precision(10)
	{
	}

	Fixed(const Fixed & fixed):
		State(fixed.State),Scale(fixed.Scale),Value(fixed.Value),Precision(10)
	{

	}

	Fixed & operator = (const Fixed & fixed)
	{
		Copy((Fixed&)fixed);
		return *this;
	}

	Fixed & Clone(Fixed & fixed)
	{

		Copy(fixed);
		return *this;
	}

	Fixed & Copy(Fixed & fixed)
	{
		State = fixed.State;
		Scale = fixed.Scale;
		Value = fixed.Value;
		return *this;
	}

	Fixed & Move(Fixed & fixed)
	{
		State = fixed.State;
		Scale = fixed.Scale;
		Value = fixed.Value;

		fixed.State = 0;
		fixed.Scale = 0;
		fixed.Value = 0;
		return *this;
	}

	bool operator == (const Fixed & fixed)
	{
		if (State.Bits == fixed.State.Bits &&
			Scale == fixed.Scale &&
			Value == fixed.Value)
			return true;

		return false;
	}

	bool IsZero()		{return State.Is(STATE_ZERO);}
	bool IsNan()		{return State.Is(STATE_NAN);}
	bool IsInfinity()	{return State.Is(STATE_INFINITY);}
	bool IsNegative()	{return State.Is(STATE_NEGATIVE) || Value.Sign < 0;}
	bool IsPositive()	{return State.Is(STATE_POSITIVE) || Value.Sign > 0;}

	Fixed & Nan() 
	{
		Fixed fixed(0,STATE_NAN);
		Move(fixed);
		return *this;
	}

	Fixed & Trim()
	{

		int scale = Scale;
		Signed v = Value;
		Signed q = v;
		Signed r;
		while (r == 0 && q != 0)
		{
			r = q;
			r.DivideWithRemainder(10,q);
			if (r == 0)
			{
				++scale;
				v = q;
			}
		}

		if (scale != Scale)
		{
			Value = v;
			Scale = scale;
		}

		return *this;
	}

	static int Rescale(Fixed & lhs, Fixed & rhs)
	{

		if (lhs.Scale > rhs.Scale)
		{

			int factor = lhs.Scale-rhs.Scale;
			lhs.Value *= Signed(10).Pow(factor);
			lhs.Scale = rhs.Scale;
			return lhs.Scale;
		}
		else
		if (rhs.Scale > lhs.Scale)
		{
			int factor = rhs.Scale-lhs.Scale;
			rhs.Value *= Signed(10).Pow(factor);
			rhs.Scale = lhs.Scale;
			return rhs.Scale;
		}

		return lhs.Scale&rhs.Scale;
	}

	Fixed & Add(const Fixed &fixed)
	{

		Fixed & lhs = *this;
		Fixed rhs = fixed;

		if (rhs.IsNan()) return lhs.Nan();

		Rescale(lhs,rhs);

		lhs.Value += rhs.Value;

		Trim();

		return *this;
	}

	Fixed & Subtract(const Fixed & fixed)
	{
		Fixed & lhs = *this;
		Fixed rhs = fixed;

		if (rhs.IsNan()) return lhs.Nan();

		Rescale(lhs,rhs);

		lhs.Value -= rhs.Value;

		Trim();

		return *this;
	}

	Fixed & Multiply(const Fixed & fixed)
	{
		Fixed & lhs = *this;
		Fixed rhs = fixed;

		if (rhs.IsNan()) return lhs.Nan();

		Rescale(lhs,rhs);

		lhs.Value *= rhs.Value;

		Trim();

		return *this;
	}

	Fixed & Divide(const Fixed & fixed)
	{
		Fixed & lhs = *this;
		Fixed rhs = fixed;

		if (rhs.IsNan()) return lhs.Nan();

		Rescale(lhs,rhs);

		Signed q;
		Signed r = lhs.Value;
		Signed d = rhs.Value;

		if (r > d)
			lhs.Scale = 0;

		int factor = 0;

		r.DivideWithRemainder(d, q);

		lhs.Value = q;

		if (q == 0)
			lhs.Scale = 0;

		while(true)
		{

			if (r > 0 && Scale > -Precision)
			{				

				factor = 0;
				while (r < d)
				{
					r *= 10;
					++factor;
				}

				r.DivideWithRemainder(d, q);
				if (q != 0)
				{

					lhs.Value *= 10;
					lhs.Value += q;
				}

				lhs.Scale -= factor;

				continue;
			}

			break;
		}

		Trim();

		return *this;
	}

	friend Fixed operator + (const Fixed & lhs, const Fixed & rhs) {return Fixed(lhs).Add(rhs);}
	friend Fixed operator - (const Fixed & lhs, const Fixed & rhs) {return Fixed(lhs).Subtract(rhs);}
	friend Fixed operator * (const Fixed & lhs, const Fixed & rhs) {return Fixed(lhs).Multiply(rhs);}
	friend Fixed operator / (const Fixed & lhs, const Fixed & rhs) {return Fixed(lhs).Divide(rhs);}

	String Str() 
	{
		return ConvertFixedToString(*this);
	}

	static double ConvertFixedToDouble(Fixed & fixed);
	static Fixed ConvertStringToFixed(const String & str);

	static String ConvertFloatToString(float value);
	static Fixed ConvertFloatToFixed(float value);

	static String ConvertDoubleToString(double value);
	static Fixed ConvertDoubleToFixed(double value);

	static Fixed ConvertRealToFixed(long long bits, FixedSchema & schema);

	static String ConvertFixedToString(Fixed & fixed, const FixedSchema & schema=FixedSchema());
	static String ConvertFixedToString(Fixed & fixed, FixedSchema & schema);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hadouken {

class Ryu 
{
public:

	using Fixed 	= Hero::Fixed;	
	using Signed 	= Hero::Signed;
	using Unsigned 	= Hero::Unsigned;

	static const bool DEBUG = false;
	static const int DOUBLE_MANTISSA_BITS = 52;
	static const long long DOUBLE_MANTISSA_MASK = (1LL << DOUBLE_MANTISSA_BITS) - 1;

	static const int DOUBLE_EXPONENT_BITS = 11;
	static const int DOUBLE_EXPONENT_MASK = (1 << DOUBLE_EXPONENT_BITS) - 1;
	static const int DOUBLE_EXPONENT_BIAS = (1 << (DOUBLE_EXPONENT_BITS - 1)) - 1;

	static const int POS_TABLE_SIZE = 326;
	static const int NEG_TABLE_SIZE = 291;

	static Unsigned POW5[POS_TABLE_SIZE];
	static Unsigned POW5_INV[NEG_TABLE_SIZE];

	static const int POW5_BITCOUNT = 121; 
	static const int POW5_QUARTER_BITCOUNT = 31;
	static int POW5_SPLIT[POS_TABLE_SIZE][4];

	static const int POW5_INV_BITCOUNT = 122; 
	static const int POW5_INV_QUARTER_BITCOUNT = 31;
	static int POW5_INV_SPLIT[NEG_TABLE_SIZE][4];

	static int BitShiftRight(int value, int bits);
	static unsigned int BitShiftRight(unsigned int value, int bits) 
	{
		return (unsigned int)BitShiftRight((int)value,bits);
	}

	static long long BitShiftRight(long long value, int bits);
	static unsigned long long BitShiftRight(unsigned long long value, int bits)
	{
		return (unsigned long long)BitShiftRight((long long)value,bits);
	}

	static int Pow5Bits(int e);
	static int Pow5Factor(long long value);

	static int DecimalLength(long long v);
	static bool MultipleOfPowerOf5(long long value, int q);

	static long long MulPow5DivPow2(long long m, int i, int j);

	static long long MulPow5InvDivPow2(long long m, int i, int j);

	static Ryu & Instance();

	static double StringToDouble(const String & str);
	static String DoubleToString(double value);

	static Fixed StringToFixed(const String & str, FixedSchema & schema);
	static Fixed StringToFixed(const String & str)
	{
		FixedSchema schema;
		return StringToFixed(str,schema);
	}

	static Fixed DoubleToFixed(double value, FixedSchema & schema);
	static Fixed DoubleToFixed(double value)
	{
		FixedSchema schema(FixedSchema::MODE_ROUND_EVEN);
		return DoubleToFixed(value,schema);
	}  	

	static String FixedToString(Fixed & fixed, const FixedSchema & schema=FixedSchema());
	static String FixedToString(Fixed & fixed, FixedSchema & schema);

	static double FixedToDouble(Fixed & fixed, const FixedSchema & schema=FixedSchema());
	static double FixedToDouble(Fixed & fixed, FixedSchema & schema);

private:

	Ryu();	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
