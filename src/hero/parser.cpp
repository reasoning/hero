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


#include "hero/hero.h"
#include "hero/parser.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




const char * Handler::Descriptions[3] = {"ERROR","FAILURE","WARNING"};

Handler::Handler()
{
	
}



Handler::~Handler()
{
	
}




void Handler::Error(const char *message,...)
{
	va_list args;
	va_start(args,message);
	Catch(HANDLER_ERROR,message,args);
	va_end(args);	
}

void Handler::Failure(const char *message,...)
{
	va_list args;
	va_start(args,message);
	Catch(HANDLER_FAILURE,message,args);
	va_end(args);	
}

void Handler::Warning(const char *message,...)
{
	va_list args;
	va_start(args,message);
	Catch(HANDLER_WARNING,message,args);
	va_end(args);	
}



void Handler::Throw(const unsigned int type,const char * message,...)
{
	va_list args;
	va_start(args,message);
	Catch(type,message,args);
	va_end(args);
}


void Handler::Catch(const unsigned int type,const char * message,va_list args)
{
	switch(type)
	{
	case HANDLER_WARNING:
		#ifdef HERO_PRINT_WARNING
		OutputWarning("");
		vfprintf(HERO_HANDLE_OUTPUT,message,args);
		OutputPrint("\n");
		#endif
		break;
	case HANDLER_ERROR:
		#ifdef HERO_PRINT_ERROR
		Raise("");
		vfprintf(HERO_HANDLE_OUTPUT,message,args);
		OutputPrint("\n");
		#endif
		break;
	case HANDLER_FAILURE:
		#ifdef HERO_PRINT_FAILURE
		OutputFailure("");
		vfprintf(HERO_HANDLE_OUTPUT,message,args);
		OutputPrint("\n");
		#endif
		break;
	}

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char * StreamScanner::At(int n, int size)
{
	if (Token.Offset > 0 && Token.Offset >= Token.Size) return 0;

	n+=size;
	if (Token.Data == 0 || Token.Offset+n > Token.Size)
	{
		n -= Token.Size-Token.Offset;
		Token.Reserve(n);			
		
		int read = Stream->Read(Token.Data+Token.Size,n);
		Token.Size += read;
		Token.Term();
		if (read < n)
		{			
			Token.Offset = Token.Size;
			return 0;
		}
	}

    return Token.Data+Token.Offset+n-size;
}

bool StreamScanner::Mark(class Token & token) 
{
	token.Release();
	if (Token.Offset > 0 && Token.Offset >= Token.Size) return false;
	token.Position = Token.Position+Token.Offset;
	token.Offset = 0xFFFFFFFF;
	Tokens.Insert(&token);
	return true;
}

bool StreamScanner::Trap(class Token & token, int offset)
{
	
	
	
	
	if (token.Offset != 0xFFFFFFFF || Token.Offset > 0 && Token.Offset+offset+1 > Token.Size) return false;
	if (token.Position < Token.Position) return false;

	
	
	char * data = Token.Data + (token.Position-Token.Position) + offset + 1;
	int size = (Token.Data+Token.Offset) + offset + 1 - data; 
	token.Construct(data,size);

	Tokens.Remove(&token);

	class Token * minimum=0;
	Iterator<class Token *> iterator = Tokens.Iterate();
	for (iterator.Forward();iterator.Has();iterator.Move())
	{
		if (!minimum || iterator()->Position < minimum->Position)
			minimum = iterator();
	}

	if (minimum)
	{
		int difference = minimum->Position - Token.Position;
		if (difference > 0)
		{
			Token.Left(-difference);
			Token.Position += difference;
			Token.Offset -= difference;
		}
	}

	return true;
}

bool StreamScanner::Accept()
{
	if (Token.Offset > 0 && Token.Offset > Token.Size) return false;

	if (Token.Data && (Token.Data[Token.Offset] == Characters::CarriageReturn || Token.Data[Token.Offset] == Characters::LineFeed) )
	{
		++Token.Line;
		Token.Column=0;
	}
	return Next();
}

bool StreamScanner::Accept(int n)
{
	if (Token.Offset > 0 && Token.Offset >= Token.Size) return false;

	while(n-- > 0)
	{
		if (Token.Data && (Token.Data[Token.Offset] == Characters::CarriageReturn || Token.Data[Token.Offset] == Characters::LineFeed) )
		{
			++Token.Line;
			Token.Column=0;
		}
		if (!Next()) return false;
	}
	
	return true;
}


bool StreamScanner::Next(int n)
{
	if (Token.Offset > 0 && Token.Offset >= Token.Size) return false;


	if (Tokens.Length() == 0)
	{
		
		
		Token.Reduce();
		
		
		
		

		
		
		
	}

	if (Token.Data == 0 || Token.Offset+n >= Token.Size)
	{
		int size = n;
		if (Token.Data)
			size -= (Token.Data+Token.Size) - (Token.Data+Token.Offset+1);
		
		
		
		
		
		

		Token.Reserve(size);
		int read = Stream->Read(Token.Data+Token.Size,size);
		
		if (read < size && Token.Offset+n != Token.Size)
		{
			
			
			
			
			
			
			

			

			Token.Size += read;
			Token.Term();

			Token.Column += Token.Size - Token.Offset;
			Token.Offset = Token.Size;
			return false;
		}
		else
		{
			Token.Size += read;
			Token.Term();
		}
	}

	Token.Offset += n;
	Token.Column += n;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Parser::Parser(class Scanner & scanner):
	Scanner(&scanner)
{
	Token.Release();

	#ifdef HERO_USING_TRACY
	ZoneScoped;
	#endif

}

Parser::Parser(class Scanner * scanner):
	Scanner(scanner)
{
	Token.Release();
	
	#ifdef HERO_USING_TRACY
	ZoneScoped;
	#endif
}

Parser::~Parser()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Parser::SkipLine()
{
	while (!Eof() && !(Is(Characters::CarriageReturn) || Is(Characters::LineFeed)) ) Next();
	return SkipNewline();
}

bool Parser::ParseLine()
{
	Mark();
	while (!Eof() && !(Is(Characters::CarriageReturn) || Is(Characters::LineFeed)) ) Next();
	SkipNewline();
	Trap();
	return true;
}

bool Parser::SkipNewline()
{
	
	if (IsNewline())
	{
		char sentinel = *At();
		Accept();
		if (At() && sentinel != *At() && IsNewline()) 
			Next();

		return true;
	}	

	return false;
}

bool Parser::ParseNewline()
{
	
	
	
	
	

	if ( IsNewline() )
	{	
		Mark();
		SkipNewline();
		Trap();
		return true;
	}

	return false;
}

bool Parser::SkipHyphenation()
{
	
	if (SkipWord())
	{
		while (Is("-") && (IsAlpha(1) || SkipNewline()))
		{
			Next();
			SkipWord();
		}

		return true;
	}

	return false;
}


bool Parser::SkipUntil(char *data, int size)
{
	if (!Eof() && !Is(data,size))
	{
		while (!Eof() && !Is(data,size))
			Next();

		return true;
	}

	return false;
}


bool Parser::SkipUntilAny(const Substring tokens[], int length)
{
	while (length-- > 0)
		if (SkipUntilAny(tokens[length]))
			return true;

	return false;
}


bool Parser::SkipUntilAny(char * tokens, int length)
{
	if (!Eof() && !IsAny(tokens,length))
	{
		while (!Eof() && !IsAny(tokens,length))
			Next();

		return true;
	}

	return false;
}

bool Parser::ParseUntil(char *data, int size)
{
	if (!Eof() && !Is(data,size))
	{
		Mark();
		while (!Eof() && !Is(data,size))
			Next();
		Trap();
		return true;
	}

	return false;
}


bool Parser::ParseUntil(const Callback<bool> & callback)
{
	if (!Eof() && !callback())
	{
		Mark();
		while (!Eof() && !callback()) 
			Next();
		Trap();
		return true;
	}

	return false;
}



bool Parser::ParseUntil(const Callback<bool,Parser> & callback)
{
	if (!Eof() && !callback(*this))
	{
		Mark();
		while (!Eof() && !callback(*this)) 
			Next();
		Trap();
		return true;
	}
	
	return false;
}


bool Parser::ParseUntilAny(const Substring tokens[], int length)
{
	while (length-- > 0)
		if (ParseUntilAny(tokens[length]))
			return true;

	return false;
}


bool Parser::ParseUntilAny(char * tokens, int length)
{
	if (!Eof() && !IsAny(tokens,length))
	{	
		Mark();
		while (!Eof() && !IsAny(tokens,length))
			Next();
		Trap();

		return true;
	}

	return false;
}


bool Parser::ParseHyphenation()
{
	
	
	
	
	
	
	
	
	

	if (IsAlpha())
	{
		Mark();
		SkipHyphenation();
		Trap();
		return true;
	}

	return false;
}

bool Parser::SkipWord()
{
	if (IsAlpha())
	{
		while(IsAlpha()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseWord()
{
	if (IsAlpha())
	{
		Mark();
		while(IsAlpha()) Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipHex()
{
	if (IsHex())
	{
		while(IsHex()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseHex()
{
	if (IsHex())
	{
		Mark();
		while(IsHex()) Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipOctal()
{
	if (IsOctal())
	{
		while(IsOctal()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseOctal()
{
	if (IsOctal())
	{
		Mark();
		while(IsOctal()) Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipDecimal()
{
	if (IsDecimal())
	{
		while(IsDecimal()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseDecimal()
{
	if (IsDecimal())
	{
		Mark();
		while(IsDecimal()) Next();
		Trap();
		return true;
	}
	return false;
}


bool Parser::SkipNumber()
{
	if (IsNumeric())
	{
		while(IsNumeric()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseNumber()
{
	if (IsNumeric())
	{
		Mark();
		while(IsNumeric()) Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipInteger()
{
	
	
	
	
	
	

	if (IsAny("+-"))
		Next();

	if (IsRange(1,'1','9'))
		return SkipDecimal();
	else
	if (Is('0') && IsRange(1,'0','7'))
		return SkipOctal();
	else
	if (Is('0') && IsCaseless(1,'x') && IsHex(2))
		return SkipHex();

	return false;
}

bool Parser::SkipInteger(long long & value, int radix)
{
	if (SkipInteger())
	{
		value = Token.Integer(radix);
		return true;
	}
		
	return false;
}


bool Parser::ParseInteger()
{
	Mark();
	SkipInteger();
	Trap();

	return !Token.IsEmpty();
}

bool Parser::ParseInteger(long long & value, int radix)
{
	Mark();
	SkipInteger(value,radix);
	Trap();

	return !Token.IsEmpty();
}

bool Parser::SkipReal()
{
	
	
	

	

	bool numeric = false;

	if (IsAny("+-"))
		Next();

	while (!Eof() && IsNumeric())
	{
		Next();
		numeric = true;
	}

	if (!Eof() && Is('.') && IsNumeric(1))
	{
		numeric = true;
		Next();
		while (!Eof() && IsNumeric())
			Next();
	}

	if (numeric && !Eof() && IsCaseless('e') && (IsAny(1,"+-") || IsNumeric(1)))
	{
		Next();

		if (IsAny("+-"))
			Next();

		while (!Eof() && IsNumeric())
			Next();
	}

	return numeric;
}

bool Parser::SkipReal(double & value)
{
	
	
	if (SkipReal())
	{
		value = Token.Real();
		return true;
	}
		
	return false;
}

bool Parser::ParseReal()
{
	Mark();
	SkipReal();
	Trap();
	
	return !Token.IsEmpty();
}


bool Parser::ParseReal(double & value)
{	
	Mark();
	SkipReal(value);
	Trap();

	return !Token.IsEmpty();
}

bool Parser::SkipBoolean()
{
	bool value;
	return SkipBoolean(value);
}

bool Parser::SkipBoolean(bool & value)
{	
	if ((IsCaseless("true") && Skip(4)) || (IsNumeric() && Skip("1")))
	{
		value = true;
		return true;
	}
	
	if ((IsCaseless("false") && Skip(5)) || (IsNumeric() && Skip("0")))
	{
		value = false;
		return true;
	}	
	
	return false;
}
	
	
bool Parser::ParseBoolean()
{
	bool value;
	return ParseBoolean(value);
}

bool Parser::ParseBoolean(bool & value)
{
	Mark();
	SkipBoolean(value);
	Trap();

	return !Token.IsEmpty();	
}

bool Parser::SkipIdentifier()
{
	if (IsIdentifier())
	{
		while(IsIdentifier()) Next();
		return true;
	}
	return false;
}

bool Parser::ParseIdentifier()
{
	if (IsIdentifier())
	{
		Mark();
		while(IsIdentifier()) Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipPunctuation()
{
	if (IsPunctuation())
	{
		while(IsPunctuation())
			Next();
		return true;
	}
	return false;
}

bool Parser::ParsePunctuation()
{
	if (IsPunctuation())
	{
		Mark();
		while(IsPunctuation())
			Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipSymbol()
{
	if (IsSymbol())
	{
		Next();
		return true;
	}
	return false;
}

bool Parser::ParseSymbol()
{
	if (IsSymbol())
	{
		Mark();
		Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipDelimiter()
{
	if (IsDelimiter())
	{
		Next();
		return true;
	}
	return false;
}

bool Parser::ParseDelimiter()
{
	if (IsDelimiter())
	{
		Mark();
		Next();
		Trap();
		return true;
	}
	return false;
}

bool Parser::SkipWhitespace()
{
	if (IsWhitespace())
	{
		while (!Eof() && IsWhitespace()) Accept();
		return true;
	}
	return false;
}

bool Parser::ParseWhitespace()
{
	if ( Eof() || !IsWhitespace() ) return false;

	Mark();
	SkipWhitespace();
	Trap();
	return true;
}

bool Parser::ParseLiteral()
{
	
	return ParseString() || ParseIdentifier() || ParseNumber() || ParseReal() || ParseWord();
}

bool Parser::ParseQuotation()	
{
	
	if (Is(Characters::SingleQuote) || Is(Characters::DoubleQuote))
	{
		char sentinel = *At();

		Next();
		Mark();

		while (!Is(sentinel))
		{
			if (Eof())
				return false;

			Accept();
		}

		Trap();
		Next();

		return true;
	}

	return false;
}



bool Parser::ParseOuter(char open, char close, char eof, int depth)
{
	return ParseOuter(Substring(&open,1),Substring(&close,1),Substring(&eof,1),depth);
}


bool Parser::ParseOuter(const char *open, const char *close, const char *eof, int depth)
{
	return ParseOuter(Substring(open),Substring(close),Substring(eof),depth);
}

bool Parser::ParseOuter(const Substring &open, const Substring &close, const Substring &eof, int depth)
{
	
	

	
	


	if (Is(open))
	{
		Mark();
		Skip(open.Size);

		if (depth > 0)
		{
			int set=1;

			do
			{
				if (Is(open))
				{
					++set;
					Skip(open.Size);
				}
				else
				if (Is(close))
				{
					--set;
					Skip(close.Size);
				}
				else
				{
					Accept();
				}

				if (Eof() || Is(eof))
					return false;
			}
			while (set > 0 && set < depth);
		}
		else
		{
			do
			{
				if (Eof() || Is(eof))
					return false;

				Accept();
			}
			while ( !Is(close));

			Skip(close.Size);
		}


		Trap();

		return true;
	}

	return false;

}


bool Parser::ParseInner(char open, char close, char eof, int depth)
{
	return ParseInner(Substring(&open,1),Substring(&close,1),Substring(&eof,1),depth);
}

bool Parser::ParseInner(const char *open, const char *close, const char *eof, int depth)
{
	return ParseInner(Substring(open),Substring(close),Substring(eof),depth);
}

bool Parser::ParseInner(const Substring &open, const Substring &close, const Substring &eof, int depth)
{
	if (Is(open))
	{
		Skip(open.Size);
		Mark();	

		if (depth > 0)
		{
			int set=1;

			do
			{
				if (Is(open))
				{
					++set;
					Skip(open.Size);
				}
				else
				if (Is(close))
				{
					--set;					
					if (set == 0)
						Trap();

					Skip(close.Size);
				}
				else
				{
					Accept();
				}

				if (Eof() || Is(eof))
					return false;
			}
			while (set > 0 && set < depth);			
		}
		else
		{
			do
			{
				if (Eof() || Is(eof))
					return false;

				Accept();
			}
			while ( !Is(close));

			Trap();
			Skip(close.Size);
		}


		return true;
	}

	return false;

}

bool Parser::SkipAny(const Substring tokens[], int length)
{
	while (length-- > 0)
		if (SkipAny(tokens[length]))
			return true;

	return false;
}

bool Parser::SkipAny(char * tokens, int length)
{
	if (IsAny(tokens,length))
	{	
		while (IsAny(tokens,length))
			Next();

		return true;
	}

	return false;
}

bool Parser::ParseAny(const Substring tokens[], int length)
{
	while (length-- > 0)
		if (ParseAny(tokens[length]))
			return true;

	return false;
}


bool Parser::ParseAny(char * tokens, int length)
{
	if (IsAny(tokens,length))
	{	
		Mark();
		while (IsAny(tokens,length))
			Next();
		Trap();

		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BinaryScanner::Seek(int position, int origin)				
{
	StreamScanner::Seek(position,origin);
	((class Stream*)BinaryParser)->Position = Token.Position;
	return Token.Position;
}



bool BinaryScanner::Next(int n)
{
	bool next = StreamScanner::Next(n);
	((class Stream*)BinaryParser)->Position = Token.Position;
	return next;
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
