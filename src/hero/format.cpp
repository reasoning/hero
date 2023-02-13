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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/format.h"
#include "hero/string.h"
#include "hero/string.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Formatter::ConstructVa(const char * format, va_list va)
{

	String::Construct((char*)format,String::Length(format));

	StringParser parser(*this);

	#define va_get(ap, t) ((t *)ap)

	#ifdef HERO_PLATFORM_POSIX

	va_list vb;
	va_copy(vb,va);
	va_copy(va,vb);
	va_end(vb);
	#endif

	int index=0;

	while (!parser.Eof())
	{
		if (parser.Is('%'))
		{			

			Field field;

			parser.Mark(field.Format);			
			parser.Next();

			while (parser.IsAny("-+ #0"))
			{
				switch (*parser.At())
				{
					case '-': field.On(Field::FLAG_LEFT); break;
					case '+': field.On(Field::FLAG_PLUS); break;
					case ' ': field.On(Field::FLAG_SPACE); break;
					case '#': field.On(Field::FLAG_SPECIAL); break;
					case '0': field.On(Field::FLAG_PADDED); break;
				}

				parser.Next();
			}

			field.Width = -1;
			if (parser.IsNumeric())
			{
				parser.ParseNumber();
				field.Width = parser.Token.Integer();
			}
			else
			if (parser.Is('*'))
			{
				parser.Next();
				field.Width = va_arg(va,int);
				Args.Append(Arg(Arg::FIELD_WIDTH|Arg::TYPE_INT|Arg::FLAG_SIGNED,field.Width,(char*)va_get(va,int)));
				index++;

				if (field.Width < 0)
				{
					field.Width = -field.Width;
					field.On(Field::FLAG_LEFT);
				}
			}

			field.Precision = -1;
			if (parser.Is('.'))
			{
				parser.Next();

				if (parser.IsNumeric())
				{
					parser.ParseNumber();
					field.Precision = parser.Token.Integer();
				}
				else
				if (parser.Is('*'))
				{
					parser.Next();
					field.Precision = va_arg(va,int);
					Args.Append(Arg(Arg::FIELD_PRECISION|Arg::TYPE_INT|Arg::FLAG_SIGNED,field.Precision,(char*)va_get(va,int)));
					index++;

					if (field.Precision < 0)
					{
						field.Precision = 0;
					}
				}
			}

			if (parser.Is("ll"))
			{
				field.On(Field::MODIFIER_ll);
				parser.Skip(2);
			}
			else
			if (parser.IsAny("hlL"))
			{
				switch (*parser.At())
				{
					case 'h': field.On(Field::MODIFIER_h); break;
					case 'l': field.On(Field::MODIFIER_l); break;
					case 'L': field.On(Field::MODIFIER_L); break;
				}

				parser.Next();
			}

			switch (*parser.At())
			{
				case 'c': field.On(Field::TYPE_FORMAT_c);break;
				case 'C': field.On(Field::TYPE_FORMAT_C); break;
				case 'd': field.On(Field::TYPE_FORMAT_d);field.On(Field::FLAG_SIGNED);break;
				case 'i': field.On(Field::TYPE_FORMAT_i);field.On(Field::FLAG_SIGNED);break;
				case 'o': field.On(Field::TYPE_FORMAT_o);break;
				case 'u': field.On(Field::TYPE_FORMAT_u);break;
				case 'x': field.On(Field::TYPE_FORMAT_x);break;
				case 'X': field.On(Field::TYPE_FORMAT_X);field.On(Field::FLAG_LARGE);break;				
				case 'e': field.On(Field::TYPE_FORMAT_e);field.On(Field::FLAG_SIGNED);break; 
				case 'E': field.On(Field::TYPE_FORMAT_E);field.On(Field::FLAG_SIGNED);break;
				case 'f': field.On(Field::TYPE_FORMAT_f);field.On(Field::FLAG_SIGNED);break;
				case 'g': field.On(Field::TYPE_FORMAT_g);field.On(Field::FLAG_SIGNED);break;
				case 'G': field.On(Field::TYPE_FORMAT_G);field.On(Field::FLAG_SIGNED);break;
				case 'n': field.On(Field::TYPE_FORMAT_n); break;
				case 'p': field.On(Field::TYPE_FORMAT_p); 

					if (field.Width == -1)
					{
						field.Width = 2 * sizeof(void*);
						field.On(Field::FLAG_PADDED);
					}					
					break;
				case 's': field.On(Field::TYPE_FORMAT_s); break;
				case 'S': field.On(Field::TYPE_FORMAT_S); break;
				case '%': field.On(Field::TYPE_PERCENT); break;
				default:
					continue;

			}

			if (field.Is(Field::TYPE_FORMAT_c|Field::TYPE_FORMAT_C))
			{

				Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_INT,va_arg(va,int),(char*)va_get(va,int)));
			}
			else
			if (field.Is(Field::TYPE_FORMAT_d|Field::TYPE_FORMAT_i|Field::TYPE_FORMAT_o|Field::TYPE_FORMAT_u|Field::TYPE_FORMAT_x|Field::TYPE_FORMAT_X))
			{

				if (field.Is(Field::MODIFIER_ll))
				{

					Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_LONGLONG|Arg::FLAG_UNSIGNED,va_arg(va,long long),(char*)va_get(va,unsigned long long)));
				}
				else
				if (field.Is(Field::MODIFIER_l))
				{

					Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_LONG|Arg::FLAG_UNSIGNED,va_arg(va,long),(char*)va_get(va,unsigned long)));
				}
				else				
				{					

					if (field.Is(Field::FLAG_SIGNED))
						Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_INT|Arg::FLAG_SIGNED,va_arg(va,int),(char*)va_get(va,int)));
					else
						Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_INT|Arg::FLAG_UNSIGNED,va_arg(va,int),(char*)va_get(va,unsigned int)));
				}
			}
			else
			if (field.Is(Field::TYPE_FORMAT_e|Field::TYPE_FORMAT_E|Field::TYPE_FORMAT_f|Field::TYPE_FORMAT_g|Field::TYPE_FORMAT_G))
			{

				Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_DOUBLE|Arg::FLAG_SIGNED,va_arg(va,double),(char*)va_get(va,double)));
			}
			else
			if (field.Is(Field::TYPE_FORMAT_s|Field::TYPE_FORMAT_S))
			{

				Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_CHAR|Arg::FLAG_POINTER,va_arg(va,char*),(char*)va_get(va,char*)));
			}
			else
			if (field.Is(Field::TYPE_FORMAT_p))
			{

				Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_VOID|Arg::FLAG_POINTER,va_arg(va,void*),(char*)va_get(va,void*)));
			}
			else
			if (field.Is(Field::TYPE_FORMAT_n))
			{

				if (field.Is(Field::MODIFIER_l))
				{
					Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_LONG|Arg::FLAG_POINTER,va_arg(va,long*),(char*)va_get(va,long*)));
				}
				else
				{
					Args.Append(Arg(Arg::FIELD_FORMAT|Arg::TYPE_INT|Arg::FLAG_POINTER,va_arg(va,int*),(char*)va_get(va,int*)));
				}
			}

			parser.Next();
			parser.Trap(field.Format);

			field.Index = index++;
			Fields.Append(field);

		}
		else
		{
			parser.Next();
		}
	}

	#ifdef HERO_PLATFORM_POSIX
	va_end(va);
	#endif

}

void Formatter::Format(String & string)
{

	int from=0;
	int to=0;
	for (int i=0;i<Fields.Count;++i)
	{
		Field & field = Fields[i];

		to = IndexOf(field.Format);
		string << Slice(from,to);
		from = to+field.Format.Size;

		if (field.Is(Field::TYPE_FORMAT_c|Field::TYPE_FORMAT_C))
		{
			FieldChar(string,i);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_s|Field::TYPE_FORMAT_S))
		{
			FieldString(string,i);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_d|Field::TYPE_FORMAT_i|Field::TYPE_FORMAT_o|Field::TYPE_FORMAT_u|Field::TYPE_FORMAT_x|Field::TYPE_FORMAT_X))
		{
			FieldInteger(string,i);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_e|Field::TYPE_FORMAT_E|Field::TYPE_FORMAT_f|Field::TYPE_FORMAT_g|Field::TYPE_FORMAT_G))
		{
			FieldDouble(string,i);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_p))
		{
			Append(string,"%p",Args[field.Index].Kind.Value.Void);
		}
		else
		if (field.Is(Field::TYPE_FORMAT_n))
		{
			if (field.Is(Field::MODIFIER_l))
				*((long*)Args[field.Index].Kind.Value.Void) = string.Size;
			else
				*((int*)Args[field.Index].Kind.Value.Void) = string.Size;
		}
	}

	string << Slice(from);
}

void Formatter::FieldInteger(String & string, int index)
{

	String format = Fields[index].Format;
	switch (Fields[index].Index-index)
	{
	case 0:Append(string,format.Print(),Args[index].Kind.Value.Int);break;
	case 1:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index].Kind.Value.Int);break;
	case 2:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index++].Kind.Value.Int,Args[index].Kind.Value.Int);break;
	}
}

void Formatter::FieldString(String & string, int index)
{
	String format = Fields[index].Format;
	switch (Fields[index].Index-index)
	{
	case 0:Append(string,format.Print(),Args[index].Kind.Value.Void);break;
	case 1:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index].Kind.Value.Void);break;
	case 2:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index++].Kind.Value.Int,Args[index].Kind.Value.Void);break;
	}
}

void Formatter::FieldDouble(String & string, int index)
{
	String format = Fields[index].Format;
	switch (Fields[index].Index-index)
	{
	case 0:Append(string,format.Print(),Args[index].Kind.Value.Double);break;
	case 1:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index].Kind.Value.Double);break;
	case 2:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index++].Kind.Value.Int,Args[index].Kind.Value.Double);break;
	}
}

void Formatter::FieldChar(String & string, int index)
{
	String format = Fields[index].Format;
	switch (Fields[index].Index-index)
	{
	case 0:Append(string,format.Print(),Args[index].Kind.Value.Char);break;
	case 1:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index].Kind.Value.Char);break;
	case 2:Append(string,format.Print(),Args[index++].Kind.Value.Int,Args[index++].Kind.Value.Int,Args[index].Kind.Value.Char);break;
	}
}

void Formatter::FieldVa(int index, va_list * va)
{

	switch (Fields[index].Index-index)
	{
	case 0:break;
	case 1:va_arg(*va,int);break;
	case 2:va_arg(*va,int);va_arg(*va,int);break;
	}

	if (Args[Fields[index].Index].Is(Formatter::Arg::FLAG_POINTER))
	{
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_CHAR))
			va_arg(*va,char*);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_VOID))
			va_arg(*va,void*);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_INT))
			va_arg(*va,int*);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_LONG))
			va_arg(*va,long*);
	}
	else
	{
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_INT))
			va_arg(*va,int);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_LONG))
			va_arg(*va,long);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_LONGLONG))
			va_arg(*va,long long);
		else
		if (Args[Fields[index].Index].Is(Formatter::Arg::TYPE_DOUBLE))
			va_arg(*va,double);
	}
}

int Formatter::FormatVa(String & string, const char * format, va_list va)
{
	int amount = String::Length(format)*1.25;

	string.Reserve(amount);

	int retry=0;
	int count=0;
	while (true) 
	{

		if (retry > 100)
		{
			Raise("Formatter::FormatVa: Retry limit exceeded");
			break;
		}

		amount = string.Allocated;
		count = Characters::FormatVa(string.Data,amount,format,va);

		if (count > -1 && count < amount)
			break;

		if (count > -1)		
		{

			string.Allocate(count+1+1);
		}
		else 
		{

			#ifdef HERO_PLATFORM_WINDOWS

			++retry;
			string.Allocate(0);
			continue;

			#endif

			Raise("Formatter::FormatVa: Invalid format specifier or output error");
			count = 0;
			break;
		}	

	}

	string.Size = count;

	string.Term();

	return count;
}

int Formatter::FormatVa(String & string, int index, const char * format, va_list va)
{

	int amount = String::Length(format)*1.25;
	string.Reserve(amount);

	int count=0;
	while (true) 
	{		
		amount = string.Allocated-string.Size;		
		count = Characters::FormatVa(string.Data+string.Size,amount,format,va);

		if (count > -1 && count < amount)
			break;		

		if (count > -1)		
		{
			string.Reserve(count+1+1);
		}
		else 
		{
			string.Reserve(0);
		}			
	}

	if (count > -1 && count < amount)
	{			
		int size = string.Size;
		string.Size += count;

		if (size > 0 && index < size)
		{

			string.Reserve(count);

			amount = size+count-index;
			memmove(string.Data+index+count,string.Data+index,amount);
			memmove(string.Data+index,string.Data+index+amount,count);		
		}

		string.Term();
		return count;
	}

	string.Term();
	return -1;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Formatting::Construct(char * data, int size, int mode)
{
	String::Construct(data,size);
	Construct(mode);
}

void Formatting::Construct(int mode)
{
	Specifiers.Release();
	Mode = mode;

	if (Mode == 0) return;

	StringParser parser(*this);

	int index=0;

	while (!parser.Eof())
	{

		if (parser.Is('%'))
		{			
			Specifier specifier;

			parser.Mark(specifier.Format);			
			parser.Next();

			if (Mode == Formatting::MODE_WRITE)
			{

				while (parser.IsAny("-+ #0"))
				{
					switch (*parser.At())
					{
						case '-': specifier.On(Specifier::FLAG_JUSTIFY); break;
						case '+': specifier.On(Specifier::FLAG_SIGN); break;
						case ' ': specifier.On(Specifier::FLAG_SPACE); break;
						case '#': specifier.On(Specifier::FLAG_ALTERNATE); break;
						case '0': specifier.On(Specifier::FLAG_PADDED); break;
					}

					parser.Next();
				}

				specifier.Width = -1;
				if (parser.IsNumeric())
				{
					parser.ParseNumber();
					specifier.Width = parser.Token.Decimal();
				}
				else
				if (parser.Is('*'))
				{
					parser.Next();
					specifier.On(Specifier::TYPE_WIDTH);
				}

				specifier.Precision = -1;
				if (parser.Is('.'))
				{
					parser.Next();

					if (parser.IsNumeric())
					{
						parser.ParseNumber();
						specifier.Precision = parser.Token.Decimal();
					}
					else
					if (parser.Is('*'))
					{
						parser.Next();
						specifier.On(Specifier::TYPE_PRECISION);
					}
				}
			}
			else
			if (Mode == Formatting::MODE_READ)
			{
				if (parser.Is('*'))
				{
					parser.Next();
					specifier.On(Specifier::FLAG_SUPPRESSOR);
				}

				specifier.Width = -1;
				if (parser.IsNumeric())
				{
					parser.ParseNumber();
					specifier.Width = parser.Token.Decimal();
				}
			}

			if (parser.Is("ll"))
			{
				specifier.On(Specifier::MODIFIER_ll);
				parser.Skip(2);
			}
			else
			if (parser.IsAny("hlL"))
			{
				switch (*parser.At())
				{
					case 'h': specifier.On(Specifier::MODIFIER_h); break;
					case 'l': specifier.On(Specifier::MODIFIER_l); break;
					case 'L': specifier.On(Specifier::MODIFIER_L); break;
				}

				parser.Next();
			}

			switch (*parser.At())
			{
				case 'c': specifier.On(Specifier::TYPE_FORMAT_c);break;

				case 'd': specifier.On(Specifier::TYPE_FORMAT_d);break;
				case 'i': specifier.On(Specifier::TYPE_FORMAT_i);break;
				case 'o': specifier.On(Specifier::TYPE_FORMAT_o);break;
				case 'u': specifier.On(Specifier::TYPE_FORMAT_u);break;
				case 'x': specifier.On(Specifier::TYPE_FORMAT_x);break;
				case 'X': specifier.On(Specifier::TYPE_FORMAT_X);break;				
				case 'e': specifier.On(Specifier::TYPE_FORMAT_e);break; 
				case 'E': specifier.On(Specifier::TYPE_FORMAT_E);break;
				case 'f': specifier.On(Specifier::TYPE_FORMAT_f);break;
				case 'g': specifier.On(Specifier::TYPE_FORMAT_g);break;
				case 'G': specifier.On(Specifier::TYPE_FORMAT_G);break;
				case 'n': specifier.On(Specifier::TYPE_FORMAT_n);break;
				case 'p': specifier.On(Specifier::TYPE_FORMAT_p);break; 

					if (specifier.Width == -1)
					{
						specifier.Width = 2 * sizeof(void*);
						specifier.On(Specifier::FLAG_PADDED);
					}					
					break;
				case 's': specifier.On(Specifier::TYPE_FORMAT_s); break;

				case '%': specifier.On(Specifier::TYPE_PERCENT); break;
				default:
					continue;
			}

			parser.Next();
			parser.Trap(specifier.Format);

			Specifiers.Append(specifier);
		}
		else
		{
			parser.Next();
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FormattingStream::FormattingStream(const char * data, int mode):
	Formatting(data,mode),Index(0)
{
}

FormattingStream::FormattingStream():
	Formatting(),Index(0)
{
}

FormattingStream::~FormattingStream()
{

	if (Mode == Formatting::MODE_WRITE)
	{
		if (Index < Specifiers.Length())
		{

			Specifier & specifier = Specifiers[Index];

			Substring token;
			if (Index > 0)
			{
				token = Specifiers[Index-1].Format;
				token.Data += token.Size;
				token.Size = (Formatting::Data+Formatting::Size) - token.Data;
			}
			else
			{
				token.Data = Formatting::Data;
				token.Size = (Formatting::Data+Formatting::Size)- token.Data;
			}

			if (!token.IsEmpty())	
			{

				Stream->Write(token);
			}
		}
	}

	if (Stream)
		delete Stream;
}

void FormattingStream::Format(String & string)
{
	if (Stream)
		delete Stream;

	Stream = new StringStream::StreamAdapter(string);
}

int FormattingStream::Read(String & string)
{

	StreamParser parser(Stream);
	parser.Scanner->Token = Token;

	int position = parser.Position();
	if (parser.Parse(string))
	{
		return parser.Position()-position;
	}

	return 0;
}

int FormattingStream::Read(Value & value)
{

	if (Index < 0 || Index > Specifiers.Length()-1) return 0;

	class Token token;
	StreamParser parser(Stream);
	parser.Scanner->Token = Token;

	if (parser.Eof())
		return 0;

	int position = parser.Position();

	if (Index == 0)
	{

		String before = Formatting::Before(Index);
		if (!before.IsEmpty() && !parser.Parse(before))
		{
			Index = -1;
			return parser.Position()-position;
		}
	}

	Specifier & specifier = Specifiers[Index];

	unsigned int width = specifier.Width;

	if (width <= 0)
		width = Number::MaximumUnsigned();

	if (specifier.Is(Specifier::TYPE_FORMAT_d|Specifier::TYPE_FORMAT_i|Specifier::TYPE_FORMAT_o|Specifier::TYPE_FORMAT_u|Specifier::TYPE_FORMAT_x|Specifier::TYPE_FORMAT_X))
	{	

		int sign = 1;
		int radix = 0;

		parser.Mark(token);

		if (parser.IsAny("-+"))
		{
			sign = parser.Is('-')?-1:1;
			parser.Next();					
			--width;
		}

		if (specifier.Is(Specifier::TYPE_FORMAT_d))
		{

			radix = 10;
		}
		else
		if (specifier.Is(Specifier::TYPE_FORMAT_i))
		{

			if (parser.IsRange('1','9'))
			{
				radix = 10;		
			}
			else
			if (parser.Is('0') && parser.IsRange(1,'1','7'))
			{
				parser.Next();
				--width;
				radix = 8;
			}
			else
			if (parser.Is('0') && parser.IsAny(1,"xX"))
			{
				parser.Skip(2);
				width -= 2;
				radix = 16;
			}
		}
		else
		if (specifier.Is(Specifier::TYPE_FORMAT_o))
		{

			radix = 8;
		}			
		else
		if (specifier.Is(Specifier::TYPE_FORMAT_u))
		{

			radix = 10;
		}
		else
		if (specifier.Is(Specifier::TYPE_FORMAT_x|Specifier::TYPE_FORMAT_X))
		{

			if (parser.Is('0') && parser.IsAny(1,"xX"))
			{
				parser.Skip(2);
				width -= 2;
			}

			radix = 16;
		}

		switch(radix)
		{
		case 16:
			if (parser.IsHex())
			{
				parser.Mark();
				while (width > 0 && !parser.Eof() && parser.IsHex())
				{
					parser.Next();
					--width;
				}
				parser.Trap();	

				value.Number = Characters::Hex(parser.Token.Data,parser.Token.Size);
				parser.Trap(token);
				value.String = token;
				value = Value::TYPE_NUMBER;
			}
			break;
		case 10:
			if (parser.IsDecimal())
			{
				parser.Mark();
				while (width > 0 && !parser.Eof() && parser.IsDecimal())
				{
					parser.Next();
					--width;
				}
				parser.Trap();

				value.Number = Characters::Decimal(parser.Token.Data,parser.Token.Size);
				parser.Trap(token);
				value.String = token;
				value = Value::TYPE_NUMBER;
			}
			break;
		case 8:
			if (parser.IsRange('0','7'))
			{
				parser.Mark();		
				while (width > 0 && !parser.Eof() && parser.IsRange('0','7'))
				{
					parser.Next();
					--width;
				}

				parser.Trap();

				value.Number = Characters::Octal(parser.Token.Data,parser.Token.Size);
				parser.Trap(token);
				value.String = token;
				value = Value::TYPE_NUMBER;
			}
			break;
		}

		value.Number *= sign;

	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_e|Specifier::TYPE_FORMAT_E|Specifier::TYPE_FORMAT_f|Specifier::TYPE_FORMAT_g|Specifier::TYPE_FORMAT_G))
	{

		bool numeric = false;

		parser.Mark(token);
		parser.Mark();			
		if (parser.IsAny("-+"))
		{
			parser.Next();					
			--width;
		}

		while (width > 0 && !parser.Eof() && parser.IsNumeric())
		{
			parser.Next();
			--width;
			numeric = true;
		}

		if (width > 0 && !parser.Eof() && parser.Is('.') && parser.IsNumeric(1))
		{
			numeric = true;
			parser.Next();
			while (width > 0 && !parser.Eof() && parser.IsNumeric())
			{
				parser.Next();
				--width;				
			}
		}

		if (numeric && width > 0 && !parser.Eof() && parser.IsCaseless('e') && (parser.IsAny(1,"+-") || parser.IsNumeric(1)))
		{
			parser.Next();

			if (parser.IsAny("+-"))
			{
				parser.Next();
				--width;
			}

			while (width > 0 && !parser.Eof() && parser.IsNumeric())
			{
				parser.Next();
				--width;
			}
		}

		parser.Trap();
		if (numeric)
		{
			value.Number = Characters::Real(parser.Token.Data,parser.Token.Size);
			parser.Trap(token);
			value.String = token;
			value = Value::TYPE_NUMBER;
		}

	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_s))
	{

		parser.Mark();
		while (width > 0 && !parser.Eof() && !parser.IsWhitespace())
		{
			parser.Next();
			--width;
		}
		parser.Trap();

		if (!parser.Token.IsEmpty())
		{
			value.String = parser.Token;
			value = Value::TYPE_STRING;
		}
	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_c))
	{

		parser.Mark();
		while (width > 0 && !parser.Eof())
		{
			parser.Next();
			--width;
		}
		parser.Trap();

		if (!parser.Token.IsEmpty())
		{
			value.String = parser.Token;
			value = Value::TYPE_STRING;
		}

	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_p))
	{

		int digits=0;

		parser.Mark();		
		while (width > 0 && digits < 8 && !parser.Eof() && parser.IsNumeric())
		{
			parser.Next();
			--width;
			++digits;
		}
		parser.Trap();

		if (!parser.Token.IsEmpty())
		{
			value.String = parser.Token;
			value.Pointer = (void*)parser.Token.Integer();
			value = Value::TYPE_POINTER;
		}
	}
	else
	if (specifier.Is(Specifier::TYPE_FORMAT_n))
	{

	}

	if (specifier.Is(Specifier::FLAG_SUPPRESSOR))
		value.Clear();

	String after = Formatting::After(Index);
	if (!after.IsEmpty() && !parser.Parse(after))
	{
		Index = -1;
		return parser.Position()-position;
	}

	Token = parser.Scanner->Token;
	Token.Reduce();

	++Index;
	return parser.Position()-position;
}

int FormattingStream::ReadInteger(int & value)
{
	int read = 0;

	Value val;
	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String.Integer();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))

			value = (int)(long)val.Pointer;
		else
			value = (int)val.Number;
	}

	return read;
}

int FormattingStream::ReadDouble(double & value)
{
	int read = 0;

	Value val;
	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = (double)val.String.Real();
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))

			value = (double)(int)(long)val.Pointer;
		else
			value = (double)val.Number;
	}

	return read;
}

int FormattingStream::ReadString(String & value)
{
	int read = 0;

	Value val;
	read += Read(val);

	if (val.Is(Value::TYPE_STRING))
	{
		value = val.String;
	}
	else
	{
		if (val.Is(Value::TYPE_POINTER))

			value = (double)(int)(long)val.Pointer;
		else
			value = (double)val.Number;
	}

	return read;
}

int FormattingStream::ReadChar(char & value)
{
	int read = 0;
	String string;
	read = ReadString(string);
	value = string.Data[0];
	return read;
}

int FormattingStream::Write(String & string)
{
	return Stream->Write(string);
}

int FormattingStream::Write(Value & value)
{

	if (Index < 0 || Index > Specifiers.Length()-1) return 0;

	int position = Stream->Position;

	if (Index == 0)
	{

		String before = Formatting::Before(Index);
		if (!before.IsEmpty() && Stream->Write(before) < before.Length())
		{
			Index = -1;
			return Stream->Position-position;
		}
	}

	Specifier & specifier = Specifiers[Index];

	String format = specifier.Format;

	String string;
	switch(value)
	{
	case Value::TYPE_STRING:
		string.Format(format.Print(),value.String.Print());
		break;
	case Value::TYPE_NUMBER:
		{

			if (specifier.Is(Specifier::TYPE_FORMAT_d|Specifier::TYPE_FORMAT_i))
				string.Format(format.Print(),(specifier.Is(Specifier::MODIFIER_h)?value.Number.SignedShort():value.Number.SignedInt()));
			else
			if (specifier.Is(Specifier::TYPE_FORMAT_o|Specifier::TYPE_FORMAT_u|Specifier::TYPE_FORMAT_x|Specifier::TYPE_FORMAT_X))
				string.Format(format.Print(),(specifier.Is(Specifier::MODIFIER_h)?value.Number.UnsignedShort():value.Number.UnsignedInt()));
			else
			if (specifier.Is(Specifier::TYPE_FORMAT_f|Specifier::TYPE_FORMAT_e|Specifier::TYPE_FORMAT_E|Specifier::TYPE_FORMAT_g|Specifier::TYPE_FORMAT_G))
				string.Format(format.Print(),value.Number.Double());

		}
		break;
	case Value::TYPE_POINTER:
		string.Format(format.Print(),value.Pointer);
		break;
	}

	Stream->Write(string);

	String after = Formatting::After(Index);
	if (!after.IsEmpty() && Stream->Write(after) < after.Length())
	{
		Index = -1;
		return Stream->Position-position;
	}

	++Index;
	return Stream->Position-position;
}

int FormattingStream::WriteInteger(int & value)
{

	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int FormattingStream::WriteString(String & value)
{
	Value val;
	val = Value::TYPE_STRING;
	val.String = value;
	return Write(val);
}

int FormattingStream::WriteDouble(double & value)
{
	Value val;
	val = Value::TYPE_NUMBER;
	val.Number = value;
	return Write(val);
}

int FormattingStream::WriteChar(char & value)
{
	Value val;
	val = Value::TYPE_STRING;
	val.String = value;
	return Write(val);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Format & operator << (String & string, Format & format)
{
	format.Formatting.Index = 0;
	format.Formatting.Format(string);
	format.Formatting.Construct(Formatting::MODE_WRITE);
	return format;
}

Format & operator >> (String & string, Format & format)
{
	format.Formatting.Index = 0;
	format.Formatting.Format(string);
	format.Formatting.Construct(Formatting::MODE_READ);
	return format;
}

Format & operator << (const String & string, const Format & format)
{
	((Format&)format).Formatting.Index = 0;
	((Format&)format).Formatting.Format((String &)string);
	((Format&)format).Formatting.Construct(Formatting::MODE_WRITE);
	return (Format&)format;
}

Format & operator >> (const String & string, const Format & format)
{
	((Format&)format).Formatting.Index = 0;
	((Format&)format).Formatting.Format((String &)string);
	((Format&)format).Formatting.Construct(Formatting::MODE_READ);
	return (Format&)format;
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
