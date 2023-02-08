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
#include "hero/stream.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







class Buffer : public String, public Reader, public Writer
{
public:

	enum Modes
	{
		MODE_FIFO,
		MODE_LIFO,
	};

	int Mode;

	Buffer(int mode=MODE_FIFO):Mode(mode)
	{
	}

	bool IsWriteable() {return true;}
	bool IsReadable() {return true;}

	using Writer::Write;
	int Write(char * data , int size);

	using Reader::Read;
	int Read(char * data, int size);

	int Erase(int amount=1)
	{
		static char erase=0;
		int read=0;
		for (;amount > 0;--amount)
			read += Read(&erase,1);	
		return read;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StreamBuffer : public StreamFilter
{
protected:

	virtual int ReadIndirect();
	virtual int WriteIndirect();

	virtual int ReadIndirect(char *data, int size);
	virtual int WriteIndirect(char *data, int size);

	virtual int ReadDirect(char *data, int size)
	{
		return StreamFilter::Read(data,size);
	}

	virtual int WriteDirect(char * data, int size)
	{
		return StreamFilter::Write(data,size);
	}


public:

	char * Offset;
	String Buffer;
	
	StreamBuffer(class Stream &stream, int buffer=0);
	StreamBuffer(class Stream *stream, int buffer=0);
	~StreamBuffer();

	int Seek(int position, int origin=-1);

	virtual void Reset()
	{
		
		
		Flush();
		Clear();
	}

	virtual void Flush()
	{
		if (IsWriting())
            WriteIndirect();

		if (IsReading())
		{
			Buffer.Right(-(int)(Buffer.Data-Offset));
			Offset=Buffer.Data;
		}
	}

	virtual void Clear()
	{
		Buffer.Release();
		Offset = Buffer.Data;
	}

	bool IsBuffered() 
	{
		return Buffer.Allocated > 0;
	}

	virtual bool IsReading()
	{
		Assert(Offset <= Buffer.Data+Buffer.Size);
		return (Buffer.Size > 0 && Offset < Buffer.Data+Buffer.Size);
	}

	bool IsReadable()					
	{
		
		
		return (Buffer.Size > 0 && Offset < Buffer.Data+Buffer.Size) || Stream->IsReadable();
	}
	
	using StreamFilter::Read;
	int Read(char * data, int size)		
	{
		if (IsBuffered())
		{
			return ReadIndirect(data,size);
		}
		else
		{
			return ReadDirect(data,size);
		}
	}

	virtual bool IsWriting()
	{
		Assert(Offset <= Buffer.Data+Buffer.Size);
		return Offset > Buffer.Data && Buffer.Size > 0 && Offset == Buffer.Data+Buffer.Size;
	}

	bool IsWriteable()
	{
		
		return Stream->IsWriteable();
	}
	
	using StreamFilter::Write;	
	int Write(char * data , int size)	
	{
		if (IsBuffered())
		{
			return WriteIndirect(data,size);
		}
		else
		{
			return WriteDirect(data,size);
		}
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
