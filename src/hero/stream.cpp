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

#include "hero/stream.h"
#include "hero/binary.h"




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Reader::Read(Substring &str, int amount)
{
	
	

	if (str.IsNull())
		return 0;

	if (amount > str.Size)
		return 0;

	if (amount == 0)
		amount = str.Size;

	return Read(str.Data,amount);	
}


int Reader::Read(String &string, int amount)
{
	Assert(amount >= 0);
	if (amount < 0) return 0;

	
	

	if (amount)
	{

		if (string.Allocated <= amount)
			string.Reserve(amount);
		
		string.Size = Read(string.Data,amount);
	}
	else
	{
	
		
		
		
		String reader;
		if (string.Allocated == 0)
			reader.Reserve(0x2000);
		else
			reader.Reserve(string.Allocated);

		amount = reader.Allocated-1;
		
		Assert(amount > 0);

		int offset = 0;
		reader.Size = 0;
		

		while(amount > 0 && (amount=Read(reader.Data+offset,amount)))
		{
			reader.Size += amount;
			if (reader.Size == (reader.Allocated-1))
			{
				
				
				
				

				if (IsReadable())
				{
					
					
					reader.Reserve(0);
				}
			}

			offset += amount;
			amount = (reader.Allocated-1)-reader.Size;		
		}
		
		string.Replace(reader);
	}

	
	string.Term();
	return string.Size;
}


int Reader::Read(Writer & writer, int amount)
{
	Assert(amount >= 0);
	if (amount < 0) return 0;
	
	String reader;
	reader.Resize((amount>0)?amount:0x2000);
	int r=0;
	int w=0;

	if (amount > 0)
	{
		r = amount = Read(reader,amount);
		w = writer.Write(reader,amount);		
	}
	else
	{
		while ((r += amount = Read(reader)) && amount)
			w += writer.Write(reader,amount);
	}

	if (r != w)
	{
		Assert(r == w);
		return 0;
	}

	return r;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Writer::Write(Substring &str,int amount)
{
	
	

	if (str.IsNull())
		return 0;

	if (amount > str.Size)
		return 0;

	if (amount == 0)
		amount = str.Size;
	
	return Write(str.Data,amount);	
}


int Writer::Write(String & string, int amount)
{
	Assert(amount >= 0);
	if (amount < 0) return 0;

	
	
	
	
	
	

	if (amount > string.Size)
		amount = 0;

	if (amount == 0)
		amount = string.Size;

	return Write(string.Data,amount);
}

int Writer::Write(Reader & reader, int amount)
{
	Assert(amount >= 0);
	if (amount < 0) return 0;

	String writer;
	writer.Resize((amount>0)?amount:0x2000);
	int r=0;
	int w=0;

	if (amount > 0)
	{
		r = amount = reader.Read(writer,amount);
		w = Write(writer,amount);		
	}
	else
	{
		while ((r += amount = reader.Read(writer)) && amount)
			w += Write(writer,amount);
	}

	if (r != w)
	{
		Assert(r == w);
		return 0;
	}

	return w;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int StreamWrite(Stream & stream, int & value)
{
	BinaryStream binary(stream);
	return binary.Write(value);
}

int StreamRead(Stream & stream, int & value)
{
	BinaryStream binary(stream);
	return binary.Write(value);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



