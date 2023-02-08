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


#include "hero/folder.h"
#include "hero/storage.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


FolderPath::FolderPath()
{
	Construct();
}

FolderPath::FolderPath(const Substring & str)
{
	Construct(str);
}

FolderPath::FolderPath(const char * name)
{
	Construct(name);
}

FolderPath::FolderPath(char * name, int size)
{
	Construct(name,size);
}

FolderPath::~FolderPath()
{

}


void FolderPath::Construct(char * path, int size)
{
	String::Construct(path,size);
	Construct();
}

void FolderPath::Construct()
{
	
	

	
	

	#ifdef HERO_PLATFORM_WINDOWS

	
	
	

	if (! IsEmpty() && CharAt(1) == ':' )
	{
		if ( ! isalpha(CharAt(0)) )
		{
			
			Raise("Folder::Construct - Invalid drive letter specified \"%c\"\n", Path[0].CharAt(0));
		}
	}

	#endif
	
	FilePath::Construct();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Identity Folder::Instance;

Folder::Folder(const Folder & folder):
		Storage(0)
{
	if (&folder == this)
		return;

	
	operator = (folder);
}

Folder::Folder(FolderStorage * driver):
		Storage(0)
{
	Storage = new DriveFolder();
	Storage = driver;
}

Folder::Folder(Strong<FolderStorage*> & driver):
		Storage(0)
{
	Storage = new DriveFolder();
	Storage = driver;
}


Folder::Folder(const Substring & str):
	Storage(0)
{
	Storage = new DriveFolder();
	Construct(str);
}

Folder::Folder(const char * path):
	Storage(0)
{
	Storage = new DriveFolder();
	Construct(path);
}


Folder::Folder(char * path, int size):
	Storage(0)
{
	Storage = new DriveFolder();
	Construct(path,size);
}

Folder::Folder():
		Storage(0)
{
	Storage = new DriveFolder();
}

Folder::~Folder()
{
	Files.Destroy();
	Folders.Destroy();
}

void Folder::Construct()
{
	Folders.Destroy();
	Files.Destroy();

	FolderPath::Construct();
}

void Folder::Construct(char * path, int size)
{
	Folders.Destroy();
	Files.Destroy();

	FolderPath::Construct(path,size);
}

Folder & Folder::operator = (const Folder & folder)
{
	if (&folder == this)
		return *this;
			
	Path::Construct(folder);
	Storage = folder.Storage;
	
	
	int offset = 0;
	if (!((Folder&)folder).Path.IsEmpty())
	{
		offset = (int)(folder.Path.Data-folder.Data);
		Path.Data = Data+offset;
		Path.Size = folder.Path.Size;
	}
	
	if (!((Folder&)folder).Name.IsEmpty())
	{
		offset = (int)(folder.Name.Data-folder.Data);
		Name.Data = Data+offset;
		Name.Size = folder.Name.Size;
	}
	
	return *this;
}


bool Folder::CreateFolder(char * name)
{
	return (Storage)?Storage->CreateFolder(*this,name):false;
}


bool Folder::DeleteFolder(char * name)
{
	return (Storage)?Storage->DeleteFolder(*this,name):false;
}


bool Folder::CreateFile(char * name)
{
	return (Storage)?Storage->CreateFile(*this,name):false;
}


bool Folder::DeleteFile(char * name)
{
	return (Storage)?Storage->DeleteFile(*this,name):false;
}

bool Folder::Rename(char * name, int size)
{
	return (Storage)?Storage->Rename(*this,name,size):false;
}


bool Folder::Exists()
{
	return (Storage)?Storage->Exists(*this):false;
}


bool Folder::Create()
{
	return (Storage)?Storage->Create(*this):false;
}


bool Folder::Delete(bool recursive)
{
	return (Storage)?Storage->Delete(*this,Files,Folders,recursive):false;
}

bool Folder::List(bool recursive)
{
	return (Storage)?Storage->List(*this,Files,Folders,recursive):false;
}

bool Folder::Status(Stat & stat)
{
	if (Storage)
	{
		stat = Storage->Status(*this);
		return true;
	}
	
	return false;
}

Stat Folder::Status()
{
	if (Storage)
	{
		return Storage->Status(*this);
	}
	
	return Stat();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FolderStorage::FolderStorage()
{
}

FolderStorage::~FolderStorage()
{
}

void FolderStorage::Construct()
{

}


void FolderStorage::Construct(char * path, int size)
{

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
