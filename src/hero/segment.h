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




#include "hero/hero.h"
#include "hero/interface.h"
#include "hero/indices.h"
#include "hero/generic.h"
#include "hero/string.h"



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

class Segments;

class Segment : public String, public Placeable
{
public:

	static Identity Instance;
	virtual Identity& Identify() {return Instance;};

	virtual bool InstanceOf(Identity &identity)
	{
		return Instance == identity || Substring::Instance == identity;
	}

public:

	struct Leaf
	{
		Segment * Parent;
	};
	
	struct Node
	{
		Segment * That;
		Segment * Parent;
		int Count;
		
		Segment * Child[];
		
		
		
		
		
	};
	
	
	union Model
	{		
		
		
		
		struct Node * Node;
		void * Empty;
		

		
		Model(void * seg=0):Empty(seg)
		
		{
		}
		
		

		
		
		
		
		
		operator void * () 
		{
			return Empty;
		}

		
	};
	
	union Model Model;

		
	
	bool IsPath();
	bool IsSegment() {return !IsPath();}

	bool IsLeaf();
	bool IsNode();

	
	

	Segment * Parent();
	Segment * First();
	Segment * Last();
	Segment * Prev();
	Segment * Next();		

	
	
	int Children();
	
	Segment(Segment & segment);
	Segment(const Substring & str);
	Segment(char * data, int size);
	Segment(const char * data);
	Segment(char c);
	Segment(int i);
	Segment(float f);
	Segment(double d);
	Segment();
	~Segment();
	
	Segment & operator = (const Segment & segment) {Release();Construct(segment.Data,segment.Size);return *this;}
	Segment & operator = (const Substring & data) {Release();Construct(data.Data,data.Size);return *this;}
	Segment & operator = (char * data) {Release();Construct(data);return *this;}
	Segment & operator = (char c) {Release();Construct(c);return *this;}
	Segment & operator = (int i) {Release();Construct(i);return *this;}
	Segment & operator = (float f) {Release();Construct(f);return *this;}
	Segment & operator = (double d) {Release();Construct(d);return *this;}
	
	Segment & operator[] (int index);
	class String & operator () (){return *(class String *)this;};

	void Destroy() {Destroy(true);}
	void Destroy(bool complete);
	
	void Release() {Release(true);}
	void Release(bool complete);
	
	bool IsSegmentOf(Segment * segment) {return (segment)?IsSubstringOf(*segment):false;}

	
	
	


	Hero::Segments Segments();

	using String::Append;
	Segment & Append(Segment * segment) {return Insert(segment,0,PLACE_AFTER);};

	using String::Prepend;
	Segment & Prepend(Segment * segment) {return Insert(segment,0,PLACE_BEFORE);};

	using String::Insert;
	Segment & Insert(Segment * segment);
	Segment & Insert(Segment * segment, Segment * place, int placement = PLACE_AFTER);
	Segment & Insert(Segment * segment, int index);

	
	
	Segment & Insert(const char * data, Segment * place, int placement = PLACE_AFTER) {return Insert((char*)data,String::Length(data),place,placement);}
	Segment & Insert(char * data, int size, Segment * place, int placement = PLACE_AFTER);


	Segment & Remove(Segment *segment);
	Segment & Remove();
	Segment & Delete(Segment *segment);
	Segment & Delete();

	Segment * Select(const char *data,bool caseless = false) {return Select((char*)data,String::Length(data),caseless);}
	Segment * Select(const Substring &data,bool caseless = false) {return Select(data.Data,data.Size,caseless);}
	Segment * Select(char *data, int size, bool caseless = false);
	Segment * Select(Segment *from, char *data, int size, bool caseless = false);

	Segment * Containing(const char *data,bool caseless = false) {return Containing((char*)data,String::Length(data),caseless);}
	Segment * Containing(const Substring &data,bool caseless = false) {return Containing(data.Data,data.Size,caseless);}
	Segment * Containing(char *data, int size, bool caseless = false);
	Segment * Containing(Segment *from, char *data, int size, bool caseless = false);


	using String::Slice;
	Substring Slice(Segment * from, Segment * to) {return String::Slice(IndexAt(from->Data),IndexAt(to->Data));}
	Substring Slice(Segment * from) {return String::Slice(IndexAt(from->Data));}


	using String::CharAt;

	using String::IndexAt;
	int IndexAt(Segment * segment) {return (segment)?String::IndexAt(segment->Data):-1;}
	
	int Tokenise(const char * tokens, bool keep = false);	
	int Tokenise(Regex & regex, bool keep=false);

	
	
	
	
	
	int Split(const char data, bool keep=false, bool nested=false) {return Split((char*)&data,1,keep,nested);}
	int Split(const Substring & str, bool keep=false, bool nested=false) {return Split(str.Data,str.Size,keep,nested);}
	int Split(const char * data, bool keep=false, bool nested=false) {return Split((char*)data,String::Length(data),keep,nested);}
	int Split(char * data, int size, bool keep = false, bool nested=false);
	int Split(int index, bool keep=false, bool nested=false);
	int Split(int from, int to, bool keep=false, bool nested=false);
	int Split(Regex & regex, bool keep=false, bool nested=false);


	
	
	
	
	
	int Span(const char data, bool keep=false) {return Span((char*)&data,1,keep);}
	int Span(const Substring & str, bool keep=false) {return Span(str.Data,str.Size,keep);}
	int Span(const char *data,bool keep=false) {return Span((char*)data,String::Length(data),keep);}
	int Span(char *data, int size, bool keep=false);
	int Span(int index, bool keep=false);
	int Span(int from, int to, bool keep=false);
	int Span(Regex & regex, bool keep=false);

	

	Superstring Join(const char data, bool nested=false) {return Join(&((char&)data),1,nested);}
	Superstring Join(const Substring & str, bool nested=false) {return Join(str.Data,str.Size,nested);}
	Superstring Join(const char * data, bool nested=false) {return Join((char*)data,String::Length(data),nested);}
	Superstring Join(char * data, int size, bool nested=false);
	Superstring Join(bool nested=false) {return Join(0,0,nested);}


	Segment & AttachBefore(const Substring & str) {return AttachBefore(str.Data,str.Size);}
	Segment & AttachBefore(const char *data) {return AttachBefore((char*)data,String::Length(data));}
	Segment & AttachBefore(char *data, int size) {return Attach(data,size,PLACE_BEFORE);}
	Segment & AttachBefore(Segment * segment) {return Attach(segment,PLACE_BEFORE);}

	Segment & AttachAfter(const Substring & str) {return AttachAfter(str.Data,str.Size);}
	Segment & AttachAfter(const char *data) {return AttachAfter((char*)data,String::Length(data));}
	Segment & AttachAfter(char *data, int size) {return Attach(data,size,PLACE_AFTER);}
	Segment & AttachAfter(Segment * segment) {return Attach(segment,PLACE_AFTER);}

	Segment & Attach(const Substring & str, int placement = PLACE_AFTER) {return Attach(str.Data,str.Size,placement);}
	Segment & Attach(const char * data, int placement = PLACE_AFTER) {return Attach((char*)data,String::Length(data),placement);}
	Segment & Attach(char * data, int size, int placement = PLACE_AFTER);
	Segment & Attach(Segment * segment, int placement = PLACE_AFTER);


	using String::Assign;
	Segment & Assign(char * data, int size);


	using String::At;
	class Substring At(Segment * from, Segment * to) {return String::Slice(IndexAt(from->Data),IndexAt(to->Data));}
	class Substring At(Segment * from) {return String::Slice(IndexAt(from->Data));}

	
	

	void Activate(bool assign=false);
	void Deactivate();

	int Activated();

	bool HasActivated();
	bool IsActivated()	{return !IsNull();};

	

	Segment & Allocate(int amount);
	Segment & Reserve(int amount);
	Segment & Resize(int amount);

	Segment & Left(int amount);
	Segment & Right(int amount);

	Segment & Clear();
	Segment & Term();


	using String::Remaining;
	int Remaining();

	virtual bool IsSubstring();

protected:


	void Distribute(Segment * origin);

	void Relocate(int offset);
	void Relocate(char *offset);

	void Left(int amount, Segment * origin);
	void Right(int amount, Segment * origin);


	
	
	


	Segment * FirstSibling(bool active=true);
	Segment * NextSibling(bool active=true);
	Segment * PrevSibling(bool active=true);
	Segment * LastSibling(bool active=true);

};
		

		











///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Segments : public Iterable<Segment>
{
public: 
	
	Segment * That;
	Segment * Parent;
	
	
	Segment ** Child;
	
	int Count;
	int Index;

	Segments(Segment * segment, Segment * at=0):
		That(segment),Parent(0),Child(0),Count(0),Index(0)
	{
		
		
		
		
		
		
		
		
		
		Construct(segment,at);
	}


	Segments(Segment ** segment, Segment ** at=0):
		That((segment)?*segment:0),Parent(0),Child(0),Count(0),Index(0)
	{
		Construct(segment,at);
	}	

	Segments(const Segments & segments)
	{
		That   = segments.That;
		Parent = segments.Parent;
		Child  = segments.Child;
		Count  = segments.Count;
		Index  = segments.Index;
	}

	void Construct(Segment ** segment, Segment ** at=0)
	{
		Construct((segment)?*segment:0,(at)?*at:0);
	}

	void Construct(Segment * segment, Segment * at=0)
	{
		That = segment;

		Construct();

		if (Child && at)
		{
			for (Index=0;Index < Count;++Index)
				if (Child[Index] == at)
					break;
		}
	}

	void Construct();

	operator void * ()
	{
		return (void*) Empty();
	}

	bool Empty()
	{
		return That == 0 || ((Segment*)That->Model.Empty) == 0;
	}
	
	bool Leaf()
	{
		return !Empty() && ((Segment*)That->Model.Empty) != That;
	}
	
	bool Node()
	{
		return !Empty() && ((Segment*)That->Model.Empty) == That;
	}
	
	
	Segment & At(int index)
	{
		if (index >= 0 && index < Count && Child)
		{
			Segment * segment = Child[index];
			if (segment) return *segment;
		}

		
		Assert(false);
		return *That;
	}

	int Length() {return Count;}

	Segment ** operator () ()
	{
		if (Index >= 0 && Index < Count && Child)
			return &Child[Index];
		return 0;
	}
	
	Segment ** operator [] (int index)
	{
		if (Indices::Normalise(index,Count) && Child)
			return &Child[index];
		return 0;
	}	
	
	Segment ** operator ++ (int)
	{
		
		if (Index >= 0 && Index < Count-1 && Child)
		{
			Segment ** segment = &Child[Index];		
			++Index;
			return segment;
		}
		else
		{
			Index = -1;
		}

		
		return 0;		
	}
	
	Segment ** operator ++ ()
	{		
		
		if (Index >= 0 && Index < Count-1 && Child)
		{
			++Index;
			return &Child[Index];
		}
		else
		{
			Index = -1;
		}


		return 0;
	}
	
	Segment ** operator -- (int)
	{
		
		if (Index > 0 && Index < Count && Child)
		{
			Segment ** segment = &Child[Index];
			--Index;
			return segment;
		}
		else
		{
			Index = -1;
		}

		return 0;
	}
		
			
	Segment ** operator -- ()
	{
		
		if (Index > 0 && Index < Count && Child)
		{
			--Index;
			return &Child[Index];
		}
		else
		{
			Index = -1;
		}

		return 0;
	}
		
	Segment ** First(bool active=true)
	{
		for(Index=0;Index < Count;++Index)
			if (Child[Index] && (!active || Child[Index]->IsActivated()))
				return &Child[Index];
		return 0;
	}

	Segment ** Last(bool active=true)
	{
		for(Index=Count-1;Index >= 0;--Index)
			if (Child[Index] && (!active || Child[Index]->IsActivated()))
				return &Child[Index];
		return 0;
	}

	
	
	
	Segment ** Next(bool active=true)
	{
		while(Index < Count-1)
		{
			++Index;
			if (Child[Index] && (!active || Child[Index]->IsActivated()))
				return &Child[Index];
		}
		return 0;
	}

	Segment ** Prev(bool active=true)
	{
		while(Index > 0)
		{	
			--Index;
			if (Child[Index] && (!active || Child[Index]->IsActivated()))
				return &Child[Index];
		}
		return 0;
	}

	Segment ** Append(const char * data, bool assign=false) {return Append((char*)data,String::Length(data),assign);}
	Segment ** Append(Substring & str, bool assign=false) {return Append(str.Data,str.Size,assign);}
	Segment ** Append(char * data, int size, bool assign=false)
	{	
		return Insert(Count,data,size,assign);
	}
	
	Segment ** Append(Segment ** segment=0) {return Append((segment)?*segment:0);}
	Segment ** Append(Segment * segment=0)
	{
		return Insert(Count,segment);
	}


	Segment ** Prepend(const char * data, bool assign=false) {return Prepend((char*)data,String::Length(data),assign);}
	Segment ** Prepend(Substring & str, bool assign=false) {return Prepend(str.Data,str.Size,assign);}
	Segment ** Prepend(char * data, int size, bool assign=false)
	{	
		return Insert(0,data,size,assign);
	}


	Segment ** Prepend(Segment ** segment=0) {return Prepend((segment)?*segment:0);}
	Segment ** Prepend(Segment * segment=0)
	{
		return Insert(0,segment);
	}

	Segment ** Insert(int index,Substring & str, bool assign=false) {return Insert(index,str.Data,str.Size,assign);}
	Segment ** Insert(int index, const char * data, bool assign=false) {return Insert(index,(char*)data,String::Length(data),assign);}
	Segment ** Insert(int index, char * data, int size, bool assign=false)
	{	
		if (assign)
		{
			Segment * segment = new Segment();
			Segment ** insert = Insert(index,segment);
			segment->Assign(data,size);
			return insert;
		}
		else
		{
			Segment * segment = new Segment(data,size);
			return Insert(index,segment);
		}
	}

	Segment ** Insert(int index, Segment ** segment=0) {return Insert(index,(segment)?*segment:0);}
	Segment ** Insert(int index, Segment * segment=0);

	int IndexOf(Segment ** segment) {return IndexOf((segment)?*segment:0);}
	int IndexOf(Segment * segment)
	{
		
		
		if (Child)
			for (int index=0;index < Count;++index)
				if (Child[index] == segment)
					return index;
		return -1;
	}
	
	Segment ** Select(Segment ** segment) {return Select((segment)?*segment:0);}
	Segment ** Select(Segment * segment)
	{
		int index = IndexOf(segment);
		return (index < 0)?0:Select(index);
	}	

	Segment ** Select(int index)
	{
		if (Indices::Normalise(index,Count) && Child)
		{		
			return &Child[index];
		}
		return 0;
	}

	
	Segment ** Remove()
	{
		return Remove(Index);
	}

	Segment ** Remove(Segment ** segment, bool resize=false) {return Remove((segment)?*segment:0,resize);}
	Segment ** Remove(Segment * segment, bool resize=false)
	{
		int index = IndexOf(segment);
		return (index < 0)?0:Remove(index,resize);
	}

	Segment ** Remove(int index, bool resize=false);


	Segment ** Delete()
	{
		return Delete(Index);
	}

	Segment ** Delete(Segment ** segment, bool resize=false) {return Delete((segment)?*segment:0,resize);}
	Segment ** Delete(Segment * segment, bool resize=false)
	{
		int index = IndexOf(segment);
		return (index < 0)?0:Delete(index,resize);		
	}

	
	Segment ** Delete(int index, bool resize=false);

	int FirstIndex(int index=0, bool active=true);
	int NextIndex(int index, bool active=true);
	int PrevIndex(int index, bool active=true);
	int LastIndex(int index=0, bool active=true);

	
	
	

};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Iterand<Segment> begin(const Segments & segments)
{
	return ((Segments&)segments).Forward();
}

inline Iterand<Segment> end(const Segments & segments)
{
	Iterand<Segment> it = ((Segments&)segments).Forward();
	it.Index = ((Segments&)segments).Length();
	return it;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





class PathsNavigator;

class Paths : public Strong<PathsNavigator*>	
{
public:

	Paths(Segment ** segment);
	Paths(Segment * segment);
	Paths(PathsNavigator * navigator);
	Paths(const Paths & Paths);
	Paths();
	~Paths();

	static Paths Navigate(Segment ** segment);
	static Paths Navigate(Segment * segment);
	static Paths Navigate(Paths & Paths);

	operator PathsNavigator * ();
	
	Segment * operator () (); 

	

	
	
	
	
	
	Paths & operator = (Segment ** segment);
	Paths & operator = (Segment * segment);	
	Paths & operator = (PathsNavigator * navigator);
	Paths & operator = (const Paths & Paths);

	bool operator == (Segment * segment) {return operator()() == segment;}
	bool operator != (Segment * segment) {return operator()() != segment;}

	bool operator == (Paths & Paths) {return (*this)() == Paths();}
	bool operator != (Paths & Paths) {return (*this)() != Paths();}



};

class PathsNavigator
{
public:

	enum Navigation
	{
		NAVIGATE_NONE,
		NAVIGATE_NEXT,
		NAVIGATE_PREV,
		NAVIGATE_PARENT,
		NAVIGATE_FIRST,
		NAVIGATE_LAST,
	};

	int State;
	
	
	
	Paths Navigate;

	Hero::Segments Segments;

	PathsNavigator(Segment ** segment):
		Segments(segment),State(0)
	{
	}

	PathsNavigator(Segment * segment):
		Segments(segment),State(0)
	{
	}

	operator void * ()
	{
		return (void*)Segments.Empty();
	}

	int Count()
	{
		return Segments.Count;
	}

	
	
	

	Paths & Parent()
	{
		if (State == NAVIGATE_PARENT)
			return Navigate;

		Navigate = new PathsNavigator(Segments.Parent);
		State = NAVIGATE_PARENT;
		return Navigate;
	}	

	Paths & Next(bool active=true)
	{
		if (State == NAVIGATE_NEXT)
			return Navigate;

		Hero::Segments siblings(Segments.Parent,Segments.That);
		
		Segment ** segment = siblings.Next(active);
		Navigate = (segment)?new PathsNavigator(segment):0;	
		State = NAVIGATE_NEXT;
		return Navigate;
	}

	Paths & Prev(bool active=true)
	{
		if (State == NAVIGATE_PREV)
			return Navigate;

		Hero::Segments siblings(Segments.Parent,Segments.That);
		
		Segment ** segment = siblings.Prev(active);
		Navigate = (segment)?new PathsNavigator(segment):0;
		State = NAVIGATE_PREV;
		return Navigate;
	}	

	Paths & First(bool active=true)
	{
		if (State == NAVIGATE_FIRST)
			return Navigate;

		
		
		Segment ** segment = Segments.First(active);
		Navigate = (segment)?new PathsNavigator(segment):0;
		State = NAVIGATE_FIRST;
		return Navigate;
	}	


	Paths & Last(bool active=true)
	{
		if (State == NAVIGATE_LAST)
			return Navigate;

		
		
		Segment ** segment = Segments.Last(active);
		Navigate = (segment)?new PathsNavigator(segment):0;
		State = NAVIGATE_LAST;
		return Navigate;
	}			

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

