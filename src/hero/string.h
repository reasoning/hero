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

#include "hero/indices.h"
#include "hero/stream.h"
#include "hero/object.h"
#include "hero/slice.h"

#include "hero/assert.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-parameter"

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef HERO_USING_STD
#include <sstream>
#include <iosfwd>
#endif

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
class Regex;

class String;
class Superstring;
class Stackstring;
class Substring;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Characters 
{
public:

	static const int Null			= 0x00; 	
	static const int Backspace		= 0x08;		
	static const int Tab			= 0x09;		
	static const int LineFeed		= 0x0A;		
	static const int NewLine		= LineFeed;
	static const int VerticalTab	= 0x0B;		
	static const int FormFeed		= 0x0C;		
	static const int CarriageReturn = 0x0D;		
	static const int Eof			= 0x1A;		
	static const int Escape			= 0x1B;		
	static const int Space			= 0x20;		
	static const int DoubleQuote	= 0x22;		
	static const int SingleQuote	= 0x27; 	
	static const int ForwardSlash	= 0x2F;		
	static const int LessThan		= 0x3C;		
	static const int Equal			= 0x3D;		
	static const int GreaterThan	= 0x3E;		
	static const int BackwardSlash	= 0x5C;		
	static const int Delete			= 0x7F;		

	static const int LeftParenthesis	= '(';
	static const int LeftBracket		= '[';
	static const int LeftBrace			= '{';

	static const int RightParenthesis	= ')';
	static const int RightBracket		= ']';
	static const int RightBrace			= '}';

	static const int Upper[];
	static const int Lower[];

public:

	static char Case(char data, bool caseless)
	{

		return (caseless)?Caseless(data):data;
	}

	static char Caseless(char data)
	{	

		return Lower[(unsigned char) data];
	}

	static bool IsNewline(char data)
	{

		return data == CarriageReturn || data == LineFeed;
	}

	static bool	IsWhitespace(char data)
	{
		return data == Space || data == Tab || data == CarriageReturn || data == LineFeed;
	}

	static bool	IsAlphanumeric(char data)
	{

		return ((Caseless(data) > 96) && (Caseless(data) < 123)) || ((data>47) && (data<58));
	}

	static bool	IsAlpha(char data)
	{

		return (Caseless(data) > 96) && (Caseless(data) < 123);
	}

	static bool IsNumeric(char data)
	{

		return (data>47) && (data<58);
	}

	static bool IsDecimal(char data)
	{

		return IsNumeric(data);
	}

	static bool IsOctal(char data)
	{
		return data >= '0' && data <= '7';
	}

	static bool IsHex(char data)
	{

		return IsNumeric(data) || (Caseless(data) > 96 && Caseless(data) < 103);
	}

	static bool IsAscii(char data)
	{

		return isascii(data) != 0;
	}

	static bool IsControl(char data)
	{

		return data < 32 || data == 127;
	}

	static bool IsPrintable(char data)
	{

		return data > 31 && data < 127;
	}

	static bool IsPunctuation(char data)
	{

		return data > 0 && ispunct(data) != 0;
	}

	static bool IsSymbol(char data)
	{

		return !IsAlphanumeric(data);
	}

	static bool IsDelimiter(char data)
	{

		return data == Tab || data == ',';
	}

	static bool IsIdentifier(char data)
	{

		return data == '_' || IsAlphanumeric(data);
	}

	static bool IsUppercase(char data)
	{
		return ( !(data < 'A') && !(data > 'Z') );
	}

	static bool IsLowercase(char data)
	{
		return ( !(data < 'a') && !(data > 'z') );
	}

	static char Lowercase(char data)
	{

		return Lower[(unsigned char)data];
	}

	static char Uppercase(char data)
	{

		return Upper[(unsigned char)data];
	}

	static int Compare(const char data, const char compare, bool caseless)
	{
		return (caseless)?Lowercase(data)-Lowercase(compare):data-compare;
	}

	static int Compare(const char data, const char compare)
	{
		return data-compare;
	}

	static int CompareCaseless(const char data, const char compare)
	{
		return Caseless(data)-Caseless(compare);
	}

	static bool Equals(const char data, const char compare, bool caseless)
	{
		return (caseless)?Lowercase(data)==Lowercase(compare):data==compare;
	}

	static bool Equals(const char data, const char compare)
	{
		return data==compare;
	}

	static bool EqualsCaseless(const char data, const char compare)
	{
		return Caseless(data)==Caseless(compare);
	}

	static char Normalise(char character, bool caseless)
	{

		return (caseless)?Lowercase(character):character;
	}

    static char * LFind(char * left, int leftSize, char * right, int rightSize, bool caseless=false);
    static char * RFind(char * left, int leftSize, char * right, int rightSize, bool caseless=false);

    static int Compare(char * left, int leftSize, char * right, int rightSize, bool caseless=false);
    static int Compare(char * left, char * right, int size, bool caseless=false);

	static int Compare(const char * left, const char * right, bool caseless) {return (caseless)?CompareCaseless(left,right):Compare(left,right);}
	static int Compare(const char * left, const char * right, const int size, bool caseless) {return (caseless)?CompareCaseless(left,right,size):Compare(left,right,size);}
	static int Compare(const char * left, const int leftSize, const char * right, int rightSize, bool caseless) {return (caseless)?CompareCaseless(left,leftSize,right,rightSize):Compare(left,leftSize,right,rightSize);}

	static int Compare(const char * left, const char * right);
	static int Compare(const char * left, const char * right, const int size);
	static int Compare(const char * left, const int leftSize, const char * right, int rightSize);

	static int CompareCaseless(const char * left, const char * right);
	static int CompareCaseless(const char * left, const char * right, const int size);
	static int CompareCaseless(const char * left, const int leftSize, const char * right, int rightSize);

	static int CompareReverse(const char * left, const char * right, bool caseless){return (caseless)?CompareReverseCaseless(left,right):CompareReverse(left,right);}
	static int CompareReverse(const char * left, const char * right, const int size, bool caseless){return (caseless)?CompareReverseCaseless(left,right,size):CompareReverse(left,right,size);}
	static int CompareReverse(const char * left, const int leftSize, const char * right, int rightSize, bool caseless){return (caseless)?CompareReverseCaseless(left,leftSize,right,rightSize):CompareReverse(left,leftSize,right,rightSize);}

	static int CompareReverse(const char * left, const char * right);
	static int CompareReverse(const char * left, const char * right, const int size);
	static int CompareReverse(const char * left, const int leftSize, const char * right, int rightSize);

	static int CompareReverseCaseless(const char * left, const char * right);
	static int CompareReverseCaseless(const char * left, const char * right, const int size);
	static int CompareReverseCaseless(const char * left, const int leftSize, const char * right, int rightSize);

public:

    static bool Equals(char * left, int leftSize, char * right, int rightSize, bool caseless=false);
    static bool Equals(char * left, char * right, int size, bool caseless=false);

	static bool Equals(const char * left, const char * right, bool caseless){return (caseless)?EqualsCaseless(left,right):Equals(left,right);}
	static bool Equals(const char * left, const char * right, const int size, bool caseless){return (caseless)?EqualsCaseless(left,right,size):Equals(left,right,size);}
	static bool Equals(const char * left, const int leftSize, const char * right, const int rightSize, bool caseless){return (caseless)?EqualsCaseless(left,leftSize,right,rightSize):Equals(left,leftSize,right,rightSize);}

	static bool Equals(const char * left, const char * right);
	static bool Equals(const char * left, const char * right, const int size);
	static bool Equals(const char * left, const int leftSize, const char * right, const int rightSize);

	static bool EqualsCaseless(const char * left, const char * right);
	static bool EqualsCaseless(const char * left, const char * right, const int size);
	static bool EqualsCaseless(const char * left, const int leftSize, const char * right, int rightSize);

	static bool EqualsReverse(const char * left, const char * right, bool caseless){return (caseless)?EqualsReverseCaseless(left,right):EqualsReverse(left,right);}
	static bool EqualsReverse(const char * left, const char * right, const int size, bool caseless){return (caseless)?EqualsReverseCaseless(left,right,size):EqualsReverse(left,right,size);}
	static bool EqualsReverse(const char * left, const int leftSize, const char * right, const int rightSize, bool caseless){return (caseless)?EqualsReverseCaseless(left,leftSize,right,rightSize):EqualsReverse(left,leftSize,right,rightSize);}

	static bool EqualsReverse(const char * left, const char * right);
	static bool EqualsReverse(const char * left, const char * right, const int size);
	static bool EqualsReverse(const char * left, const int leftSize, const char * right, int rightSize);

	static bool EqualsReverseCaseless(const char * left, const char * right);
	static bool EqualsReverseCaseless(const char * left, const char * right, const int size);
	static bool EqualsReverseCaseless(const char * left, const int leftSize, const char * right, int rightSize);

public:

	static char * Find(const char * left, const char * right, bool caseless){return (caseless)?FindCaseless(left,right):Find(left,right);}
	static char * Find(const char * left, const char * right, const int size, bool caseless){return (caseless)?FindCaseless(left,right,size):Find(left,right,size);}
	static char * Find(const char * left, const int leftSize, const char * right, const int rightSize, bool caseless){return (caseless)?FindCaseless(left,leftSize,right,rightSize):Find(left,leftSize,right,rightSize);}

	static char * Find(const char * left, const char * right);
	static char * Find(const char * left, const char * right, const int size);
	static char * Find(const char * left, const int leftSize, const char * right, const int rightSize);

	static char * FindCaseless(const char * left, const char * right);
	static char * FindCaseless(const char * left, const char * right, const int size);
	static char * FindCaseless(const char * left, const int leftSize, const char * right, int rightSize);

	static char * FindReverse(const char * left, const char * right, bool caseless){return (caseless)?FindReverseCaseless(left,right):FindReverse(left,right);}
	static char * FindReverse(const char * left, const char * right, const int size, bool caseless){return (caseless)?FindReverseCaseless(left,right,size):FindReverse(left,right,size);}
	static char * FindReverse(const char * left, const int leftSize, const char * right, int rightSize, bool caseless){return (caseless)?FindReverseCaseless(left,leftSize,right,rightSize):FindReverse(left,leftSize,right,rightSize);}
	static char * FindReverse(const char * left, const char * right);
	static char * FindReverse(const char * left, const char * right, const int size);
	static char * FindReverse(const char * left, const int leftSize, const char * right, int rightSize);

	static char * FindReverseCaseless(const char * left, const char * right);
	static char * FindReverseCaseless(const char * left, const char * right, const int size);
	static char * FindReverseCaseless(const char * left, const int leftSize, const char * right, int rightSize);

public:

	static int Length(const char * data);

	static int Hash(const char * data){return Hash((char*)data,Length(data));}
	static int Hash(char * data, int size);

	static int HashCaseless(const char * data){return HashCaseless((char*)data,Length(data));}
	static int HashCaseless(char * data, int size);

public:

	static long long Binary(const char * data){return Binary((char*)data,Length(data));}
	static long long Binary(char * data,int size);
	static bool Binary(long long & value, const char * data){return Binary(value,(char*)data,Length(data));}
	static bool Binary(long long & value, char * data, int size);	

	static long long Integer(int radix, const char * data){return Integer(radix,(char*)data,Length(data));}
	static long long Integer(int radix, char * data, int size);
	static bool Integer(long long & value, int radix, const char * data){return Integer(value,radix,(char*)data,Length(data));}
	static bool Integer(long long & value, int radix, char * data, int size);	

	static long long Integer(const char * data){return Integer((char*)data,Length(data));}
	static long long Integer(char * data, int size);
	static bool Integer(long long & value, const char * data) {return Integer(value,(char*)data,Length(data));}
	static bool Integer(long long & value, char * data, int size);	

	static double Real(const char * data){return Real((char*)data,Length(data));}
	static double Real(char * data, int size);	
	static bool Real(double & value, const char * data){return Real(value,(char*)data,Length(data));}
	static bool Real(double & value, char * data, int size);	

	static bool Boolean(const char * data){return Boolean((char*)data,Length(data));}
	static bool Boolean(char * data, int size);	
	static bool Boolean(bool & value, const char * data){return Boolean(value,(char*)data,Length(data));}
	static bool Boolean(bool & value, char * data, int size);	

	static long long Octal(const char * data){return Octal((char*)data,Length(data));}
	static long long Octal(char * data, int size);
	static bool Octal(long long & value, const char * data){return Octal(value,(char*)data,Length(data));}
	static bool Octal(long long & value, char * data, int size);	

	static long long Hex(const char * data){return Hex((char*)data,Length(data));}
	static long long Hex(char * data, int size);
	static bool Hex(long long & value, const char * data){return Hex(value,(char*)data,Length(data));}
	static bool Hex(long long & value, char * data, int size);	

	static long long Decimal(const char * data){return Decimal((char*)data,Length(data));}
	static long long Decimal(char * data, int size);
	static bool Decimal(long long & value, const char * data){return Decimal(value,(char*)data,Length(data));}
	static bool Decimal(long long & value, char * data, int size);	

	static bool IsHex(const char * data){return IsHex((char*)data,Length(data));}
	static bool IsHex(char * data, int size);

	static bool IsOctal(const char * data){return IsOctal((char*)data,Length(data));}
	static bool IsOctal(char * data, int size);

	static bool IsDecimal(const char * data){return IsNumeric((char*)data,Length(data));}
	static bool IsDecimal(char * data, int size){return IsNumeric(data,size);}

	static bool IsBinary(const char * data){return IsBinary((char*)data,Length(data));}
	static bool IsBinary(char * data, int size);

	static bool IsProppercase(const char * data){return IsProppercase((char*)data,Length(data));}
	static bool IsProppercase(char * data, int size);

	static bool IsUppercase(const char * data){return IsUppercase((char*)data,Length(data));}
	static bool IsUppercase(char * data, int size);

	static bool IsLowercase(const char * data){return IsLowercase((char*)data,Length(data));}
	static bool IsLowercase(char *data, int size);

	static bool IsAlpha(const char * data){return IsAlpha((char*)data,Length(data));}
	static bool IsAlpha(char * data, int size);

	static bool IsNumeric(const char * data){return IsNumeric((char*)data,Length(data));}
	static bool IsNumeric(char * data, int size);

	static bool IsAlphanumeric(const char * data){return IsAlphanumeric((char*)data,Length(data));}
	static bool IsAlphanumeric(char * data, int size);

	static void Lowercase(const char * data){Lowercase((char*)data,Length(data));}
	static void Lowercase(char * data, int size);

	static void Uppercase(const char * data){Uppercase((char*)data,Length(data));}
	static void Uppercase(char *data, int size);

	static void Proppercase(const char * data){Proppercase((char*)data,Length(data));}
	static void Proppercase(char * data, int size);

public:

	static int Format(char * data, int size, const char * format, ...);
	static int FormatVa(char * data, int size, const char * format, va_list va);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bytes : public Object
{
public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;};

	using Object::InstanceOf;
	virtual bool InstanceOf(Identity &identity)	
	{
		return Identify() == identity;
	}

public:

	static inline void Copy(char * to, char * from, int size)
	{
		if (to+size >= from && to <= from+size) return;
		memcpy(to,from,size);		
	}

	static inline void Move(char * to, char * from, int size)
	{
		memmove(to,from,size);
	}

	static inline int Compare(char * lhs, char * rhs, int size)
	{
		return memcmp(lhs,rhs,size);
	}

	static inline bool Equals(char * lhs, char * rhs, int size)
	{
		return memcmp(lhs,rhs,size) == 0;
	}

public:

    char * Data;
    int Size;

	explicit Bytes():Data(0),Size(0) {}
	explicit Bytes(const char *data):Data((char*)data),Size((int)Length(data)) {}
	explicit Bytes(const Bytes & bytes):Data(bytes.Data),Size(bytes.Size) {}
	explicit Bytes(char *data, int size):Data(data),Size(size) {}

    int Length() const
    {
        return Size;
    }

    static int Length(const char * data) 
    {
        return Characters::Length(data);
    }

    char & At(int index) 
    {
		Indices::Normalise(index,Size);
        Assert(Data != 0);
		Assert(index >= 0 && index <= Size);
        return Data[index];
    }

    char At(int index) const 
    {
		Indices::Normalise(index,Size);
        Assert(Data != 0);
		Assert(index >= 0 && index <= Size);
        return Data[index];
    }

	char & ReferenceAt(int index);
	char * PointerAt(int index);

	int IndexAt(char * data)
	{

		return (data && data >= Data && data < Data+Size)?data-Data:-1;
	}

    virtual Bytes & Clear() 
	{
		Data=0;
		Size=0;
		return *this;
	}

	bool IsEmpty() const 
	{
		Assert(Size >= 0);
		return Data == 0 || Size == 0;
	}

    bool IsNull() const 
	{
		return Data == 0;
	}

    char operator[] (int index) const 
	{
		return At(index);
    }

	char & operator [] (int index)
    {
		return At(index);
    }

	Bytes & operator = (const Bytes & bytes);

	using Printable::Print;
	void Print(String & string);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Substring : public Bytes
{
public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;};

	using Bytes::InstanceOf;
	virtual bool InstanceOf(Identity &identity)
	{

		return Instance == identity || Bytes::Instance == identity;
	}

public:

    Substring() {}
    Substring(const char * data):Bytes((char*)data,Length(data)) {}
    Substring(char * data, int size):Bytes(data,size) {}        
    Substring(const Substring & str):Bytes(str.Data,str.Size) {}    

	Substring(const Substring & str, int from, int to)
	{
        Substring slice = ((Substring&)str).Slice(from,to);
        Data = slice.Data;
        Size = slice.Size;
	}

	Substring(const Substring & str, int from)
	{
        Substring slice = ((Substring&)str).Slice(from);
        Data = slice.Data;
        Size = slice.Size;        
	}

	Substring (const char * data, int from, int to)
	{
        Substring slice = Substring((char*)data, Substring::Length(data)).Slice(from,to);
        Data = slice.Data;
        Size = slice.Size;       
	}

	Substring (const char * data, int from)
	{
        Substring slice = Substring((char*)data, Substring::Length(data)).Slice(from);
        Data = slice.Data;
        Size = slice.Size;
	}

    #ifdef HERO_USING_STD
    Substring(const std::string & str):
		Bytes((char*)str.data(),(int)str.size())
    {}
    #endif

	~Substring()
	{
	}

    virtual Substring & Reset() 
    {

        return Clear();
    }

    virtual Substring & Clear() 
    {
        Data = 0;
        Size = 0;
		return *this;
    }

    virtual Substring & Assign(const char *begin, const char * end)	{return Assign((char*)begin,(int)(end-begin+1));}
	virtual Substring & Assign(const Substring & str)				{return Assign(str.Data,str.Size);}
    virtual Substring & Assign(const char * data)					{return Assign((char*)data,Length(data));}
    virtual Substring & Assign(char * data, int size) 
	{
		Data = data;Size = size;
		return *this;
	}

    virtual Substring & Move(Substring & str);
    virtual Substring & Swap(Substring & str);

    Substring & operator = (const char * data) 
    {
        Data = (char*) data;
        Size = Substring::Length(data);
        return *this;
    }

    Substring & operator = (const Substring & str)
    {
        Data = str.Data;
        Size = str.Size;
        return *this;
    }

	int Hash();	
	int Compare(Object *object, int comparitor=Comparable::COMPARE_GENERAL);
	bool Equals(Object * object, int comparitor=Comparable::COMPARE_GENERAL);

	using Bytes::Print;
	void Print(String & string);

	void Release() {Data=0;Size=0;}
	void Destroy() {Release();}

    bool Is(const Substring & str, bool caseless=false) {return Is(str.Data,str.Size,caseless);} 
	bool Is(const char * data, bool caseless=false) {return Is((char*)data,Length(data),caseless);}
    bool Is(char * data, int size, bool caseless=false);
    bool Is(const char data, bool caseless=false);

	bool IsProppercase()	{return Characters::IsProppercase(Data,Size);}
	bool IsUppercase()		{return Characters::IsUppercase(Data,Size);}
	bool IsLowercase()		{return Characters::IsLowercase(Data,Size);}
	bool IsAlpha()			{return Characters::IsAlpha(Data,Size);}
	bool IsNumeric()		{return Characters::IsNumeric(Data,Size);}
	bool IsAlphanumeric()	{return Characters::IsAlphanumeric(Data,Size);}

	bool IsOctal()			{return Characters::IsOctal(Data,Size);}
	bool IsHex()			{return Characters::IsHex(Data,Size);}
	bool IsDecimal()		{return Characters::IsDecimal(Data,Size);}
	bool IsBinary()			{return Characters::IsBinary(Data,Size);}

	long long Octal()		{return Characters::Octal(Data,Size);}
	long long Hex()			{return Characters::Hex(Data,Size);}
	long long Decimal()		{return Characters::Decimal(Data,Size);}
	long long Binary()		{return Characters::Binary(Data,Size);}

	bool Octal(long long & value)	{return Characters::Octal(value,Data,Size);}
	bool Hex(long long & value)		{return Characters::Hex(value,Data,Size);}
	bool Decimal(long long & value)	{return Characters::Decimal(value,Data,Size);}
	bool Binary(long long & value)	{return Characters::Binary(value,Data,Size);}

	bool Boolean()						{return Characters::Boolean(Data,Size);}
	long long Integer(int radix=10)		{return Characters::Integer(radix,Data,Size);}
	double Real()						{return Characters::Real(Data,Size);}

	bool Boolean(bool & value)						{return Characters::Boolean(value,Data,Size);}
	bool Integer(long long & value,int radix=10)	{return Characters::Integer(value,radix,Data,Size);}
	bool Real(double & value)						{return Characters::Real(value,Data,Size);}

    bool Equals(Substring & str, bool caseless=false) {return Equals(str.Data,str.Size,caseless);}
    bool Equals(const char * data, bool caseless=false) {return Equals((char*)data,Length(data),caseless);}
    bool Equals(const char c, bool caseless=false) {return Equals((char*)&c,1,caseless);}
    bool Equals(char * data, int size, bool caseless=false) 
    {
        if (size != Size) return false;
		return (caseless)?Characters::EqualsCaseless(Data,Size,data,size):Characters::Equals(Data,Size,data,size);
    }

    int Compare(Substring & str, bool caseless=false) {return Compare(str.Data,str.Size,caseless);}
    int Compare(const char * data, bool caseless=false) {return Compare((char*)data,Length(data),caseless);}
    int Compare(const char c, bool caseless=false) {return Compare((char*)&c,1,caseless);}
    int Compare(char * data, int size, bool caseless=false) 
    {
        return (caseless)?Characters::CompareCaseless(Data,Size,data,size):Characters::Compare(Data,Size,data,size);
    }

	bool operator == (const char * data) {return Equals(data);}
	bool operator == (const Substring & str) {return Equals((Substring &)str);}	

    bool operator != (const char * data) {return !Equals(data);}
    bool operator != (const Substring & str) {return !Equals((Substring &)str);}	

	bool operator < (const Substring & str) {return Compare((Substring &)str) < 0;}
	bool operator < (const char * data) {return Compare(data) < 0;}

	bool operator <= (const Substring & str) {return Compare((Substring &)str) <= 0;}
	bool operator <= (const char * data) {return Compare(data) <= 0;}

	bool operator > (const Substring & str) {return Compare((Substring &)str) > 0;}
	bool operator > (const char * data) {return Compare(data) > 0;}

	bool operator >= (const Substring & str) {return Compare((Substring &)str) >= 0;}
	bool operator >= (const char * data) {return Compare(data) >= 0;}   

    friend bool operator == (Substring & lhs, Substring & rhs) {
        if (lhs.Size != rhs.Size) return false;
        return Characters::Equals(lhs.Data,lhs.Size,rhs.Data,rhs.Size);
    }

    friend bool operator < (const Substring & lhs, const Substring & rhs) {
        return ((Substring&)lhs).Compare((Substring&)rhs) < 0;
    }

    virtual Substring & Left(int amount);
    virtual Substring & Right(int amount);   

	Substring & Trim(int amount);
	Substring & Trim(const char * matches = " \t\n\r")		{return Trim((char*)matches,Length(matches));}
    Substring & Trim(char * matches, int matchesSize);

    Substring & LTrim(int amount);
	Substring & LTrim(const char * matches = " \t\n\r")		{return LTrim((char*)matches,Length(matches));}
    Substring & LTrim(char * matches, int matchesSize);

    Substring & RTrim(int amount);
	Substring & RTrim(const char * matches = " \t\n\r")		{return RTrim((char*)matches,Length(matches));}
    Substring & RTrim(char * matches, int matchesSize);

	Substring& Strip(const char* matches=" \t\n\r")			{return Trim((char*)matches,Length(matches));}
	Substring& Strip(char* matches,int matchesSize)			{return Trim(matches,matchesSize);}
	Substring& Strip(int n)									{LStrip(n);RStrip(n);return *this;}

	Substring& LStrip(const char* matches=" \t\n\r")		{return LTrim((char*)matches,Length(matches));}
	Substring& LStrip(char* matches,int matchesSize)		{return LTrim(matches,matchesSize);}
	Substring& LStrip(int n)								{if(n > 0) Left(-n);return *this;}

	Substring& RStrip(const char* matches=" \t\n\r")		{return RTrim((char*)matches,Length(matches));}
	Substring& RStrip(char* matches,int matchesSize)		{return RTrim(matches,matchesSize);};
	Substring& RStrip(int n)								{if(n > 0) Right(-n);return *this;}

    int IndexOf(const Substring & str, bool caseless=false) {return IndexOf(0,str.Data,str.Size,caseless);}
    int IndexOf(const char * data, bool caseless=false) {return IndexOf(0,(char*)data, Length(data),caseless);}
    int IndexOf(char c, bool caseless=false) {return IndexOf(0,&c,1,caseless);}
    int IndexOf(char * data, int size, bool caseless=false) {return IndexOf(0,data,size,caseless);}

    int IndexOf(int from, const Substring & str, bool caseless=false) {return IndexOf(from,str.Data,str.Size,caseless);}
    int IndexOf(int from, const char * data, bool caseless=false) {return IndexOf(from,(char*)data, Length(data),caseless);}
    int IndexOf(int from, char c, bool caseless=false) {return IndexOf(from,&c,1,caseless);}
    int IndexOf(int from, char * data, int size, bool caseless=false);

	int LastIndexOf(int from, const char c, bool caseless = false);
	int LastIndexOf(const char c, bool caseless = false) {return LastIndexOf(Size-1,c,caseless);};

	virtual int LastIndexOf(int from, char * data, int size, bool caseless = false);
	int LastIndexOf(int from, const char * data, bool caseless = false) {return LastIndexOf(from, (char*)data,Length(data),caseless);};
	int LastIndexOf(int from, const Substring & str, bool caseless = false) {return LastIndexOf(from, str.Data,str.Size,caseless);};
	int LastIndexOf(char * data, int size, bool caseless = false) {return LastIndexOf(Size-1, data,size,caseless);};
	int LastIndexOf(const char * data, bool caseless = false) {return LastIndexOf(Size-1, (char*)data,Length(data),caseless);};
	int LastIndexOf(const Substring & str, bool caseless = false) {return LastIndexOf(Size-1, str.Data,str.Size,caseless);};

	int Match(const Substring & str) {return Match(0,str.Data,str.Size);}
	int Match(const char * data) {return Match(0,(char*)data,Length(data));}
	int Match(char * data, int size) {return Match(0,data,size);}
	int Match(Regex & regex) {return Match(0,regex);}
	int Match(int from, const Substring & str) {return Match(from,str.Data,str.Size);}
	int Match(int from, const char * data) {return Match(from,(char*)data,Length(data));}
	int Match(int from, char * data, int size);
	int Match(int from, Regex & regex);

    bool Contains(const Substring & str, bool caseless = false) {return Contains(str.Data,str.Size,caseless);};
    bool Contains(const char *data, bool caseless = false) {return Contains((char*)data,Length(data),caseless);};	
	bool Contains(const char c, bool caseless=false) {return IndexOf(0,(char*)&c,1,caseless) != -1;}
    bool Contains(char * data, int size, bool caseless=false);

    bool StartsWith(const Substring & str, bool caseless=false) {return StartsWith(str.Data,str.Size,caseless);}
    bool StartsWith(const char * data, bool caseless=false) {return StartsWith((char*)data, Length(data),caseless);}
    bool StartsWith(const char c, bool caseless=false) {return StartsWith((char*)&c,1,caseless);}
    bool StartsWith(char * data, int size, bool caseless=false);

    bool EndsWith(const Substring & str, bool caseless=false) {return EndsWith(str.Data,str.Size,caseless);}
    bool EndsWith(const char * data, bool caseless=false) {return EndsWith((char*)data, Length(data),caseless);}
    bool EndsWith(const char c, bool caseless=false) {return EndsWith((char*)&c,1,caseless);}
    bool EndsWith(char * data, int size, bool caseless=false);    

	Substring Before(char data) {return Before(0,&data,1);}
	Substring Before(const char * data) {return Before(0,data);}
	Substring Before(const Substring & str) {return Before(0,str);}
	Substring Before(char * data, int size) {return Before(0,data,size);}
	Substring Before(int from, const char * data) {return Before(from,(char*)data,Length(data));}
	Substring Before(int from, const Substring & str) {return Before(from,str.Data,str.Size);}

	Substring Before(int from, char * data, int size)	
	{
		if (Indices::Normalise(from,Size))
		{

			int index = IndexOf(from,data,size);
			if (index >= 0)
				return Slice(0,index);
		}

		return Substring();
	}	

	Substring Before(int index);

	Substring After(char data) {return After(0,&data,1);}
	Substring After(const char * data) {return After(0,data);}
	Substring After(const Substring & str) {return After(0,str);}
	Substring After(char * data, int size) {return After(0,data,size);}
	Substring After(int from, const char * data) {return After(from,(char*)data,Length(data));}
	Substring After(int from, const Substring & str) {return After(from,str.Data,str.Size);}
	Substring After(int from, char * data, int size);

	Substring After(int index);

	bool IsSubstringOf(const Substring & str) {return IsSubstringOf(str.Data,str.Size);}
	bool IsSubstringOf(char * data, int size)
	{
		return Data >= data && Data+Size <= data+size;
	}

	virtual bool IsSubstring() {return true;}
	bool IsSlice() {return IsSubstring();}

	Substring Slice(int from);
	Substring Slice(int from, int to);

    #ifdef HERO_USING_STD
    std::string Std() const 
    {
        return std::string((char*)Data,(size_t)Size);
    }
    #endif

	Superstring Super();
    String Str();

	Substring Sub(int from) {return Slice(from);}
	Substring Sub(int from, int to) {return Slice(from,to);}

	Substring Substr(int from) {return Slice(from);}
	Substring Substr(int from, int to) {return Slice(from,to);}

    #ifdef HERO_USING_STD
    int Write(std::ostream & stream, int amount=0);

    friend std::ostream & operator << (std::ostream & stream, const Substring & str) 
    {
        ((Substring&)str).Write(stream);        
        return stream;
    }
    #endif

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class String : public Substring 
{
public:

    int Allocated;

    String():Allocated(0) {}
    String(int size, int allocated):Allocated(0) {
        Reserve(allocated);
        Fill(0);
        Size = size;
    }

    String(const char * data):Allocated(0)			{Construct((char *)data,Substring::Length(data));}    
    String(char * data, int size):Allocated(0)		{Construct(data,size);}    
    String(const String & str):Allocated(0)			{Construct(str);}
    String(const Substring & str):Allocated(0)      {Construct(str);}
    String(const Superstring & str):Allocated(0)	{Construct(str);}

    #ifdef HERO_USING_STD
    String(const std::string & str):Allocated(0)    {Construct((char*)str.data(),(int)str.size());}
    #endif

    String(char * data, int size, int allocated):Allocated(0) 
    {
		Construct(data,size,allocated);
    }

    String(String && str):Allocated(0)         
    {
        Data = str.Data;
        Size = str.Size;
        Allocated = str.Allocated;

        str.Data = 0;
        str.Size = 0;
        str.Allocated = 0;
    }

    String(char c):Allocated(0)                     {Construct(c);}
	String(unsigned char c):Allocated(0)			{Construct(c);}
	String(short s):Allocated(0)					{Construct(s);}
	String(unsigned short s):Allocated(0)			{Construct(s);}
    String(int i):Allocated(0)                      {Construct(i);}
    String(unsigned int i):Allocated(0)             {Construct(i);}
    String(long l):Allocated(0)                     {Construct(l);}
    String(unsigned long l):Allocated(0)            {Construct(l);}
    String(long long ll):Allocated(0)               {Construct(ll);}
    String(unsigned long long ll):Allocated(0)      {Construct(ll);}    

	String(float f):Allocated(0)					{Construct(f);}
    String(double d):Allocated(0)                   {Construct(d);}
    String(void * p):Allocated(0)                   {Construct(p);}

	String(int times, const Substring & str):Allocated(0) {Construct(times,str.Data,str.Size);}	
	String(int times, char * data, int size):Allocated(0) {Construct(times,data,size);}

	String(String & string, bool attach):Allocated(0) {(attach)?Attach(string):Construct(string);}

	String(const Substring & str, int from, int to);
	String(const Substring & str, int from);

    ~String() 
    {        
        if (Allocated) 
        {
            Assert(Data != 0);
            delete [] Data;        
        }

        Data = 0;
        Size = 0;        
        Allocated = 0;
    }

	String & Construct(char * data, int size, int allocated) 
    {
        Assert(allocated == 0 || (data != 0 && allocated > 0));
        return Assign(data,size,allocated);    
    }

	String & Construct(const Superstring & str);
	String & Construct(const Substring &str);
	String & Construct(const char * data)			{return Construct((char*)data,Length(data));}
	String & Construct(char * data, int size);

    String & Construct(char c);
	String & Construct(signed char c)		{return Construct((char)c);}
    String & Construct(unsigned char c)		{return Construct((char)c);}

	String & Construct(short i);
	String & Construct(unsigned short i);
	String & Construct(int i);
	String & Construct(unsigned int i);
	String & Construct(long l);
	String & Construct(unsigned long l);
	String & Construct(long long ll);
	String & Construct(unsigned long long ll);
	String & Construct(float f);
	String & Construct(double d);	
    String & Construct(void * p);

	String & Construct();

	String & Construct(int times, const Substring & str) {return Construct(times,str.Data,str.Size);}	
	String & Construct(int times, char * data, int size);

	void Release();	
	void Destroy();	

	String & operator = (const Superstring & str)	{Construct(str);return *this;}
	String & operator = (const Substring & str)		{Construct(str);return *this;}
	String & operator = (const String & str)		{Construct(str);return *this;}	
	String & operator = (const char * data)			{Construct(data);return *this;}
	String & operator = (signed char c)				{Construct(c);return *this;}
	String & operator = (unsigned char c)			{Construct(c);return *this;}
	String & operator = (int i)						{Construct(i);return *this;}
	String & operator = (unsigned int i)			{Construct(i);return *this;}
	String & operator = (long l)					{Construct(l);return *this;}
	String & operator = (unsigned long l)			{Construct(l);return *this;}
	String & operator = (long long ll)				{Construct(ll);return *this;}
	String & operator = (unsigned long long ll)		{Construct(ll);return *this;}
	String & operator = (float f)					{Construct(f);return *this;}
	String & operator = (double d)					{Construct(d);return *this;}
	String & operator = (void * p)					{Construct(p);return *this;}

	#ifdef HERO_USING_STD
    String & operator = (const std::string & str) {Substring sub(str);return operator = (sub);}
    #endif

	String & operator = (String && str) 
	{
		Move(str);
		return *this;
	}

    String & Move(Substring & str);
    virtual String & Move(String & str);

    String & Swap(Substring & str);
    virtual String & Swap(String & str);

	using Substring::Compare;
	int Compare(const String & str) 
	{
		return Characters::Compare(Data,Size,(char*)str.Data,(int)str.Size);
	}

	using Substring::Print;
	char * Print(); 

public:

	virtual float Increment() {return 0.25;}
    virtual int Remaining();

    virtual String & Allocate(int amount=0);

    virtual String& Resize(int amount=0);

    virtual String& Reserve(int amount=0);

    virtual String & Reset() 
    {
		Destroy();
		return *this;
    }

    virtual String & Clear() 
    {
		Release();
		return *this;     
    }

    virtual String & Term() 
    {

		Assert((Allocated == 0 || Allocated > Size) && Size >= 0);
		if (Data && Allocated > Size && Size >= 0) Data[Size]=0;
		return *this;
    }

    virtual String & Left(int amount);
    virtual String & Right(int amount);

    String & Fill(char data=0);

public:

    String & Assign(const Substring & str)	{return Assign(str.Data,str.Size);}
    String & Assign(const char * data)		{return Assign((char*)data,Length(data));}
    String & Assign(char * data, int size);

    virtual String & Assign(char * data, int size, int allocated);
    virtual String & Assign(String & str, bool move);

	String & Attach(char *& data, int & size, int & allocated);
	String & Attach(char *& data, int & size);
	String & Attach(const String & str);
	String & Attach(const Substring & str);

	char * Detach();

public:

	String & Append(const Superstring & str);	
    String & Append(const Substring & str)		{return Append((char*)str.Data,str.Size);}
    String & Append(const char * data)			{return Append((char*)data,Length(data));}
    String & Append(char * data, int size)		{return Insert(data,size,Size);}

    String & Append(char c)                     {return Insert(c,Size);}
	String & Append(short s)					{return Insert(s,Size);}
	String & Append(unsigned short s)			{return Insert(s,Size);}	
    String & Append(int i)                      {return Insert(i,Size);}
    String & Append(unsigned int i)             {return Insert(i,Size);}
    String & Append(long l)                     {return Insert(l,Size);}
    String & Append(unsigned long l)            {return Insert(l,Size);}
    String & Append(long long ll)               {return Insert(ll,Size);}
    String & Append(unsigned long long ll)      {return Insert(ll,Size);}        
    String & Append(double d)                   {return Insert(d,Size);}
	String & Append(float f)					{return Insert(f,Size);}
    String & Append(const void * p)             {return Insert(p,Size);}

    #ifdef HERO_USING_STD
    String & Append(const std::string & str)	{return Append((char*)str.data(),(int)str.size());}
    #endif

	String & Prepend(const Superstring & str);
    String & Prepend(const Substring & str)     {return Prepend((char*)str.Data,str.Size);}
    String & Prepend(const char * data)         {return Prepend((char*)data,Length(data));}
    String & Prepend(char * data, int size)     {return Insert(data,size,0);}

    String & Prepend(char c)                    {return Insert(c,0);}
	String & Prepend(short s)					{return Insert(s,0);}
	String & Prepend(unsigned short s)			{return Insert(s,0);}
    String & Prepend(int i)                     {return Insert(i,0);}
    String & Prepend(unsigned int i)            {return Insert(i,0);}
    String & Prepend(long l)                    {return Insert(l,0);}
    String & Prepend(unsigned long l)           {return Insert(l,0);}
    String & Prepend(long long ll)              {return Insert(ll,0);}
    String & Prepend(unsigned long long ll)     {return Insert(ll,0);}        
    String & Prepend(double d)                  {return Insert(d,0);}
	String & Prepend(float f)					{return Insert(f,0);}
    String & Prepend(const void * p)            {return Insert(p,0);}

    #ifdef HERO_USING_STD
    String & Prepend(const std::string & str)   {return Prepend((char*)str.data(),(int)str.size());}
    #endif

    String & Insert(const Substring & str, int index)		{return Insert((char*)str.Data,str.Size,index);}
    String & Insert(const char * data, int index)			{return Insert((char*)data,Length(data),index);}
    String & Insert(char * data, int size, int index);

    String & Insert(char c, int index)                  {return Insert(&c,1,index);}
	String & Insert(short s, int index)					{return Insert((int)s,index);}
	String & Insert(unsigned short s, int index)		{return Insert((unsigned int)s,index);}
    String & Insert(int i, int index);
    String & Insert(unsigned int i, int index);
    String & Insert(long l, int index);
    String & Insert(unsigned long l, int index);
    String & Insert(long long ll, int index);
    String & Insert(unsigned long long ll, int index);
    String & Insert(float f, int index);
	String & Insert(double d, int index);
    String & Insert(const void * p, int index);

    #ifdef HERO_USING_STD
    String & Insert(const std::string & str, int index)		{return Insert((char*)str.data(),(int)str.size(),index);}
    #endif

	String & Replace(const Superstring & str)	{return Construct(str);}
	String & Replace(const Substring & str)		{return Replace(str.Data,str.Size);}
    String & Replace(const char * data)			{return Replace((char*)data, Length(data));}
    String & Replace(char * data, int size); 

    String & Replace(char c);
	String & Replace(short s)					{return Replace((int)s);}
	String & Replace(unsigned short s)			{return Replace((unsigned int)s);}
    String & Replace(int i);
    String & Replace(unsigned int i);
    String & Replace(long l);
    String & Replace(unsigned long l);
    String & Replace(long long ll);
    String & Replace(unsigned long long ll);
    String & Replace(double d);
	String & Replace(float f);
    String & Replace(const void * p);

    String & Replace(int from, int to, const Substring & str)   {return Replace(from,to,str.Data,str.Size);}
    String & Replace(int from, int to, const char * data)       {return Replace(from,to,(char*)data,Length(data));}
    String & Replace(int from, int to, char * data, int size);

    String & Replace(int from, int to, char c) {return Replace(from,to,&c,1);}

    String & Replace(char match, char with)                                 {return Replace(0,match,with);}
    String & Replace(const Substring & match, const Substring & with)       {return Replace(0,match.Data,match.Size,with.Data,with.Size);}
    String & Replace(const Substring & match, const char * with)            {return Replace(0,match.Data,match.Size,(char*)with,Length(with));}
    String & Replace(const char * match, const char * with)                 {return Replace(0,(char*)match,Length(match),(char*)with,Length(with));}
    String & Replace(const char * match, const Substring & with)            {return Replace(0,(char*)match,Length(match),with.Data,with.Size);}
    String & Replace(char * match, int matchSize, char *with, int withSize) {return Replace(0,match,matchSize,with,withSize);} 

    String & Replace(int limit, char match, char with);	
    String & Replace(int limit, const Substring & match, const Substring & with)    {return Replace(limit,match.Data,match.Size,with.Data,with.Size);}
    String & Replace(int limit, const Substring & match, const char * with)         {return Replace(limit,match.Data,match.Size,(char*)with,Length(with));}
    String & Replace(int limit, const char * match, const char * with)              {return Replace(limit,(char*)match,Length(match),(char*)with,Length(with));}
    String & Replace(int limit, const char * match, const Substring & with)         {return Replace(limit,(char*)match,Length(match),with.Data,with.Size);}
    String & Replace(int limit, char * match, int matchSize, char *with, int withSize);

	String & Replace(Regex & regex, const Substring & str)		{return Replace(0,regex,str.Data,str.Size);}
	String & Replace(Regex & regex, const char * data)			{return Replace(0,regex,(char*)data,String::Length(data));}
	String & Replace(Regex & regex, char * data, int size)		{return Replace(0,regex,data,size);}

	String & Replace(int limit, Regex & regex, const Substring & str)	{return Replace(regex,str.Data,str.Size);}
	String & Replace(int limit, Regex & regex, const char * data)		{return Replace(regex,(char*)data,String::Length(data));}
	String & Replace(int limit, Regex & regex, char * data, int size);

	String & Translate(const Substring & matches, const Substring & replacements)	{return Translate(matches.Data,matches.Size,replacements.Data,replacements.Size);}
	String & Translate(const char * matches, const char *replacements)				{return Translate((char*)matches,Length(matches),(char*)replacements,Length(replacements));}
	String & Translate(char * matches, int matchesSize, char * replacements, int replacementsSize);

	String & Trim(int amount);
	String & Trim(const char * matches = " \t\n\r") {return Trim((char*)matches,Length(matches));}
	String & Trim(char * matches, int matchesSize);

	String & RTrim(int amount);
	String & RTrim(const char* matches = " \t\n\r") { return RTrim((char*)matches,Length(matches)); }
	String & RTrim(char * matches, int matchesSize);

	String & LTrim(int amount);
	String & LTrim(const char * matches = " \t\n\r") {return LTrim((char*)matches,Length(matches));}	
	String & LTrim(char * matches, int matchesSize);

	String & Upper();
	String & Lower();
	String & Propper();

	String & Capitalize() {}

	String & Reverse();

public:

    String & FormatVa(const char * fmt, va_list va);
    String & FormatVa(int index, const char * fmt, va_list va);

    String & Format(const char * fmt, ...);
    String & Format(int index, const char * fmt, ...);

    static String Formatted(const char * format, ...);
    static String Formatted(const char * format, va_list va);

    template<typename _Value_>
    String & FormatValue(int index, const char * fmt, _Value_ value) 
	{
        int amount = (int)Allocated-(int)(Size+1);
        if (amount < 32)
            Reserve(32);

        return Format(index,fmt,value);
    }

	String & FormatValue(int index, const char * fmt, float value) 
	{
        int amount = (int)Allocated-(int)(Size+1);
        if (amount < 32)
            Reserve(64);

        return Format(index,fmt,value);
    }

    String & FormatValue(int index, const char * fmt, double value) 
	{
        int amount = (int)Allocated-(int)(Size+1);
        if (amount < 32)
            Reserve(512);

        return Format(index,fmt,value);
    }

public:

    using Substring::operator ==;
    bool operator == (const String & str) {
        if (str.Size != Size) return false;
        return Characters::Equals((char*)Data,(int)Size,(char*)str.Data,(int)str.Size);
    }

    friend bool operator == (String & lhs, String & rhs) {
        if (lhs.Size != rhs.Size) return false;
        return Characters::Equals(lhs.Data,lhs.Size,rhs.Data,rhs.Size);
    }

    friend bool operator < (const String & lhs, const String & rhs) {
        return ((String&)lhs).Compare((String&)rhs) < 0;
    }

    friend bool operator > (const String & lhs, const String & rhs) {
        return ((String&)lhs).Compare((String&)rhs) > 0;
    }

	String & operator += (const Superstring & str)	{return Append(str);}
	String & operator += (const Substring & str)	{return Append(str);}
    String & operator += (const char * data)        {return Append(data);}
	String & operator += (char c)					{return Append(c);}
    String & operator += (short s)                  {return Append(s);}
	String & operator += (unsigned short s)         {return Append(s);}
    String & operator += (int i)                    {return Append(i);}
    String & operator += (unsigned int i)           {return Append(i);}
    String & operator += (long l)                   {return Append(l);}
    String & operator += (unsigned long l)          {return Append(l);}
    String & operator += (long long ll)             {return Append(ll);}
    String & operator += (unsigned long long ll)    {return Append(ll);}
    String & operator += (float f)					{return Append(f);}
	String & operator += (double d)                 {return Append(d);}	
    String & operator += (const void * p)           {return Append(p);}

    #ifdef HERO_USING_STD
    String & operator += (const std::string & str)  {return Append(str);}
    #endif

	String & operator *= (int times);

    String & operator << (const Substring & str)    {return Append(str);}
    String & operator << (const char * data)        {return Append(data);}
	String & operator << (char c)                   {return Append(c);}
    String & operator << (short s)                  {return Append(s);}
	String & operator << (unsigned short s)         {return Append(s);}
    String & operator << (int i)                    {return Append(i);}
    String & operator << (unsigned int i)           {return Append(i);}
    String & operator << (long l)                   {return Append(l);}
    String & operator << (unsigned long l)          {return Append(l);}
    String & operator << (long long ll)             {return Append(ll);}
    String & operator << (unsigned long long ll)    {return Append(ll);}
	String & operator << (float f)                  {return Append(f);}
    String & operator << (double d)                 {return Append(d);}
    String & operator << (const void * p)           {return Append(p);}

    #ifdef HERO_USING_STD
    String & operator << (const std::string & str)  {return Append(str);}
    #endif

    String & operator >> (const Substring & str)    {return Prepend(str);}
    String & operator >> (const char * data)        {return Prepend(data);}
	String & operator >> (char c)                   {return Prepend(c);}
    String & operator >> (short s)                  {return Prepend(s);}
	String & operator >> (unsigned short s)         {return Prepend(s);}
    String & operator >> (int i)                    {return Prepend(i);}
    String & operator >> (unsigned int i)           {return Prepend(i);}
    String & operator >> (long l)                   {return Prepend(l);}
    String & operator >> (unsigned long l)          {return Prepend(l);}
    String & operator >> (long long ll)             {return Prepend(ll);}
    String & operator >> (unsigned long long ll)    {return Prepend(ll);}
	String & operator >> (float f)                  {return Prepend(f);}
    String & operator >> (double d)                 {return Prepend(d);}
    String & operator >> (const void * p)           {return Prepend(p);}

    #ifdef HERO_USING_STD
    String & operator >> (const std::string & str)  {return Prepend(str);}
    #endif

    #ifdef HERO_USING_STD
    typedef std::ostream & (*ManipFunc) (std::ostream &);
    String & operator << (ManipFunc func);

    int Read(std::istream & stream, int amount=0);
    #endif

	virtual bool IsAllocated() {return Allocated!=0;};
	virtual bool IsSubstring() {return Allocated==0;}

	String Str() {return String(Data,Size);}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Superstring : public String
{
public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;};

	virtual bool InstanceOf(Identity &identity)
	{

		return Instance == identity || Substring::InstanceOf(identity);
	}

public:

	Superstring(const Superstring & left, const Superstring & right)
	{
		Attach((String&)left);
		Append(right);
	}

	Superstring(const Superstring & left, const Substring & right)
	{
		Attach((String&)left);
		Append(right);
	}

	Superstring(const Superstring & left, const char * right)
	{
		Attach((String&)left);
		Append(right);
	}

	Superstring(const Substring & left, const Superstring & right)
	{
		Attach((String&)right);
		Prepend(left);
	}

	Superstring(const Substring & left, const Substring & right)
	{
		Allocate(left.Size+right.Size);
		Append(left);
		Append(right);
	}

	Superstring(const Substring & left, const char * right)
	{
		Allocate(left.Size+Length(right));
		Append(left);
		Append(right);
	}

	Superstring(const char * left, const Superstring & right)
	{
		Attach((String&)right);
		Prepend(left);
	}

	Superstring(const char * left, const Substring & right)
	{
		Allocate(Length(left)+right.Size);
		Append(left);
		Append(right);
	}

	Superstring(const Substring & str, int from, int to):
		String((char*)str.Data+(int)Min(str.Size-1,from),Min(str.Size,to-from))
	{
	}

	Superstring(const Substring & str, int from):
		String((char*)str.Data+(int)Min(str.Size-1,from),str.Size-Min(str.Size,from))
	{
	}

	Superstring(const Superstring & superstring)
	{
		Attach((String&)superstring);
	}

	Superstring(const Substring & str):String(str)
	{

	}

	Superstring(const String & string):
		String(string)
	{
	}

	Superstring(const char * data):
		String(data)
	{
	}

	Superstring(char *data, int size):
		String(data,size)
	{
	}

	Superstring()
	{
	}

	~Superstring()
	{

	}

public:

	static Superstring Partial(const String & string);
	static Superstring Partial(const Superstring & superstring);
	static Superstring Partial(const Substring & str) {return Partial(str.Data,str.Size);}
	static Superstring Partial(const char * data) {return Partial((char*)data,Length(data));}
	static Superstring Partial(char * data, int size);

	static Superstring Literal(const String & string);
	static Superstring Literal(const Superstring & superstring);
	static Superstring Literal(const Substring & str);
	static Superstring Literal(const char * data) {return Literal((char*)data,Length(data));}
	static Superstring Literal(char * data, int size);

	static Superstring Actual(const Superstring & superstring) {return superstring;}
	static Superstring Actual(const Substring & str) {return Superstring(str);}
	static Superstring Actual(const char * data) {return Superstring(data);}
	static Superstring Actual(char * data, int size) {return Superstring(data,size);}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Superstring operator + (const Superstring & left, const Superstring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Superstring & left, const String & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Superstring & left, const Substring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Superstring & left, const char * right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const char * left, const Superstring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const char * left, const String & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const char * left, const Substring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const String & left, const Superstring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const String & left, const String & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const String & left, const Substring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const String & left, const char * right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Substring & left, const Superstring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Substring & left, const String & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Substring & left, const Substring & right)
{
	return Superstring(left,right);
};

inline Superstring operator + (const Substring & left, const char * right)
{
	return Superstring(left,right);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Superstring operator * (const Substring & sequence, int times)
{
	Superstring mul = sequence;
	return mul *= times;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Stackstring
{
public:

    const static int Stacksize=256;

    union Storage {
        char Stack[Stacksize];
        char * Ptr;

        Storage():Ptr(0) {}

        int Size() {
            return ((Stackstring*) ((char*)this-(char*)&((Stackstring*)0)->Data))->Size;
        }

        operator char * () {
            return (char*) (Size() > (Stacksize-1)?Ptr:Stack);
        }

        char & operator[] (int index) {
            return (Size() > (Stacksize-1)?Ptr[index]:Stack[index]);
        }

    } Data;

    int Size;

    Stackstring():Size(0) {}
    Stackstring(const Substring & str):Size(0) {Assign(str.Data,str.Size);}
	Stackstring(const String & str):Size(0) {Assign(str.Data,str.Size);}
	Stackstring(const char * data):Size(0) {Assign((char*)data,String::Length(data));}
    Stackstring(char * data, int size):Size(0) {Assign(data,size);}
    Stackstring(char c):Size(0) {Assign(&c,1);}
    ~Stackstring() {Reset();}

	void Construct(const String & string)		{Construct(string.Data,string.Size);}
	void Construct(const Bytes &bytes)			{Construct(bytes.Data,bytes.Size);}
	void Construct(const char * data)			{Construct((char*)data,String::Length(data));}
	void Construct(char * data, int size)		{Assign(data,size);}

    bool IsEmpty() {return Size == 0;}

    operator Substring () {
        return Substring((char*)Data,Size);
    }

    void Assign(const Substring & str) {Assign(str.Data,str.Size);}
    void Assign(char * data, int size) {
        Reset();

        if (size > (Stacksize-1)) {
            Data.Ptr = new char[size+1];
            memcpy(Data.Ptr,data,size);
            Data.Ptr[size] = 0;
        }
        else {
            memcpy(Data.Stack,data,size);
            Data.Stack[size] = 0;
        }

        Size = size;
    }

    void Reset() {
        if (Size > (Stacksize-1)) {
            Assert(Data.Ptr);
            delete [] Data.Ptr;
            Data.Ptr = 0;
        }

        Clear();
    }

    void Clear() {
        Size = 0;
        Term();
    }

	void Destroy() {Reset();}
	void Release() {Clear();}

    Stackstring & Term() {
        if (Size > (Stacksize-1))
            Data.Ptr[Size] = 0;
        else
            Data.Stack[Size] = 0;

        return *this;
    }

    Stackstring & operator = (const Substring & str) {Assign(str.Data,str.Size);return *this;}
    Stackstring & operator = (const char * data) {Assign((char*)data,Substring::Length(data));return *this;}
    Stackstring & operator = (char c) {Assign(&c,1);return *this;}

    bool operator == (const char * data)
    {
        int size = Substring::Length(data);
        if (size != Size) return false;
        return Characters::Equals((char*)Data,Size,(char*)data,size);
    }

    bool operator == (const Substring & str) {
        if (str.Size != Size) return false;
        return Characters::Equals((char*)Data,Size,str.Data,str.Size);
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SubstringStream : public Substring, public Stream
{
public:

	SubstringStream(const String & str):Substring(str) {}	
	SubstringStream(const Substring & str):Substring(str) {}
	SubstringStream(const Substring & str, int from, int to):Substring(str,from,to) {}
	SubstringStream(const Substring & str, int from):Substring(str,from) {}
	SubstringStream(const char * data):Substring(data) {}
	SubstringStream(char *data, int size):Substring(data,size) {}
	SubstringStream() {}
	~SubstringStream() {}

	SubstringStream & operator = (const char *data)			{Assign((char*)data,String::Length(data));return *this;}
	SubstringStream & operator = (const Substring & str)	{Assign(str.Data,str.Size);return *this;}

	void Release()
	{
		Substring::Release();
		Position=0;
	}

	int Seek(int position, int origin = -1)	
	{
		if (origin < 0)
		{

			if (position < 0) return Position;

			Position = position;
		}
		else
		if (origin == 0)
		{

			if (position == 0 || Position+position < 0) return Position;

			Position += position;
		}
		else
		if (origin > 0)
		{

			if (position+Size < 0) return Position;			

			Position = position+Size;
		}

		return Position;
	}

	bool IsReadable()					
	{
		return !IsEmpty() && Position < Size;
	}

	using Stream::Read;

	int Read(char &data)				
	{
		if (Position < Size)
		{
			data = Data[Position++];
			return 1;
		}

		return 0;
	}

	int Read(char * data, int size)		
	{
		if (Position+size <= Size)
		{
			memcpy(data,Data+Position,size);
			Position+=size;
			return size;
		}
		else
		{
			int remaining = Size - Position;
			if (remaining > 0)
			{
				memcpy(data,Data+Position,remaining);
				Position += remaining;
				return remaining;
			}
		}

		return 0;
	}

	bool IsWriteable()					
	{
		return Position < Size;
	}

	using Stream::Write;

	int Write(const char &data)				
	{
		if (Position+1 > Size) return 0;
		Data[Position++] = data;
		return 1;
	}

	int Write(char * data , int size)	
	{
		if (Position+size > Size)
		{

			int remaining = Size - Position;
			size = remaining;
		}

		memcpy(Data+Position,data,size);
		Position+=size;
		return size;
	}

	int Write(const char * data)
	{
		return Write((char*)data,(int)strlen(data));
	}

public:

	class StreamAdapter : public Stream
	{
	public:

		class Substring & Substring;

		StreamAdapter(class Substring &str):Substring(str){};
		~StreamAdapter(){};

		int Seek(int position, int origin = -1)	
		{
			if (origin < 0)
			{

				if (position < 0) return Position;
				Position = position;
			}
			else
			if (origin == 0)
			{

				if (position == 0 || Position+position < 0) return Position;
				Position += position;
			}
			else
			if (origin > 0)
			{

				if (Position+position < 0) return Position;
				Position = Substring.Size-position;
			}

			return Position;
		}

		bool IsReadable()					
		{
			return !Substring.IsEmpty() && Position < Substring.Size;
		}

		using Stream::Read;
		int Read(char &data)				
		{
			if (Position < Substring.Size)
			{
				data = Substring.Data[Position++];
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int Read(char * data, int size)		
		{
			if (Position+size <= Substring.Size)
			{
				memcpy(data,Substring.Data+Position,size);
				Position+=size;
				return size;
			}
			else
			{
				int remaining = Substring.Size - Position;
				memcpy(data,Substring.Data+Position,remaining);
				Position += remaining;
				return remaining;
			}
		}

		bool IsWriteable()					
		{
			return Position < Substring.Size;
		}

		using Stream::Write;
		int Write(char &data)				
		{
			if (Position+1 > Substring.Size) return 0;
			Substring.Data[Position++] = data;
			return 1;
		}

		int Write(char * data , int size)	
		{
			if (Position+size > Substring.Size)
			{
				int remaining = Substring.Size - Position;
				size = remaining;
			}

			memcpy(Substring.Data+Position,data,size);
			Position+=size;
			return size;
		}

	};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StringStream : public String, public Stream
{
public:

	StringStream & operator = (const StringStream & str)	{Clear();Replace(str.Data,str.Size);return *this;}
	StringStream & operator = (const String & str)			{Clear();Replace(str.Data,str.Size);return *this;}
	StringStream & operator = (const Substring & str)		{Clear();Replace(str.Data,str.Size);return *this;}
	StringStream & operator = (const char * data)			{Clear();Replace(data);return *this;}
	StringStream & operator = (char c)						{Clear();Replace(c);return *this;}
	StringStream & operator = (int i)						{Clear();Replace(i);return *this;}
	StringStream & operator = (float f)						{Clear();Replace(f);return *this;}
	StringStream & operator = (double d)					{Clear();Replace(d);return *this;}

	StringStream(const String & str):String(str) {}
	StringStream(const Substring & str):String(str) {}
	StringStream(const char * data):String(data) {}
	StringStream(char * data, int size): String(data,size) {}
	StringStream(char c):String(c) {}
	StringStream(int i):String(i) {}
	StringStream(float f):String(f) {}
	StringStream(double d):String(d) {}
	StringStream() {}
	~StringStream() {}

	StringStream & Clear()
	{
		String::Clear();
		Position=0;
        return *this;
	}

	int Seek(int position, int origin = -1)	
	{
		if (origin < 0)
		{

			if (position < 0) return Position;

			Position = position;
		}
		else
		if (origin == 0)
		{

			if (position == 0 || Position+position < 0) return Position;

			Position += position;
		}
		else
		if (origin > 0)
		{

			if (position+Size < 0) return Position;			

			Position = position+Size;
		}

		return Position;
	}

	bool IsReadable()					
	{
		return !IsEmpty() && Position < Size;
	}

	using Stream::Read;

	int Read(char &data)				
	{
		if (Position < Size)
		{
			data = Data[Position++];
			return 1;
		}

		return 0;
	}

	int Read(char * data, int size)		
	{
		if (Position+size <= Size)
		{
			memcpy(data,Data+Position,size);
			Position+=size;
			return size;
		}
		else
		{
			int remaining = Size - Position;
			if (remaining > 0)
			{
				memcpy(data,Data+Position,remaining);
				Position += remaining;
				return remaining;
			}
		}

		return 0;
	}

	bool IsWriteable()					
	{
		return true;
	}

	using Stream::Write;

	int Write(const char &data)				
	{
		if (Position+1 > Size)
			Right((Position-Size)+1);

		Data[Position++] = data;
		Term();
		return 1;
	}

	int Write(char * data , int size)	
	{
		if (Position+size > Size)
		{

			int remaining = Size - Position;
			Right(size-remaining);
		}

		memcpy(Data+Position,data,size);
		Position+=size;
		return size;
	}

	int Write(const char * data)
	{
		return Write((char*)data,Substring::Length(data));
	}

public:

	class StreamAdapter : public Stream
	{
	public:

		class String * String;

		StreamAdapter(class String & str):String(&str){};
		StreamAdapter(class String * str):String(str){};
		~StreamAdapter(){};

		int Seek(int position, int origin = -1)	
		{
			if (origin < 0)
			{

				if (position < 0) return Position;
				Position = position;
			}
			else
			if (origin == 0)
			{

				if (position == 0 || Position+position < 0) return Position;
				Position += position;
			}
			else
			if (origin > 0)
			{

				if (Position+position < 0) return Position;
				Position = String->Size-position;
			}

			return Position;
		}

		bool IsReadable()					
		{
			return !String->IsEmpty() && Position < String->Size;
		}

		using Stream::Read;
		int Read(char &data)				
		{
			if (Position < String->Size)
			{
				data = String->Data[Position++];
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int Read(char * data, int size)		
		{
			if (Position+size <= String->Size)
			{
				memcpy(data,String->Data+Position,size);
				Position+=size;
				return size;
			}
			else
			{
				int remaining = String->Size - Position;
				memcpy(data,String->Data+Position,remaining);
				Position += remaining;
				return remaining;
			}
		}

		bool IsWriteable()					
		{
			return true;
		}

		using Stream::Write;
		int Write(const char &data)				
		{
			if (Position+1 > String->Size)
				String->Right((Position-String->Size)+1);

			String->Data[Position++] = data;
			String->Term();
			return 1;
		}

		int Write(char * data , int size)	
		{
			if (Position+size > String->Size)
			{
				int remaining = String->Size - Position;
				String->Right(size-remaining);
			}

			memcpy(String->Data+Position,data,size);
			Position+=size;
			return size;
		}

	};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Printer
{

public:

	operator char * (void);

	static int Remaining();
	static char * Load(Substring &str);
	static char * Load(const char *data, const int size);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef Print

void Print(const char *, ...);
void PrintLn(const char *, ...);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

using namespace Hero;

inline void PrintArgFormatStr(String&) {}

template <typename _Kind_> 
void PrintArgFormatStr(String& str, _Kind_ arg)
{
    str.Append(arg);
}

template <typename _Kind_, typename... _Args_> 
void PrintArgFormatStr(String & str, _Kind_ arg, _Args_... args)
{
    str.Append(arg);
    PrintArgFormatStr(str, args...);
}

template <typename... Args> 
String PrintArgFormat(Args... args)
{
    String str;
    PrintArgFormatStr(str,args...);

    return String().Move(str);
}

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HERO_HANDLE_STDOUT  stdout
#define HERO_HANDLE_STDERR  stderr
#define HERO_HANDLE_STDIN   stdin

void HandleStdout(int level, const char *line);
void HandleStderr(int level, const char *line);

static void (*HandlerPrint)(int level, const char * line) = HandleStderr;

void SetHandlePrint(int level, void (*Handler)(int level, const char *line));

template <typename... _Args_> 
inline void PrintArg(_Args_...args) 
{
    String line = PrintArgFormat(args...);
    HandlerPrint(0,line.Data);

}

template <typename... _Args_> 
inline void PrintArgLn(_Args_...args) 
{
    String line = PrintArgFormat(args...);
	line.Append("\n");
    HandlerPrint(0,line.Data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_USING_STD

namespace std {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PrintStream 
{
  public:

    std::ostringstream Stream;

    typedef std::ostream& (*ManipFunc)(std::ostream&);

    PrintStream() {}

    ~PrintStream()
    {

        ::PrintArg(Stream.str());
    }

    template <typename T> PrintStream& operator<<(const T& any)
    {

        ::PrintArg(any);
        return *this;
    }

    PrintStream& operator<<(ManipFunc func)
    {

        if (func == (ManipFunc)std::endl) {
            ::Print("\n");

        } else {

        }

        return *this;
    }
};

static PrintStream print;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX
#pragma clang diagnostic pop

#endif
