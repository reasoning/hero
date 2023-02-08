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


#include "hero/error.h"
#include "hero/parser.h"


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

void RaiseError(char * data, int size)
{
	
	
	Substring error(data,size);

	Unwind & unwind = Unwind::Singleton();
	if (unwind.Length())
	{
		
		
		Unwind::Result res = unwind.Peek();
		unwind[res.Index]->Push(Level(error));
	}
}

void RaiseError(Error * error)
{
	Unwind & unwind = Unwind::Singleton();
	if (unwind.Length())
	{
		
		
		Unwind::Result res = unwind.Peek();
		unwind[res.Index]->Push(Level(error));
	}
}

Raise::Raise(Error & error)
{
	RaiseError(&error);
}

Raise::Raise(Error * error)
{
	RaiseError(error);
}

Raise::Raise(const Substring & error)
{
	RaiseError(error.Data,error.Size);
}



Raise::Raise(char * data, int size) 
{
	RaiseError(data,size);
}

Raise::Raise(const char * format, ...)
{
	va_list va;
	va_start(va, format);
	String str;
	str.FormatVa(format,va);
	va_end(va);

	RaiseError(str.Data,str.Size);
}

void Raise::Construct(Error * error)
{
	RaiseError(error);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Try::Try()
{
	
	
	Unwind & unwind = Unwind::Singleton();
	unwind.Push(this);
}




Try::~Try()
{
	Unwind & unwind = Unwind::Singleton();
	
	
	
	if (!unwind.Pop(this))
		unwind.Remove(this);
}


Catch::Catch() 
{
	Unwind & unwind = Unwind::Singleton();
	if (unwind.Length())
	{
		Unwind::Result res = unwind.Peek();

		
		unwind[res.Index]->Swap(*this);
		unwind.RemoveAt(res.Index);
	}
}

Unwind & Unwind::Singleton()
{
	static Unwind unwind;
	return unwind;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ParserError::ParserErrorToken::ParserErrorToken(const class Token & token)
{
	Position = token.Position;
	Offset = token.Offset;
	Line = token.Line;
	Column = token.Column;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////