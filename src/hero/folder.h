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
#include "hero/stat.h"
#include "hero/file.h"
#include "hero/structure.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FolderStorage;


class FolderPath : public FilePath
{
public:

	FolderPath();
	FolderPath(const Substring & sequence);
	FolderPath(const char * name);
	FolderPath(char * name, int size);
	~FolderPath();

	void Construct();
	void Construct(char * path, int size);
	void Construct(const Substring &path) {return Construct(path.Data,path.Size);};
	void Construct(const char * path) {return Construct((char*)path,String::Length(path));};

	FolderPath & operator = (const char * path)
	{
		Construct(path);
		return *this;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
class Folder : public FolderPath
{
public:


public:
	
	static Identity Instance;

	virtual Identity& Identify()
	{
		return Instance;
	}

	virtual bool InstanceOf(Identity &identity)
	{
		return Instance == identity || Path::InstanceOf(identity);
	}

public:

	Strong<FolderStorage*> Storage;
	
	Array<File>	Files;
	Array<Folder> Folders;

	Folder(const Folder & folder);
	Folder(FolderStorage * driver);
	Folder(Strong<FolderStorage*> & driver);	
	Folder(const Substring & path);
	Folder(const char *path);
	Folder(char * path, int size);
	Folder();
	~Folder();

	Folder & operator = (const Folder & folder);

	void Construct();
	void Construct(char * path, int size);
	void Construct(const Substring &path) {return Construct(path.Data,path.Size);};
	void Construct(const char * path) {return Construct((char*)path,String::Length(path));};

	
	bool CreateFolder(char *name);	
	bool DeleteFolder(char *name);

	bool CreateFile(char *name);
	bool DeleteFile(char *name);

	bool Rename(Substring & name) {Rename(name.Data,name.Size);};
	bool Rename(const char *name) {Rename((char*)name,String::Length(name));}
	bool Rename(char * name, int size); 


	bool Exists();
	
	bool Create();
	bool Delete(bool recursive = false);

	bool List(bool recursive = false);

	bool Status(Stat & stat);
	Stat Status();
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class FolderStorage : public Kind
{
public:

	FolderStorage();
	~FolderStorage();

	virtual void Construct();
	void Construct(const Path & path) {return Construct(path.Data,path.Size);};
	void Construct(const char *path) {return Construct((char*)path,String::Length(path));};
	void Construct(char *path, int size);

	virtual bool CreateFolder(const Path & path, char *name)=0;	
	virtual bool DeleteFolder(const Path & path, char *name)=0;

	virtual bool CreateFile(const Path & path, char *name)=0;
	virtual bool DeleteFile(const Path & path, char *name)=0;

	bool Rename(const Path & path, Substring & name) {Rename(path,name.Data,name.Size);};
	bool Rename(const Path & path, const char *name) {Rename(path,(char*)name,String::Length(name));}
	virtual bool Rename(const Path & path, char * name, int size)=0;
	
	virtual bool Exists(const Path & path)=0;
	virtual bool Create(const Path & path)=0;
	virtual bool Delete(const Path & path, Hero::Array<File> & files, Hero::Array<Folder> & folders, bool recursive=false)=0;

	virtual bool List(const Path & path, Hero::Array<File> & files, Hero::Array<Folder> & folders, bool recursive=false)=0;

	virtual Stat Status(const Path & path)=0;
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


