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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/stat.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Stat & Stat::operator = (const struct stat &st)
{
	Device = st.st_dev;
	Inode = st.st_dev;
	Mode = st.st_mode;
	Links = st.st_nlink;
	User = st.st_uid;
	Group = st.st_gid;
	Extended = st.st_rdev;
	Size = st.st_size;
	Accessed = st.st_atime;
	Modified = st.st_mtime;
	Changed = st.st_ctime;
	
	
	
	Exists |= (Size > 0 || (Modified != 0 && Mode != 0));
	
	return *this;
}

void Stat::Construct(char * data, int size)
{
	
	
	
	
	
	

	
	

	if (data != 0 && size > 0)
	{
		Superstring super(data,size);
		
		
		
		
		
		
		
		
		
		while(super.Size > 1  && (super.EndsWith('\\') || super.EndsWith('/')))
			super.Right(-1);
			
		#ifdef HERO_PLATFORM_WINDOWS
			
		if (super[1] == ':' && super.Size == 2)
			super.Append('\\');
			
		#endif

		struct stat st;
		memset(&st,0,sizeof(struct stat));
		Exists = ( ::stat(super.Data, &st) == 0 );
		*this = st;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////