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




#pragma warning(disable: 4293)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>














#include "hero/string.h"
#include "hero/parser.h"
#include "hero/structure.h"
#include "hero/number.h"






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



class Formatter : public String
{
public:

	struct Field : public Flags<32>
	{

		
		
		
		

		
		

		enum FieldOptions
		{
			TYPE_FORMAT_c	=(1),
			TYPE_FORMAT_C	=(1)<<1,
			TYPE_FORMAT_d	=(1)<<2,
			TYPE_FORMAT_i	=(1)<<3,
			TYPE_FORMAT_o	=(1)<<4,
			TYPE_FORMAT_u	=(1)<<5,
			TYPE_FORMAT_x	=(1)<<6,
			TYPE_FORMAT_X	=(1)<<7,
			TYPE_FORMAT_e	=(1)<<8,
			TYPE_FORMAT_E	=(1)<<9,
			TYPE_FORMAT_f	=(1)<<10,
			TYPE_FORMAT_g	=(1)<<11,
			TYPE_FORMAT_G	=(1)<<12,
			
			
			TYPE_FORMAT_n	=(1)<<15,
			TYPE_FORMAT_p	=(1)<<16,
			TYPE_FORMAT_s	=(1)<<17,
			TYPE_FORMAT_S	=(1)<<18,
			TYPE_FORMAT		= TYPE_FORMAT_c|TYPE_FORMAT_C|TYPE_FORMAT_d|
			TYPE_FORMAT_i|TYPE_FORMAT_o|TYPE_FORMAT_u|TYPE_FORMAT_x|
			TYPE_FORMAT_X|TYPE_FORMAT_e|TYPE_FORMAT_E|TYPE_FORMAT_f|
			TYPE_FORMAT_g|TYPE_FORMAT_G|TYPE_FORMAT_n|TYPE_FORMAT_p|
			TYPE_FORMAT_s|TYPE_FORMAT_S,

			
			
			

			
			
			

			TYPE_PERCENT	=(1)<<20,

			MODIFIER_h		=(1)<<21,
			MODIFIER_l		=(1)<<22,
			MODIFIER_ll		=(1)<<23,
			MODIFIER_L		=(1)<<24,

			FLAG_LEFT		=(1)<<25,	
			FLAG_PLUS		=(1)<<26,	
			FLAG_SPACE		=(1)<<27,	
			FLAG_SPECIAL	=(1)<<28,	
			FLAG_PADDED		=(1)<<29,	
			FLAG_LARGE		=(1)<<30,
			FLAG_SIGNED		=(1)<<31,
		};

		
		
		
		short Index;
		short Width;
		short Precision;
		Substring Format;

		Field():Index(0),Width(0),Precision(0) {}
		Field(int options):Flags<32>(options),Index(0),Width(0),Precision(0) {}
	};

	struct Arg : public Flags<32>
	{
		
		
		
		enum ArgOptions
		{
			TYPE_CHAR		=(1),
			TYPE_INT		=(1)<<1,
			TYPE_LONG		=(1)<<2,
			TYPE_LONGLONG	=(1)<<3,
			TYPE_VOID		=(1)<<4,
			TYPE_DOUBLE		=(1)<<5,
			FLAG_UNSIGNED	=(1)<<6,
			FLAG_SIGNED		=(1)<<7,
			FLAG_POINTER	=(1)<<8,

			FIELD_FORMAT		=(1)<<10,
			FIELD_WIDTH			=(1)<<11,
			FIELD_PRECISION		=(1)<<12,
		};

		
		
		
		
		
		
		
		
		
		struct ArgKind
		{
			ArgKind():Value(0),Pointer(0) {}
			ArgKind(char value, char * pointer):Value(value),Pointer(pointer) {}
			ArgKind(int value, char * pointer):Value(value),Pointer(pointer) {}
			ArgKind(long value, char * pointer):Value(value),Pointer(pointer) {}
			ArgKind(long long value, char * pointer):Value(value),Pointer(pointer) {}
			ArgKind(double value, char * pointer):Value(value),Pointer(pointer) {}
			ArgKind(void * value, char * pointer):Value(value),Pointer(pointer) {}

			union ArgValue
			{
				ArgValue(char value):Char(value) {}
				ArgValue(int value):Int(value) {}
				ArgValue(long value):Long(value) {}
				ArgValue(long long value):LongLong(value) {}
				ArgValue(double value):Double(value) {}
				ArgValue(void * value):Void(value) {}

				char Char;
				int Int;
				long Long;
				long long LongLong;
				double Double;
				void * Void;
			};
			
			
			ArgValue Value;
			char * Pointer;
		};
		
		ArgKind Kind;

		Arg():Kind() {}
		Arg(int options, char value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, int value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, long value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, long long value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, double value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, char * value, char * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Arg(int options, int * value, char * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}
		Arg(int options, long * value, char * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}
		Arg(int options, void * value, char * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}

		bool IsFormat() {return Is(FIELD_FORMAT);}
		bool IsWidth() {return Is(FIELD_WIDTH);}
		bool IsPrecision() {return Is(FIELD_PRECISION);}

		char * String() {return *(char**)Kind.Pointer;}
		void * Void() {return *(void**)Kind.Pointer;}
		char Char() {return *(char*)Kind.Pointer;}
		int Int() {return *(int*)Kind.Pointer;}
		short Short() {return *(short*)Kind.Pointer;}
		long Long() {return *(long*)Kind.Pointer;}
		long LongLong() {return *(long long*)Kind.Pointer;}
		double Double() {return *(double*)Kind.Pointer;}

	};
	
	List<Field> Fields;
	List<Arg> Args;
	
	Formatter()
	{
	}

	Formatter(const char * format,...)
	{
		va_list va;
		va_start(va, format);
		ConstructVa(format,va);
		va_end(va);
	}

	Formatter(const char * format, va_list va)
	{
		ConstructVa(format,va);
	}


	
	void Construct(const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		ConstructVa(format,va);
		va_end(va);
	}

	void ConstructVa(const char * format, va_list va);
	
	void Format(String & string);


	virtual void FieldInteger(String & string, int index);
	virtual void FieldString(String & string, int index);
	virtual void FieldDouble(String & string, int index);
	virtual void FieldChar(String & string, int index);
	virtual void FieldVa(int index, va_list * va);


public:

	
	
	

	static void Insert(String & string, int index, const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		String insert;
		insert.Allocate(String::Length(format)*1.25);
		FormatVa(insert,format,va);
		va_end(va);
		string.Insert(insert,index);
	}

	static void InsertVa(String & string, int index, const char * format, va_list va)
	{
		String insert;
		insert.Allocate(String::Length(format)*1.25);
		FormatVa(insert,format,va);
		string.Insert(insert,index);
	}
	
	static void Append(String & string, const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		String append;
		append.Allocate(String::Length(format)*1.25);
		FormatVa(append,format,va);
		va_end(va);
		string.Append(append);
	}

	static void AppendVa(String & string, const char * format, va_list va)
	{
		String append;
		append.Allocate(String::Length(format)*1.25);
		FormatVa(append,format,va);
		string.Append(append);
	}

	static void Prepend(String & string, const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		String prepend;
		prepend.Allocate(String::Length(format)*1.25);
		FormatVa(prepend,format,va);
		va_end(va);
		string.Prepend(prepend);
	}

	static void PrependVa(String & string, const char * format, va_list va)
	{
		String prepend;
		prepend.Allocate(String::Length(format)*1.25);
		FormatVa(prepend,format,va);
		string.Prepend(prepend);
	}

	static Superstring Format(const char * format, ...)
	{
		va_list va;
		va_start(va, format);
		Superstring superstring;
		superstring.Allocate(String::Length(format)*1.25);	
		FormatVa(superstring,format,va);
		va_end(va);
		return superstring;
	}
	
	static Superstring FormatVa(const char * format, va_list va)
	{
		Superstring superstring;
		superstring.Allocate(String::Length(format)*1.25);	
		FormatVa(superstring,format,va);
		return superstring;
	}
	

	static int Format(String & string, const char * format, ...) 
	{
		
		string.Release();
		
		va_list va;
		va_start(va, format);		
		int res = Format(string,0,format,va);
		va_end(va);
		return res;
	}
	

	static int Format(String & string, int index, const char * format, ...)
	{
		
		va_list va;
		va_start(va, format);
		int res = FormatVa(string,index,format,va);
		va_end(va);
		return res;
	}


	static int FormatVa(String & string, const char * format, va_list va);	
	static int FormatVa(String & string, int index, const char * format, va_list va);


	template<typename _Value_>
	static int Format(String & string, int index, const char * format, _Value_ value)
	{
		
		
		
		
		Assert(Traits::IsPrimitive<_Value_>::Value);				
		int amount = string.Remaining();
		if (amount < 128)
			string.Reserve(128);
				
		
		
		amount = 64;
		
		int count = Characters::Format(string.Data+string.Size,amount,format,value);
				
		if (count > -1 && count < amount)
		{			
			memmove(string.Data+index+count,string.Data+index,string.Size+count-index);
			memmove(string.Data+index,string.Data+string.Size+count,count);
			
			string.Size += count;				
			string.Term();
			return count;
		}	
		
		
		string.Term();
		return -1;				
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Formatting : public String
{
public:

	
	struct Specifier : public Flags<64>
	{
		enum SpecifierOptions
		{

			
			
			
			
			
			
			TYPE_FORMAT_c	=(1)<<0,
			

			
			
			TYPE_FORMAT_d	=(1)<<2,
			TYPE_FORMAT_i	=(1)<<3,

			
			
			TYPE_FORMAT_o	=(1)<<4,

			
			
			TYPE_FORMAT_u	=(1)<<5,

			
			
			TYPE_FORMAT_x	=(1)<<6,

			
			
			TYPE_FORMAT_X	=(1)<<7,

			
			
			
			
			TYPE_FORMAT_e	=(1)<<8,
			TYPE_FORMAT_E	=(1)<<9,

			
			
			TYPE_FORMAT_f	=(1)<<10,

			
			
			
			
			TYPE_FORMAT_g	=(1)<<11,
			TYPE_FORMAT_G	=(1)<<12,

			
			
			
			
			
			
			TYPE_FORMAT_n	=(1)<<15,

			
			
			TYPE_FORMAT_p	=(1)<<16,
			
			TYPE_FORMAT_s	=(1)<<17,
			
			
			TYPE_FORMAT		= TYPE_FORMAT_c|TYPE_FORMAT_d|
			TYPE_FORMAT_i|TYPE_FORMAT_o|TYPE_FORMAT_u|TYPE_FORMAT_x|
			TYPE_FORMAT_X|TYPE_FORMAT_e|TYPE_FORMAT_E|TYPE_FORMAT_f|
			TYPE_FORMAT_g|TYPE_FORMAT_G|TYPE_FORMAT_n|TYPE_FORMAT_p|
			TYPE_FORMAT_s,

			
			
			
			
			
			
			
			TYPE_INCLUSION	=(1)<<20,

			
			
			TYPE_EXCLUSION	=(1)<<21,
			
			
			
			
			TYPE_SHIFT		=(1)<<22,	

			
			TYPE_UNSIGNED	=(1)<<23,
			
			
			TYPE_SIGNED		=(1)<<24,

			
			TYPE_WIDTH		=(1)<<19,
			
			
			TYPE_PRECISION	=(1)<<20,
			
			
			
			TYPE_PERCENT	=(1)<<21,
			
			

			
			
			
			MODIFIER_h		=(1)<<30,

			
			
			
			MODIFIER_l		=(1)<<31,

			
			
			MODIFIER_L		=(1)<<32,

			
			
			MODIFIER_ll		=(1)<<33,

			
			
			FLAG_JUSTIFY	=(1)<<40,	

			
			
			FLAG_SIGN		=(1)<<41,	

			
			
			FLAG_SPACE		=(1)<<42,	
			
			
			
			
			
			
			
			FLAG_ALTERNATE	=(1)<<43,	
			
			
			
			
			FLAG_PADDED		=(1)<<44,		

			
			
			
			FLAG_SUPPRESSOR	=(1)<<45,

 		};

		int Width;
		int Precision;
		Substring Format;

		Specifier():Width(0),Precision(0) {}
		Specifier(int options):Flags<64>(options),Width(0),Precision(0) {}
	};

	struct Value : public Flags<32>
	{
		enum ValueTypes
		{
			TYPE_NUMBER		=(1)<<0,
			TYPE_STRING		=(1)<<1,
			TYPE_POINTER	=(1)<<2,
		};

		String String;
		Number Number;
		void * Pointer;

		Value():Pointer(0) {}

		Value & operator = (int mask) {Flags<32>::operator =(mask);return *this;}
	};

	struct Argument : public Flags<32>
	{
		
		
		
		enum ArgumentOptions
		{
			KIND_CHAR		=(1),
			KIND_INT		=(1)<<1,
			KIND_LONG		=(1)<<2,
			KIND_LONGLONG	=(1)<<3,
			KIND_VOID		=(1)<<4,
			KIND_DOUBLE		=(1)<<5,
			KIND_STRING		=(1)<<6,

			FLAG_UNSIGNED	=(1)<<10,
			FLAG_SIGNED		=(1)<<11,
			FLAG_POINTER	=(1)<<12,

			TYPE_FORMAT		=(1)<<20,
			TYPE_WIDTH		=(1)<<21,
			TYPE_PRECISION	=(1)<<22,
		};

		
		
		
		
		
		
		
		
		
		struct ArgumentKind
		{
			ArgumentKind():Value(0),Pointer(0) {}
			ArgumentKind(char value, void * pointer):Value(value),Pointer(pointer) {}
			ArgumentKind(int value, void * pointer):Value(value),Pointer(pointer) {}
			ArgumentKind(long value, void * pointer):Value(value),Pointer(pointer) {}
			ArgumentKind(long long value, void * pointer):Value(value),Pointer(pointer) {}
			ArgumentKind(double value, void * pointer):Value(value),Pointer(pointer) {}
			ArgumentKind(void * value, void * pointer):Value(value),Pointer(pointer) {}
			
			ArgumentKind(String & value, void * pointer):Value(),String(value),Pointer(pointer) {}

			union ArgumentValue
			{
				ArgumentValue():LongLong(0) {}
				ArgumentValue(char value):Char(value) {}
				ArgumentValue(int value):Int(value) {}
				ArgumentValue(long value):Long(value) {}
				ArgumentValue(long long value):LongLong(value) {}
				ArgumentValue(double value):Double(value) {}
				ArgumentValue(void * value):Void(value) {}
				

				char Char;
				int Int;
				long Long;
				long long LongLong;
				double Double;
				void * Void;
				
			};
						
			ArgumentValue Value;
			
			
			
			String String;
			void * Pointer;
		};
		
		ArgumentKind Kind;

		Argument():Kind() {}
		Argument(int options, char value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, int value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, long value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, long long value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, double value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, char * value, void * pointer=0):Flags<32>(options),Kind(value,pointer) {}
		Argument(int options, int * value, void * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}
		Argument(int options, long * value, void * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}
		Argument(int options, void * value, void * pointer=0):Flags<32>(options),Kind((void*)value,pointer) {}

		bool IsFormat() {return Is(TYPE_FORMAT);}
		bool IsWidth() {return Is(TYPE_WIDTH);}
		bool IsPrecision() {return Is(TYPE_PRECISION);}

		char * String() {return *(char**)Kind.Pointer;}
		void * Void() {return *(void**)Kind.Pointer;}
		char Char() {return *(char*)Kind.Pointer;}
		int Int() {return *(int*)Kind.Pointer;}
		short Short() {return *(short*)Kind.Pointer;}
		long Long() {return *(long*)Kind.Pointer;}
		long LongLong() {return *(long long*)Kind.Pointer;}
		double Double() {return *(double*)Kind.Pointer;}

	};
	
	enum FormattingModes
	{
		MODE_WRITE	=(1)<<0,		
		MODE_READ	=(1)<<1,		
	};

	int Mode;
	Array<Specifier> Specifiers;

	Formatting():Mode(0)
	{

	}

	Formatting(const char * data, int mode):Mode(0)
	{
		Construct(data,mode);
	}

	void Construct(Substring & sequence, int mode) {Construct(sequence.Data,sequence.Size,mode);}
	void Construct(const char * data, int mode) {Construct((char*)data,String::Length(data),mode);}
	void Construct(char * data, int size, int mode);
	void Construct(int mode);




	Substring Before(int index)
	{
		Substring token;
		if (index > Specifiers.Length()-1)
			return token;

		if (index > 0)
		{
			Specifier & specifier = Specifiers[index];
			token = Specifiers[index-1].Format;
			token.Data += token.Size;
			token.Size = specifier.Format.Data - token.Data;
		}
		else
		{
			Specifier & specifier = Specifiers[index];
			token.Data = Formatting::Data;
			token.Size = specifier.Format.Data - token.Data;
		}

		return token;
	}

	Substring After(int index)
	{		
		Substring token;
		if (index > Specifiers.Length()-1)
			return token;
		
		if (index < Specifiers.Length()-1)
		{
			Specifier & specifier = Specifiers[index+1];
			token = Specifiers[index].Format;
			token.Data += token.Size;
			token.Size = specifier.Format.Data - token.Data;
		}
		else
		{
			token = Specifiers[Specifiers.Length()-1].Format;
			token.Data += token.Size;
			token.Size = (Formatting::Data+Formatting::Size) - token.Data;
		}

		return token;
	}

	Argument Specify(int mode, int index)
	{
		
		
		
		Argument argument;
		argument.On();

		if (mode == MODE_READ)
		{

		}

		
		return Argument();
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class FormattingStream : public Formatting, public StreamFilter
{
public:


	int Index;
	
	
	class Token Token;

	
	Formatting::Value Width;
	Formatting::Value Precision;

	FormattingStream(const char * data, int mode);
	FormattingStream();
	~FormattingStream();

	void Format(String & string);

	int Read(String & string);
	int Read(Value & value);

	int ReadInteger(int & value);
	int ReadString(String & value);
	int ReadDouble(double & value);
	int ReadChar(char & value);

	int Write(String & string);
	int Write(Value & value);

	int WriteInteger(int & value);
	int WriteString(String & value);
	int WriteDouble(double & value);
	int WriteChar(char & value);

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Format
{
public:

	FormattingStream Formatting;
	
	
	
	Format()
	{
	}

	~Format()
	{

	}


	Format(const char * format):Formatting(format,0)
	{
	}


	Format & operator << (const String & s) {Formatting.WriteString((String&)s);return *this;}
	
	Format & operator << (const char & c) {Formatting.WriteChar((char&)c);return *this;}
	Format & operator << (const int & i) {Formatting.WriteInteger((int&)i);return *this;}
	
	
	
	
	
	
	Format & operator << (const double & d) {Formatting.WriteDouble((double&)d);return *this;}


	
	Format & operator >> (String & s) {Formatting.ReadString(s);return *this;}
	Format & operator >> (int &i) {Formatting.ReadInteger(i);return *this;}
	Format & operator >> (char & c) {Formatting.ReadChar(c);return *this;}
	
	
	
	Format & operator >> (double &d) {Formatting.ReadDouble(d);return *this;}

	
	
	
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Format & operator << (String & string, Format & format);
Format & operator << (const String & string, const Format & format);

Format & operator >> (String & string, Format & format);
Format & operator >> (const String & string, const Format & format);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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


