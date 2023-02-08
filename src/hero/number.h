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




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "hero/hero.h"
#include "hero/string.h"
#include "hero/error.h"
#include "hero/structure.h"
#include "hero/signed.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_WINDOWS
#pragma warning(disable:4244)
#endif

#include <math.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Hero {


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




class Prime
{
public:

	static bool IsPrime(int x)
	{
		int i=0;
		int j=NumberCount;

		while (i < j)
		{
			int pivot = (j-i)/2;
			if (Number[pivot] == x)
			{
				return true;
			}
			else
			if (Number[pivot] < x)
			{
				i = pivot;
			}
			else
			if (Number[pivot] > x)
			{
				j = pivot;
			}
		}

		return false;
	}

	static const int NumberCount;
	static const int Number[];

	
	struct PowerOffset
	{
        const int Offset[10];

		int operator[] (int index)
		{
			return Offset[index];
		}
	};

	static const int PowerCount;
	static const PowerOffset Power[];


};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



























////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Float
{
public:

	static float Infinity()
	{
		static float inf = (float)1.0/(float)atof("0.0");
		return inf;
	}

	static float Nan()
	{
		static float nan = (float)0.0/(float)atof("0.0");
		return nan;
	}

	static const float PositiveZero;
	static const float NegativeZero;
	static const float PositiveInfinity;
	static const float NegativeInfinity;
	static const float NotANumber;
};

class Double
{
public:

	static bool IsNan(double value) {return value == Nan();}
	static bool IsInfinity(double value) {return value == Infinity();}

	static double Infinity()
	{
		static double inf = 1.0/atof("0.0");
		return inf;
	}

	static double Nan()
	{
		static double nan = 0.0/atof("0.0");
		return nan;
	}

	static const double PositiveZero;
	static const double NegativeZero;
	static const double PositiveInfinity;
	static const double NegativeInfinity;
	static const double NotANumber;
};




class Int
{
};

class LongLong
{
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Numbers
{
public:

	static int Max(signed x, signed y)		{return x > y ? x : y;}
	static int Max(unsigned x, unsigned y)	{return x > y ? x : y;}
	static float Max(float x, float y)		{return x > y ? x : y;}
	static double Max(double x, double y)	{return x > y ? x : y;}

	static int Min(signed x, signed y)		{return x < y ? x : y;}
	static int Min(unsigned x, unsigned y)	{return x < y ? x : y;}
	static float Min(float x, float y)		{return x < y ? x : y;}
	static double Min(double x, double y)	{return x < y ? x : y;}

	static int Abs(int x)					{return abs(x);}
	static long long Abs(long long x)		{return abs(x);}
	static float Abs(float x)				{return abs(x);}
	static double Abs(double x)				{return abs(x);}

	static float Ceil(float x)				{return ceil(x);}
	static double Ceil(double x)			{return ceil(x);}

	static float Floor(float x)				{return floor(x);}
	static double Floor(double x)			{return floor(x);}

	static float Sign(float x)				{return x > 0.0 ? 1.0 : -1.0;}
	static double Sign(double x)			{return x > 0.0 ? 1.0 : -1.0;}

	static int Clamp(long long a)
	{
		bool sign = (a < 0);
		int b = (int)a;
		
		if ((long long)(b) == a)
			return b;
    	
		return (sign)?0x80000000:0x7FFFFFFF;
	}

	template <typename _Kind_>
	static int Diff(_Kind_ lhs, _Kind_ rhs)
	{
		
		Hero::Clamp<int,_Kind_>::Diff(lhs,rhs);
	}

	template <typename _Kind_>
	static int Compare(_Kind_ lhs, _Kind_ rhs)
	{
		
		return Hero::Comparer<_Kind_>::Compare(lhs,rhs);
	}


	
	
	

	static float Log(float x)				{return log10(x);}
	static double Log(double x)				{return log10(x);}

	static float Ln(float x)				{return log(x);}
	static double Ln(double x)				{return log(x);}

	static float Pow(float x, float y)		{return pow(x,y);}
	static double Pow(double x, double y)	{return pow(x,y);}


	

	static float Round(float x)				{return (x-(int)x>=0.5)?ceil(x):floor(x);}
	static double Round(double x)			{return (x-(int)x>=0.5)?ceil(x):floor(x);}

	static long Random();
	
	static void Random(float & value)
	{
		value = (float)Numbers::Random();
	}
	
	static void Random(double &value) 
	{
		Random((long long&)value);
	}
	
	static void Random(long long &value) 
	{
		
		
		value = (((long long)Numbers::Random()<<32)|(long long)Numbers::Random());
	}
	
	static void Random(int & value) 
	{
		value = Numbers::Random();
	}
	
	static double Zero()
	{
		return 0.0;
	}

	static double PositiveZero()
	{
		return Double::PositiveZero;
	}

	static double NegativeZero()
	{
		return Double::NegativeZero;
	}

	static double Infinity()
	{
		
		

		
		
		
		

		
		static const double inf = (double)HUGE_VAL;
		
		
		return inf;
	}

	static double PositiveInfinity()
	{
		return Double::PositiveInfinity;
	}

	static double NegativeInfinity()
	{
		return Double::NegativeInfinity;
	}
	
	
	static signed short MinimumSignedShort()
	{
		static const short minimum = pow((double)2,(int)(sizeof(short)*8-1));
		return minimum;
	}
	
	static signed short MaximumSignedShort()
	{
		static const short maximum = pow((double)2,(int)(sizeof(short)*8-1))-1;
		return maximum;
	}
	
	static unsigned short MaximumUnsignedShort()
	{
		static const unsigned short maximum = pow((double)2,(int)(sizeof(short)*8))-1;
		return maximum;
	}		

	static signed MinimumSigned()
	{
		static const int minimum = pow((double)2,(int)(sizeof(int)*8-1));
		return minimum;
	}

	static signed MaximumSigned()
	{
		static const int maximum = pow((double)2,(int)(sizeof(int)*8-1))-1;
		
		return maximum;
	}

	static unsigned MaximumUnsigned()
	{
		static const unsigned int maximum = pow((double)2,(int)(sizeof(int)*8))-1;
		
		return maximum;
	}
		
	static signed int MinimumSignedInt() {return MinimumSigned();}
	static signed int MaximumSignedInt() {return MaximumSigned();}
	static unsigned int MaximumUnsignedInt() {return MaximumUnsigned();}
	
	
	static signed long MinimumSignedLong()
	{
		static const long minimum = pow((double)2,(int)(sizeof(long)*8-1));
		return minimum;
	}
	
	static signed long MaximumSignedLong()
	{
		static const long maximum = pow((double)2,(int)(sizeof(long)*8-1))-1;
		return maximum;
	}
	
	static unsigned long MaximumUnsignedLong()
	{
		static const unsigned long maximum =  pow((double)2,(int)(sizeof(long)*8-1));
		
		return maximum;
	}	
	
	static signed long long MinimumSignedLongLong()
	{
		static const long long minimum = (1LL<<(sizeof(long long)*8-1));
		return minimum;
	}
	
	static signed long long MaximumSignedLongLong()
	{
		
		
		static const long long maximum = ~(1LL<<(sizeof(long long)*8-1));
		return maximum;
	}
	
	static unsigned long long MaximumUnsignedLongLong()
	{
		static const unsigned long long maximum = ~0LL;
		return maximum;
	}	
	
	static float MaximumFloat()
	{
		
		static const float maximum = (float)3.40282347e+38; 
		return maximum;
	}

	static double MaximumDouble()
	{
		
		static const double maximum = (double)1.7976931348623157e+308;	
		return maximum;
	}

	static bool IsInfinity(float x)				{return x == Infinity() || x == -Infinity();}
	static bool IsInfinity(double x)			{return x == Infinity() || x == -Infinity();}
	
	static bool IsZero(float x)					{return IsPositiveZero(x) || IsNegativeZero(x);}
	static bool IsZero(double x)				{return IsPositiveZero(x) || IsNegativeZero(x);}
	
	static bool IsPositiveInfinity(float x)		{return x == Infinity();}
	static bool IsPositiveInfinity(double x)	{return x == Infinity();}

	static bool IsPositiveZero(float x)			{return x == 0.0;}
	static bool IsPositiveZero(double x)		{return x == 0.0;}

	static bool IsPositive(int x)				{return x > 0;}
	static bool IsPositive(float x)				{return IsPositiveZero(x) || IsPositiveInfinity(x) || x > 0.0;}
	static bool IsPositive(double x)			{return IsPositiveZero(x) || IsPositiveInfinity(x) || x > 0.0;}

	static bool IsNegativeInfinity(float x)		{return x == -Infinity();}
	static bool IsNegativeInfinity(double x)	{return x == -Infinity();}

	static bool IsNegativeZero(float x)			{return x == -0.0;}
	static bool IsNegativeZero(double x)		{return x == -0.0;}

	static bool IsNegative(int x)				{return x < 0;}
	static bool IsNegative(float x)				{return IsNegativeZero(x) || IsNegativeInfinity(x) || x < 0.0;}
	static bool IsNegative(double x)			{return IsNegativeZero(x) || IsNegativeInfinity(x) || x < 0.0;}
	
	static bool IsFinite(double x)				{return !IsNan(x) && !IsInfinity(x);}

	static double Nan()
	{
		
		
		
		

		
		
		static const double nan = sqrt(-2.01);
		
		return nan;
	}

	static bool IsNan(float x)
	{
		return x != x;
	}


	static bool IsNan(double x)
	{
		return x != x;
	}

	
	
	
	
	
	
	
	static int Compare(float left, float right, float epsilon = 0.00001)
	{
		float result = left-right;
		return (Abs(result)<epsilon)?0:(result<0?-1:1);
	}

	static int Compare(double left, double right, double epsilon = 0.00001)
	{
		double result = left-right;
		return (Abs(result)<epsilon)?0:(result<0?-1:1);
	}

	static int Compare(int left, int right)
	{
		return left-right;
	}

	static long Compare(long left, long right)
	{
		return left-right;
	}	
	
	static long long Compare(long long left, long long right)
	{
		return left-right;
	}
	
	static int Compare(unsigned int left, unsigned int right)
	{
		return left-right;
	}
	
	static long Compare(unsigned long left, unsigned long right)
	{
		return left-right;
	}

	static long long Compare(unsigned long long left, unsigned long long right)
	{
		return left-right;
	}
	
	static void * Compare(void * left, void * right)
	{
		return (void *)((long long)left-(long long)right);
	}
	
	static bool Equals(float left, float right, float epsilon = 0.00001) {return Compare(left,right,epsilon)==0;}
	static bool Equals(double left, double right, double epsilon = 0.00001) {return Compare(left,right,epsilon)==0;}
	static bool Equals(int left, int right) {return Compare(left,right)==0;}
	static bool Equals(long left, long right) {return Compare(left,right)==0;}
	static bool Equals(long long left, long long right) {return Compare(left,right)==0;}
	static bool Equals(unsigned int left, unsigned int right) {return Compare(left,right)==0;}	
	static bool Equals(unsigned long left, unsigned long right) {return Compare(left,right)==0;}
	static bool Equals(unsigned long long left, unsigned long long right) {return Compare(left,right)==0;}
	static bool Equals(void * left, void * right) {return Compare(left,right)==0;}

	static int Hash(float value) {return Hash((int)value);}
	static int Hash(int value);
	
	
	
	
	static int Hash(long value);

	static int Hash(double value) 
	{
		
		
		return Hash(*(long long*)&value);
	}
	
	
	
	static int Hash(void * value) {return Hash((long long)value);}
	static int Hash(long long value)
	{
		return Hash((int)(value>>32)) ^ Hash((int)(value&0x00000000FFFFFFFF));
	}
	

	static const char * Suffix(int number)
	{
		
		if (number > 10 && number < 20) number = 0;
		switch (number%10)
		{
			case 0: return "th";break;
			case 1: return "st";break;
			case 2: return "nd";break;
			case 3: return "rd";break;
			case 4: return "th";break;
			case 5: return "th";break;
			case 6: return "th";break;
			case 7: return "th";break;
			case 8: return "th";break;
			case 9: return "th";break;
		}
		return "";
	}


	static Superstring Binary(long long number)
	{
		Superstring string;
		if (number == 0)
			string << "0";
		
		while (number > 0)
		{
			bool bit = (number&0x00000001);
			number >>= 1;
			string << ((bit)?'1':'0');
		}
		
		return string;
	}

	static Superstring Hex(long long number)
	{
		Superstring string;

		
		

		if (number == 0)
			string << "0";

		while (number > 0)
		{
			char hex = (number&0x0000000F);
			number >>= 4;

			
			if (hex >=0 && hex <= 9)
				string << (char)(hex+0x30);
			else
				string << (char)(hex+0x37);
		}
		
		return string;
	}
	
	static int ConvertToString(long long number, int radix, char * data, int size, bool padded=false);
	static Superstring ConvertToString(long long number, int radix);
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Number: public Object, public Numbers
{
public:

	static Identity Instance;
	virtual Identity& Identify(){return Instance;};

public:

	enum NumberTypes
	{

		TYPE_NONE		=(0),
		
		
		TYPE_CHAR		=(1)<<2,
		TYPE_SHORT		=(1)<<3,
		TYPE_INT		=(1)<<4,
		TYPE_LONG		=(1)<<5,
		TYPE_LONGLONG	=(1)<<6,
		
		TYPE_FLOAT		=(1)<<7,
		TYPE_DOUBLE		=(1)<<8,

		TYPE_INTEGER	= TYPE_CHAR|TYPE_SHORT|TYPE_INT|TYPE_LONG|TYPE_LONGLONG,
		TYPE_REAL		= TYPE_FLOAT|TYPE_DOUBLE,

		
		TYPE_SIGNED		=(1)<<14,
		TYPE_UNSIGNED	=(1)<<15,
	};

	union NumberStorage
	{
		
		

		
		
		NumberStorage():Integer(0) {}
		NumberStorage(long long x):Integer(x) {}
		NumberStorage(double x):Real(x) {}
		
		long long Integer;
		double Real;
	} Data;
	
	int Size;
	Flags<16> Type;


	Number():Data(),Size(sizeof(int)),Type(0)
	{
		
	}

	Number(const Number &number)
	{
		Data = number.Data;
		Type = number.Type;
	}
	
	Number(unsigned char value):Data((long long)value),Size(sizeof(char)),Type(TYPE_UNSIGNED|TYPE_CHAR) {}
	Number(char value):Data((long long)value),Size(sizeof(char)),Type(TYPE_SIGNED|TYPE_CHAR) {}	
	Number(unsigned short value):Data((long long)value),Size(sizeof(short)),Type(TYPE_UNSIGNED|TYPE_SHORT){}
	Number(short value):Data((long long)value),Size(sizeof(short)),Type(TYPE_SIGNED|TYPE_SHORT) {}
	Number(unsigned int value):Data((long long)value),Size(sizeof(int)),Type(TYPE_UNSIGNED|TYPE_INT){}
	Number(int value):Data((long long)value),Size(sizeof(int)),Type(TYPE_SIGNED|TYPE_INT) {}
	Number(unsigned long value):Data((long long)value),Size(sizeof(long)),Type(TYPE_UNSIGNED|TYPE_LONG){}
	Number(long value):Data((long long)value),Size(sizeof(long)),Type(TYPE_SIGNED|TYPE_LONG) {}
	Number(unsigned long long value):Data((long long)value),Size(sizeof(long long)),Type(TYPE_UNSIGNED|TYPE_LONGLONG){}
	Number(long long value):Data((long long)value),Size(sizeof(long long)),Type(TYPE_SIGNED|TYPE_LONGLONG) {}
	Number(float value):Data((double)value),Size(sizeof(float)),Type(TYPE_FLOAT) {}
	Number(double value):Data((double)value),Size(sizeof(double)),Type(TYPE_DOUBLE) {}

	int Hash()
	{
		
		return Numbers::Hash(SignedLongLong());
	}

	long long Integer() {return SignedLongLong();}
	double Real() {return Double();}


	void Print(String & print);

	signed long long SignedLongLong();
	unsigned long long UnsignedLongLong();



	
	
	

	signed long SignedLong()		{return (signed long)SignedLongLong();}
	unsigned long UnsignedLong()	{return (unsigned long)UnsignedLongLong();}		

	signed int SignedInt()			{return (signed int)SignedLongLong();}
	unsigned int UnsignedInt()		{return (unsigned int)UnsignedLongLong();}		

	signed short SignedShort()		{return (signed short)SignedLongLong();}
	unsigned short UnsignedShort()	{return (unsigned short)UnsignedLongLong();}		
	
	signed char SignedChar()		{return (signed char)SignedLongLong();}
	unsigned char UnsignedChar()	{return (unsigned char)UnsignedLongLong();}			

	double Double()
	{
		if (IsInteger())
			return (double)Data.Integer;
		else
		if (IsReal())
			return (double)Data.Real;
	
		Raise(new TypeError());
		return 0.0;
	}


	float Float()
	{
		if (IsInteger())
			return (float)Data.Integer;
		else
		if (IsReal())
			return (float)Data.Real;
		
		Raise(new TypeError());
		return 0.0;
	}


	


	
	

	bool IsInteger()	{return Type.Is(TYPE_INTEGER);}	
	bool IsSigned()		{return Type.Is(TYPE_SIGNED);}
	bool IsUnsigned()	{return Type.Is(TYPE_UNSIGNED);}
	
	bool IsReal()		{return Type.Is(TYPE_REAL);}
	bool IsFloat()		{return Type.Is(TYPE_FLOAT);}
	bool IsDouble()		{return Type.Is(TYPE_DOUBLE);}

	int Sign()			
	{
		
		
		
		if (IsInteger())
		{
			if (IsSigned() && Data.Integer < 0) return -1;
			if (Data.Integer == 0) return 0;
		}
		else
		if (IsReal())
		{
			if (Data.Real < 0.0) return -1;
			if (Data.Real == 0.0) return 0;
		}

		return 1;
	}

	bool IsNegative()	{return Sign() < 0;}
	bool IsPositive()	{return Sign() > 0;}
	bool IsZero()		{return Sign() == 0;}
	
	bool IsLongLong()	{return Type.Is(TYPE_LONGLONG);}
	bool IsLong()		{return Type.Is(TYPE_LONG);}
	bool IsInt()		{return Type.Is(TYPE_INT);}
	bool IsShort()		{return Type.Is(TYPE_SHORT);}
	bool IsChar()		{return Type.Is(TYPE_CHAR);}

	using Numbers::IsNan;
	using Numbers::IsInfinity;
	using Numbers::IsPositiveInfinity;
	using Numbers::IsNegativeInfinity;
	using Numbers::IsZero;
	using Numbers::IsPositiveZero;
	using Numbers::IsNegativeZero;
	using Numbers::IsFinite;


	bool IsNan()
	{
		
		
		
		switch ((Type.Bits&(TYPE_INTEGER|TYPE_REAL)))
		{
		case TYPE_FLOAT:
			return (Float() != Float());
		case TYPE_DOUBLE:	
			return (Double() != Double());
		default: return false;
		}		
	}

	
	bool IsInfinity()
	{
		return IsPositiveInfinity() || IsNegativeInfinity();
	}

	bool IsPositiveInfinity()
	{
		switch ((Type.Bits&(TYPE_INTEGER|TYPE_REAL)))
		{
		case TYPE_FLOAT:
			return Float() == Infinity();
		case TYPE_DOUBLE:	
			return Double() == Infinity();
		default: return false;
		}		
	}


	bool IsNegativeInfinity()
	{
		switch ((Type.Bits&(TYPE_INTEGER|TYPE_REAL)))
		{
		case TYPE_FLOAT:
			return Float() == -Infinity();
		case TYPE_DOUBLE:	
			return Double() == -Infinity();
		default: return false;
		}		
	}

	
	
	
	

	
	
	

	bool IsPositiveZero()
	{
		switch ((Type.Bits&(TYPE_INTEGER|TYPE_REAL)))
		{
		case TYPE_FLOAT:
			return Float() == 0.0;
		case TYPE_DOUBLE:	
			return Double() == 0.0;
		default: return false;
		}	
	}

	bool IsNegativeZero()
	{
		switch ((Type.Bits&(TYPE_INTEGER|TYPE_REAL)))
		{
		case TYPE_FLOAT:
			return Float() == -0.0;
		case TYPE_DOUBLE:	
			return Double() == -0.0;
		default: return false;
		}	
	}

	bool IsFinite()
	{
		return !IsNan() && !IsInfinity();
	}

	

	using Numbers::Compare;
	int Compare(Object * object,int comparitor);

	using Numbers::Equals;

	
	
	

	
	


	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	Number & operator = (signed long long x) {return operator = (Number(x));}
	Number & operator = (unsigned long long x) {return operator = (Number(x));}
	Number & operator = (signed long x) {return operator = (Number(x));}
	Number & operator = (unsigned long x) {return operator = (Number(x));}	
	Number & operator = (signed int x) {return operator = (Number(x));}
	Number & operator = (unsigned int x) {return operator = (Number(x));}
	Number & operator = (signed short x) {return operator = (Number(x));}
	Number & operator = (unsigned short x) {return operator = (Number(x));}
	Number & operator = (signed char x) {return operator = (Number(x));}
	Number & operator = (unsigned char x) {return operator = (Number(x));}
	Number & operator = (char x) {return operator = (Number(x));}
	Number & operator = (float x) {return operator = (Number(x));}
	Number & operator = (double x) {return operator = (Number(x));}	
	Number & operator = (const Number & number) {return operator = ((Number&)number);}
	Number & operator = (Number & number);

	
	bool operator == (  signed long long x) {return operator == (Number(x));}
	bool operator == (unsigned long long x) {return operator == (Number(x));}
	bool operator == (  signed long x) {return operator == (Number(x));}
	bool operator == (unsigned long x) {return operator == (Number(x));}	
	bool operator == (  signed int x) {return operator == (Number(x));}
	bool operator == (unsigned int x) {return operator == (Number(x));}
	bool operator == (  signed short x) {return operator == (Number(x));}
	bool operator == (unsigned short x) {return operator == (Number(x));}
	bool operator == (  signed char x) {return operator == (Number(x));}
	bool operator == (unsigned char x) {return operator == (Number(x));}
	bool operator == (char x) {return operator == (Number(x));}
	bool operator == (float x) {return operator == (Number(x));}
	bool operator == (double x) {return operator == (Number(x));}
	bool operator == (const Number & number) {return operator == (((Number&)number));}
	bool operator == (Number & number);



	bool operator <= (  signed long long x) {return operator <= (Number(x));}
	bool operator <= (unsigned long long x) {return operator <= (Number(x));}
	bool operator <= (  signed long x) {return operator <= (Number(x));}
	bool operator <= (unsigned long x) {return operator <= (Number(x));}	
	bool operator <= (  signed int x) {return operator <= (Number(x));}
	bool operator <= (unsigned int x) {return operator <= (Number(x));}
	bool operator <= (  signed short x) {return operator <= (Number(x));}
	bool operator <= (unsigned short x) {return operator <= (Number(x));}
	bool operator <= (  signed char x) {return operator <= (Number(x));}
	bool operator <= (unsigned char x) {return operator <= (Number(x));}
	bool operator <= (char x) {return operator <= (Number(x));}
	bool operator <= (float x) {return operator <= (Number(x));}
	bool operator <= (double x) {return operator <= (Number(x));}
	bool operator <= (const Number & number) {return operator <= ((Number&)number);}
	bool operator <= (Number & number);
	

	bool operator < (  signed long long x) {return operator < (Number(x));}
	bool operator < (unsigned long long x) {return operator < (Number(x));}
	bool operator < (  signed long x) {return operator < (Number(x));}
	bool operator < (unsigned long x) {return operator < (Number(x));}	
	bool operator < (  signed int x) {return operator < (Number(x));}
	bool operator < (unsigned int x) {return operator < (Number(x));}
	bool operator < (  signed short x) {return operator < (Number(x));}
	bool operator < (unsigned short x) {return operator < (Number(x));}
	bool operator < (  signed char x) {return operator < (Number(x));}
	bool operator < (unsigned char x) {return operator < (Number(x));}
	bool operator < (char x) {return operator < (Number(x));}
	bool operator < (float x) {return operator < (Number(x));}
	bool operator < (double x) {return operator < (Number(x));}
	bool operator < (const Number & number) {return operator < ((Number&)number);}
	bool operator < (Number & number);

	bool operator >= (  signed long long x) {return operator >= (Number(x));}
	bool operator >= (unsigned long long x) {return operator >= (Number(x));}
	bool operator >= (  signed long x) {return operator >= (Number(x));}
	bool operator >= (unsigned long x) {return operator >= (Number(x));}	
	bool operator >= (  signed int x) {return operator >= (Number(x));}
	bool operator >= (unsigned int x) {return operator >= (Number(x));}
	bool operator >= (  signed short x) {return operator >= (Number(x));}
	bool operator >= (unsigned short x) {return operator >= (Number(x));}
	bool operator >= (  signed char x) {return operator >= (Number(x));}
	bool operator >= (unsigned char x) {return operator >= (Number(x));}
	bool operator >= (char x) {return operator >= (Number(x));}
	bool operator >= (float x) {return operator >= (Number(x));}
	bool operator >= (double x) {return operator >= (Number(x));}
	bool operator >= (const Number & number) {return operator >= ((Number&)number);}
	bool operator >= (Number & number);


	bool operator > (  signed long long x) {return operator > (Number(x));}
	bool operator > (unsigned long long x) {return operator > (Number(x));}
	bool operator > (  signed long x) {return operator > (Number(x));}
	bool operator > (unsigned long x) {return operator > (Number(x));}	
	bool operator > (  signed int x) {return operator > (Number(x));}
	bool operator > (unsigned int x) {return operator > (Number(x));}
	bool operator > (  signed short x) {return operator > (Number(x));}
	bool operator > (unsigned short x) {return operator > (Number(x));}
	bool operator > (  signed char x) {return operator > (Number(x));}
	bool operator > (unsigned char x) {return operator > (Number(x));}
	bool operator > (char x) {return operator > (Number(x));}
	bool operator > (float x) {return operator > (Number(x));}
	bool operator > (double x) {return operator > (Number(x));}
	bool operator > (const Number & number) {return operator > ((Number&)number);}
	bool operator > (Number & number);

	Number operator - (  signed long long x) {return operator - (Number(x));}
	Number operator - (unsigned long long x) {return operator - (Number(x));}
	Number operator - (  signed long x) {return operator - (Number(x));}
	Number operator - (unsigned long x) {return operator - (Number(x));}	
	Number operator - (  signed int x) {return operator - (Number(x));}
	Number operator - (unsigned int x) {return operator - (Number(x));}
	Number operator - (  signed short x) {return operator - (Number(x));}
	Number operator - (unsigned short x) {return operator - (Number(x));}
	Number operator - (  signed char x) {return operator - (Number(x));}
	Number operator - (unsigned char x) {return operator - (Number(x));}
	Number operator - (char x) {return operator - (Number(x));}
	Number operator - (float x) {return operator - (Number(x));}
	Number operator - (double x) {return operator - (Number(x));}
	Number operator - (const Number & number) {return operator - ((Number&)number);}
	Number operator - (Number & number);

	Number & operator -= (  signed long long x) {return operator -= (Number(x));}
	Number & operator -= (unsigned long long x) {return operator -= (Number(x));}
	Number & operator -= (  signed long x) {return operator -= (Number(x));}
	Number & operator -= (unsigned long x) {return operator -= (Number(x));}	
	Number & operator -= (  signed int x) {return operator -= (Number(x));}
	Number & operator -= (unsigned int x) {return operator -= (Number(x));}
	Number & operator -= (  signed short x) {return operator -= (Number(x));}
	Number & operator -= (unsigned short x) {return operator -= (Number(x));}
	Number & operator -= (  signed char x) {return operator -= (Number(x));}
	Number & operator -= (unsigned char x) {return operator -= (Number(x));}
	Number & operator -= (char x) {return operator -= (Number(x));}
	Number & operator -= (float x) {return operator -= (Number(x));}
	Number & operator -= (double x) {return operator -= (Number(x));}
	Number & operator -= (const Number & number) {return operator -= ((Number&)number);}
	Number & operator -= (Number & number);


	Number operator + (  signed long long x) {return operator + (Number(x));}
	Number operator + (unsigned long long x) {return operator + (Number(x));}
	Number operator + (  signed long x) {return operator + (Number(x));}
	Number operator + (unsigned long x) {return operator + (Number(x));}	
	Number operator + (  signed int x) {return operator + (Number(x));}
	Number operator + (unsigned int x) {return operator + (Number(x));}
	Number operator + (  signed short x) {return operator + (Number(x));}
	Number operator + (unsigned short x) {return operator + (Number(x));}
	Number operator + (  signed char x) {return operator + (Number(x));}
	Number operator + (unsigned char x) {return operator + (Number(x));}
	Number operator + (char x) {return operator + (Number(x));}
	Number operator + (float x) {return operator + (Number(x));}
	Number operator + (double x) {return operator + (Number(x));}
	Number operator + (const Number & number) {return operator + ((Number&)number);}
	Number operator + (Number & number);


	Number & operator += (  signed long long x) {return operator += (Number(x));}
	Number & operator += (unsigned long long x) {return operator += (Number(x));}
	Number & operator += (  signed long x) {return operator += (Number(x));}
	Number & operator += (unsigned long x) {return operator += (Number(x));}	
	Number & operator += (  signed int x) {return operator += (Number(x));}
	Number & operator += (unsigned int x) {return operator += (Number(x));}
	Number & operator += (  signed short x) {return operator += (Number(x));}
	Number & operator += (unsigned short x) {return operator += (Number(x));}
	Number & operator += (  signed char x) {return operator += (Number(x));}
	Number & operator += (unsigned char x) {return operator += (Number(x));}
	Number & operator += (char x) {return operator += (Number(x));}
	Number & operator += (float x) {return operator += (Number(x));}
	Number & operator += (double x) {return operator += (Number(x));}
	Number & operator += (const Number & number) {return operator += ((Number&)number);}
	Number & operator += (Number & number);


	Number operator / (  signed long long x) {return operator / (Number(x));}
	Number operator / (unsigned long long x) {return operator / (Number(x));}
	Number operator / (  signed long x) {return operator / (Number(x));}
	Number operator / (unsigned long x) {return operator / (Number(x));}	
	Number operator / (  signed int x) {return operator / (Number(x));}
	Number operator / (unsigned int x) {return operator / (Number(x));}
	Number operator / (  signed short x) {return operator / (Number(x));}
	Number operator / (unsigned short x) {return operator / (Number(x));}
	Number operator / (  signed char x) {return operator / (Number(x));}
	Number operator / (unsigned char x) {return operator / (Number(x));}
	Number operator / (char x) {return operator / (Number(x));}
	Number operator / (float x) {return operator / (Number(x));}
	Number operator / (double x) {return operator / (Number(x));}
	Number operator / (const Number & number) {return operator / ((Number&)number);}
	Number operator / (Number & number);


	Number & operator /= (  signed long long x) {return operator /= (Number(x));}
	Number & operator /= (unsigned long long x) {return operator /= (Number(x));}
	Number & operator /= (  signed long x) {return operator /= (Number(x));}
	Number & operator /= (unsigned long x) {return operator /= (Number(x));}	
	Number & operator /= (  signed int x) {return operator /= (Number(x));}
	Number & operator /= (unsigned int x) {return operator /= (Number(x));}
	Number & operator /= (  signed short x) {return operator /= (Number(x));}
	Number & operator /= (unsigned short x) {return operator /= (Number(x));}
	Number & operator /= (  signed char x) {return operator /= (Number(x));}
	Number & operator /= (unsigned char x) {return operator /= (Number(x));}
	Number & operator /= (char x) {return operator /= (Number(x));}
	Number & operator /= (float x) {return operator /= (Number(x));}
	Number & operator /= (double x) {return operator /= (Number(x));}
	Number & operator /= (const Number & number) {return operator /= ((Number&)number);}
	Number & operator /= (Number & number);


	Number operator * (  signed long long x) {return operator * (Number(x));}
	Number operator * (unsigned long long x) {return operator * (Number(x));}
	Number operator * (  signed long x) {return operator * (Number(x));}
	Number operator * (unsigned long x) {return operator * (Number(x));}	
	Number operator * (  signed int x) {return operator * (Number(x));}
	Number operator * (unsigned int x) {return operator * (Number(x));}
	Number operator * (  signed short x) {return operator * (Number(x));}
	Number operator * (unsigned short x) {return operator * (Number(x));}
	Number operator * (  signed char x) {return operator * (Number(x));}
	Number operator * (unsigned char x) {return operator * (Number(x));}
	Number operator * (char x) {return operator * (Number(x));}
	Number operator * (float x) {return operator * (Number(x));}
	Number operator * (double x) {return operator * (Number(x));}
	Number operator * (const Number & number) {return operator * ((Number&)number);}
	Number operator * (Number & number);


	Number & operator *= (  signed long long x) {return operator *= (Number(x));}
	Number & operator *= (unsigned long long x) {return operator *= (Number(x));}
	Number & operator *= (  signed long x) {return operator *= (Number(x));}
	Number & operator *= (unsigned long x) {return operator *= (Number(x));}	
	Number & operator *= (  signed int x) {return operator *= (Number(x));}
	Number & operator *= (unsigned int x) {return operator *= (Number(x));}
	Number & operator *= (  signed short x) {return operator *= (Number(x));}
	Number & operator *= (unsigned short x) {return operator *= (Number(x));}
	Number & operator *= (  signed char x) {return operator *= (Number(x));}
	Number & operator *= (unsigned char x) {return operator *= (Number(x));}
	Number & operator *= (char x) {return operator *= (Number(x));}
	Number & operator *= (float x) {return operator *= (Number(x));}
	Number & operator *= (double x) {return operator *= (Number(x));}
	Number & operator *= (const Number & number) {return operator *= ((Number&)number);}
	Number & operator *= (Number & number);

	
	

	
	
	
	

	operator char () {return SignedChar();}
	operator   signed char () {return SignedChar();}
	operator unsigned char () {return UnsignedChar();}
	operator   signed short () {return SignedShort();}
	operator unsigned short () {return UnsignedShort();}
	operator   signed int () {return SignedInt();}
	operator unsigned int () {return UnsignedInt();}
	operator   signed long () {return SignedLong();}
	operator unsigned long () {return UnsignedLong();}
	operator   signed long long () {return SignedLongLong();}
	operator unsigned long long () {return UnsignedLongLong();}

	operator float ()			{return Float();}
	operator double ()			{return Double();}

	friend bool operator  == (char x, Number & number) {return x == number.SignedChar();}
	friend bool operator  == (  signed char x, Number & number) {return x == number.SignedChar();}
	friend bool operator  == (unsigned char x, Number & number) {return x == number.UnsignedChar();}
	friend bool operator  == (  signed short x, Number & number) {return x == number.SignedShort();}
	friend bool operator  == (unsigned short x, Number & number) {return x == number.UnsignedShort();}
	friend bool operator  == (  signed int x, Number & number) {return x == number.SignedInt();}
	friend bool operator  == (unsigned int x, Number & number) {return x == number.UnsignedInt();}
	friend bool operator  == (  signed long x, Number & number) {return x == number.SignedLong();}
	friend bool operator  == (unsigned long x, Number & number) {return x == number.UnsignedLong();}
	friend bool operator  == (  signed long long x, Number & number) {return x == number.SignedLongLong();}
	friend bool operator  == (unsigned long long x, Number & number) {return x == number.UnsignedLongLong();}
	friend bool operator  == (float x, Number & number) {return x == number.Float();}
	friend bool operator  == (double x, Number & number) {return x == number.Double();}

	
	
	
	
	Superstring Suffixed()
	{
		return Suffixed(*this);
	}

	static Superstring Suffixed(Number & number)
	{
		Superstring string;
		if (number.IsSigned())
			string << number.SignedInt() << Suffix(number.SignedInt());
		else
		if (number.IsUnsigned())
			string << (int)number.UnsignedInt() << Suffix(number.UnsignedInt());
		else
			string << "";
		return string;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Magnitude
{
public:

	
	
	
	
	
	
	

	static int Abs(int x)				{return (x >= 0) ? x : -x;}
	static int Max(int x, int y)		{return Abs(x) > Abs(y) ? x : y;}
	static int Min(int x, int y)		{return Abs(x) < Abs(y) ? x : y;}
	static int Compare(int x, int y)	{return Abs(x) - Abs(y);}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Frequency : public Array<Number>
{
public:

	double Sum();
	double Min();
	double Max();
	double Mean();
	double Mode();
	double Variance();
	double Deviation();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Statistics
{
public:

	Number Min;
	Number Total;
	int Count;
	Number Max;

	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Composite
{
public:

	Number Numerator;
	Number Denominator;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



