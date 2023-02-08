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



#include <string.h>
#include <stdio.h>
#include <ctype.h>


#include "hero/hero.h"
#include "hero/string.h"
#include "hero/structure.h"
#include "hero/object.h"

#include "hero/segment.h"


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

class Path : public Segment
{
public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;};

	virtual bool InstanceOf(Identity &identity)
	{
		return Instance == identity || Substring::Instance == identity;
	}

public:


	
	Path(const Path & path);
	Path(const Substring & str);
	Path(char * data, int size);
	Path(const char * data);	
	Path(char c);
	Path(int i);
	Path(float f);
	Path(double d);
	Path();
	~Path();

	
	
	Path(bool b) {};
	
	Path & operator = (const Path & path) {Release();Construct(path.Data,path.Size);return *this;}
	Path & operator = (const Substring & str) {Release();Construct(str.Data,str.Size);return *this;}
	Path & operator = (char * data) {Release();Construct(data);return *this;}
	Path & operator = (char c) {Release();Construct(c);return *this;}
	Path & operator = (int i) {Release();Construct(i);return *this;}
	Path & operator = (float f) {Release();Construct(f);return *this;}
	Path & operator = (double d) {Release();Construct(d);return *this;}


public:

	class Enumerator;
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



class Field
{
public:

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Path::Enumerator : public Hero::Enumerator
{
public:

	
	
	

	Segment * EnumerandFirst;
	Segment * EnumerandLast;
	Segment * Enumerand;

	int EnumerandDepth;		
	int	EnumerandIndex;		
	int EnumerandCount;		
	int EnumerandDirection;

	Enumerator():EnumerandFirst(0),EnumerandLast(0),Enumerand(0),EnumerandIndex(0),EnumerandDepth(0),EnumerandCount(0),EnumerandDirection(0)
	{

	};

	Enumerator(Path &path):EnumerandFirst(0),EnumerandLast(0),Enumerand(0),EnumerandIndex(0),EnumerandDepth(0),EnumerandCount(0),EnumerandDirection(0)
	{
		Enumerate(path);
	};

	~Enumerator()
	{

	};

	void Enumerate(Segment & path, int depth=0)
	{
		Paths paths(&path);
		EnumerandFirst = paths->First()();
		EnumerandLast = paths->Last()();
		EnumerandCount = paths->Count();
		EnumerandIndex = 0;
		EnumerandDepth = depth;
		EnumerandDirection = 1;
	};

	int Index()			{return EnumerandIndex;};
	int Length()		{return EnumerandCount;};
	bool Has()			{return Enumerand != 0;};
	
	bool Move()			
	{
		return Move(1);
	}
    
	bool Move(int amount)		
	{
		amount *= EnumerandDirection;

		if (amount > 0)
		{
			while (amount-- > 0)
				if ((Enumerand = Next()) != 0)
					++EnumerandIndex;
				else
					return false;
		}
		else
		if (amount < 0)
		{
			while (amount++ < 0)
				if ((Enumerand = Prev()) != 0)
					--EnumerandIndex;
				else
					return false;
		}

		return true;
	}



	bool Forward()		
	{
		EnumerandIndex = 0;
		EnumerandDirection = 1; 
		return (Enumerand = EnumerandFirst) != 0;
	}

	bool Reverse()		
	{
		EnumerandIndex = EnumerandCount-1; 
		EnumerandDirection = -1;
		return (Enumerand = EnumerandLast) != 0;
	}


	
	virtual Segment * operator ()(void)	{return Enumerand;}
	virtual Segment * Pointer()			{return Enumerand;}
	virtual Segment & Reference()		{return *Enumerand;}


private:

	Segment * Next()
	{
		Paths paths = Enumerand;
		
		if (EnumerandDepth > 0)
		{	
			if (paths)
			{
				if (paths->First())
				{
					paths = paths->First();
				}
				else
				if (paths->Next())
				{
					paths = paths->Next();
				}
				else
				{
					while(paths && paths->Next())
					{
						paths = (paths->Parent()->Parent())?Paths():paths->Parent();
					}

					if (paths)
						paths = paths->Next();
				}
			}
		}
		else
		{
			paths = paths->Next();
		}

		return paths();
	}

	Segment * Prev()
	{
		Paths paths = Enumerand;
		if (EnumerandDepth > 0)
		{
			if (paths)
			{
				if (paths->Last())
				{
					paths = paths->Last();
				}
				else
				if (paths->Prev())
				{
					paths = paths->Prev();
				}
				else
				{
					while(paths && paths->Prev())
					{
						paths = (paths->Parent()->Parent())?Paths():paths->Parent();
					}

					if (paths)
						paths = paths->Prev();
				}
			}
		}
		else
		{
			paths = paths->Prev();
		}

		return paths();
	}

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

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


