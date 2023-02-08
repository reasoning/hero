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


#include "hero/platform.h"
#include "hero/string.h"
#include "hero/stream.h"
#include "hero/path.h"

#include "hero/object.h"
#include "hero/structure.h"

#include "hero/storage.h"
#include "hero/file.h"
#include "hero/folder.h"

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





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






class Drive : public Path
{
public:

	
	
	

	enum FileExtensionTypes
	{
		
		 TEXT					=0x20,	
		 TEXT_PLAIN				=0x21,	
		 TEXT_HTML				=0x22,
		 TEXT_XML				=0x23,
		 TEXT_SCRIPT			=0x24,	
		 TEXT_CSS				=0x25,

		
		 BINARY					=0x80,

		
		 BINARY_EXEC			=0xA0,
		 BINARY_EXEC_EXE		=0xA1,
		 BINARY_EXEC_SCRIPT		=0xA2,
		 BINARY_EXEC_BAT		=0xA3,
		 BINARY_EXEC_COM		=0xA4,
		 BINARY_EXEC_DLL		=0xA5,

		
		 BINARY_ARCH			=0xC0,
		 BINARY_ARCH_ZIP		=0xC1,
		 BINARY_ARCH_ARJ		=0xC2,
		 BINARY_ARCH_RAR		=0xC3,
		 BINARY_ARCH_GZ			=0xC4,
		 BINARY_ARCH_TAR		=0xC5,


		
		 BINARY_MEDIA			=0xE0,
		
		 BINARY_MEDIA_WAV		=0xE1,
		 BINARY_MEDIA_MP3		=0xE2,
		 BINARY_MEDIA_RM		=0xE3,
		
		 BINARY_MEDIA_GIF		=0xE4,
		 BINARY_MEDIA_JPG		=0xE5,
		 BINARY_MEDIA_PNG		=0xE6,
		 BINARY_MEDIA_BMP		=0xE7,
		 BINARY_MEDIA_TIF		=0xE8,
		 BINARY_MEDIA_PSD		=0xE9,
	};

	struct FileExtensionMap
	{
		const char	* Name;
		unsigned char Type;
	};

	static const int FileExtensionCount = 27;
	static const FileExtensionMap FileExtension[FileExtensionCount];



	void ResolveContentType();
    void ResolveMimeType();
    
public:

	
	enum 
	{
		DRIVE_DISC,
		DRIVE_TAPE,
		DRIVE_SSD,
		DRIVE_OPTICAL,
	};


	Drive(const Drive & filesystem);
	Drive(const Substring & sequence);
	Drive(const char * path);
	Drive(char * path, int size);
	Drive();
	~Drive();

	Drive & operator = (const Drive & filesystem);
	
	
	
	
	
	
	
	
	
	
	
	


	void Normalise();
	
	static bool Status(const Path & path, class Stat & stat); 
	static Stat Status(const Path & path) {return Stat(path.Data,path.Size);}


	

	Stat Status() {return Stat(*this);}
	
	bool IsFolder()		{return (Status().Mode & Stat::MODE_FOLDER) != 0;}
	bool IsFile()		{return (Status().Mode & Stat::MODE_FILE) != 0;}
	bool IsReadable()	{return (Status().Mode & Stat::MODE_READ) != 0;}
	bool IsWriteable()	{return (Status().Mode & Stat::MODE_WRITE) != 0;}
	bool IsExecutable() {return (Status().Mode & Stat::MODE_EXEC) != 0;}


	
	

	
	

	void Release();

	bool Current() {return Drive::Current(*this);}	
	static bool Current(const Path & path); 


	static bool Exists(const Substring & sequence) {return Exists(sequence.Data,sequence.Size);}
	static bool Exists(const char * data) {return Exists((char*)data, String::Length(data));}
	static bool Exists(char * data, int size) {return Stat(data,size).Exists;}
	bool Exists() {return Stat(Data,Size).Exists;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





class DriveFile : public FileStorage
{
public:

	
	
	

	static Identity	Instance;

	virtual Identity & Identify() 
	{
		return Instance;
	} 

public:

	FILE * Handle;
	int Options;
	
	
	int Mode;

	DriveFile();
	~DriveFile();
	
	
	
	void Construct(const Path & path);
	
	void Construct(const Path & path, char * data, int size);		
	
	
	
	virtual bool Open(const Path & path, int options = File::OPTIONS_OPEN_BINARY_MUTABLE);
	virtual bool Open(const Path & path, int mode, int type, int access, int usage);

	
	
	
	

	virtual bool Close(const Path & path);

	bool IsReadable(const Path & path);
	bool IsWriteable(const Path & path);

	virtual int Read(const Path & path, char * data, int size);	

	virtual int ReadLine(const Path & path, String & string);
	virtual int ReadLine(const Path & path, char * data, int size);


	virtual int Write(const Path & path, char * data, int size);
	virtual int WriteLine(const Path & path, char * data, int size);

	virtual bool Flush(const Path & path);
	
	virtual bool Create(const Path & path, int access = File::ACCESS_MUTABLE);
	virtual bool Delete(const Path & path);
	virtual bool Truncate(const Path & path);

	virtual bool Rename(const Path & path, char * name, int size);

	
	virtual int Seek(const Path & path, int position, int origin=-1);	

	virtual bool Eof(const Path & path) {return Handle && feof(Handle);};
	
	virtual bool Exists(const Path & path) {return Drive::Exists(path);}
	
	int Error(const Path & path) {return ferror(Handle);};

	bool IsOpen(const Path & path) {return Handle != 0;}
	bool IsClosed(const Path & path) {return Handle == 0;}
	
	virtual Stat Status(const Path & path);
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class DriveFolder : public FolderStorage
{
public:

	static Identity	Instance;

	virtual Identity & Identify() 
	{
		return Instance;
	} 

public:


	DriveFolder();
	~DriveFolder();

	bool CreateFolder(const Path & path, char *name);	
	bool DeleteFolder(const Path & path, char *name);

	bool CreateFile(const Path & path, char *name);
	bool DeleteFile(const Path & path, char *name);

	bool Rename(const Path & path, char * name, int size);

	bool Exists(const Path & path);
	bool Create(const Path & path);
	bool Delete(const Path & path, Hero::Array<File> & files, Hero::Array<Folder> & folders, bool recursive=false);

	bool List(const Path & path, Hero::Array<File> & files, Hero::Array<Folder> & folders, bool recursive=false);
	
	Stat Status(const Path & path);
	

};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



