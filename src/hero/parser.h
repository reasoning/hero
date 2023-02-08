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




#include "hero/string.h"
#include "hero/object.h"
#include "hero/stream.h"
#include "hero/error.h"
#include "hero/binary.h"

#include "hero/structure.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Handler
{
public:

	
	

	const static int TypeBoundary	= (1)<<2;
	const static int TypeReserved	= 2;
	const static int TypeMask		= 3;

	static const char * Descriptions[3];

	
	
	
	
	
	

	enum HandlerTypes
	{
		HANDLER_ERROR		=(0),
		HANDLER_FAILURE		=(1),
		HANDLER_WARNING		=(1)<<1,
			
		
		
		
		
		
		
	};

	
	Flags<32>	Type;

	Handler();
	
	
	~Handler();
	
	inline bool IsFailure()	{return Type.Is(HANDLER_FAILURE);};
	inline bool IsError()	{return Type.Is(HANDLER_ERROR);};
	inline bool IsWarning()	{return Type.Is(HANDLER_WARNING);};
	
	
	inline const char * Description(unsigned int type){return Descriptions[type&0x03];};

	
	
	

	
	void Error(const char *message,...);
	void Failure(const char *message,...);
	void Warning(const char *message,...);

	
	void Throw(const unsigned int type,const char * message,...);
	
	
	virtual void Catch(const unsigned int type,const char * message,va_list args);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class Token : public String
{
public:

	int Position;	
	int Offset;		

	int Line;		
	int Column;		

	Token(const Token & token):Position(token.Position),Offset(token.Offset),Line(token.Line),Column(token.Column)
	{
		if (token.Allocated)
		{
			Construct(token.Data,token.Size);
		}
		else
		{
			Data = token.Data;
			Size = token.Size;
		}
	}


	Token(char * data, int size):Position(0),Offset(0),Line(0),Column(0)
	{
		Data = data;
		Size = size;
	}

	Token():Position(0),Offset(0),Line(0),Column(0)
	{
	}

	~Token()
	{
	}

	void Release()
	{
		Line = Column = Position = Offset = 0;
		String::Release();
	}

	void Reduce()
	{
		
		

		
		
		
		
		
		

		int trim = Offset;
		Left(-trim);
		Position += trim;
		Offset -= trim;
	}

	Token & operator = (const Token & token)
	{
		Position = token.Position;
		Offset = token.Offset;

		Line = token.Line;
		Column = token.Column;		

		if (token.Allocated)
		{
			Construct(token.Data,token.Size);
		}
		else
		{
			Data = token.Data;
			Size = token.Size;
		}

		return *this;
	}
};
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Scanner
{
public:

	

	class Token Token;

	Scanner(char * data, int size):Token(data,size) {}
	Scanner(const Substring & sequence):Token(sequence.Data,sequence.Size) {}
	Scanner() {}

	bool operator () () {return At()!=0;}
	bool operator () (int size) {return At(size)!=0;}
	bool operator () (int n, int size) {return At(n,size)!=0;}

	virtual char * At()=0;
	virtual char * At(int size)=0;
	virtual char * At(int n, int size)=0;

	virtual bool Accept() {return Accept(1);}
	virtual bool Accept(int n)=0;

	virtual bool Next() {return Next(1);}
	virtual bool Next(int n)=0;

	virtual bool Abort()=0;

	virtual bool Mark(class Token & token)=0;
	virtual bool Trap(class Token & token, int offset)=0;

	virtual bool Mark(Substring & substring)=0;
	virtual bool Trap(Substring & substring, int offset)=0;

	virtual bool Load(class Token & token)=0;
	virtual bool Store(class Token & token)=0;

	virtual int Position()=0;
	virtual int Column()=0;
	virtual int Line()=0;
};


class StringScanner : public Scanner
{
public:

	

	StringScanner(const char * data):Scanner((char*)data,String::Length(data))
	{
	}

	StringScanner(char * data, int size):Scanner(data,size)
	{
	}
	
	StringScanner(const Substring & sequence):Scanner(sequence.Data,sequence.Size)
	{
	}

	StringScanner()
	{
	}

	void Release()
	{
		Token.Release();
	}

	char * At() {return At(0,1);}
	char * At(int size) {return At(0,size);}
	char * At(int n, int size)
	{
		if (Token.Data == 0) return 0;
		return (Token.Offset+n+size <= Token.Size)?Token.Data+Token.Offset+n:0;
	}

	bool Mark(Substring & substring) 
	{
		substring.Release();
		if (Token.Data == 0 || Token.Offset >= Token.Size) return false;
		substring.Data = Token.Data+Token.Offset; 
		return true;
	}

	bool Trap(Substring & substring, int offset) 
	{
		if (Token.Data == 0 || Token.Offset+offset+1 > Token.Size || substring.Data < Token.Data || substring.Data >= Token.Data+Token.Size) return false;
		substring.Size = Token.Data+Token.Offset+offset+1-substring.Data; return true;
	}

	bool Mark(class Token & token) 
	{
		token.Release();
		if (Token.Data == 0 || Token.Offset >= Token.Size) return false;
		token.Data = Token.Data+Token.Offset; return true;
	}

	bool Trap(class Token & token, int offset) 
	{
		if (Token.Data == 0 || Token.Offset+offset+1 > Token.Size || token.Data < Token.Data || token.Data >= Token.Data+Token.Size) return false;
		token.Size = Token.Data+Token.Offset+offset+1-token.Data; return true;
	}

	bool Load(class Token & token) 
	{
		Token = token; return true;
	}

	bool Store(class Token & token)
	{
		token = Token; return true;
	}

	
	
	
	
	bool Accept()
	{
		
		if (Token.Data[Token.Offset] == Characters::CarriageReturn || Token.Data[Token.Offset] == Characters::LineFeed )
		{
			++Token.Line;
			Token.Column=0;
		}
		++Token.Offset;
		++Token.Position;
		return true;
	}

	bool Accept(int n)
	{
		while (n-- > 0)
		{
			if (Token.Data[Token.Offset] == Characters::CarriageReturn || Token.Data[Token.Offset] == Characters::LineFeed )
			{
				++Token.Line;
				Token.Column=0;
			}
			++Token.Offset;
			++Token.Position;
		}

		return true;
	}
	


	bool Next() 
	{
		if (Token.Data == 0 || Token.Offset >= Token.Size) return false;
		++Token.Offset;
		++Token.Position;
		++Token.Column;
		return true;
	}

	bool Next(int n) 
	{
		if (Token.Data == 0 || Token.Offset+n > Token.Size) return false;

		Token.Offset+=n;
		Token.Position+=n;
		Token.Column+=n;
		return true;
	}

	bool Abort() {Token.Offset = Token.Size;return true;}

	int Position() {return Token.Position;}
	int Column() {return Token.Column;}
	int Line() {return Token.Line;}
};


class StreamScanner : public Scanner
{
public:

	class Stream * Stream;
	ArraySet<class Token *> Tokens;
	
	StreamScanner(class Stream & stream):Stream(&stream)
	{
	}

	StreamScanner(class Stream * stream):Stream(stream)
	{
	}

	StreamScanner():Stream(0) {}
	
	~StreamScanner()
	{
	}
	
	int Seek(int position, int origin = -1)				
	{
		
		
		
		

		Token.Release();
		
		if (Stream->IsAbsolute())
			Token.Position = Stream->Seek(position,origin);

		return Token.Position;
	}

	char * At() {return At(0,1);}
	char * At(int size) {return At(0,size);}
	char * At(int n, int size);

	bool Mark(class Substring & substring) {substring.Release();return false;}
	bool Trap(class Substring & substring, int offset) {return false;}

	bool Mark(class Token & token);
	bool Trap(class Token & token, int offset);

	bool Load(class Token & token) 
	{
		Token = token;
		return true;
	}

	bool Store(class Token & token) 
	{
		token = Token;
		return true;
	}

	bool Accept();
	bool Accept(int n);

	bool Next() {return Next(1);}
	bool Next(int n);

	bool Abort() {Token.Offset = Token.Size+1;return true;}
	
	int Position() {return Token.Position+Token.Offset;}
	int Column() {return Token.Column;}
	int Line() {return Token.Line;}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Parser
{
public:

	class Scanner * Scanner;
	class Token	Token;


	Parser(class Scanner * scanner);
	Parser(class Scanner & scanner);
	Parser();
	~Parser();

	void Release();

	bool IsEmpty();

	int Position() {return Scanner->Position();}
	int Column() {return Scanner->Column();}
	int Line() {return Scanner->Line();}

	
	
	
	

	char * At() {return Scanner->At();}
	char * At(int size)	{return Scanner->At(size);}
	char * At(int n, int size) {return Scanner->At(n,size);}

	bool Abort()			{return Scanner->Abort();}
	bool Next()				{return Scanner->Next();}
	
	
	bool Accept()		{return Scanner->Accept();}

	
	bool Load(class Token & token) {return Scanner->Load(token);}
	bool Store(class Token & token) {return Scanner->Store(token);}
	class Token State() {return Scanner->Token;}

	bool Mark() {return Scanner->Mark(Token);}					
	bool Mark(class Token & token) {return Scanner->Mark(token);}
	bool Mark(Substring & substring) {return Scanner->Mark(substring);}
	bool Trap(int offset = -1) {return Scanner->Trap(Token,offset);}
	bool Trap(class Token & token, int offset = -1) {return Scanner->Trap(token,offset);}
	bool Trap(Substring & substring, int offset = -1) {return Scanner->Trap(substring,offset);}

	bool IsWhitespace(int n)		{return (*Scanner)(n,1) && Characters::IsWhitespace(*At(n,1));}
	bool IsAlphanumeric(int n)		{return (*Scanner)(n,1) && Characters::IsAlphanumeric(*At(n,1));}
	bool IsAlpha(int n)				{return (*Scanner)(n,1) && Characters::IsAlpha(*At(n,1));}
	bool IsHex(int n)				{return (*Scanner)(n,1) && Characters::IsHex(*At(n,1));}
	bool IsOctal(int n)				{return (*Scanner)(n,1) && Characters::IsOctal(*At(n,1));}
	bool IsDecimal(int n)			{return (*Scanner)(n,1) && Characters::IsNumeric(*At(n,1));}
	bool IsNumeric(int n)			{return (*Scanner)(n,1) && Characters::IsNumeric(*At(n,1));}
	bool IsDigit(int n)				{return (*Scanner)(n,1) && Characters::IsNumeric(*At(n,1));}
	bool IsAscii(int n)				{return (*Scanner)(n,1) && Characters::IsAscii(*At(n,1));}
	bool IsControl(int n)			{return (*Scanner)(n,1) && Characters::IsControl(*At(n,1));}
	bool IsPrintable(int n)			{return (*Scanner)(n,1) && Characters::IsPrintable(*At(n,1));}
	bool IsIdentifier(int n)		{return (*Scanner)(n,1) && Characters::IsIdentifier(*At(n,1));}
	bool IsPunctuation(int n)		{return (*Scanner)(n,1) && Characters::IsPunctuation(*At(n,1));}
	bool IsSymbol(int n)			{return (*Scanner)(n,1) && Characters::IsSymbol(*At(n,1));}
	bool IsDelimiter(int n)			{return (*Scanner)(n,1) && Characters::IsDelimiter(*At(n,1));}
	bool IsUppercase(int n)			{return (*Scanner)(n,1) && Characters::IsUppercase(*At(n,1));}
	bool IsLowercase(int n)			{return (*Scanner)(n,1) && Characters::IsLowercase(*At(n,1));}
	bool IsNewline(int n)			{return (*Scanner)(n,1) && Characters::IsNewline(*At(n,1));}

	
	
	
	
	bool IsWhitespace()		{return IsWhitespace(0);}
	bool IsAlphanumeric()	{return IsAlphanumeric(0);}
	bool IsAlpha()			{return IsAlpha(0);}
	bool IsHex()			{return IsHex(0);}
	bool IsOctal()			{return IsOctal(0);}
	bool IsDecimal()		{return IsNumeric(0);}
	bool IsNumeric()		{return IsNumeric(0);}
	bool IsDigit()			{return IsNumeric(0);}
	bool IsAscii()			{return IsAscii(0);}
	bool IsControl()		{return IsControl(0);}
	bool IsPrintable()		{return IsPrintable(0);}
	bool IsIdentifier()		{return IsIdentifier(0);}
	bool IsPunctuation()	{return IsPunctuation(0);}
	bool IsSymbol()			{return IsSymbol(0);}
	bool IsDelimiter()		{return IsDelimiter(0);}
	bool IsUppercase()		{return IsUppercase(0);}
	bool IsLowercase()		{return IsLowercase(0);}
	bool IsNewline()		{return IsNewline(0);}

	

	
	
	
	bool Eof()						
	{ 
		return !(*Scanner)(0,1);
	}

	bool Eof(const int n)
	{
		return !(*Scanner)(n,1);
	}


	
	
	

	
	
	
	

	bool Is(char data) {return (*Scanner)() && Characters::Equals(*At(),data);}
	bool Is(const Substring & data) {return Is(data.Data,data.Size);}
	bool Is(const char *data) {return Is((char*)data,String::Length(data));}	
	bool Is(char *data, int size)
	{
		
		
		


		return (*Scanner)(size) && Characters::Equals(At(size),data,size);
	}

	bool Is(const int n, char data) {return Is(n,&data,1);}
	bool Is(const int n, char *data)	{return Is(n,data,String::Length(data));}
	bool Is(const int n, char *data, int size)
	{		
		return (*Scanner)(n,size) && Characters::EqualsCaseless(At(n,size),data,size);
	}

	bool IsCaseless(char data)	{return (*Scanner)() && Characters::EqualsCaseless(*At(),data);}
	bool IsCaseless(const Substring & data) {return IsCaseless(data.Data,data.Size);}
	bool IsCaseless(const char *data) {return IsCaseless((char*)data,String::Length(data));}	
	bool IsCaseless(char *data, int size)
	{	
		return (*Scanner)(size) && Characters::EqualsCaseless(At(size),data,size);
	}

	bool IsCaseless(const int n, char data)	{return (*Scanner)(n,1) && Characters::EqualsCaseless(*At(n,1),data);}
	bool IsCaseless(const int n, const Substring & data) {return IsCaseless(n,data.Data,data.Size);}
	bool IsCaseless(const int n, const char *data) {return IsCaseless(n,(char*)data,String::Length(data));}	
	bool IsCaseless(const int n, char *data, int size)
	{	
		return (*Scanner)(n,size) && Characters::EqualsCaseless(At(n,size),data,size);
	}
	
	
	
	
	

	bool IsAny(const Substring & tokens) {return IsAny(tokens.Data,tokens.Size);}
	bool IsAny(const char *tokens) {return IsAny((char*)tokens, Substring::Length(tokens));}

	bool IsAny(char *tokens, int length)	
	{
		if ((*Scanner)())
		{
			for (int i=0;i<length;++i)
				if (*At() == tokens[i]) return true;
		}
		return false;
	}

	bool IsAny(const int n, const Substring & tokens) {return IsAny(n,tokens.Data,tokens.Size);}
	bool IsAny(const int n, const char *tokens) {return IsAny(n,(char*)tokens, Substring::Length(tokens));}
	bool IsAny(const int n, char *tokens, int length)	
	{
		if ((*Scanner)(n,1))
		{
			for (int i=0;i<length;++i)
				if (*At(n,1) == tokens[i]) return true;
		}
		return false;
	}


	bool IsAnyCaseless(const Substring & tokens) {return IsAnyCaseless(tokens.Data,tokens.Size);}
	bool IsAnyCaseless(const char *tokens) {return IsAnyCaseless((char*)tokens, Substring::Length(tokens));}
	bool IsAnyCaseless(char *tokens, int length)	
	{
		if ((*Scanner)())
		{
			for (int i=0;i<length;++i)
				if (Characters::EqualsCaseless(*At(),tokens[i])) return true;
		}
		return false;
	}


	bool IsAnyCaseless(const int n, const Substring & tokens) {return IsAnyCaseless(n,tokens.Data,tokens.Size);}
	bool IsAnyCaseless(const int n, const char *tokens) {return IsAnyCaseless(n,(char*)tokens, Substring::Length(tokens));}
	bool IsAnyCaseless(const int n, char *tokens, int length)	
	{
		if ((*Scanner)(n,1))
		{
			for (int i=0;i<length;++i)
				if (Characters::EqualsCaseless(*At(n,1),tokens[i])) return true;
		}
		return false;
	}

	bool IsRange(const char lower, const char upper )
	{
		Assert(lower < upper);
		return (*Scanner)() && (*At() >= lower && *At() <= upper);
	}

	bool IsRange(const int n, const char lower, const char upper )
	{
		Assert(lower < upper);
		return (*Scanner)(n,1) && (*At(n,1) >= lower && *At(n,1) <= upper);
	}

	bool SkipAny(const Substring tokens[], int length);
	bool SkipAny(const Substring & tokens) {return SkipAny(tokens.Data,tokens.Size);}
	bool SkipAny(const char * tokens) {return SkipAny((char*)tokens,String::Length(tokens));}
	bool SkipAny(char * tokens, int length);
	
	bool Skip(const Substring & sequence) {return Skip(sequence.Data,sequence.Size);}
	bool Skip(const char *data) {return Skip((char*)data,String::Length(data));}	
	bool Skip(char *data, int size) {return Is(data,size) && Skip(size);}	
	bool Skip(const char data) {return Is(data) && Skip(1);}

	bool Skip(const int n) {return Scanner->Next(n);}
	

	bool SkipLine();
	bool SkipNewline();
	bool SkipWord();
	
	bool SkipInteger();
	bool SkipInteger(long long & value, int radix=10);
	bool SkipReal();
	bool SkipReal(double & value);
	bool SkipBoolean();
	bool SkipBoolean(bool & value);
	
		
	bool SkipHex();
	bool SkipOctal();
	bool SkipDecimal();
	bool SkipNumber();
	bool SkipSymbol();
	bool SkipIdentifier();
	bool SkipDelimiter();
	bool SkipPunctuation();
	bool SkipWhitespace();
	bool SkipHyphenation();
	
	bool SkipUntil(const Substring & sequence) {return SkipUntil(sequence.Data,sequence.Size);}
	bool SkipUntil(const char *data) {return SkipUntil((char*)data,String::Length(data));}	
	bool SkipUntil(char *data, int size);
	bool SkipUntil(const char data) {return SkipUntil((char*)&data,1);}
	
	bool SkipUntilAny(const Substring tokens[], int length);
	bool SkipUntilAny(const Substring & tokens) {return SkipUntilAny(tokens.Data,tokens.Size);}
	bool SkipUntilAny(char token) {return SkipUntilAny((char*)&token,1);}
	bool SkipUntilAny(const char * tokens) {return SkipUntilAny((char*)tokens,String::Length(tokens));}
	bool SkipUntilAny(char * tokens, int length);

	bool ParseAny(const Substring tokens[], int length);
	bool ParseAny(const Substring & tokens) {return ParseAny(tokens.Data,tokens.Size);}
	bool ParseAny(const char * tokens) {return ParseAny((char*)tokens,String::Length(tokens));}
	bool ParseAny(char * tokens, int length);

	bool ParseUntil(const Substring & sequence) {return ParseUntil(sequence.Data,sequence.Size);}
	bool ParseUntil(const char *data) {return ParseUntil((char*)data,String::Length(data));}	
	bool ParseUntil(char *data, int size);
	bool ParseUntil(const char data) {return ParseUntil((char*)&data,1);}

	
	
	bool ParseUntil(const Callback<bool> & callback);
	
	
	bool ParseUntil(const Callback<bool,Parser> & callback);
	
	bool ParseUntilAny(const Substring tokens[], int length);
	bool ParseUntilAny(const Substring & tokens) {return ParseUntilAny(tokens.Data,tokens.Size);}
	bool ParseUntilAny(char token) {return ParseUntilAny((char*)&token,1);}
	bool ParseUntilAny(const char * tokens) {return ParseUntilAny((char*)tokens,String::Length(tokens));}
	bool ParseUntilAny(char * tokens, int length);

	bool Parse(const Substring & sequence) {return Parse(sequence.Data,sequence.Size);}
	bool Parse(const char *data) {return Parse((char*)data,String::Length(data));}	
	bool Parse(char *data, int size) 
	{
		if (Is(data,size))
		{
			Mark();
			Scanner->Next(size);
			Trap();
			return true;
		}
		return false;
	}	
	
	bool Parse(const char data) 
	{
		if (Is(data))
		{
			Mark();
			Next();
			Trap();
			return true;
		}
		return false;
	}
	
	
	
	

	bool Parse(const int n) {return (*Scanner)(n,1) && Mark() && Scanner->Next(n) && Trap();}
	bool ParseLine();
	bool ParseNewline();
	bool ParseWord();

	bool ParseInteger();
	bool ParseInteger(long long & value, int radix=10);
	bool ParseReal();
	bool ParseReal(double & value);
	bool ParseBoolean();
	bool ParseBoolean(bool & value);
	
	bool ParseHex();
	bool ParseOctal();
	bool ParseDecimal();
	bool ParseNumber();
	bool ParseSymbol();
	bool ParseIdentifier();
	bool ParseDelimiter();
	bool ParsePunctuation();
	bool ParseWhitespace();
	bool ParseQuotation();

	bool ParseHyphenation();

	
		
	
	
	

	bool ParseLiteral();
	bool ParseString() {return ParseQuotation();}


	bool ParseInner(char open, char close, char eof=0, int depth=0);
	bool ParseInner(const Substring &open, const Substring &close, const Substring &eof, int depth=0);
	bool ParseInner(const char *open, const char *close, const char *eof, int depth=0);

	bool ParseOuter(char open, char close, char eof=0, int depth=0);
	bool ParseOuter(const char *open, const char *close, const char *eof, int depth=0);
	bool ParseOuter(const Substring &open, const Substring &close, const Substring &eof, int depth=0);

	
	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class StringParser : public Parser
{
public:
	
	class StringScanner StringScanner;

	StringParser(const char * data):StringScanner(data),Parser(StringScanner)
	{
	}

	StringParser(const Substring & sequence):StringScanner(sequence),Parser(StringScanner)
	{
	}

	StringParser(char * data, int size):StringScanner(data, size),Parser(StringScanner)
	{
	}

	StringParser():Parser(StringScanner)
	{
	}

	void Assign(char *data, int size) 
	{
		
		
		StringScanner.Token.Release();
		StringScanner.Token.Assign(data,size);
	}
	void Assign(const char *data) {Assign((char*)data,String::Length(data));};
	void Assign(const char *start, const char * end) {Assign((char*)start,(end-start+1));};
	void Assign(const Substring & sequence) {Assign(sequence.Data,sequence.Size);};
};


class StreamParser : public Parser
{
public:
	class StreamScanner StreamScanner;

	StreamParser(Stream & stream):StreamScanner(stream),Parser(StreamScanner)
	{
	}

	StreamParser(Stream * stream):StreamScanner(stream),Parser(StreamScanner)
	{
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BinaryParser;

class BinaryScanner : public StreamScanner
{
public:
	
	
	class BinaryParser * BinaryParser;

	
	
	
	BinaryScanner(class BinaryParser * parser, class Stream & stream):StreamScanner(&stream),BinaryParser(parser)
	
	{
	}

	BinaryScanner(class BinaryParser * parser, class Stream * stream):StreamScanner(stream),BinaryParser(parser)
	
	{
	}

	BinaryScanner():BinaryParser(0) {}
	
	~BinaryScanner()
	{
	}
	
	int Seek(int position, int origin = -1);
			
	bool Accept()
	{
		if (Token.Offset > 0 && Token.Offset > Token.Size) return false;

		
		return Next();
	}

	bool Accept(int n)
	{
		if (Token.Offset > 0 && Token.Offset >= Token.Size) return false;
		
		
		while(n-- > 0)
		{
			if (!Next()) return false;
		}
		
		return true;
	}
	
	using StreamScanner::Next;
	bool Next(int n);
	
	
};













class BinaryParser : public Parser, public Stream
{
public:

	
	
	class BinaryStream BinaryStream;
	class BinaryScanner BinaryScanner;
	
	
	

	BinaryParser(Stream & stream):BinaryStream(stream),BinaryScanner(this,BinaryStream),Parser(BinaryScanner)
	
	{
	}

	BinaryParser(Stream * stream):BinaryStream(stream),BinaryScanner(this,BinaryStream),Parser(BinaryScanner)
	
	{
	}
	
	BinaryParser() {}
	~BinaryParser() {}
	
	
	
	
	int Position() {return BinaryScanner.Position();}	
	
	
	using Parser::Is;
	bool Is(bool & data)						{return Is((char*) &data,1);}
	bool Is(char & data)						{return Is((char*) &data,sizeof(char));}
	bool Is(unsigned char & data)				{return Is((char*) &data,sizeof(unsigned char));}
	bool Is(short & data)						{return Is((char*) &data,sizeof(short));}
	bool Is(unsigned short &data)				{return Is((char*) &data,sizeof(unsigned short));}
	bool Is(int & data)							{return Is((char*) &data,sizeof(int));}
	bool Is(unsigned int & data)				{return Is((char*) &data,sizeof(unsigned int));}
	bool Is(long & data)						{return Is((char*) &data,sizeof(long));}
	bool Is(unsigned long & data)				{return Is((char*) &data,sizeof(unsigned long));}
	bool Is(long long & data)					{return Is((char*) &data,sizeof(long long));}
	bool Is(unsigned long long & data)			{return Is((char*) &data,sizeof(unsigned long long));}
	bool Is(float & data)						{return Is((char*) &data,sizeof(float));}
	bool Is(double & data)						{return Is((char*) &data,sizeof(double));}
	bool Is(void * & data)						{return Is((char*) &data,sizeof(void*));}

	using Parser::Parse;
	bool Parse(bool & data)						{return Parse((char*) &data,1);}
	bool Parse(char & data)						{return Parse((char*) &data,sizeof(char));}
	bool Parse(unsigned char & data)			{return Parse((char*) &data,sizeof(unsigned char));}
	bool Parse(short & data)					{return Parse((char*) &data,sizeof(short));}
	bool Parse(unsigned short &data)			{return Parse((char*) &data,sizeof(unsigned short));}
	bool Parse(int & data)						{return Parse((char*) &data,sizeof(int));}
	bool Parse(unsigned int & data)				{return Parse((char*) &data,sizeof(unsigned int));}
	bool Parse(long & data)						{return Parse((char*) &data,sizeof(long));}
	bool Parse(unsigned long & data)			{return Parse((char*) &data,sizeof(unsigned long));}
	bool Parse(long long & data)				{return Parse((char*) &data,sizeof(long long));}
	bool Parse(unsigned long long & data)		{return Parse((char*) &data,sizeof(unsigned long long));}
	bool Parse(float & data)					{return Parse((char*) &data,sizeof(float));}
	bool Parse(double & data)					{return Parse((char*) &data,sizeof(double));}
	bool Parse(void * & data)					{return Parse((char*) &data,sizeof(void*));}	



	
	
	
	
	
	int Seek(int position, int origin)
	{
		Token.Release();
		int seek = BinaryScanner.Seek(position,origin);			
		return seek;
	}


	bool IsReadable()					{return BinaryStream.IsReadable();}
	bool IsWriteable()					{return BinaryStream.IsWriteable();}
	
	
		
		
	
	int Read(char * data, int size)
	{
		
		
		Token.Release();
		
		while (size-- > 0)
		{
			char * byte = At();
			*data++ = *byte;
			Next();
		}
		
		
		return Token.Size;
	}
	
	int Read(bool & data)						{return Read((char*) &data,1);}
	int Read(char & data)						{return Read((char*) &data,sizeof(char));}
	int Read(unsigned char & data)				{return Read((char*) &data,sizeof(unsigned char));}
	int Read(short & data)						{return Read((char*) &data,sizeof(short));}
	int Read(unsigned short &data)				{return Read((char*) &data,sizeof(unsigned short));}
	int Read(int & data)						{return Read((char*) &data,sizeof(int));}
	int Read(unsigned int & data)				{return Read((char*) &data,sizeof(unsigned int));}
	int Read(long & data)						{return Read((char*) &data,sizeof(long));}
	int Read(unsigned long & data)				{return Read((char*) &data,sizeof(unsigned long));}
	int Read(long long & data)					{return Read((char*) &data,sizeof(long long));}
	int Read(unsigned long long & data)			{return Read((char*) &data,sizeof(unsigned long long));}
	int Read(float & data)						{return Read((char*) &data,sizeof(float));}
	int Read(double & data)						{return Read((char*) &data,sizeof(double));}
	int Read(void * & data)						{return Read((char*) &data,sizeof(void*));}

	int Read8(char & bits)						{return Read((char*)&bits,1);}
	int Read8(unsigned char & bits) 			{return Read((char*)&bits,1);}
	int Read16(short & bits)					{return Read((char*)&bits,2);}
	int Read16(unsigned short & bits)			{return Read((char*)&bits,2);}
	int Read32(int & bits)						{return Read((char*)&bits,4);}
	int Read32(unsigned int & bits)				{return Read((char*)&bits,4);}
	#ifdef HERO_PLATFORM_WINDOWS
	int Read32(const long & bits)				{return Read((char*)&bits,4);}
	int Read32(const unsigned long & bits)		{return Read((char*)&bits,4);}
	#else
	int Read64(const long & bits)				{return Read((char*)&bits,8);}
	int Read64(const unsigned long & bits)		{return Read((char*)&bits,8);}
	#endif	
	int Read64(long long & bits)				{return Read((char*)&bits,8);}
	int Read64(unsigned long long & bits)		{return Read((char*)&bits,8);}

	unsigned char Read8()						{unsigned char bits;Read8(bits);return bits;}
	unsigned short Read16()						{unsigned short bits;Read16(bits);return bits;}
	unsigned int Read32()						{unsigned int bits;Read32(bits);return bits;}
	unsigned long long Read64()					{unsigned long long bits;Read64(bits);return bits;}
	
	
	int ReadNull8(int amount)					{unsigned char data=0;int size=0;while(amount-- > 0) size += Read8(data);return size;}
	int ReadNull16(int amount)					{unsigned short data=0;int size=0;while(amount-- > 0) size += Read16(data);return size;}
	int ReadNull32(int amount)					{unsigned int data=0;int size=0;while(amount-- > 0) size += Read32(data);return size;}
	int ReadNull64(int amount)					{unsigned long long data=0;int size=0;while(amount-- > 0) size += Read64(data);return size;}
	
	

	
	
	using Stream::Write;
	int Write(char * data , int size)	
	{
		int write = BinaryStream.Write(data,size);					
		
		
		((Stream*)this)->Position += write;
		
		
		
		
		Token.Release();
		BinaryScanner.Token.Release();
		BinaryScanner.Token.Position = ((Stream*)this)->Position;
		
		
		return write;
	}
	
	
	int Write(const bool & data)				{return Write((char&)data);}
	int Write(const char & data)				{return Write((char *) &data,sizeof(char));}
	int Write(const unsigned char & data)		{return Write((char *) &data,sizeof(unsigned char));}
	int Write(const short & data)				{return Write((char *) &data,sizeof(short));}
	int Write(const unsigned short &data)		{return Write((char *) &data,sizeof(unsigned short));}
	int Write(const int & data)					{return Write((char *) &data,sizeof(int));}
	int Write(const unsigned int & data)		{return Write((char *) &data,sizeof(unsigned int));}
	int Write(const long & data)				{return Write((char *) &data,sizeof(long));}
	int Write(const unsigned long & data)		{return Write((char *) &data,sizeof(unsigned long));}
	int Write(const long long & data)			{return Write((char *) &data,sizeof(long long));}
	int Write(const unsigned long long & data)	{return Write((char *) &data,sizeof(unsigned long long));}
	int Write(const float & data)				{return Write((char *) &data,sizeof(float));}
	int Write(const double & data)				{return Write((char *) &data,sizeof(double));}
	int Write(const void * & data)				{return Write((char*) &data,sizeof(void*));}

	int Write8(const char & bits)					{return Write((char*)&bits,1);}
	int Write8(const unsigned char & bits)			{return Write((char*)&bits,1);}
	int Write16(const short & bits)					{return Write((char*)&bits,2);}
	int Write16(const unsigned short & bits)		{return Write((char*)&bits,2);}
	int Write32(const int & bits)					{return Write((char*)&bits,4);}
	int Write32(const unsigned int & bits)			{return Write((char*)&bits,4);}
	#ifdef HERO_PLATFORM_WINDOWS
	int Write32(const long & bits)					{return Write((char*)&bits,4);}
	int Write32(const unsigned long & bits)			{return Write((char*)&bits,4);}
	#else
	int Write64(const long & bits)					{return Write((char*)&bits,8);}
	int Write64(const unsigned long & bits)			{return Write((char*)&bits,8);}
	#endif
	int Write64(const long long & bits)				{return Write((char*)&bits,8);}
	int Write64(const unsigned long long & bits)	{return Write((char*)&bits,8);}
	
	int WriteNull8(int amount)					{unsigned char data=0;int size=0;while(amount-- > 0) size += Write8(data);return size;}
	int WriteNull16(int amount)					{unsigned short data=0;int size=0;while(amount-- > 0) size += Write16(data);return size;}
	int WriteNull32(int amount)					{unsigned int data=0;int size=0;while(amount-- > 0) size += Write32(data);return size;}
	int WriteNull64(int amount)					{unsigned long long data=0;int size=0;while(amount-- > 0) size += Write64(data);return size;}
	
	
};

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




