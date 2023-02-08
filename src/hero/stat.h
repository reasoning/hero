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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/string.h"
#include "hero/path.h"

#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







class Stat
{
public: 

	
	
	

	unsigned int Device;		
	unsigned short Inode;		
	unsigned short Mode;		
	short Links;				
	short User;					
	short Group;				
	unsigned int Extended;		
	long Size;					
	time_t Accessed;			
	time_t Modified;			
	time_t Changed;				

	bool Exists;
	
	
	

	enum StatModes
	{
		MODE_FOLDER		= S_IFDIR,
		MODE_FILE		= S_IFREG,
		MODE_READ		= S_IREAD,
		MODE_WRITE		= S_IWRITE,
		MODE_EXEC		= S_IEXEC,
	};
	
	
	bool IsFolder()		{return (Mode & MODE_FOLDER) != 0;}
	bool IsFile()		{return (Mode & MODE_FILE) != 0;}
	bool IsReadable()	{return (Mode & MODE_READ) != 0;}
	bool IsWriteable()	{return (Mode & MODE_WRITE) != 0;}
	bool IsExecutable() {return (Mode & MODE_EXEC) != 0;}

	Stat(): 
		Exists(false),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
	}


	Stat(Path & path): 
		Exists(false),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
		Construct(path.Data,path.Size);
	}	

	Stat(const Substring & str): 
		Exists(false),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
		Construct(str);
	}

	Stat(char * data, int size): 
		Exists(false),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
		Construct(data,size);
	}

	Stat(const char * data): 
		Exists(false),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
		Construct(data);
	}
	
	Stat(const struct stat & st, bool exists=false):
		Exists(exists),Device(0),Inode(0),Mode(0),Links(0),User(0),Group(0),Extended(0),Size(0),Accessed(0),Modified(0),Changed(0)
	{
		operator = (st);
	}
	
	Stat & operator = (const struct stat &st);

	void Construct(Stat & stat) {*this = stat;}
	void Construct(const Substring & str) {Construct(str.Data,str.Size);}
	void Construct(const char * data) {Construct((char*)data,String::Length(data));}
	void Construct(char * data, int size);

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



