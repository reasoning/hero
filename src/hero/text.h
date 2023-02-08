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

#include "hero/structure.h"
#include "hero/string.h"
#include "hero/stream.h"
#include "hero/buffer.h"
#include "hero/file.h"
#include "hero/path.h"
#include "hero/number.h"
#include "hero/error.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




class Text : public String
{
public:

	Text();
	~Text();

	
	int IndexOf(int from, char *data, int size,  bool caseless=false);
	
	
	int IndexOfXor(int from, char *data, int size,  bool caseless=false);
	
	int IndexOfRabinKarp(int from, char *data, int size,  bool caseless=false);

	int IndexOfBoyerMoore(int from, char *data, int size,  bool caseless=false);
	

	
	
	
	void Wrap(int columns, const char * indent=0, int tabs=8, const char * first=0) {}
	void Pad() {}
	
	
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TextFile : public File
{
public:

	TextFile(){}
	TextFile(const File & file):File(file){}
	
	TextFile(const Substring & str):File(str){}
	TextFile(const char * path):File(path){}
	TextFile(char * path, int size):File(path,size){}
	~TextFile(){}

	using File::Open;
	virtual bool Open(int options = File::OPTIONS_OPEN_TEXT_MUTABLE)
	{
		return File::Open(options);
	}

	virtual bool OpenReadable()
	{
		if ( !IsOpen() && !Open() && !Open(File::OPTIONS_OPEN_TEXT_READ)) 
		{
			
			
			

			Raise("TextFile::OpenReadable: The file does not exist or could not be opened for reading.\n");	
			return false;
		}


		if (!IsReadable())
		{
			Raise("TextFile::OpenReadable: The file could not be opened for reading.\n");	
			return false;
		}

		return true;
	}

	virtual bool OpenWriteable()
	{
		if ( !IsOpen() && !Open() && !Open(File::OPTIONS_OPEN_TEXT_WRITE))
		{
			
			
			
			
			Raise("TextFile::OpenWriteable: The file does not exist or could not be opened for writing.\n");	
			return false;
		}

		if (!IsWriteable())
		{
			Raise("TextFile::OpenWriteable: The file has not be opened for writing.\n");	
			return false;
		}

		return true;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TextFileStream : public FileStream
{
public:

	TextFileStream(){}
	TextFileStream(const char * path):FileStream(path){}
	TextFileStream(char * path, int size):FileStream(path,size){}
	TextFileStream(const Substring & str):FileStream(str){}
	~TextFileStream(){}

	using File::Open;
	virtual bool Open(int options = File::OPTIONS_OPEN_TEXT_MUTABLE)
	{
		return File::Open(options);
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class TextStream : public StreamBuffer
{
public:

	TextStream(class Stream & stream):StreamBuffer(stream){};
	~TextStream(){};

	int ReadLine(char * data, int size)
	{
		
		

		int read=0, amount=0;
		char at;
		while (size > 0 && (amount=Read(at)) && amount)
		{
			if (at == Characters::CarriageReturn || at == Characters::LineFeed)
			{
				if (at != *Offset && (*Offset == Characters::CarriageReturn || *Offset == Characters::LineFeed))
					Read(at);
		
				break;
			}

			data[read++] = at;
			--size;
		}

		return read;
	}

	int ReadLine(Substring &str, int amount=0)
	{
		if (str.IsNull())
		{
			Raise("TextStream::ReadLine: Substring was null.\n");
			return 0;
		}

		if (amount > str.Size)
		{
			Raise("TextStream::ReadLine: Amount is greater than the str size.\n");
			return 0;
		}

		if (amount==0)
			amount = str.Size;

		return ReadLine(str.Data,amount);	
	}

	int ReadLine(String & string, int amount=0)
	{
		
		
		

		
		
		if (amount)
		{
			
			
			if (string.Allocated <= amount)
				string.Allocate(amount+1);

			string.Size = ReadLine(string.Data,amount);
		}
		else
		{
			if (string.Allocated == 0)
				string.Allocate(0);

			
			

			string.Size = 0;
			char at;
			while ((amount=Read(at)) && amount)
			{
				if (at == Characters::CarriageReturn || at == Characters::LineFeed)
				{
					if (at != *Offset && (*Offset == Characters::CarriageReturn || *Offset == Characters::LineFeed))
						Read(at);

					break;
				}

				if (string.Size == string.Allocated-1)
				{
					string.Allocate(0);
				}
				
				string.Data[string.Size] = at;
				string.Size++;
			}
		}

		string.Term();
		return string.Size;
	}

	int ReadLine(Writer & writer, int amount=0)
	{
		String reader;
		reader.Resize((amount>0)?amount:4096);
		reader.Size = ReadLine(reader,amount);
		
		int write = writer.Write(reader);		
		if (reader.Size != write)
		{
			Raise("TextStream::ReadLine: Amount read and amount written differ.\n");
			return 0;
		}

		return reader.Size;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Locale
{
public:

};

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
