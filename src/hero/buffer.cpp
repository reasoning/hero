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


#include "hero/buffer.h"
#include "hero/number.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Buffer::Write(char * data , int size)
{
	String::Append(data,size);
	return size;
}

int Buffer::Read(char * data, int size)
{
	Assert((data == 0 && size == 0) || (data != 0 && size >= 0)); 

	if (data == 0 || size < 0) return 0;

	switch(Mode)
	{
	case MODE_FIFO:
		{
			if (size <= Size)
			{
				memcpy(data,Data,size);
				LTrim(size);
				return size;
			}
			else
			{
				int remaining = Size;
				if (remaining > 0)
				{
					memcpy(data,Data,remaining);
					LTrim(remaining);
					return remaining;
				}
			}
		}
		break;
	case MODE_LIFO:
		{
			if (size <= Size)
			{
				memcpy(data,Data+Size-size,size);
				RTrim(size);
				return size;
			}
			else
			{
				int remaining = Size;
				if (remaining > 0)
				{
					memcpy(data,Data+Size-remaining,remaining);
					RTrim(remaining);
					return remaining;
				}
			}
		}
		break;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

StreamBuffer::StreamBuffer(class Stream & stream, int buffer):
	StreamFilter(stream),Offset(0)
{
	if (buffer == 0)
	{

		Buffer.Allocate(4096+1);
	}
	else
	{
		Buffer.Allocate(buffer+1);
	}

	Offset = Buffer.Data;
}

StreamBuffer::StreamBuffer(class Stream *stream, int buffer):
	StreamFilter(stream),Offset(0)
{

	Buffer.Allocate((buffer == 0)?4096+1:buffer+1);
	Offset = Buffer.Data;
}

StreamBuffer::~StreamBuffer()
{

	Reset();
}

int StreamBuffer::Seek(int position, int origin)				
{
	if (IsWriting())
	{

		Reset();
	}

	if (origin < 0)
	{
		if (position < 0) return Position;

		int relative = position - Position;
		if (relative > 0)
		{

			position -= Position;
		}
		else

		if (Offset+relative >= Buffer.Data && Offset+relative <= Buffer.Data+Buffer.Size)
		{

			Offset += relative;
			Position += relative;
			position = 0;
		}
		else
		{

			if (IsRelative()) return Position;

			Position = 0;
			Stream->Seek(0);
		}
	}
	else
	if (origin == 0)
	{		
		if (position == 0) return Position;

		int relative = position;
		if (relative > 0)
		{

		}
		else

		if (Offset+relative >= Buffer.Data && Offset+relative <= Buffer.Data+Buffer.Size)
		{

			Offset += relative;
			Position += relative;
			position = 0;
		}
		else
		{

			if (IsRelative()) return Position;

			position = Position + relative;
			Position = 0;
			Stream->Seek(0);
		}
	}
	else
	if (origin > 0)
	{
		if (position > 0) return Position;

		if (IsRelative()) return Position;

		String read;
		read.Resize(Buffer.Allocated-1);
		int size = 0;
		do 
		{

			size = Read(read,read.Size);
		}
		while (size > 0);

		int relative = position;

		if (Offset+relative >= Buffer.Data && Offset+relative <= Buffer.Data+Buffer.Size)
		{

			Offset += relative;
			Position += relative;
			position = 0;
		}
		else
		{

			position = Position + relative;
			Position = 0;
			Stream->Seek(0);
		}
	}

	if (Stream->Position == 0)
	{

		Reset();	
	}

	if (position > 0)
	{

		String read;
		read.Resize(Number::Min(position,Buffer.Allocated-1));

		while (position > 0)
		{
			int size = Read(read,Number::Min(position,Buffer.Allocated-1));
			if (size == 0) break;
			position -= size;				
		}
	}

	return Position;

}

int StreamBuffer::ReadIndirect()
{

	Clear();
	int read = ReadDirect(Buffer.Data,Buffer.Allocated-1);
	Position -= read;
	Buffer.Size = read;
	Buffer.Term();
	return read;
}

int StreamBuffer::ReadIndirect(char *data, int size)
{
	if (!IsReadable()) return 0;

	if (IsWriting())
	{

		Flush();
	}

	if( Offset+size < Buffer.Data+(Buffer.Size) )
	{

		memcpy(data,Offset,size);
		Offset += size;
		Position += size;
		return size;
	}
	else
	{

		int remaining = Buffer.Data+(Buffer.Size) - Offset;
		if (remaining > 0)
		{
			memcpy(data,Offset,remaining);
			Offset += remaining;
			Position += remaining;			
		}

		if (size-remaining < Buffer.Allocated)
		{

			if (ReadIndirect() == 0 && IsReadable())
			{
				Raise("StreamBuffer::ReadIndirect - Failed to read stream buffer.\n");
				return 0;
			}

			return remaining + ReadIndirect(data+remaining,size-remaining);
		}
		else
		{

			Clear();

			int read = remaining + ReadDirect(data+remaining,size-remaining);
			return read;
		}
	}

}

int StreamBuffer::WriteIndirect()	
{
	int write = WriteDirect(Buffer.Data,Buffer.Size);
	Position -= Buffer.Size;
	Clear();
	return write;
}

int StreamBuffer::WriteIndirect(char *data, int size)
{
	if (!IsWriteable()) return 0;

	if (IsReading())
	{

		Flush();
		Stream->Seek(-Buffer.Size,0);
		Clear();
	}

	if( Offset+size <= Buffer.Data+(Buffer.Allocated-1))
	{

		memcpy(Offset,data,size);
		Buffer.Size += size;
		Offset += size;
		Position += size;	
		Buffer.Term();
		return size;		
	}
	else
	{

		if (size < Buffer.Allocated)
		{

			int remaining = Buffer.Data+(Buffer.Allocated-1) - Offset;
			if (remaining > 0)
			{
				memcpy(Offset,data,remaining);
				Buffer.Size += remaining;
				Offset += remaining;
				Position += remaining;
				Buffer.Term();
			}

			if (WriteIndirect() == 0 && IsWriteable())
			{
				Raise("StreamBuffer::WriteIndirect - Failed to write stream buffer.\n");
				return 0;
			}

			return remaining + WriteIndirect(data+remaining,size-remaining);
		}
		else
		{

			Flush();
			int write = WriteDirect(data,size);
			return write;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

