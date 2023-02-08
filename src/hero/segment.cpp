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

#include "hero/hero.h"
#include "hero/string.h"
#include "hero/regex.h"
#include "hero/segment.h"
#include "hero/path.h"
#include "hero/indices.h"


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

Identity Segment::Instance;


Segment * Segment::Parent()
{
	Hero::Segments segments(this);
	Segment * segment = segments.Parent;
	return (segment)?segment:0;
}

Segment * Segment::First()	
{
	Hero::Segments segments(this);
	Segment ** segment = segments.First(false);
	return (segment)?*segment:0;
}

Segment * Segment::Last()	
{
	Hero::Segments segments(this);
	Segment ** segment = segments.Last(false);
	return (segment)?*segment:0;
}

Segment * Segment::Next()
{
	Hero::Segments segments(Hero::Segments(this).Parent,this);
	Segment ** next = ++segments;
	if (next && *next)
		return *next;
	
	return 0;		
}

Segment * Segment::Prev()
{
	Hero::Segments segments(Hero::Segments(this).Parent,this);
	Segment ** prev = --segments;
	if (prev && *prev)
		return *prev;
	
	return 0;		
}	

int Segment::Children()
{
	return Hero::Segments(this).Length();
}



Segment::Segment(char c):
	Model(0)
{
	Construct(c);
}

Segment::Segment(int i):
	Model(0)
{
	Construct(i);
}

Segment::Segment(float f):
	Model(0)
{
	Construct(f);
}

Segment::Segment(double d):
	Model(0)
{
	Construct(d);
}

Segment::Segment(char * data, int size):
	Model(0),String(data,size)
{

}

Segment::Segment(const char * data):
	Model(0),String(data)
{
	
}

Segment::Segment(const Substring & str):
	Model(0),String(str.Data,str.Size)
{

}

Segment::Segment(Segment & segment):
	Model(0),String(segment.Data,segment.Size)
{

}

Segment::Segment():Model(0)
{
	
}

Segment::~Segment()
{
	
	
	if (Model == 0)
		return;

	Hero::Segments segments(this);
	while (segments())
	{
		segments.Delete();
		++segments;
	}	


	
	
	
	
	if (segments.Parent)
	{
		segments.Construct(segments.Parent);
		*segments[segments.IndexOf(this)] = 0;
	}

	
	
	if (Model && *((Segment**)Model.Empty) == this)
	{		
		delete [] Model;

		
		
		Model = 0;
	}
}

Segment & Segment::operator[] (int index)
{	
	Hero::Segments segments(this);	

	if (Indices::Normalise(index,segments.Count))
	{
		Segment ** segment = segments[index];
		if (segment && *segment)
			return **segment;
	}
		
	
	return *this;
}

Hero::Segments Segment::Segments()
{
	return Hero::Segments(this);
}

void Segment::Destroy(bool complete)
{	
	

	Hero::Segments segments(this);
	while (segments())
	{
		Segment * segment = *segments();

		if (segment)
		{
			Assert(segment->Parent() == this);

			segment->Destroy();
			if (!segment->InstanceOf(Path::Instance))
			{
				delete segment;
				
				Assert(*segments() == 0);
				*segments() = 0;
			}
		}

		++segments;
	}

	if (Model && *((Segment**)Model.Empty) == this)
	{		
		delete [] Model;
		
		
		
		Model = segments.Parent;
		
	}


	if (complete)
	{
		String::Destroy();
		Deactivate();
	}
	else
	if (segments.Parent)
	{
		Deactivate();
	}

}

void Segment::Release(bool complete)
{
	
	
	Hero::Segments segments(this);
	while (segments())
	{
		Segment * segment = *segments();
		if (segment)
		{
			Assert(segment->Parent() == this);

			segment->Release();
			if (!segment->InstanceOf(Path::Instance))
			{
				delete segment;
				
				Assert(*segments() == 0);
				*segments() = 0;
			}
		}

		++segments;
	}
	
	if (Model && *((Segment**)Model.Empty) == this)
	{		
		delete [] Model.Node;

		
		
		Model = segments.Parent;
		
	}


	
	
	if (complete)
	{
		String::Release();
		Deactivate();
	}
	else
	if (segments.Parent)
	{
		Deactivate();
	}

}



int Segment::Activated()
{
	int count=0;
	Hero::Segments segments(this);
	while (segments() != 0)
	{
		if ((*segments())->Data) ++count;
		++segments;
	}
	return count;
}

bool Segment::HasActivated()
{
	Hero::Segments segments(this);
	while (segments() != 0)
	{
		if ((*segments())->Data) return true;
		++segments;
	}

	return false;
}

bool Segment::IsPath() 
{
	return InstanceOf(Path::Instance);
}

bool Segment::IsLeaf() 
{
	
	
	return Model.Empty != 0 && *((Segment**)Model.Empty) != this;
}

bool Segment::IsNode() 
{
	return Model.Empty != 0 && *((Segment**)Model.Empty) == this;
}




Segment & Segment::Attach(char * data, int size, int placement)
{
	Assert(Parent());

	
	Parent()->Insert(data,size,this,placement);
	return *this;
}

Segment & Segment::Attach(Segment * segment, int placement)
{
	Assert(Parent());

	
	Parent()->Insert(segment,this,placement);
	return *this;
}



Segment & Segment::Insert(char * data, int size, Segment * place, int placement)
{
	
	
	
	
	
	
	



	
	
	
	if (!place->IsActivated()) place->Activate();

	
	Hero::Segments segments(this);
	int index = -1;
	if (place == 0)
	{
		if (!segments.Count || placement == PLACE_BEFORE)
			index = 0;
		else
			index = segments.Count;
	}
	else
	{
		index = segments.IndexOf(place);
	}

	if (index >= 0)
	{
		String::Insert(data,size,index);
	}

	Assert(place == 0 || index != -1);

	return *this;

}

Segment & Segment::Insert(Segment * segment)
{
	Hero::Segments segments(this);
	return Insert(segment,segments.Count);
}

Segment & Segment::Insert(Segment * segment, const int index)
{
	
	
	Hero::Segments segments(this);
	Assert(index >= 0 && index <= segments.Count);

	
	
	
	segments.Insert(index,segment);

	return *this;
}

Segment & Segment::Insert(Segment * segment, Segment * place, int placement)
{
	
	Assert(place == 0 || place->Parent());
	Assert(segment->Next() == 0 && segment->Prev() == 0 && segment->Parent() == 0);


	
	Hero::Segments segments(this);
	int index = -1;
	if (place == 0)
	{
		if (!segments.Count || placement == PLACE_BEFORE)
			index = 0;
		else
			index = segments.Count;
	}
	else
	{
		Assert(segments.Count > 0);

		index = segments.IndexOf(place);
		
		
		
		if (segments.Count && placement == PLACE_AFTER)
			++index;
	}

	Assert(place == 0 || index != -1);

	if (index >= 0)
	{
		Insert(segment,index);
	

		if (!segment->IsEmpty() && !segment->IsAllocated())
		{
			Assert(!segment->Prev() || (segment->Data >= segment->Prev()->Data+segment->Prev()->Size));
			Assert(!segment->Next() || (segment->Data+segment->Size <= segment->Next()->Data));
		}


		if (!segment->IsEmpty())
		{
			if (segment->IsAllocated())
			{
				String string;
				string.Attach(*segment);
			
				segment->Data = Data+index;
				segment->Size = 0;

				segment->Replace(string);
			}

			
			Assert(segment->Data >= Data && segment->Data <= (Data+Size));
		}

	}


	return *this;
}


Segment & Segment::Remove(Segment * segment)
{
	if (segment == 0) return *this;

	Hero::Segments segments(this);
	segments.Remove(segment);
	return *this;
}

Segment & Segment::Remove()
{
	Segment *  parent = Parent();
	if (parent) parent->Remove(this);
	return *this;
}


Segment & Segment::Delete(Segment * segment)
{
	Remove(segment);
	delete segment;
	return *this;
}


Segment & Segment::Delete()
{
	Segment *  parent = Parent();
	if (parent) parent->Delete(this);
	return *this;
}



Segment * Segment::Select(char *data, int size, bool caseless) 
{
	Hero::Segments segments(this);
	int index = segments.FirstIndex();
	if (index)
		return Select(*segments[index],data,size,caseless);
	
	return 0;
}


Segment * Segment::Select(Segment *from, char *data, int size, bool caseless)
{
	if (data == 0)
		return 0;

	if (from && from->Is(data,size,caseless))
		return from;

	
	

	
	
	Segment * segment = 0;
	Hero::Segments segments(this);
	int index = segments.IndexOf(from);
	if (index != -1)
	{
		
		while(segments())
		{
			while (index++ < segments.Count)
				if ((segment = *(segments[index])) && segment->Select(data,size,caseless))
					return segment;

			segment = segments.That;
			segments.Construct(segments.Parent);
			index = segments.IndexOf(segment);
			Assert(index != -1);
		}
	}
	else
	{
		
		while(segments())
		{
			if ((*segments()) && (segment = (*segments())->Select(from,data,size,caseless)))
				return segment;

			++segments;
		}
	}

	return segment;
}


Segment * Segment::Containing(char *data, int size, bool caseless) 
{
	Hero::Segments segments(this);
	int index = segments.FirstIndex();
	if (index)
		return Containing(*segments[index],data,size,caseless);
	
	return 0;
}

Segment * Segment::Containing(Segment *from, char *data, int size, bool caseless)
{
	if (data == 0)
		return 0;

	Segment * place = from;
	if (place && place->Is(data,size,caseless))
		return place;

	

	
	
	

	Paths paths(place);
	if (paths->Count() > 0)
	{
		while (paths != 0)
		{
			Segment * next=0;
			while (paths != 0 && ((next = paths()->Containing(data,size,caseless)) == 0))
				paths = paths->Next();
			place = next;
		}
	}

	if (!place && Contains(data,size,caseless))
	{
		place = this;
	}

	return place;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Segment::Tokenise(const char *tokens, bool keep)
{
	
	
	

	int count=0;
	char *p = (char *)tokens;
	while (*p != 0)
	{
		count += Split(p,1,keep);
		++p;
	}

	return count;
}

int Segment::Tokenise(Regex & regex, bool keep)
{
	return Split(regex,keep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






int Segment::Split(char *data, int size, bool keep, bool nested)
{
	
	
	

	
	
	
	

	Segment * segment;

	if (!HasActivated())
	{
		
		

		segment = new Segment();
		segment->Assign(((Substring&)*this));	
		Insert(segment,0,PLACE_AFTER);
		return segment->Span(data,size,keep);
	}
	else
	{
		segment = First()->FirstSibling();
	}

	int count=0;
	while (segment)
	{
		if (!segment->HasActivated())
		{
			if (nested)
			{
				
				

				Segment *split = new Segment();
				split->Assign(((Substring&)*segment));	
				segment->Insert(split,0,PLACE_AFTER);
				count += split->Span(data,size,keep);
			}
			else
			{
				
				count += segment->Span(data,size,keep);
			}
		}
		else
		{
			
			
			count += segment->Split(data,size,keep,nested);
		}

		segment = segment->NextSibling();
	}

	return count;
}


int Segment::Span(char *data, int size, bool keep)
{

	
	

	if (!Parent())
		return 0;

	int count=0;

	if (IsActivated())
	{
		int index=0;

		if (!HasActivated())
		{
			index = IndexOf(data,size);

			Segment * segment = this;
			while (index != -1)
			{
				if (keep)
				{

					if (index == 0)
					{
						Segment * prev = new Segment();
						prev->Data = segment->Data;
						prev->Size = size;

						
						segment->Data += size;
						segment->Size -= size;									
						
						Parent()->Insert(prev,segment,PLACE_BEFORE);

					}
					else
					if (index == (segment->Size - size))
					{
						Segment * prev = new Segment();
						prev->Data = segment->Data;
						prev->Size = index;

						segment->Data += index;
						segment->Size -= index;

						Parent()->Insert(prev,segment,PLACE_BEFORE);
						index = -1;
					}
					else
					{
						
						Segment * prev = new Segment();
						prev->Data = segment->Data;
						prev->Size = index;

						Segment * delim = new Segment();
						delim->Data = segment->Data + index;
						delim->Size = size;

						segment->Data += index+size;
						segment->Size -= prev->Size+delim->Size;

						Parent()->Insert(prev,segment,PLACE_BEFORE);
						Parent()->Insert(delim,segment,PLACE_BEFORE);

					}

					
					++count;

					
					if (index != -1)
						index = segment->IndexOf(data,size);

				}
				else
				{
					
					
					
					

					if (index == 0)
					{
						
						segment->Data += size;
						segment->Size -= size;
					}
					else
					if (index == ( segment->Size - size ))
					{
						
						segment->Size -= size;
						index = -1;
					}
					else
					{
						
						Segment * prev = new Segment();
						prev->Data = segment->Data;
						prev->Size = index;

						segment->Data += (index + size);
						segment->Size -= (index + size);

						Parent()->Insert(prev,segment,PLACE_BEFORE);					

						
						++count;
					}

					
					if (index != -1)
						index = segment->IndexOf(data,size);
				}
			}
		}
		else
		{
			
			count += Split(data,size,keep);
		}
	}

	return count;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Segment::Split(int index, bool keep, bool nested)
{
	if (index > 0 && index < ((Substring&)*this).Size)
	{
		
		Segment * segment;

		if (!HasActivated())
		{
			segment = new Segment();
			segment->Assign(((Substring&)*this));	
			Insert(segment,0,PLACE_AFTER);
		}
		else
		{
			segment = First()->FirstSibling();
		}

		while (segment)
		{
			if (index > (segment->Data-Data) && index < (segment->Data-Data+segment->Size))
			{
				int offset = index-(segment->Data-Data);

				if (!segment->HasActivated())
				{
					if (nested)
					{
						Segment * split = new Segment();
						split->Assign(((Substring&)*segment));	
						segment->Insert(split,0,PLACE_AFTER);
						segment = split;
					}

					return segment->Span(index,keep);	
				}
				else
				{
					
					return segment->Split(offset,keep,nested);
				}
			}

			segment = segment->NextSibling();
		}
	}

	return 0;
}



int Segment::Span(int index, bool keep)
{
	if (!Parent())
		return 0;

	if (IsActivated() && index > 0 && index < Size)
	{
		if (!HasActivated())
		{
			

			int size = 0;

			if (keep)
			{
				
				
				
				
				
				if (index == 0)
				{
					Segment * prev = new Segment();
					prev->Data = Data;
					prev->Size = size;

					Parent()->Insert(prev,this,PLACE_BEFORE);
				}
				else
				if (index == (Size - size))
				{
					Segment * prev = new Segment();
					prev->Data = Data;
					prev->Size = index;

					Data += index;
					Size -= index;

					Parent()->Insert(prev,this,PLACE_BEFORE);
				}
				else
				{
					Segment * prev = new Segment();
					prev->Data = Data;
					prev->Size = index;

					Segment * delim = new Segment();
					delim->Data = Data + index;
					delim->Size = size;

					Data += index+size;
					Size -= prev->Size+delim->Size;
					
					Parent()->Insert(prev,this,PLACE_BEFORE);
					Parent()->Insert(delim,this,PLACE_BEFORE);
				}
			}
			else
			{
				Segment * prev = new Segment();
				prev->Data = Data;
				prev->Size = index;

				Data += index;
				Size -= index;
				
				Parent()->Insert(prev,this,PLACE_BEFORE);
			}
						
			

			return 1;
		}
		else
		{
			
			return Split(index,keep);
		}
	}

	return 0;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Segment::Split(int from, int to, bool keep, bool nested)
{
	Segment * segment;

	if (!HasActivated())
	{
		
		

		segment = new Segment();
		segment->Assign(((Substring&)*this));
		Insert(segment,0,PLACE_AFTER);
		return segment->Span(from,to,keep);
	}
	else
	{
		segment = First()->FirstSibling();
	}

	int count=0;
	while (segment)
	{
		if (!segment->HasActivated())
		{
			Segment * split = segment;
			if (nested)
			{
				Segment *split = new Segment();
				split->Assign(((Substring&)*segment));
				segment->Insert(split,0,PLACE_AFTER);
			}

			int offset = split->Data-Data;			
			if ((from-offset) >= 0 && (to-offset) < (offset+split->Size))
				count += split->Span((from-offset),(to-offset),keep);
		}
		else
		{
			int offset = segment->Data-Data;			
			if ((from-offset) >= 0 && (to-offset) < (offset+segment->Size))
				count += segment->Split((from-offset),(to-offset),keep,nested);
		}

		segment = segment->NextSibling();
	}

	return count;
}




int Segment::Span(int from, int to, bool keep)
{
	
	

	if (!Parent())
		return 0;

	int count=0;

	Assert(from < to  && to <= Size);

	if (from > 0 && from <= to && (to-from) < Size)
	{
		if (IsActivated())
		{
			int index = from;
			int size = to-from;

			if (!HasActivated())
			{
				if (keep)
				{
					if (index == 0)
					{
						Segment * prev = new Segment();
						prev->Data = Data;
						prev->Size = size;

						Data += size;
						Size -= size;									
						
						Parent()->Insert(prev,this,PLACE_BEFORE);
					}
					if (index == (Size - size))
					{
						Segment * prev = new Segment();
						prev->Data = Data;
						prev->Size = index;

						Data += index;
						Size -= index;

						Parent()->Insert(prev,this,PLACE_BEFORE);
					}
					else
					{
						Segment * prev = new Segment();
						prev->Data = Data;
						prev->Size = index;

						Segment * delim = new Segment();
						delim->Data = Data + index;
						delim->Size = size;
					
						Data += index+size;
						Size -= prev->Size+delim->Size;

						Parent()->Insert(prev,this,PLACE_BEFORE);
						Parent()->Insert(delim,this,PLACE_BEFORE);
					}
					
				}
				else
				{

					Segment * prev = new Segment();
					prev->Data = Data;
					prev->Size = index;

					Data += (index + size);
					Size -= (index + size);

					Parent()->Insert(prev,this,PLACE_BEFORE);
				}				

				
				
				
				++count;
			}
			else
			{
				count += Split(from,to,keep);
			}
		}

	}
	
	return count;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int Segment::Split(Regex & regex, bool keep, bool nested)
{
	
	

	
	

	int matches = -1;

	int count = 0;
	Segment * segment;
	if (!HasActivated())
	{

		
		
		
		
		
		matches = regex.Match(0,Data,Size);
		if (matches == 0)
			return 0;

		segment = new Segment();
		segment->Assign(((Substring&)*this));
		Insert(segment,0,PLACE_AFTER);
		
		
		for (int m=0;m<regex.Matches.Length();++m)
		{
			int from = regex.Matches[m][0].Data-segment->Data;
			int to = from+regex.Matches[m][0].Size;
			
			count += segment->Span(from,to,keep);
			segment = segment->LastSibling();
		}

		return count;
	}
	else
	{
		segment = First()->FirstSibling();
	}

	while (segment)
	{
		if (!segment->HasActivated())
		{

			
			
			
			matches = regex.Match(0,segment->Data,segment->Size);
			if (matches > 0)
			{
				Segment * splice = segment;
				if (nested)
				{
					splice = new Segment();
					splice->Assign(((Substring&)*segment));
					segment->Insert(splice,0,PLACE_AFTER);				
				}

				for (int m=0;m<regex.Matches.Length();++m)
				{
					int from = regex.Matches[m][0].Data-splice->Data;
					int to = from+regex.Matches[m][0].Size;

					if (from > 0 && from <= to && (to-from) < splice->Size)
					{
						
						count += splice->Span(from,to,keep);
					}
					else
					{
						
						
						
						
						
						Segment * next = splice->NextSibling();
						if (next)
							splice = next;
					}
				}
			}
		}
		else
		{		
			
			
			

			segment->Split(regex,keep,nested);
		}

		segment = segment->NextSibling();
	}	
	return count;
}


int Segment::Span(Regex & regex, bool keep)
{
	
	
	int count=0;
	regex.Match(0,Data,Size);
	for (int m=0;m<regex.Matches.Length();++m)
	{
		count += Span(regex.Matches[m][0],keep);
	}
	return count;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Superstring Segment::Join(char * data, int size, bool nested)
{
	Superstring join;
	if (HasActivated())
	{
		Segment * segment = 0;
		Hero::Segments segments(this);

		segments.Index = segments.FirstIndex(0);
		while (segments())
		{
			segment = segments()?(*segments()):0;
			if (!segment)
			{
				++segments;
				continue;
			}

			if (segment->HasActivated() && nested)
			{
				if (!join.IsEmpty())
					join.Append(data,size);
				join.Append(segment->Join(data,size,nested));
			}
			else
			{
				if (!join.IsEmpty())
					join.Append(data,size);
				join.Append(segment->Data,segment->Size);
			}

			++segments;
		}	
	}

	return join;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool Segment::IsSubstring() 
{
	Hero::Segments segments(this);
	return !segments.Parent && Allocated==0;
}

int Segment::Remaining()
{
	Hero::Segments segments(this);
	if (segments.Parent)
	{
		return 0;
	}
	else
	{
		return String::Remaining();
	}
}

Segment & Segment::Allocate(int amount)
{
	Segment * segment = 0;
	Hero::Segments segments(this);
	if (segments.Parent)
	{
		segments.Parent->Allocate(amount);
		Activate();
		return *this;
	}
	else
	{
		
		
		

		char * data = Data;

		String::Allocate(amount);

		
		char *offset = Data;
		while (segments())
		{
			if ((segment = *segments()) != 0)
				segment->Relocate(offset+(segment->Data-data));
			++segments;
		}
	}

	return *this;
}


Segment & Segment::Assign(char * data, int size)
{
	
	
	
	

	if (data == 0 || size == 0)
	{
		
		if (IsActivated()) String::Assign(data,size);
	}
	else
	{	

		Deactivate();
		
		
		

		
		
		
		
		Paths path(this);
		Paths parent = path->Parent();
		Paths next;		
		Paths prev;
		
		prev = path->Prev();
		if (prev && prev()->Data > data)
		{
			while ((path = prev->Prev()) && path()->Data > data)
				prev = path;
				
			parent()->Remove(this);
			parent()->Insert(this,prev(),Placeable::PLACE_BEFORE);
			
			path = this;
			next = prev;			
			prev = path->Prev();		
		}
		
		
		next = path->Next();
		if (next && next()->Data < data)
		{
			while ((path = next->Next()) && path()->Data < data)
				next = path;
				
			parent()->Remove(this);
			parent()->Insert(this,next(),Placeable::PLACE_AFTER);
			
			path = this;
			prev = next;
			next = path->Next();		
		}
						

		String::Assign(data,size);

		
		

		Activate();

		if (parent && (Data < parent()->Data || Data+Size > parent()->Data+parent()->Size))
			Distribute(this);
		else
		if (prev && prev()->Data+prev()->Size > Data)
			Distribute(this);
		else
		if (next && Data+Size > next()->Data)
			Distribute(this);
	}

	return *this;
}


void Segment::Distribute(Segment * origin)
{
	
	
	

	char * data = origin->Data;
	int size = origin->Size; 

	Paths parent(origin);
	Paths grand;
	Paths path;
	
	
	int overlap = 0;


	
	
	while (parent)
	{

		
		path = parent->Next();
		while(path)
		{
			if (path->Count() > 0)
			{
				path = path->First();
			}
			else
			{	
				grand = parent->Parent();
				while (path != grand && !path->Next())
					path = path->Parent();	
					
				if (path == grand) break;
								
				path = path->Next();			
			}		

			if (path()->IsActivated())
			{
				int overlap = (data+size) - path()->Data;
				if (overlap < 0) break;
				path()->Data += overlap;
				path()->Size -= (overlap > path()->Size)?path()->Size:overlap;	
			}
		}
		

		
		path = parent->Prev();
		while (path)
		{
			if (path->Count() > 0)
			{
				path = path->Last();
			}
			else
			{
				grand = parent->Parent();
				while (path != grand && !path->Prev())
					path = path->Parent();
					
				if (path == grand) break;
				
				path = path->Prev();				
			}

			if (path()->IsActivated())
			{
				int overlap = path()->Data+path()->Size - data;
				if (overlap < 0) break;
				path()->Size -= (overlap > path()->Size)?path()->Size:overlap;	
			}
		}
		
		
		
		parent = parent->Parent();
		if (parent)
		{
			overlap = (data+size) - (parent()->Data+parent()->Size);
			if (overlap > 0)
			{
				parent()->Size += overlap;
			}

			overlap = parent()->Data - data;
			if (overlap > 0)
			{

				parent()->Data -= overlap;
				parent()->Size += overlap;
			}

			data = parent()->Data;
			size = parent()->Size;
		}

	}

}



Segment & Segment::Reserve(int amount)
{	
	
	Hero::Segments segments(this);
	if (segments.Parent)
	{
		segments.Parent->Reserve(amount);
		Activate();
	}
	else
	{		
		String::Reserve(amount);

		
	}

	return *this;
}











Segment & Segment::Resize(int amount)
{

	if (Parent())
	{
		
		
		Segment * parent = Parent();
		while(parent->Parent())
			parent = parent->Parent();
		
		if (!parent->Allocated) 
		{	
			
			
			
			
			
			parent->Allocate(amount);
			Activate();

			
			
			
			
			

			
			
			
		}

		if (amount == 0 || amount > 0)
		{
			return Right(amount-Size);
		}
		else
		if (amount < 0)
		{
			Raise("Segment::Resize - Cannot resize a negative amount.\n");
			Assert(amount >= 0);
			return *this;
		}
	}
	else
	{
		
		
		
		
		

		String::Resize(amount);
		return *this;
	}

	
	return *this;
}



Segment & Segment::Clear()
{
	if (Size > 0) 
	{
		Hero::Segments segments(this);
		if (segments.Parent)
		{
			
			Assert(Data >= segments.Parent->Data);
			Right(-Size);
		}
		else
		{
			
			Right(-Size);
		}
	}

	return *this;
}


Segment & Segment::Term()	
{
	Hero::Segments segments(this);
	if (segments.Parent)
		segments.Parent->Term();
	else
	if (Data && Allocated > Size && Size >= 0)
		Data[Size]=0;

	Assert(Allocated == 0 || Allocated > Size);
	return *this;
}

void Segment::Relocate(int offset)
{	
	Relocate(Data+offset);
}

void Segment::Relocate(char *offset)
{
	if (IsActivated())
	{	
		Hero::Segments segments(this);
		while(segments())
		{
			Segment * segment = *segments();
			if (segment)
				segment->Relocate(offset + (segment->Data - Data));
			++segments;
		}
		Data = (char*) offset;
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Segment & Segment::Left(int amount)
{
	if (amount < 0 && amount < -Size)
		amount = -Size;
		
	if (amount != 0)
	{
		int size = Size;
		Left(amount,this);

		Paths path;

		if (amount > 0)
		{

			path = this;
			path = path->First();
			while(path)
			{
				path()->Relocate(amount);
				path = path->Next();
			}			
		}
		else
		if (amount < 0)
		{
			path = this;
			while(path)
			{
				while (path && path->Count() > 0 )
					path = path->First();

				if (path == this) break;

				Paths next = path;
				while(next)
				{
					if (next()->IsActivated())
					{
						if (next()->Data > Data-amount)
						{
							next()->Data += amount;
						}
						else
						{			

							int overlap = (Data-amount) - next()->Data;
							if (overlap < 0) break;

							if (overlap > next()->Size)
							{	
								next()->Data = Data;
								next()->Size = 0;
							}
							else
							{
								next()->Data = Data;
								next()->Size -= overlap;					
							}
						}
					}
					next = next->Next();
				}

				path = path->Parent();

				if (path == this) break;

				while (path && path->Next() == 0) 
					path = path->Parent();
				
				if (path)
					path = path->Next();
			}
		}

	}
		
	return *this;
}

void Segment::Left(int amount, Segment * origin)
{
	Hero::Segments segments(this);
	if (segments.Parent)
	{
		segments.Parent->Left(amount,origin);
		Size += amount;

		segments.Construct(segments.Parent,this);
		++segments;
		while(segments())
		{
			if (*segments())
				(*segments())->Relocate(amount);
			++segments;
		}
	}
	else
	{
		Paths path;
		
		Assert(origin != 0);

		int offset = 0;
		int size = 0;

		
		
		if (amount > 0)
		{
			origin->Reserve(amount);
			origin->Activate();

			Assert(Size+amount < Allocated);

			offset = (origin->Data)-Data;
			size = Size-offset;		
		}
		else
		if (amount < 0)
		{

			if (!Allocated)
				return;

			
			
			if (!origin->IsActivated())
				return;
				
			offset = (origin->Data-amount)-Data;
			size = Size-offset;	
				
		}	
		else
		{
			return;
		}
			
			
		Assert(origin->IsActivated());
				
		memmove(Data+offset+amount,Data+offset,size);
	
		Assert(offset >= 0);
		Assert(offset <= Size);
		Assert(Data+offset+size <= Data+Size);
	
		Size += amount; 
		Term();
	}

}


Segment & Segment::Right(int amount)
{
	if (amount < 0 && amount < -Size)
		amount = -Size;
		
	if (amount != 0)
	{
	
		
		
		int size = Size;

		Right(amount,this);
		
		if (amount < 0)
		{			
			

			Paths path = this;
			while(path)
			{
				while (path && path->Count() > 0 )
					path = path->Last();

				if (path == this) break;

				Paths prev = path;
				while(prev)
				{
					if (prev()->IsActivated())
					{
						int overlap = prev()->PointerAt(prev()->Size) - PointerAt(size+amount);
						if (overlap < 0) break;
						
						
						
						
						if (overlap > prev()->Size)
						{
							prev()->Data -= overlap-prev()->Size;
							prev()->Size = 0;
						}
						else
						{
							prev()->Size -= overlap;
						}
						

					}
					prev = prev->Prev();
				}

				path = path->Parent();
				
				if (path == this) break;

				while (path && path->Prev() == 0) 
					path = path->Parent();
				
				if (path)
					path = path->Prev();
			}
		}


	}
	
	return *this;
}

void Segment::Right(int amount, Segment * origin)
{
	Hero::Segments segments(this);
	if (segments.Parent)
	{
		segments.Parent->Right(amount,origin);
		Size += amount;

		segments.Construct(segments.Parent,this);
		++segments;
		while(segments())
		{
			if (*segments())
				(*segments())->Relocate(amount);
			++segments;
		}
	}
	else
	{
		Paths path;
		
		Assert(origin != 0);

		int offset = 0;
		int size = 0;

		
		
		if (amount > 0)
		{
			origin->Reserve(amount);
			origin->Activate();

			Assert(Size+amount < Allocated);

			offset = (origin->Data+origin->Size)-Data;
			size = Size-offset;		
		}
		else
		if (amount < 0)
		{
			if (!Allocated)
				return;

			
			
			if (!origin->IsActivated())
				return;
				
			offset = (origin->Data+origin->Size)-Data;
			size = Size-(offset);
				
		}	
		else
		{
			return;
		}
			
			
		Assert(origin->IsActivated());
				
		memmove(Data+offset+amount,Data+offset,size);
	
		Assert(offset >= 0);
		Assert(offset <= Size);
		Assert(Data+offset+size <= Data+Size);
	
		Size += amount; 
		Term();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void Segment::Activate(bool assign)
{
	
	
	
	

	
	
	
	


	Hero::Segments segments(this);
	Segment * segment = 0;
	if (segments.Parent)
	{
		if (!segments.Parent->IsActivated())
		{
			if (IsActivated())
			{
				segments.Parent->Data = Data;
				segments.Parent->Size = Size;
			}

			segments.Parent->Activate();
		}
	

		if (!IsActivated())
		{
			segment = PrevSibling();
			if (segment)
				Data = segment->PointerAt(segment->Size);
			else
				Data = segments.Parent->Data;

			if (assign)
			{
				segment = NextSibling();
				if (segment)
				{
					Size = segment->Data-Data;
				}
				else
				{
					Size = segments.Parent->PointerAt(segments.Parent->Size)-Data;
				}
			}
			else
			{
				
				
				
				
				
				Size = 0;
			}
		}

	}
}


void Segment::Deactivate()
{
	Hero::Segments segments(this);
	Segment ** segment = 0;
	if (segments.Parent && IsActivated())
	{
		Data = 0;
		Size = 0;

		int index = 0;
		while (index < segments.Count)
		{
			segment = segments[index];
			if (segment && *segment)
				(*segment)->Deactivate();
		}
	}
}




Segment * Segment::FirstSibling(bool active)
{
	Hero::Segments segments(this);
	segments.Construct(segments.Parent,this);

	Segment * segment = 0;

	if (!active || IsActivated())
		segment = *segments();

	Segment * prev = 0;
	while ((prev = segment->PrevSibling(active)) != 0)
		segment = prev;

	if (segment)
		return segment;	

	
	segment = *segments();
	segment = segment->NextSibling(active);

	if (segment)
		return segment;	

	if (!active || IsActivated())
		return *segments();

	return 0;
}

Segment * Segment::LastSibling(bool active)
{
	Hero::Segments segments(this);
	segments.Construct(segments.Parent,this);

	Segment * segment = 0;

	if (!active || IsActivated())
		segment = *segments();

	
	Segment * next = 0;
	while ((next = segment->NextSibling(active)) != 0)
		segment = next;

	if (segment)
		return segment;	

	
	segment = *segments();
	segment = segment->PrevSibling(active);

	if (segment)
		return segment;	

	if (!active || IsActivated())
		return *segments();

	return 0;
}

Segment * Segment::NextSibling(bool active)
{
	Hero::Segments segments(this);
	segments.Construct(segments.Parent,this);

	Segment ** segment = ++segments;
	if (active)
	{
		while (segment != 0 && (*segment == 0 || (*segment)->IsNull()))
			segment = ++segments;
	}
	
	if (segment)
		return *segment;

	return 0;
}

Segment * Segment::PrevSibling(bool active)
{
	Hero::Segments segments(this);
	segments.Construct(segments.Parent,this);

	Segment ** segment = --segments;
	if (active)
	{
		while (segment != 0 && (*segment == 0 || (*segment)->IsNull()))
			segment = --segments;
	}
	
	if (segment)
		return *segment;

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Segments::Construct()
{
	Parent = 0;
	Child = 0;
	Count = 0;
	Index = 0;	

	if (That == 0)
		return;
	
	if (((Segment*)That->Model.Empty) == 0)
	{
		
	}
	else
	if (*((Segment**)That->Model.Empty) != That)
	{
		
		Parent = (Segment*)That->Model.Empty;
	}
	else
	{
		Segment ** segments = (Segment**)That->Model.Empty;
		
		Parent = segments[1];
		Count = Cast::Primitive<int>(segments[2]);	
		Child = segments+3;
	}			
}

Segment ** Segments::Insert(int index, Segment * segment)
{
	
	
	Indices::Normalise(index,Count);
	
	
	
	
	
	Assert(IndexOf(segment) == -1);
	
	if (index < 0 || index > Count)
		return 0;		

	
	
	

	if (That == 0)
		return 0;

	if (segment->Model.Empty != 0)
	{
		
		Assert(segment->Model.Empty == 0);
		return 0;
	}

	
	
	

	Segment ** segments = (Segment **)new void *[3+Count+1];			
	
	if (index == Count)
	{
		memcpy(segments+3,Child,Count*sizeof(void*));
	}
	else
	if (index == 0)
	{
		memcpy(segments+3+1,Child,Count*sizeof(void*));
	}
	else
	if (index > 0)
	{
		memcpy(segments+3,Child,index*sizeof(void*));
		memcpy(segments+3+index+1,Child+index,(Count-index)*sizeof(void*));	
	}	

	Child = segments+3;	
	Child[index] = segment;
	segments[0] = That;
	segments[1] = Parent;
	segments[2] = (Segment*)++Count;	


	
	

	
	
	
	
	
	
	if (That->Model && *((Segment**)That->Model.Empty) == That)
	{		
		delete [] That->Model;
	}

	That->Model = segments;	

	
	if (segment->Model == 0)
		
		segment->Model = That;
	else
		
		*(((Segment **)segment->Model.Empty)+1) = That;



	
	
	if (!segment->IsEmpty())
	{
		if (segment->IsAllocated())
		{
			
			
			
			String string;
			string.Attach(*segment);

			segment->Data = 0;
			segment->Size = 0;
			segment->Replace(string);
		}

		
		Assert(segment->Data >= That->Data && segment->Data <= (That->Data+That->Size));
	}

	return &Child[index];
}

Segment ** Segments::Remove(int index, bool resize)
{
	Indices::Normalise(index,Count);

	Segment ** result = Select(index);
	if (result)
	{
		if (resize)
		{
			Segment ** segments = new Segment *[3+Count-1];			
			memcpy(segments+3,Child,index*sizeof(void*));
			memcpy(segments+3+index,Child,(Count-index)*sizeof(void*));	
			Child = segments+3;
			segments[0] = That;
			segments[1] = Parent;
			segments[2] = (Segment*)--Count;			
		}
		else
		{
			Segment ** segments = (Segment**)That->Model.Empty;
			segments[2] = (Segment*)--Count;
		}
		
		Segment * segment = *result;
		if (segment->Model != segment)
			segment->Model = 0;
		else
			*(((Segment **)segment->Model.Empty)+1) = 0;

		*result = 0;
	}
	
	return result;
}


Segment ** Segments::Delete(int index, bool resize)
{
	Segment ** select = Select(index);
	if (select && *select)
	{	
		Segment * segment = *select;

		
		
		
		
		
			
		Segments segments(segment);

		Assert(segments.Parent == That);

		if (segments.Count)
		{
			while (segments() != 0)
			{
				segments.Delete();
				++segments;
			}	

			delete [] segment->Model;
		}

		segment->Model = 0;

		if (!segment->InstanceOf(Path::Instance))
			delete segment;

		*select = 0;			
	}
	return select;
			
}

int Segments::FirstIndex(int index, bool active)
{
	if (!Indices::Normalise(index,Count)) 
		return -1;

	Segment ** segment = 0;
	int first = -1;

	segment = (*this)[index];
	if (segment && (*segment != 0 && (!active || (*segment)->IsActivated())))
		first = index;

	
	int prev = index;
	while ((prev = PrevIndex(prev,active)) != -1)
		first = prev;

	if (first != -1)
		return first;	

	
	first = NextIndex(index,active);

	if (first != -1)
		return first;	

	if (!active || (*(*this)[index])->IsActivated())
		return index;

	return -1;
}

int Segments::LastIndex(int index, bool active)
{
	if (!Indices::Normalise(index,Count)) 
		return -1;

	Segment ** segment = 0;
	int last = -1;

	segment = (*this)[index];
	if (segment && (*segment != 0 && (!active || (*segment)->IsActivated())))
		last = index;

	
	int next = index;
	while ((next = NextIndex(next,active)) != -1)
		last = next;

	if (last != -1)
		return last;	

	
	last = PrevIndex(index,active);

	if (last != -1)
		return last;	

	if (!active || (*(*this)[index])->IsActivated())
		return index;

	return -1;
}

int Segments::NextIndex(int index, bool active)
{
	if (!Indices::Normalise(index,Count)) 
		return -1;

	Segment ** segment = 0;
	int next = -1;

	while (++index < Count && (segment = (*this)[index]) != 0)
		if (*segment != 0 && (!active || (*segment)->IsActivated()))
		{
			next = index;
			break;
		}
	
	return next;
}

int Segments::PrevIndex(int index, bool active)
{
	if (!Indices::Normalise(index,Count)) 
		return -1;

	Segment ** segment = 0;
	int prev = -1;

	while (--index >= 0 && (segment = (*this)[index]) != 0)
		if (*segment != 0 && (!active || (*segment)->IsActivated()))
		{
			prev = index;
			break;
		}

	return prev;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Paths::Paths(Segment ** segment):
	Strong<PathsNavigator*>(new PathsNavigator((*segment)?*segment:0))
{
}

Paths::Paths(Segment * segment):
	Strong<PathsNavigator*>(new PathsNavigator(segment))
{
}

Paths::Paths(PathsNavigator * navigator):
	Strong<PathsNavigator*>(navigator)
{
}

Paths::Paths(const Paths & paths):
	Strong<PathsNavigator*>(paths)
{

}

Paths::Paths()
{

}

Paths::~Paths()
{

}

Paths Paths::Navigate(Segment ** segment)
{
	return new PathsNavigator(segment);
}	

Paths Paths::Navigate(Segment * segment)
{
	Paths navigate = new PathsNavigator(segment);
	return navigate;
}

Paths Paths::Navigate(Paths & paths)
{
	Paths navigate = new PathsNavigator(paths->Segments.That);
	return navigate;
}

Paths::operator PathsNavigator * ()
{
	
	
	

	PathsNavigator * navigator = Strong<PathsNavigator*>::operator PathsNavigator * ();
	return (navigator && navigator->Segments.That)?navigator:0;	
}
	

Segment * Paths::operator () ()
{
	
	
	
	
	PathsNavigator * navigator = Strong<PathsNavigator*>::operator PathsNavigator * ();
	return (navigator)?navigator->Segments.That:0;		
}


Paths & Paths::operator = (Segment ** segment)
{
	Strong<PathsNavigator*>::operator = (new PathsNavigator((*segment)?*segment:0));
	return *this;
}

Paths & Paths::operator = (Segment * segment)
{
	Strong<PathsNavigator*>::operator = (new PathsNavigator(segment));
	return *this;
}

Paths & Paths::operator = (PathsNavigator * navigator)
{
	Strong<PathsNavigator*>::operator = (navigator);
	return *this;
}

Paths & Paths::operator = (const Paths & paths)
{
	Strong<PathsNavigator*>::operator = (paths);
	return *this;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
