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
#include "hero/generic.h"
#include "hero/path.h"
#include "hero/stream.h"
#include "hero/stat.h"
#include "hero/error.h"

#include <sys/stat.h>

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FileStorage;

class FilePath : public Path
{
public:
	class Path Path;
	class Path Name;

	FilePath();
	FilePath(const Substring & str);
	FilePath(const char * name);
	FilePath(char * name, int size);
	~FilePath();

	Substring Foldername();
	Substring Basename();
	Substring Extension();

	void Normalise();		
	void Construct();

	void Construct(char * path, int size);
	void Construct(const Substring & path) {Construct(path.Data,path.Size);}
	void Construct(const char * path) {Construct((char*)path,String::Length(path));}	

	FilePath & operator = (const char * path)
	{
		Construct(path);
		return *this;
	}

};

class File : public FilePath, public Reader, public Writer
{	
public:

	enum FileOptions
	{

		MODE_APPEND			=(1)<<1,		
		MODE_CREATE			=(1)<<2,		
		MODE_OPEN			=(1)<<3,		
		MODE				=MODE_APPEND|MODE_CREATE|MODE_OPEN,

		TYPE_BINARY			=(1)<<4,
		TYPE_TEXT			=(1)<<5,
		TYPE				=TYPE_BINARY|TYPE_TEXT,

		ACCESS_READ			=(1)<<6,		
		ACCESS_WRITE		=(1)<<7,		
		ACCESS_MUTABLE		=(1)<<8,		
		ACCESS				=ACCESS_READ|ACCESS_WRITE|ACCESS_MUTABLE,

		USAGE_SEQUENTIAL	=(1)<<9,		
		USAGE_RANDOM		=(1)<<10,		
		USAGE_SCRATCH		=(1)<<11,		
		USAGE_TEMPORARY		=(1)<<12,		
		USAGE				=USAGE_SEQUENTIAL|USAGE_RANDOM|USAGE_SCRATCH|USAGE_TEMPORARY,

		OPTIONS_OPEN_TEXT_MUTABLE		=MODE_OPEN|TYPE_TEXT|ACCESS_MUTABLE,
		OPTIONS_OPEN_TEXT_READ			=MODE_OPEN|TYPE_TEXT|ACCESS_READ,
		OPTIONS_OPEN_TEXT_WRITE			=MODE_OPEN|TYPE_TEXT|ACCESS_WRITE,

		OPTIONS_CREATE_TEXT_MUTABLE		=MODE_CREATE|TYPE_TEXT|ACCESS_MUTABLE,
		OPTIONS_CREATE_TEXT_READ		=MODE_CREATE|TYPE_TEXT|ACCESS_READ,
		OPTIONS_CREATE_TEXT_WRITE		=MODE_CREATE|TYPE_TEXT|ACCESS_WRITE,

		OPTIONS_APPEND_TEXT_MUTABLE		=MODE_APPEND|TYPE_TEXT|ACCESS_MUTABLE,
		OPTIONS_APPEND_TEXT_READ		=MODE_APPEND|TYPE_TEXT|ACCESS_READ,
		OPTIONS_APPEND_TEXT_WRITE		=MODE_APPEND|TYPE_TEXT|ACCESS_WRITE,

		OPTIONS_OPEN_BINARY_MUTABLE		=MODE_OPEN|TYPE_BINARY|ACCESS_MUTABLE,
		OPTIONS_OPEN_BINARY_READ		=MODE_OPEN|TYPE_BINARY|ACCESS_READ,
		OPTIONS_OPEN_BINARY_WRITE		=MODE_OPEN|TYPE_BINARY|ACCESS_WRITE,

		OPTIONS_CREATE_BINARY_MUTABLE	=MODE_CREATE|TYPE_BINARY|ACCESS_MUTABLE,
		OPTIONS_CREATE_BINARY_READ		=MODE_CREATE|TYPE_BINARY|ACCESS_READ,
		OPTIONS_CREATE_BINARY_WRITE		=MODE_CREATE|TYPE_BINARY|ACCESS_WRITE,

		OPTIONS_APPEND_BINARY_MUTABLE	=MODE_APPEND|TYPE_BINARY|ACCESS_MUTABLE,
		OPTIONS_APPEND_BINARY_READ		=MODE_APPEND|TYPE_BINARY|ACCESS_READ,
		OPTIONS_APPEND_BINARY_WRITE		=MODE_APPEND|TYPE_BINARY|ACCESS_WRITE,

	};

public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;}

	virtual bool InstanceOf(Identity &identity)	
	{
		return Instance == identity || Path::Instance == identity || Substring::Instance == identity || Object::InstanceOf(identity);	
	}

	int Compare(Object *object, int comparitor);

public:

	Strong<FileStorage *> Storage;

	File();

	File(FileStorage * driver);
	File(FileStorage * driver, const Substring & path);
	File(Strong<FileStorage*> & driver);
	File(Strong<FileStorage*> & driver, const Substring & path);

	File(const File & file);
	File(const Substring & path);
	File(const char * path);
	File(char * path, int size);
	~File();

	void Construct();
	void Construct(char * path, int size);
	void Construct(const Substring & path) {Construct(path.Data,path.Size);}
	void Construct(const char * path) {Construct((char*)path,String::Length(path));}	

	virtual bool Open(int options = File::OPTIONS_OPEN_BINARY_MUTABLE);
	virtual bool Open(int mode, int type, int access, int usage);

	virtual bool OpenReadable();
	virtual bool OpenWriteable();

	virtual bool Close();

	bool IsReadable();
	bool IsWriteable();

	using Reader::Read;
	virtual int Read(char * data, int size);

	using Writer::Write;
	int Write(const char * data) {return Write((char*)data,String::Length(data));}

	virtual int Write(char * data, int size);

	virtual int ReadLine(String & string);
	virtual int ReadLine(char * data, int size);

	int WriteLine()	{return Write((char*)"\n",1);}
	int WriteLine(const char * data) {return WriteLine((char*)data,String::Length(data));}	
	int WriteLine(const Substring & str) {return WriteLine(str.Data,str.Size);}
	virtual int WriteLine(char * data, int size);

	virtual bool Flush();

	virtual bool Create(int access = File::ACCESS_MUTABLE);
	virtual bool Delete();
	virtual bool Truncate();

	bool Rename(const Substring & name) {return Rename(name.Data,name.Size);}
	bool Rename(const char *name) {return Rename((char*)name,String::Length(name));}
	virtual bool Rename(char * name, int size);

	virtual int Seek(int position, int origin=-1);	
	virtual bool Eof();

	virtual int Error();

	File & operator = (const File & file);

	bool Status(Stat & stat);
	Stat Status();

	static bool Exists(const Substring & str) {return Stat(str).Exists;}

	bool Exists();
	bool IsOpen();
	bool IsClosed();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FileStorage : public Kind
{
public:

	FileStorage() {}

	~FileStorage() {}

	virtual void Construct(const Path & path);

	virtual bool Open(const Path & path, int options = File::OPTIONS_OPEN_BINARY_MUTABLE);	
	virtual bool Open(const Path & path, int mode, int type, int access, int usage)=0;

	virtual bool OpenReadable(const Path & path);
	virtual bool OpenWriteable(const Path & path);

	virtual bool Close(const Path & path)=0;

	virtual int Read(const Path & path, char * data, int size)=0;
	virtual int Write(const Path & path, char * data, int size)=0;

	virtual int ReadLine(const Path & path, char * data, int size)=0;
	virtual int ReadLine(const Path & path, String & string);

	virtual int WriteLine(const Path & path, char * data, int size)=0;

	virtual bool Flush(const Path & path)=0;

	virtual bool Create(const Path & path, int access = File::ACCESS_MUTABLE)=0;
	virtual bool Delete(const Path & path)=0;
	virtual bool Truncate(const Path & path)=0;

	virtual bool Rename(const Path & path, char * name, int size)=0;

	virtual int Seek(const Path & path, int position, int origin=-1)=0;	

	virtual bool Eof(const Path & path)=0;

	virtual int Error(const Path & path)=0;

	virtual bool Exists(const Path & path)=0;

	virtual bool IsOpen(const Path & path)=0;
	virtual bool IsClosed(const Path & path)=0;

	virtual bool IsReadable(const Path & path)=0;
	virtual bool IsWriteable(const Path & path)=0;

	virtual Stat Status(const Path & path)=0;

};	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FileStream : public Stream, public File
{
public:

	FileStream(){}
	FileStream(const char * path):File(path){}
	FileStream(char * path, int size):File(path,size){}
	FileStream(const Substring & str):File(str){}
	FileStream(File &file):File(file){}
	~FileStream(){}

	int Seek(int position, int origin=-1)				
	{
		return Position = File::Seek(position, origin);
	}

	bool IsReadable()					
	{
		if (!File::IsReadable())
		{
			return IsOpen()?!Eof():File::OpenReadable();
		}

		return true;
	}

	using File::Read;
	int Read() {return File::Read();}
	int Read(char &data) {return File::Read(data);}
	int Read(Writer & writer, int amount=0) {return File::Read(writer,amount);}
	int Read(Substring & str, int amount=0) {return File::Read(str,amount);}
	int Read(String &string, int amount=0) {return File::Read(string,amount);}

	int Read(char * data, int size)		
	{
		int read = File::Read(data,size);
		if (read) Position += read;
		return read;
	}

	bool IsWriteable()					
	{
		if (!File::IsWriteable())
		{
			return IsOpen()?true:File::OpenWriteable();
		}

		return true;
	}

	using File::Write;
	int Write() {return File::Write();}
	int Write(char & data) {return File::Write(data);}
	int Write(Reader & reader, int amount=0) {return File::Write(reader,amount);}
	int Write(Substring & str, int amount=0) {return File::Write(str,amount);}
	int Write(String & string, int amount=0) {return File::Write(string,amount);}

	int Write(char * data , int size)	
	{
		int write = File::Write(data,size);
		if (write) Position += write;
		return write;
	}

public:

	class StreamAdapter : public Stream
	{
	public:

		class File & File;

		StreamAdapter(class File & file):File(file) {}
		~StreamAdapter() {}

		int Seek(int position, int origin=-1)				
		{
			return Position = File.Seek(position, origin);
		}

		bool IsReadable()					
		{
			if (File.IsReadable())
			{
				return File.IsOpen()?!File.Eof():File.Open();
			}

			return false;
		}

		int Read(char * data, int size)		
		{
			int read = File.Read(data,size);
			if (read) Position += read;
			return read;
		}

		int Read(Writer & writer, int amount=0)
		{
			return File.Read(writer,amount);
		}

		int Read(Substring & str, int amount=0)
		{
			return File.Read(str,amount);
		}

		bool IsWriteable()					
		{
			if (File.IsWriteable())
			{
				return File.IsOpen()?true:File.Open();
			}

			return false;
		}

		int Write(char * data , int size)	
		{
			int write = File.Write(data,size);
			if (write) Position += write;
			return write;
		}

		int Write(Reader & reader, int amount=0)
		{
			return File.Write(reader,amount);
		}

		int Write(Substring & str, int amount=0)
		{
			return File.Write(str,amount);
		}
	};

	class FileAdapter : public File
	{
	public:

		Stream & Bridge;

		FileAdapter(Stream & stream):Bridge(stream) {}

		bool Open(int options = 0) {return true;}
		bool Open(int mode, int type, int access, int usage) {return true;}
		bool Close() {return true;}

		using File::Read;
		int Read(char * data, int size)
		{
			if (!Bridge.IsReadable()) return 0;

			if (data == 0 || !(size > 0))
			{
				RaiseError<FileError>("FileAdapter::Read - The destination is null or has zero size.\n");	
				return 0;
			}

			return Bridge.Read(data,size);
		}

		using File::ReadLine;
		int ReadLine(char * data, int size)
		{
			if (!Bridge.IsReadable()) return 0;

			int remaining=size;
			while (remaining > 0 && Bridge.Read(*(data++)))
			{
				if (*data == Characters::CarriageReturn || *data == Characters::LineFeed)
				{
					while ((*data == Characters::CarriageReturn || *data == Characters::LineFeed) && Bridge.Read());
					break;
				}
				--remaining;
			}
			return size-remaining;
		}

		using File::Write;
		int Write(char * data, int size)
		{
			Construct();
			if (!Bridge.IsWriteable()) return 0;

			if (data == 0 || !(size > 0))
			{
				RaiseError<FileError>("FileAdapter::Write - The source is null or has zero size.\n");	
				return 0;
			}

			int write = 0;
			write = Bridge.Write((char*)data,size);

			return write;
		}

		using File::WriteLine;
		int WriteLine(char * data, int size)
		{
			if (!Bridge.IsWriteable()) return 0;

			int write = Write(data,size);
			if (write == size && WriteLine())
				++write;
			return write;
		}

		bool Flush()
		{

			return true;
		}

		bool Create(int access = File::ACCESS_MUTABLE)
		{

			return true;
		}

		bool Delete()
		{

			return true;
		}

		using File::Rename;
		bool Rename(char * data, int size)
		{
			Name.Replace(data,size);
			return true;
		}

		bool Truncate()
		{
			return false;
		}

		int Seek(int position, int origin=-1)
		{

			return Bridge.Seek(position,origin);
		}	

		bool Eof() 
		{
			return Bridge.IsReadable() || Bridge.IsWriteable();
		}
	};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

