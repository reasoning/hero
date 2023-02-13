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

#include "hero/file.h"
#include "hero/storage.h"
#include "hero/number.h"
#include "hero/error.h"

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

void FileStorage::Construct(const Path & path)
{

}

bool FileStorage::Open(const Path & path, int options)
{
	return Open(path,options&File::MODE,options&File::TYPE,options&File::ACCESS,options&File::USAGE);
}

int FileStorage::ReadLine(const Path & path, String & string)
{

	int amount=0;

	char data;
	int read = Read(path,&data,1);
	while (read && data != '\n' && data != '\r')
	{
		string.Append(&data,1);
		read = Read(path,&data,1);
		++amount;
	}

	return amount;
}

bool FileStorage::OpenReadable(const Path & path)
{
	if (!IsOpen(path) && !Open(path) && !Open(path,File::OPTIONS_OPEN_BINARY_READ) && !Open(path,File::OPTIONS_CREATE_BINARY_READ)) 
	{

		RaiseError<FileError>("FileSystem::OpenReadable - The file could not be opened for reading.\n");	
		return false;
	}

	if (!IsReadable(path))
	{
		RaiseError<FileError>("FileSystem::OpenReadable - The file could not be opened for reading.\n");	
		return false;
	}

	return true;
}

bool FileStorage::OpenWriteable(const Path & path)
{
	if (!IsOpen(path) && !Open(path) && !Open(path,File::OPTIONS_OPEN_BINARY_WRITE) && !Open(path,File::OPTIONS_CREATE_BINARY_WRITE))
	{

		RaiseError<FileError>("FileSystem::OpenWriteable - The file could not be opened for writing.\n");	
		return false;
	}

	if (!IsWriteable(path))
	{
		RaiseError<FileError>("FileSystem::OpenWriteable - The file could not be opened for writing.\n");	
		return false;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FilePath::FilePath()
{
	Append(&Path);
	Append(&Name);

	Construct();
}

FilePath::FilePath(const Substring & str):
	Hero::Path(str)
{

	Append(&Path);
	Append(&Name);

	Construct();
}

FilePath::FilePath(const char * name):
	Hero::Path(name)
{

	Append(&Path);
	Append(&Name);

	Construct();
}

FilePath::FilePath(char * name, int size):
	Hero::Path(name,size)
{
	Append(&Path);
	Append(&Name);

	Construct();
}

FilePath::~FilePath()
{

}

void FilePath::Normalise()
{

	Translate(":*?\"<>|","");
	Construct();
}

void FilePath::Construct(char * path, int size)
{
	String::Construct(path,size);
	Construct();
}

void FilePath::Construct()
{

	if (Path.IsActivated() || Path.IsNode()) Path.Release();
	if (Path.IsActivated() || Path.IsNode()) Name.Release();

	if (IsEmpty()) return;

	int size = 0;
	do
	{
		size = Size;
		Replace("//","/");
		Replace("\\\\","\\");
	}
	while (size != Size);

	if (Size > 1 && Data[Size-1] == '/' || Data[Size-1] == '\\')
	{
		Size--;
		Term();
	}

	Trim(" \t\n\r");

	int index = Number::Max(LastIndexOf('/'),LastIndexOf('\\'));
	if (index != -1)
	{
		Path.Assign(Slice(0,index));
		Path.Split("/");
		Path.Split("\\");
		Name.Assign(Data+index+1,Size-(index+1));

		if (Path.At(1) == ':')
		{

			Path.Substring::Left(-2);
		}
	}
	else
	{

		Name.Assign(*this);
		if (Name.Size > 1 && Name.At(1) == ':')
		{

			Name.Substring::Left(-2);
		}
	}

	Assert(Path.Segments().Parent == this);
	Assert(Name.Segments().Parent == this);
}

Substring FilePath::Foldername()
{
	return Substring(Path.Data,Path.Size);
}

Substring FilePath::Basename() 
{
	Substring basename;
	if (Path.Children())
	{
		basename.Assign(Path.Last()->Data,Path.Last()->Size);
	}
	else
	{
		basename.Assign(Path.Data,Path.Size);
	}
	return basename;
}

Substring FilePath::Extension() 
{
	Substring extension;
	int index = Name.LastIndexOf('.');
	if (index != -1)
	{
		extension.Assign(Name.Data+index+1,Name.Size-index-1);

	}
	return extension;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Identity File::Instance;

File::File() 
{
	Storage = new DriveFile();	
}

File::File(FileStorage * driver) 
{
	Storage = driver;
}

File::File(FileStorage * driver, const Substring & path):
	FilePath((Substring&)path)
{
	Storage = driver;
}

File::File(Strong<FileStorage*> & driver) 
{
	Storage = driver;
}

File::File(Strong<FileStorage*> & driver, const Substring & path):
	FilePath((Substring&)path)
{
	Storage = driver;
}

File::File(const File & file)
{
	if (&file == this)
		return;
	operator = (file);
}

File::File(const Substring & path):
	FilePath((Substring&)path)
{
	Storage = new DriveFile();		
}

File::File(const char * name):
	FilePath(name)
{
	Storage = new DriveFile();		
}

File::File(char * name, int size):
	FilePath(name,size)
{
	Storage = new DriveFile();	
}

File::~File() 
{
	if (Storage) Storage->Close(*this);
}

void File::Construct()
{
	if (Storage) Storage->Close(*this);
	FilePath::Construct();
}

void File::Construct(char * path, int size)
{
	if (Storage) Storage->Close(*this);

	FilePath::Construct(path,size);
}

File & File::operator = (const File & file)
{
	if (&file == this)
		return *this;

	Path::Construct(file);
	Storage = file.Storage;

	int offset = 0;

	if (!((File&)file).Path.IsEmpty())
	{
		offset = file.Path.Data-file.Data;
		Path.Data = Data+offset;
		Path.Size = file.Path.Size;
	}

	if (!((File&)file).Name.IsEmpty())
	{
		offset = file.Name.Data-file.Data;
		Name.Data = Data+offset;
		Name.Size = file.Name.Size;
	}

	return *this;
}

int File::Compare(Object *object, int comparitor)
{
	if (Inheritance::IsHomozygous(this,object) || Inheritance::IsDominant(this,object))
	{
		return Path::Compare(object);
	}
	else
	{
		return Inheritance::Compare(this,object);
	}
}

bool File::Open(int options)
{
	return (Storage)?Storage->Open(*this,options):false;
}

bool File::Open(int mode, int type, int access, int usage)
{
	return (Storage)?Storage->Open(*this,mode,type,access,usage):false;
}	

bool File::OpenReadable()
{
	return (Storage)?Storage->OpenReadable(*this):false;
}

bool File::OpenWriteable()
{	
	return (Storage)?Storage->OpenWriteable(*this):false;
}

bool File::Close()
{
	return (Storage)?Storage->Close(*this):false;
}

bool File::IsReadable()
{
	return (Storage)?Storage->IsReadable(*this):false;
}

bool File::IsWriteable()
{
	return (Storage)?Storage->IsWriteable(*this):false;
}

int File::Read(char * data, int size)
{
	return (Storage)?Storage->Read(*this,data,size):false;
}

int File::ReadLine(String & string)
{
	return (Storage)?Storage->ReadLine(*this,string):false;
}

int File::ReadLine(char * data, int size)
{
	return (Storage)?Storage->ReadLine(*this,data,size):false;
}

int File::Write(char * data, int size)
{
	return (Storage)?Storage->Write(*this,data,size):false;
}

int File::WriteLine(char * data, int size)
{	
	return (Storage)?Storage->WriteLine(*this,data,size):false;
}

bool File::Flush()
{
	return (Storage)?Storage->Flush(*this):false;
}

bool File::Rename(char * name, int size)
{
	return (Storage)?Storage->Rename(*this,name,size):false;
}

bool File::Create(int access)
{
	return (Storage)?Storage->Create(*this,access):false;
}

bool File::Delete()
{
	return (Storage)?Storage->Delete(*this):false;
}

bool File::Truncate()
{
	return (Storage)?Storage->Truncate(*this):false;
}

int File::Seek(int position, int origin)
{
	return (Storage)?Storage->Seek(*this,position,origin):0;
}

bool File::Eof()
{
	return (Storage)?Storage->Eof(*this):false;
}

int File::Error()
{
	return (Storage)?Storage->Error(*this):false;
}

bool File::Status(Stat & stat)
{
	if (Storage)
	{
		stat = Storage->Status(*this);
		return true;
	}

	return false;
}

Stat File::Status()
{
	if (Storage)
	{
		return Storage->Status(*this);
	}

	return Stat();
}

bool File::Exists()
{
	return (Storage)?Storage->Exists(*this):false;	
}

bool File::IsOpen()
{
	return (Storage)?Storage->IsOpen(*this):false;
}

bool File::IsClosed()
{
	return (Storage)?Storage->IsClosed(*this):false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////