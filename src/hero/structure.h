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

#ifdef HERO_PLATFORM_POSIX
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hero/hero.h"
#include "hero/callback.h"
#include "hero/interface.h"
#include "hero/generic.h"
#include "hero/traits.h"
#include "hero/assert.h"
#include "hero/sync.h"

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if HERO_PLATFORM_CPP >= 2011

#include <initializer_list>

namespace Hero {
template<typename _Kind_>
using Constructor = std::initializer_list<_Kind_>;
}

#else

namespace Hero {
template<typename _Kind_>
class Constructor
{
public:

	typedef int iterator;
};
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Range
{
	int First;
	int Last;

	Range(int first=0,int last=0):First(first),Last(last){}

	bool IsEmpty() {return First == Last;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _First_, typename _Second_, typename _Third_,typename _Fourth_=None, typename _Fifth_=None>
class Tuple
{
public:

	Variable<_First_> First;
	Variable<_Second_> Second;
	Variable<_Third_> Third;
	Variable<_Fourth_> Fourth;
	Variable<_Fifth_> Fifth;

};

template <typename _First_, typename _Second_, typename _Third_>
class Tuple<_First_,_Second_,_Third_>
{
public:

	Variable<_First_> First;
	Variable<_Second_> Second;
	Variable<_Third_> Third;

};

template <typename _First_, typename _Second_, typename _Third_, typename _Fourth_>
class Tuple<_First_,_Second_,_Third_,_Fourth_>
{
public:

	Variable<_First_> First;
	Variable<_Second_> Second;
	Variable<_Third_> Third;
	Variable<_Fourth_> Fourth;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _First_, typename _Second_> class Pair: public Object
{
public:

	static Identity Instance;
	virtual Identity& Identify(){return Instance;};

public:
	Variable<_First_> First;
	Variable<_Second_> Second;

	Pair()
	{
	}

	Pair(const Pair<_First_,_Second_> & pair):First(pair.First),Second(pair.Second)
	{
	}

	Pair(typename Template<_First_>::ConstReference first):First(first)
	{
	}

	Pair(typename Template<_First_>::ConstReference first, typename Template<_Second_>::ConstReference second):First(first),Second(second)
	{
	}

	~Pair()
	{
	}

	int Compare(Object * object, int comparitor = Comparable::COMPARE_GENERAL)
	{	
		if (object->InstanceOf(this))
		{
			Pair<_First_,_Second_> * pair = (Pair<_First_,_Second_> *)object;
			int result = Comparer<_First_>::Compare(this->First(),pair->First(),comparitor);
			if (result == 0)
				return Comparer<_Second_>::Compare(this->Second(),pair->Second(),comparitor);
			else 
				return result;
		}
		return Identity::Error;
	}

	int Hash()
	{

		int hash = (Hasher<_First_>::Hash(this->First())<<8) ^ Hasher<_Second_>::Hash(this->Second());
		return hash;
	}
};

template <typename _First_, typename _Second_> 
Identity Pair<_First_,_Second_>::Instance;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Key_, typename _Value_> 
class Mapped: public Object
{
public:

	static Identity Instance;
	virtual Identity& Identify(){return Instance;};

public:
	Variable<_Key_> Key;

	Optional<_Value_> Value;

	Mapped()
	{
	}

	Mapped(const Mapped & mapped):Key(mapped.Key),Value(mapped.Value)
	{
	}

	Mapped(typename Template<_Key_>::ConstReference key):Key(key)
	{
	}

	Mapped(typename Template<_Key_>::ConstReference key, typename Template<_Value_>::ConstReference value):Key(key),Value(value)
	{
	}

	~Mapped()
	{
	}

	Mapped & operator = (const Mapped & mapped)
	{
		Key = mapped.Key;
		Value = mapped.Value;
		return *this;
	}

	int Compare(Object * object, int comparitor = Comparable::COMPARE_GENERAL)
	{	
		if (object->InstanceOf(this))
		{
			Mapped<_Key_,_Value_> * mapped = (Mapped<_Key_,_Value_> *)object;
			return Comparer<_Key_>::Compare(this->Key(),mapped->Key(),comparitor);
		}
		return Identity::Error;
	}

	int Hash()
	{
		return Hasher<_Key_>::Hash(Key());
	}
};

template <typename _Key_, typename _Value_> 
Identity Mapped<_Key_,_Value_>::Instance;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class Iterand;

template<typename _Kind_>
class Iterator;

template<typename _Kind_>
class Iterable
{
public:

	Weak<Iterable<_Kind_> *,Shared> Self;	

	virtual ~Iterable()
	{
		Self = 0;
	}

	virtual _Kind_ & At(int index)=0;

	virtual int Length()=0;

	virtual int Move(Iterand<_Kind_> & iterand, int amount);

	virtual Iterator<_Kind_> Iterate();	
	virtual Iterator<_Kind_> Iterate(int from, int to);
	virtual Iterator<_Kind_> Iterate(Iterand<_Kind_> & from, Iterand<_Kind_> & to);

	Iterand<_Kind_> Forward();
	Iterand<_Kind_> Reverse();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class Iterand
{
public:

	enum IterandOptions
	{
		MODE_NONE		=(0),
		MODE_FORWARD	=(1)<<0,

		MOVE_REVERSE	=0xFFFFFFFF 
	};

	Weak<class Iterable<_Kind_>*,Shared> Iterable;
	int Index;
	int Option;

	Iterand():
		Iterable(),Option(0),Index(0) 
	{

	}

	Iterand(class Iterable<_Kind_> & iterable, int option=0, int index=0):
		Iterable(&iterable),Option(option),Index(index) 
	{

	}

	Iterand(const Iterand<_Kind_> & iterand)	
	{
		operator = (iterand);
	}

	Iterand(const Iterator<_Kind_> & iterator)
	{
		operator = (iterator.Iterand);
	}

	Iterand<_Kind_> & operator = (const Iterator<_Kind_> & iterator)
	{		
		return operator = (iterator.Iterand);
	}

	Iterand<_Kind_> & operator = (const Iterand<_Kind_> & iterand)
	{		
		Iterable = iterand.Iterable;
		Index = iterand.Index;
		Option = iterand.Option;
		return *this;
	}

	Iterand<_Kind_> & operator ++ (int)
	{

		if (!Iterable) return *this;
		Iterand<_Kind_> iterand = *this;
		Iterable->Move(*this,1);
		return iterand;
	}

	Iterand<_Kind_> & operator ++ ()
	{

		if (!Iterable) return (*this);
		Iterable->Move(*this,1);
		return *this;
	}

	Iterand<_Kind_> & operator -- (int)
	{

		if (!Iterable) return *this;
		Iterand<_Kind_> iterand = *this;
		Iterable->Move(*this,-1);
		return iterand;		
	}

	Iterand<_Kind_> & operator -- ()
	{

		if (!Iterable) return (*this);
		Iterable->Move(*this,-1);
		return *this;
	}

	bool Has()
	{
		return operator void *() != 0;
	}	

	operator void * () const
	{
		if (Iterable && Index >= 0 && Index < Iterable->Length())
		{
			return &Iterable->At(Index);
		}

		return 0;
	}

	bool operator == (const Iterand<_Kind_> & iterand) 
	{	
		return Iterable == iterand.Iterable && Index == iterand.Index;
	}

	typename Hero::Template<_Kind_>::Reference operator () (void) const
	{

		return ((Iterand<_Kind_>*)this)->operator()();
	}

	typename Hero::Template<_Kind_>::Reference operator () (void) 
	{
		if (Iterable && Index >= 0 && Index < Iterable->Length())
		{
			return Iterable->At(Index);
		}

		return Null<_Kind_>::Value();
	}

	typename Hero::Template<_Kind_>::Reference operator * () {return (*this)();}
	typename Hero::Template<_Kind_>::Pointer operator -> () {return & (*this)();}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Iterator
{
public:

	class Iterand<_Kind_> Iterand;
	class Range Range;

	Iterator(Iterable<_Kind_> & iterable) 
	{
		Iterand.Iterable = &iterable;
	}

	Iterator(Iterable<_Kind_> & iterable, int from, int to) 
	{
		Iterand.Iterable = &iterable;
		Range.First = from;
		Range.Last = to;
	}

	Iterator(const Iterator<_Kind_> & iterator) 
	{
		Iterand = iterator.Iterand;
		Range = iterator.Range;
	}

	Iterator(const Hero::Iterand<_Kind_> & iterand) 
	{
		Iterate(iterand);
	}

	Iterator(const Hero::Iterand<_Kind_> & from, const Hero::Iterand<_Kind_> & to) 
	{
		Iterate(from,to);
	}

	Iterator() {}
	~Iterator() {}

	void Iterate(Iterator<_Kind_> & iterator)
	{
		(*this) = iterator;
	}

	void Iterate(Iterable<_Kind_> & iterable)
	{
		(*this) = iterable.Iterate();
	}

	void Iterate(const Iterable<_Kind_> & iterable, int from, int to)
	{
		(*this) = iterable.Iterate(from,to);
	}

	void Iterate(const Iterable<_Kind_> & iterable, const Hero::Iterand<_Kind_> & from, const Hero::Iterand<_Kind_> & to)
	{
		(*this) = iterable.Iterate(from,to);
	}

	void Iterate(int from, int to)
	{
		if (Iterand.Iterable)
		{
			(*this) = Iterand.Iterable->Iterate(from,to);
		}
		else
		{
			Iterand = Hero::Iterand<_Kind_>();
		}
	}

	void Iterate(const Hero::Iterand<_Kind_> & iterand)
	{
		Iterand = iterand;
	}

	void Iterate(const Hero::Iterand<_Kind_> & from, const Hero::Iterand<_Kind_> & to)
	{
		if (Iterand.Iterable)
		{
			(*this) = Iterand.Iterable->Iterate(from,to);
		}
		else
		if (from.Iterable != 0 && from.Iterable == to.Iterable)
		{
			(*this) = from.Iterable->Iterate(from,to);
		}
		else
		{
			Iterand = Hero::Iterand<_Kind_>();
		}
	}

	Iterator<_Kind_> & operator = (const Iterator<_Kind_> & iterator)
	{
		Iterand = iterator.Iterand;
		Range = iterator.Range;

		return *this;
	}

	Iterator<_Kind_> & operator = (const Hero::Iterand<_Kind_> & iterand)
	{

		Iterand = iterand;
		return *this;
	}

	void Forward(class Iterand<_Kind_> & iterand) 
	{

		if (iterand.Iterable != Iterand.Iterable) 
			iterand.Iterable = Iterand.Iterable;

		if (Iterand.Iterable) 
		{
			iterand = Iterand.Iterable->Forward();
			if (!Range.IsEmpty())
				iterand.Index = Range.First;
		}
		else
		{
			iterand = Hero::Iterand<_Kind_>();
		}
	}

	void Reverse(class Iterand<_Kind_> & iterand) 
	{
		if (iterand.Iterable != Iterand.Iterable) 
			iterand.Iterable = Iterand.Iterable;

		if (Iterand.Iterable) 
		{
			iterand = Iterand.Iterable->Reverse();
			if (!Range.IsEmpty())
				iterand.Index = Range.Last-1;
		}
		else
		{
			iterand = Hero::Iterand<_Kind_>();
		}
	}

	void Move(class Iterand<_Kind_> & iterand)
	{			
		Move(iterand,1);
	}

	void Move(class Iterand<_Kind_> & iterand, int amount)
	{	
		if (iterand == 0)
			return;

		if (iterand.Iterable && iterand.Option == Hero::Iterand<_Kind_>::MODE_NONE)
			Forward(iterand);

		if (iterand.Iterable != Iterand.Iterable)
		{

			Forward(iterand);
		}

		if (Iterand.Iterable && Range.IsEmpty() || (Iterand.Index < Range.Last-1 && Iterand.Index >= Range.First))  
			Iterand.Iterable->Move(iterand,amount);
		else

			iterand = Hero::Iterand<_Kind_>();
	}

	int Index(class Iterand<_Kind_> & iterand)
	{
		if (iterand != 0 && iterand.Iterable == this->Iterand.Iterable)
		{
			return (this->Iterand.Iterable)?this->Iterand.Index:-1;
		}

		return -1;
	}

	class Iterand<_Kind_> & Forward()
	{
		Forward(Iterand);
		return Iterand;
	}

	class Iterand<_Kind_> & Reverse()
	{
		Reverse(Iterand);
		return Iterand;
	}

	class Iterand<_Kind_> & Move()
	{
		Move(Iterand);
		return Iterand;
	}

	class Iterand<_Kind_> & Move(int amount)
	{
		Move(Iterand,amount);
		return Iterand;
	}

	bool Has()
	{

		return Iterand != 0 && Iterand.Iterable;
	}

	int Index()
	{
		return (Iterand != 0 && Iterand.Iterable)?Iterand.Index:-1;
	}

	void Release() 
	{
		Iterand.Release();
	}

	void Destroy()
	{
		Iterand.Destroy();
	}

	typename Type<_Kind_>::Pointer Pointer() {return Iterand.Pointer();}
	typename Type<_Kind_>::Reference Reference() {return Iterand.Reference();}
	typename Template<_Kind_>::Reference operator () (void) {return Iterand();}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
int Iterable<_Kind_>::Move(Iterand<_Kind_> & iterand, int amount)
{
	if (amount > 0)
	{

		if (iterand.Index < Length())
			iterand.Index += amount;					
	}
	else
	if (amount < 0)
	{
		if (iterand.Index > 0)
			iterand.Index -= amount;
	}

	return iterand.Index;

}

template<typename _Kind_>
Iterand<_Kind_> Iterable<_Kind_>::Forward()
{
	if (Length() > 0)
	{
		return Iterand<_Kind_>(*this,Iterand<_Kind_>::MODE_FORWARD,0);
	}

	return Iterand<_Kind_>();
}

template<typename _Kind_>
Iterand<_Kind_> Iterable<_Kind_>::Reverse()
{

	if (Length() > 0)
	{	
		return Iterand<_Kind_>(*this,Iterand<_Kind_>::MODE_REVERSE,Length()-1);
	}

	return Iterand<_Kind_>();
}

template<typename _Kind_>
Iterator<_Kind_> Iterable<_Kind_>::Iterate()
{
	return Iterator<_Kind_>(*this);
}

template<typename _Kind_>
Iterator<_Kind_> Iterable<_Kind_>::Iterate(int from, int to) 
{
	return Iterator<_Kind_>(*this,from,to);
}

template<typename _Kind_>
Iterator<_Kind_> Iterable<_Kind_>::Iterate(Iterand<_Kind_> & from, Iterand<_Kind_> & to) 
{
	Assert(from.Iterable == this && from.Iterable == to.Iterable);
	if (from.Iterable == this && from.Iterable == to.Iterable)
	{
		return Iterator<_Kind_>(*this,from.Index,to.Index);
	}

	return Iterator<_Kind_>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_, int _Factor_=175>
class Array : public Iterable<_Kind_>
{
public:

	using Result = Result<bool,int>;

	static const float Factor;

	struct PointerOrPrimitive
	{
		enum {Value = Traits::Or<Traits::Pointer<_Kind_>::Value,Traits::Intrinsic<_Kind_>::Value>::Value};		
	};

	static void Allocate(_Kind_ *& data, int allocated)
	{
		if (allocated <= 0)
		{

			data = 0;
		}
		else
		if (PointerOrPrimitive::Value)
		{
			data = (_Kind_*)malloc(allocated*sizeof(_Kind_));	
		}
		else
		{
			data = new _Kind_[allocated];									
		}
	}

	static void Reallocate(_Kind_ *& data, int size, int allocated)
	{
		if (PointerOrPrimitive::Value)
		{

			_Kind_ * mem = (_Kind_*)realloc(data,(size_t)allocated*sizeof(_Kind_));

			data = mem;
		}
		else
		{
			_Kind_ * mem = new _Kind_[(size_t)allocated];
			Move(mem,data,size);
			delete [] data;

			data = mem;			
		}	
	}

	static void Delete(_Kind_ * data)
	{
		if (PointerOrPrimitive::Value)
		{
			free(data);
		}
		else
		{
			delete [] data;		
		}	
	}

	static _Kind_ * Copy(_Kind_ * to, _Kind_ * from, int size)
	{

		Assert(size >= 0);
		Assert(to+size <= from || to >= from+size);

		if ((to+size >= from && to <= from+size) || size < 0) return 0;

		if (PointerOrPrimitive::Value)
		{
			memcpy(to,from,sizeof(_Kind_)*(size_t)size);
		}
		else
		{

			int loop = (size+7)/8;
			if (loop > 0)
			{
				switch (size%8)
				{
					case 0: do {	*to++ = *from++;
					case 7:			*to++ = *from++;
					case 6:			*to++ = *from++;
					case 5:			*to++ = *from++;
					case 4:			*to++ = *from++;
					case 3:			*to++ = *from++;
					case 2:			*to++ = *from++;
					case 1:			*to++ = *from++;
							} while (--loop > 0);
				}
			}
		}

		return to;
	}

	static _Kind_ * Set(_Kind_ * to,typename Template<_Kind_>::ConstReference kind, int size)
	{
		Assert(size >= 0);
		if (size < 0) return 0;

		if (PointerOrPrimitive::Value)
		{
			memset(to,((typename Template<_Kind_>::Reference)kind),sizeof(_Kind_)*size);
		}
		else
		{

			int loop = (size+7)/8;
			if (loop > 0)
			{
				switch (size%8)
				{
					case 0: do {	*to++ = (typename Template<_Kind_>::Reference)kind;
					case 7:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 6:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 5:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 4:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 3:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 2:			*to++ = (typename Template<_Kind_>::Reference)kind;
					case 1:			*to++ = (typename Template<_Kind_>::Reference)kind;
							} while (--loop > 0);
				}
			}
		}

		return to;
	}

	static _Kind_ * Move(_Kind_ * to, _Kind_ *from, int size)
	{	
		Assert(from != 0 && size >= 0);
		if (!from || size < 0) return to;

		if (PointerOrPrimitive::Value)
		{
			memmove(to,from,sizeof(_Kind_)*size);
		}
		else
		{ 
			if (to > from+size || to < from )
			{
				int loop = (size+7)/8;
				if (loop > 0)
				{
					switch (size%8)
					{
						case 0: do {	*to++ = (_Kind_&&)(*from++); 
						case 7:			*to++ = (_Kind_&&)(*from++); 
						case 6:			*to++ = (_Kind_&&)(*from++); 
						case 5:			*to++ = (_Kind_&&)(*from++); 
						case 4:			*to++ = (_Kind_&&)(*from++); 
						case 3:			*to++ = (_Kind_&&)(*from++); 
						case 2:			*to++ = (_Kind_&&)(*from++); 
						case 1:			*to++ = (_Kind_&&)(*from++); 
								} while (--loop > 0);
					}
				}

			}
			else
			if (to > from)
			{
				to += size-1;
				from += size-1;				

				int loop = (size+7)/8;
				if (loop > 0)
				{
					switch (size%8)
					{
						case 0: do {	*to-- = (_Kind_&&)(*from--); 
						case 7:			*to-- = (_Kind_&&)(*from--); 
						case 6:			*to-- = (_Kind_&&)(*from--); 
						case 5:			*to-- = (_Kind_&&)(*from--); 
						case 4:			*to-- = (_Kind_&&)(*from--); 
						case 3:			*to-- = (_Kind_&&)(*from--); 
						case 2:			*to-- = (_Kind_&&)(*from--); 
						case 1:			*to-- = (_Kind_&&)(*from--); 
								} while (--loop > 0);
					}
				}
			}
		}

		return to;
	}

	template<typename _Compare_>
	static int Index(_Kind_ * data, int size, _Compare_ && compare, int seek=1)
	{

		int first	= 0;
		int last	= size-1;
		int middle	= first + (int)((last-first+1)>>1);
		int result	= 0;

		while (first <= last)
		{
			result = compare(data+middle);
			if (result == 0 && seek == 0) break;

			if (seek == 0)
			{

				if (result < 0)
					last = middle-1;
				else
					first = middle+1;
			}
			else
			if (seek > 0)
			{

				if (result >= 0)
					first = middle+1;
				else
					last = middle-1;
			}
			else
			if (seek < 0)
			{

				if (result <= 0)
					last = middle-1;
				else
					first = middle+1;
			}

			middle = first + (int)((last-first+1)>>1);		

			if (middle == size && first <= last)
				--middle;
		}	

		return middle;		
	}

public:

	static int Move(Array & to, int toIndex, Array & from, int fromIndex, int size)
	{

		if (!Indices::Normalise(fromIndex,from.Size)) 
			return 0;

		if (!Indices::Normalise(toIndex,to.Size,false)) 
			return 0;

		int res = Array<_Kind_>::Copy(to,toIndex,from,fromIndex,size);

		if (size > res) size = res;
		if (size <= 0) return 0;

		_Kind_ * offset;
		int left=0,right=0;

		offset = from.Data+fromIndex;		
		left = fromIndex;
		right = from.Size-fromIndex;

		Array<_Kind_>::Move(offset,offset+size,right-size);

		from.Size -= size;

		return size;
	}

	static int Copy(Array & to, int toIndex, Array & from, int fromIndex, int size)
	{

		if (!Indices::Normalise(fromIndex,from.Size)) return 0;

		int amount = from.Size-fromIndex;

		if (size > amount) size = amount;		
		if (size <= 0) return 0;		

		_Kind_ * offset, * data;
		int left=0,right=0;

		data = from.Data+fromIndex;
		offset = to.Data+toIndex;
		left = toIndex;
		right = to.Size-toIndex;

		Array<_Kind_>::Move(offset+size,offset,right);
		Array<_Kind_>::Copy(offset,data,size);

		to.Size += size;
		return size;
	}

	static int Insert(Array & to, int toIndex, _Kind_ * data, int size)
	{

		if (!Indices::Normalise(toIndex,to.Size,false)) return 0;

		int amount = to.Allocated-to.Size;

		if (amount < size) size = amount;
		if (size <= 0) return 0;		

		_Kind_ * offset;
		int left=0,right=0;

		offset = to.Data+toIndex;
		left = toIndex;
		right = to.Size-toIndex;

		Array<_Kind_>::Move(offset+size,offset,right);
		Array<_Kind_>::Copy(offset,data,size);

		to.Size += size;
		return size;
	}

public:

	_Kind_ * Data;
	int Size;
	int Allocated;

	Array(Constructor<_Kind_> && init):Data(0),Size(0),Allocated(0)
	{
		Reserve((int)init.size());
		typename Constructor<_Kind_>::iterator it = init.begin();
		while(it != init.end()) 
		{
			Append((*it));
			++it;
		}
	}

	Array(int allocated):Data(0),Size(0),Allocated(0)
	{
		Array<_Kind_>::Allocate(Data,allocated);
		Size = 0;
		Allocated = allocated;
	}

	Array():Data(0),Size(0),Allocated(0)
	{
	}

	Array(_Kind_ * data, int size):Data(0),Size(0),Allocated(0) 
	{
		if (size > 0 && data != 0)
		{
			Array<_Kind_>::Allocate(Data,size);
			Allocated = size;					
			Array<_Kind_>::Copy(Data,data,size);	
			Size = size;
		}
	}

	Array(const Array & array):Data(0),Size(0),Allocated(0)
	{
		Clone(array);	
	}

	Array(Array && array):Data(0),Size(0),Allocated(0)
	{
		Move((Array&&)array);	
	}

	~Array()
	{
		if (Data != 0)
		{
			Array<_Kind_>::Delete(Data);
		}

		Data = 0;
		Size = 0;
		Allocated = 0;
	}

	Array & Clone(const Array & array)
	{
		if (&array == this)
			return *this;

		if (Data)
			Array<_Kind_>::Delete(Data);

		Array<_Kind_>::Allocate(Data,array.Allocated);
		Array<_Kind_>::Copy(Data,array.Data,array.Size);

		Allocated = array.Allocated;
		Size = array.Size;

		return *this;
	}

	Array & Copy(const Array & array)
	{
		if (&array == this)
			return *this;

		if (Data)
			Array<_Kind_>::Delete(Data);

		Array<_Kind_>::Allocate(Data,array.Size);
		Array<_Kind_>::Copy(Data,array.Data,array.Size);
		Size = Allocated = array.Size;

		return *this;
	}

	Array & Move(Array && array)
	{
		if (&array == this)
			return *this;

		if (Data)
			Array<_Kind_>::Delete(Data);

		Data = array.Data;
		Allocated = array.Allocated;
		Size = array.Size;		

		array.Data = 0;
		array.Allocated = 0;
		array.Size = 0;		

		return *this;
	}

	Array & Swap(const Array & array)
	{
		_Kind_ * data = array.Data;
		int size = array.Size;
		int allocated = array.Allocated;

		((Array&)array).Data = Data;
		((Array&)array).Size = Size;
		((Array&)array).Allocated = Allocated;

		Data = data;
		Size = size;
		Allocated = allocated;

		return *this;
	}

	void Release()
	{
		if (Data)
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Release(Data[i]);
			}

			Size=0;
		}
	}

	void Destroy()
	{
		if (Data)
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Destroy(Data[i]);
			}

			Array<_Kind_>::Delete(Data);
			Data=0;
			Size=0;
			Allocated=0;
		}
	}

	int Length() {return Size;}		

	_Kind_ & At(int index)
	{

		Indices::Normalise(index,Size);

		if (index < Allocated)
			return Data[index];

		Assert(false);

		return Null<_Kind_>::Value();
	}

	bool IsEmpty()		{return (Data==0||Size==0);}
	bool IsNull()		{return (Data==0);}

	_Kind_ & operator [] (int index)
	{
		return At(index);
	}

	Array& Allocate(int amount=0)
	{

		if (Allocated > 0)
		{
			Assert(Allocated >= Size);

			if (amount == 0)
				amount = (int)(Allocated*Factor)+1;
			else
			if (amount < 0 || amount < Size)
				amount = Size;

			Array<_Kind_>::Reallocate(Data,Size,amount);
		}
		else
		{
			if (amount == 0)
				amount = (int)(((Size>0)?Size:1)*Factor)+1;			
			else
			if (amount < 0 || amount < Size)
				amount = Size;

			_Kind_ * data;
			Array<_Kind_>::Allocate(data,amount);

			if (Size > 0 && Data != 0)
				Array<_Kind_>::Copy(data,Data,Size);		

			Data = data;
		}

		Allocated = amount;			
		return *this;

	}

	Array& Reserve(int amount)
	{
		int remaining = Allocated-Size;

		if (amount < 0)
		{

			amount = Size+(remaining-amount);
			if (amount < Size) amount = Size;
		}
		else
		if (amount > 0)
		{
			if (amount <= remaining) return *this;

			amount = Size + amount;
		}
		else
		{

		}

		return Allocate(amount);
	}

	Array& Resize(int amount)
	{

		Allocate(amount);

		if (amount > Size)
			Size = amount;

		return *this;
	}

	Array & operator = (const Array & array)
	{
		return Copy(array);
	}		

	Array & operator = (Array && array)
	{
		return Move((Array&&)array);
	}		

	Array & operator << (typename Template<_Kind_>::ConstReference kind)
	{
		Append(kind);
		return *this;
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert((_Kind_&&)_Kind_(kind));
	}

	Result Insert(_Kind_ && kind)
	{
		if (IsNull() || Size >= Allocated)
			Allocate();				

		Data[Size++] = (_Kind_&&)kind;
		return Result(true,Size-1);;
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind, int index)
	{
		return Insert((_Kind_&&)_Kind_(kind),index);
	}

	Result Insert(_Kind_ && kind, int index)
	{
		Assert(index <= Size);

		if (!Indices::Normalise(index,Size,false))
			return false;

		if (IsNull() || Size >= Allocated)		
			Allocate();

		if (index < Size)
			Array<_Kind_>::Move(Data+index+1,Data+index,Size-index);		

		Data[index] = (_Kind_&&)kind;
		++Size;

		return Result(true,index);		
	}

	Result Append(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,Size);
	}

	Result Append(_Kind_ && kind)
	{
		return Insert((_Kind_&&)kind,Size);
	}

	Result Prepend(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,0);
	}

	Result Prepend(_Kind_ && kind)
	{
		return Insert((_Kind_&&)kind,0);
	}

	int Index(typename Template<_Kind_>::Reference kind, int seek=1)
	{
		return Index([&](_Kind_ & rhs) {return kind-rhs;},seek);
	}

    int Index(typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Index([&](_Kind_ & rhs) {return kind-rhs;},seek);
	}

	template <typename _Compare_>
	int Index(_Compare_ && compare, int seek=1)
	{
		return Index((_Compare_&&)compare, true, seek);
	}

	template <typename _Compare_>
	int Index(_Compare_ && compare, bool functor, int seek=1)
	{

		int first	= 0;
		int last	= this->Size-1;
		int middle	= first + (int)((last-first+1)>>1);
		int result	= 0;

		while (first <= last)
		{
			result = compare(Data[middle]);
			if (result == 0 && seek == 0) break;

			if (seek == 0)
			{
				if (result < 0)
					last = middle-1;
				else
					first = middle+1;
			}
			else
			if (seek > 0)
			{

				if (result >= 0)
					first = middle+1;
				else
					last = middle-1;
			}
			else
			if (seek < 0)
			{

				if (result <= 0)
					last = middle-1;
				else
					first = middle+1;

			}

			middle = first + (int)((last-first+1)>>1);				

			if (middle == this->Size && first <= last)
				--middle;
		}	

		return middle;		
	}

	template<typename _Equals_>
	int Find(_Equals_ && equals, int seek=1)
	{
		return Find((seek<0)?-1:0,equals,seek);
	}	

	int Find(typename Template<_Kind_>::Reference kind, int seek=1)
	{
		return Find((seek<0)?-1:0,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}

	int Find(typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Find((seek<0)?-1:0,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}

	int Find(int from, typename Template<_Kind_>::Reference kind, int seek=1)
	{
		return Find(from,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}   

	int Find(int from, typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Find(from,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}    

	template<typename _Equals_>
	int Find(int from, _Equals_ && equals, int seek=1)
	{

		if (IsEmpty() || !Indices::Normalise(from,Size))
			return -1;

		if (seek == 0)
		{
			if (equals(Data[from]) == 0)
				return from;
		}
		else
		{
			int index=from-seek;
			while((index+=seek) < Size && index >= 0)
			{
				if (equals(Data[index]))
					return index;
			}
		}

		return -1;
	}

	Result Remove(typename Template<_Kind_>::ConstReference kind)
	{
		int index=Size;
		while(index-- > 0)
		{
			if (Data[index] == kind)
			{
				Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
				return Result(true,index);					
			}
		}

		return false;
	}

	Result Remove(typename Template<_Kind_>::Reference kind, int index)
	{
		if (IsEmpty() || !Indices::Normalise(index,Size))
			return false;

		kind = Data[index];
		Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
		return Result(true,index);					
	}

	Result RemoveAt(int index)
	{

		if (IsEmpty() || !Indices::Normalise(index,Size))
			return false;

		Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
		return Result(true,index);		
	}

	Result Select(typename Template<_Kind_>::ConstReference kind)
	{
		int index=Size;
		while(index-- > 0)
			if (Data[index] == kind)
				return Result(true,index);					

		return false;				
	}

	Result Select(typename Template<_Kind_>::Reference kind, int index)
	{
		Assert(index <= Size);

		if (IsEmpty() || !Indices::Normalise(index,Size))
			return false;

		kind = Data[index];
		return Result(true,index);

	}

	void Unique(const Callback<int,_Kind_,_Kind_> & compare)
	{
		Sort();
		for (int i=0;i<Size-1;++i)
		{	
			if (compare(Data[i],Data[i+1]) == 0)
				Array<_Kind_>::Move(Data[i],Data[i+1],--Size-i);
		}	
	}

	void Swap(int i,int j)
	{

		if (i != j)
		{
			_Kind_ data = (_Kind_&&)Data[i];
			Data[i] = Data[j];
			Data[j] = (_Kind_&&)data;
		}
	}

	void Shuffle(int seed=1)
	{
		if (Size > 2)
		{
			srand((unsigned int)seed);
			for (int i=Size-1;i>0;--i)
				Swap(i,rand()%i);
		}
	}

	template<typename _Func_>
	void Sort(_Func_ && compare)
	{
		Sort(0,Size-1,compare);
	}

	void Sort(int first, int last, 
		const Callback<int,typename Template<_Kind_>::ConstReference,typename Template<_Kind_>::ConstReference> & compare)
	{
		if (!Indices::Normalise(first,Size) || !Indices::Normalise(last,Size))
			return;

		if (last < first) return;

		Array<Range> stack;
		stack.Allocate(last-first);
		stack.Insert(Range(first,last));

		while (stack.Size > 0)
		{

			Range range = stack.At(stack.Size-1);
			stack.Remove(range,stack.Size-1);

			first = range.First;
			last = range.Last;

			if (first < last)
			{

				int left = first+1;
				int right = last;

				while(right >= left)
				{
					while (left < last && compare(Data[left],Data[first]) < 0)
						++left;

					while (right > first && compare(Data[right],Data[first]) > 0)
						--right;

					if (left >= right) break;

					Swap(left,right);
				}

				if (right == first && left == first+1)
					continue;

				Swap(first,right);

				stack.Append(Range(first,right-1));
				stack.Append(Range(left+1,last));

			}

		}
	}					

};

template<typename _Kind_, int _Factor_>
const float Array<_Kind_,_Factor_>::Factor = (_Factor_>100)?(float)_Factor_/100:1.5;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename _Kind_>
class Stack : public Array<_Kind_>
{
public:

	using typename Array<_Kind_>::Result;

	Result Push(typename Template<_Kind_>::ConstReference kind)
	{
		return this->Append(kind);
	}

	bool Pop(typename Template<_Kind_>::ConstReference kind)
	{

		int index = this->Find(kind,1);
		if (index > 0 && index == this->Length()-1)
			return this->RemoveAt(index);
		return false;		

	}

	bool Pop(typename Template<_Kind_>::Reference kind)
	{

		return this->Remove(kind,this->Length()-1);		
	}

	bool Pull(typename Template<_Kind_>::ConstReference kind)
	{

		int index = this->Find(kind,-1);
		if (index == 0)
			return this->RemoveAt(index);
		return false;

	}	

	bool Pull(typename Template<_Kind_>::Reference kind)
	{

		return this->Remove(kind,0);		
	}	

	bool Pop()
	{

		this->RemoveAt(-1);
	}	

	bool Pull()
	{
		_Kind_ kind;
		return this->Remove(kind,0);	
	}

	Result Peek()
	{

		if (!this->IsEmpty())
			return Result(true,this->Length()-1);

		return Result();
	}	

	bool Peek(typename Template<_Kind_>::Reference kind)
	{
		return this->Select(kind,-1);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_, int _Allocated_=4, int _Factor_=175>
class Vector : public Iterable<_Kind_>
{
public:

	using Result = Result<bool,int>;

	static const float Factor; 

	_Kind_ * Data;
	int Size;
	int Allocated;

	static const int Default = ((_Allocated_*sizeof(_Kind_)>4*sizeof(int))?1:_Allocated_);

	_Kind_ Mem[Default] = {_Kind_()};

	Vector(Constructor<_Kind_> && init):Data(0),Size(0),Allocated(0)
	{
		Data = Mem;
		Allocated = Vector::Default;

		Reserve((int)init.size());
		typename Constructor<_Kind_>::iterator it = init.begin();
		while(it != init.end()) 
		{
			Append((*it));
			++it;
		}
	}

	Vector(int allocated):Data(0),Size(0),Allocated(0)
	{
		Data = Mem;
		Allocated = Vector::Default;

		if (allocated > sizeof(Mem))
		{
			Data = new _Kind_[allocated];		
			Allocated = allocated;
		}	
	}

	Vector(_Kind_ * data, int size):Data(0),Size(0),Allocated(0)
	{
		Data = Mem;
		Allocated = Vector::Default;

		if (size > 0 && data != 0)
		{
			if (size > Allocated)
			{
				Data = new _Kind_[size];								
				Allocated = size;	
			}

			Array<_Kind_>::Copy(Data,data,size);	
			Size = size;
		}
	}

	Vector():Data(0),Size(0),Allocated(0)
	{
		Data = Mem;
		Allocated = Vector::Default;
	}

	Vector(const Vector & vector):Data(0),Size(0),Allocated(0)
	{
		Data = Mem;
		Allocated = Vector::Default;

		if (vector.Size > Allocated)
		{
			Data = new _Kind_[vector.Size];								
			Allocated = vector.Size;					
		}				

		Array<_Kind_>::Copy(Data,vector.Data,vector.Size);	
		Size = vector.Size;
	}

	Vector(Vector && vector):Data(0),Size(0),Allocated(0)
	{
		Move((Vector&&)vector);
	}

	~Vector()
	{
		if (Data != Mem && Data != 0)
		{

			Array<_Kind_>::Delete(Data);
		}

		Data = 0;
		Size = 0;
		Allocated = 0;
	}

	Vector & Clone(const Vector & vector)
	{

		return Copy(vector);
	}

	Vector & Copy(const Vector & vector)
	{
		if (&vector == this)
			return *this;

		if (Data != Mem && Data != 0)
			Array<_Kind_>::Delete(Data);

		Data = Mem;
		Allocated = Vector::Default;	

		if (vector.Size > Allocated)
		{
			Allocated = vector.Size;																		
			Array<_Kind_>::Allocate(Data,Allocated);
		}

		Array<_Kind_>::Copy(Data,vector.Data,vector.Size);
		Size = vector.Size;

		return *this;

	}

	Vector & Move(const Vector & vector)
	{
		if (&vector == this)
			return *this;

		if (Data != Mem && Data != 0)
			Array<_Kind_>::Delete(Data);

		if (vector.Data == vector.Mem)
		{
			Data = Mem;
			Allocated = Vector::Default;
			Array<_Kind_>::Copy(Data,vector.Data,vector.Size);			
		}
		else
		{
			Allocated = vector.Allocated;
			Data = vector.Data;
		}

		Size = vector.Size;

		((Vector&)vector).Data = ((Vector&)vector).Mem;
		((Vector&)vector).Allocated = 0;
		((Vector&)vector).Size = 0;		

		return *this;
	}

	Vector & Swap(const Vector & vector)
	{
		if (&vector == this)
			return *this;

		Vector temp;
		temp.Move((Vector&)vector);
		((Vector&)vector).Move(*this);
		Move(temp);
	}

	void Release()
	{
		if (Data)
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Release(Data[i]);
			}

			Size=0;
		}
	}

	void Destroy()
	{
		if (Data)
		{
			for (int i=0;i<Length();++i)
				Disposer<_Kind_>::Destroy(Data[i]);

			if (Data != Mem)
				Array<_Kind_>::Delete(Data);

			Data=0;
			Size=0;
			Allocated=0;
		}
	}	

	int Length() {return Size;}

	_Kind_ & At(int index)
	{
		if (Indices::Normalise(index,Size))
			return Data[index];

		return Null<_Kind_>::Value();
	}

	bool IsEmpty()		{return (Data==0||Size==0);}
	bool IsNull()		{return (Data==0);}

	_Kind_ & operator [] (int index)
	{
		return At(index);
	}

	Vector& Allocate(int amount=0)
	{

		if (amount == 0)
		{	
			amount = ((Allocated <= 0)?((Size>0)?Size:1):(int)(Allocated*Factor))+1;
		}
		else
		if (amount < 0 || amount < Size)
		{
			amount = Size;
		}

		if (Data == Mem)
		{

			if (amount <= Vector::Default)
				return *this;

			_Kind_ * data;
			Array<_Kind_>::Allocate(data,amount);
			Array<_Kind_>::Copy(data,Data,Size);	

			Data = data;
		}
		else			
		if (Allocated > 0)
		{

			if (amount <= Allocated)
				return *this;;

			Array<_Kind_>::Reallocate(Data,Size,amount);
		}
		else
		{
			_Kind_ * data;
			Array<_Kind_>::Allocate(data,amount);

			if (Size > 0 && Data != 0)
				Array<_Kind_>::Copy(data,Data,Size);

			Data = data;
		}

		Allocated = amount;									
		return *this;
	}

	Vector& Reserve(int amount)
	{
		int allocated = (Data==Mem)?Vector::Default:Allocated;
		int remaining = allocated-Size;

		if (amount < 0)
		{	

			amount = Size+(remaining-amount);
			if (amount < Size) amount = Size;
		}
		else
		if (amount > 0)
		{
			if (amount <= remaining) return *this;

			amount = Size + amount;
		}
		else
		{

		}

		return Allocate(amount);
	}

	Vector& Resize(int amount)
	{

		int allocated = (Data==Mem)?Vector::Default:Allocated;
		int remaining = allocated-Size;

		if (amount > remaining)
			Allocate(amount);

		if (amount > Size)
			Size = amount;

		return *this;
	}

	Vector & operator = (const Vector & vector)
	{
		return Copy(vector);

	}		

	Vector& operator = (Vector && vector)
	{
		return Move((Vector&&)vector);
	}

	Vector& operator << (typename Template<_Kind_>::ConstReference kind)
	{
		Append(kind);
		return *this;
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert((_Kind_&&)_Kind_(kind));
	}

	Result Insert(_Kind_ && kind)
	{
		if (IsNull() || Size >= Allocated)
			Allocate();

		int index = Size++;
		Data[index] = (_Kind_&&)kind;
		return Result(true,index);
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind, int index)
	{
		return Insert((_Kind_&&)_Kind_(kind),index);
	}

	Result Insert(_Kind_ && kind, int index)
	{
		if (!Indices::Normalise(index,Size,false))
			return false;

		if (IsNull() || Size >= Allocated)		
			Allocate();

		if (index < Size)
			Array<_Kind_>::Move(Data+index+1,Data+index,Size-index);		

		Data[index] = (_Kind_&&)kind;
		++Size;

		return Result(true,index);		
	}

	Result Append(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,Size);
	}

	Result Append(_Kind_ && kind, int index)
	{
		return Insert((_Kind_&&)_Kind_(kind),Size);
	}

	Result Prepend(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,0);
	}

	Result Prepend(_Kind_ && kind, int index)
	{
		return Insert((_Kind_&&)_Kind_(kind),0);
	}

	int Index(typename Template<_Kind_>::Reference kind, int seek=1)
	{
		return Index([&](_Kind_ & rhs) {return kind-rhs;},seek);
	}

    int Index(typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Index([&](_Kind_ & rhs) {return kind-rhs;},seek);
	}

	template <typename _Compare_>
	int Index(_Compare_ && compare, int seek=1)
	{
		return Index((_Compare_&&)compare, true, seek);
	}

	template <typename _Compare_>
	int Index(_Compare_ && compare, bool functor, int seek=1)
	{

		int first	= 0;
		int last	= this->Size-1;
		int middle	= first + (int)((last-first+1)>>1);
		int result	= 0;

		while (first <= last)
		{
			result = compare(Data[middle]);
			if (result == 0 && seek == 0) break;

			if (seek == 0)
			{
				if (result < 0)
					last = middle-1;
				else
					first = middle+1;
			}
			else
			if (seek > 0)
			{

				if (result >= 0)
					first = middle+1;
				else
					last = middle-1;
			}
			else
			if (seek < 0)
			{

				if (result <= 0)
					last = middle-1;
				else
					first = middle+1;

			}

			middle = first + (int)((last-first+1)>>1);				
		}	

		return middle;		
	}

	template<typename _Equals_>
	int Find(_Equals_ && equals, int seek=1,
		typename Traits::Enable<!Traits::PointerOrPrimitive<typename Template<_Equals_>::Value>::Value>::Type ** = 0)
	{

		return Find((seek>=0)?0:-1,equals,seek);		
	}	

	int Find(typename Template<_Kind_>::Reference kind, int seek=1)
	{

		return Find((seek<0)?-1:0,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}

	int Find(typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Find((seek<0)?-1:0,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}

	int Find(int from, typename Template<_Kind_>::Reference kind, int seek=1)
	{
		return Find(from,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}   

	int Find(int from, typename Template<_Kind_>::ConstReference kind, int seek=1)
	{
		return Find(from,[&](_Kind_ & rhs) {return kind == rhs;},seek);
	}    

	template<typename _Equals_>
	int Find(int from, _Equals_ && equals, int seek=1,
		typename Traits::Enable<!Traits::PointerOrPrimitive<typename Template<_Equals_>::Value>::Value>::Type ** = 0)
	{
		if (IsEmpty() || !Indices::Normalise(from,Size))
			return -1;

		if (seek == 0)
		{
			if (equals(Data[from]))
				return from;
		}
		else
		{
			int index=from-seek;
			while((index+=seek) < Size && index >= 0)
			{

				if (equals(Data[index]))
					return index;
			}
		}

		return -1;
	}

	Result Remove(typename Template<_Kind_>::ConstReference kind)
	{
		int index=Size;
		while(index-- > 0)
		{
			if (Data[index] == kind)
			{
				Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
				return Result(true,index);
			}
		}

		return false;
	}

	Result Remove(typename Template<_Kind_>::Reference kind, int index)
	{
		if (IsEmpty() || !Indices::Normalise(index,Size))
			return false;

		kind = Data[index];
		Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
		return Result(true,index);					
	}

	Result RemoveAt(int index)
	{

		if (IsEmpty() || !Indices::Normalise(index,Size))
			return false;

		Array<_Kind_>::Move(Data+index,Data+index+1,--Size-index);
		return Result(true,index);		
	}

	Result Select(typename Template<_Kind_>::ConstReference kind)
	{
		int index=Size;
		while(index-- > 0)
			if (Data[index] == kind)
				return Result(true,index);

		return false;				
	}

	Result Select(typename Template<_Kind_>::Reference kind, int index)
	{
		if (!Indices::Normalise(index,Size))
			return false;

		kind = Data[index];
		return Result(true,index);
	}

	void Unique(const Callback<int,_Kind_,_Kind_> & compare)
	{
		Sort();
		for (int i=0;i<Size-1;++i)
		{	
			if (compare(Data[i],Data[i+1]))
				Array<_Kind_>::Move(Data[i],Data[i+1],--Size-i);
		}	
	}	

	void Swap(int i,int j)
	{

		if (i != j)
		{
			_Kind_ data = Data[i];
			Data[i] = Data[j];
			Data[j] = data;
		}
	}

	void Shuffle(int seed=1)
	{
		if (Size > 2)
		{
			srand((unsigned int)seed);
			for (int i=Size-1;i>0;--i)
				Swap(i,rand()%i);
		}
	}

	void Sort(const Callback<int,_Kind_,_Kind_> & compare)
	{
		Sort(0,Size-1,compare);
	}

	void Sort(int first, int last, const Callback<int,_Kind_,_Kind_> & compare)
	{
		if (!Indices::Normalise(first,Size) || !Indices::Normalise(last,Size))
			return;

		if (last < first) return;

		Vector<Range> stack;
		stack.Allocate(last-first);
		stack.Insert(Range(first,last));

		while (stack.Size > 0)
		{

			Range range = stack.At(stack.Size-1);
			stack.Remove(range,stack.Size-1);

			first = range.First;
			last = range.Last;

			if (first < last)
			{

				int left = first+1;
				int right = last;

				while(right >= left)
				{
					while (left < last && compare(Data[left],Data[first]) < 0)
						++left;

					while (right > first && compare(Data[right],Data[first]) > 0)
						--right;

					if (left >= right) break;

					Swap(left,right);
				}

				if (right == first && left == first+1)
					continue;

				Swap(first,right);

				stack.Append(Range(first,right-1));
				stack.Append(Range(left+1,last));
			}

		}
	}					

};	

template<typename _Kind_, int _Allocated_, int _Factor_>
const float Vector<_Kind_,_Allocated_,_Factor_>::Factor = (_Factor_>100)?(float)_Factor_/100:1.5;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class List : public Iterable<_Kind_>
{
public:

	using Result = Result<bool,int>;

	class Entry
	{
	public:

		char Mem[sizeof(_Kind_)];
		_Kind_ * Kind;

		Entry * Prev;
		Entry * Next;

		Entry(typename Template<_Kind_>::ConstReference kind):Kind(0),Prev(0),Next(0)
		{

			Kind = new ((void*)Mem) _Kind_ (typename Template<_Kind_>::Reference(kind));

		}

		Entry():Kind(0),Prev(0),Next(0)
		{

		}

		~Entry()
		{
			if (Kind == (_Kind_*)&Mem)
			{
				Kind->_Kind_::~_Kind_();
				Kind = 0;
			}	
		}
	};

	Entry * First;
	Entry * Last;

	int		Count;

	List():First(0),Last(0),Count(0)
	{
	}

	~List()
	{
		Entry * entry = First;
		while (entry)
		{
			Entry * next = entry->Next;
			delete entry;
			entry = next;		
		}

		First = Last = 0;
		Count = 0;
	}

	List & operator = (const List & list)
	{
		if (this != &list)
		{
			Entry * entry = First;
			while (entry)
			{
				Entry * next = entry->Next;
				delete entry;
				entry = next;		
			}		

			if (list.Count > 0)
			{
				entry = list.First;
				while (entry)
				{
					Insert(*entry->Kind);
					entry = entry->Next;
				}						
			}

			Count = list.Count;
		}

		return *this;
	}		

	int Length() {return Count;}

	_Kind_ & At(int index)
	{		
		if (Indices::Normalise(index,Count))
		{

			Assert(index < Count && index >= 0);

			Entry * entry = Last;
			int offset = Count;
			while (--offset > index)
			{
				entry = entry->Prev;
				if (!entry)
					return Null<_Kind_>::Value();
			}

			if (entry->Kind)
				return *entry->Kind;
		}

		return Null<_Kind_>::Value();
	}

	bool IsEmpty() {return Count==0;}

	_Kind_ & operator [] (int index)
	{
		return At(index);
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind)
	{
		Entry * entry = new Entry(kind);
		return Insert(entry);
	}

	Result Insert(Entry *& entry)
	{
		if (Last == 0)
		{
			Last = entry;
			First = entry;
		}
		else
		{
			Last->Next = entry;
			entry->Prev = Last;
			Last = entry;

		}

		return Result(true,Count++);		
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind, int index)
	{
		if (index != Length() && !Indices::Normalise(index,Count,false))
			return false;

		Entry * entry = new Entry(kind);
		Result res = Insert(entry,index);
		if (!res)
		{
			delete entry;
			return false;
		}

		return res;	
	}

	Result Insert(Entry *& entry, int index)
	{
		Entry * prev = Last;
		int offset = Count;
		while (--offset > index)
		{
			prev = prev->Prev;
			if (!prev)
				return false;
		}

		if (prev)
		{	
			Entry * next = prev->Next;		
			if (next)
			{				
				next->Prev = entry;
				entry->Next = next;			
			}

			prev->Next = entry;
			entry->Prev = entry;
		}
		else
		{
			First = Last = entry;
		}

		++Count;
		return Result(true,index);		
	}

	Result Append(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,Count);
	}

	Result Append(Entry *& entry)
	{
		return Insert(entry,Count);
	}

	Result Prepend(typename Template<_Kind_>::ConstReference kind)
	{
		return Insert(kind,0);
	}

	Result Prepend(Entry *& entry)
	{
		return Insert(entry,0);
	}

	int Index(const Callback<int,_Kind_> & compare)
	{

		int first	= 0;
		int last	= this->Count-1;
		int middle	= first + (int)((last-first+1)>>1);
		int result	= 0;

		int offset = Count-1;
		Entry * list = Last;

		while (first <= last)
		{

			if (offset > middle)
			{
				while (offset-- > middle)
				{
					list = list->Prev;
					if (!list) return -1;
				}		
			}
			else
			if (offset < middle)
			{
				while (offset++ < middle)
				{
					list = list->Next;
					if (!list) return -1;
				}		
			}

			offset = middle;

			result = compare(list->Kind);
			if (result == 0) break;

			if (result < 0)
				last = middle-1;
			else
				first = middle+1;

			middle = first + (int)((last-first+1)>>1);				
		}	

		return middle;		
	}

	int Index(typename Template<_Kind_>::ConstReference kind)
	{	
		Entry * list = Last;
		int offset = Count;
		while (offset-- > 0)
		{
			if (list->Kind == kind)
				return offset;
			list = list->Prev;
			if (!list) return -1;
		}		

		return -1;
	}

	int Index(Entry *& entry)
	{
		Entry * list = Last;
		int offset = Count;
		while (offset-- > 0)
		{
			if (list == entry)
				return offset;
			list = list->Prev;
			if (!list) return -1;
		}		

		return -1;		
	}

	int Left(Entry *& entry, int index)
	{

	}

	int Right(Entry *& entry, int index)
	{

	}

	Result Remove(typename Template<_Kind_>::ConstReference kind)
	{
		Entry * list = Last;
		int offset = Count;
		while(offset-- > 0)
		{
			if (list->Kind == kind)
			{				
				Remove(list);
				delete list;
				return Result(true,offset);
			}

			list = list->Prev;
		}

		return false;
	}

	Result Remove(Entry *& entry)
	{
		if (entry)
		{
			if (entry->Prev != 0)
				entry->Prev->Next = entry->Next;
			else
				First = entry->Next;

			if (entry->Next != 0)
				entry->Next->Prev = entry->Prev;
			else
				Last = entry->Prev;

			--Count;			
			return true;		
		}

		return false;		
	}

	Result Remove(typename Template<_Kind_>::ConstReference kind, int index)
	{
		if (!Indices::Normalise(index,Count))
			return false;

		Entry * list = Last;
		int offset = Count;
		while (--offset > index)
		{
			list = list->Prev;
			if (!list)
				return false;
		}

		if (list->Kind == kind)
		{
			Remove(list);
			delete list;				
			return Result(true,offset);
		}

		return false;			
	}

	Result Remove(Entry *& entry, int index)
	{
		if (!Indices::Normalise(index,Count))
			return false;

		Entry * list = Last;
		int offset = Count;
		while (--offset > index)
		{
			list = list->Prev;
			if (!list)
				return false;
		}

		if (list == entry)
		{
			Remove(list);	
			return Result(true,index);
		}

		return false;
	}

	Result Select(typename Template<_Kind_>::ConstReference kind)
	{
		Entry * list = Last;
		int offset = Count;
		while(--offset > 0)
		{
			if (list->Kind == kind)
				return Result(true,offset);

			list = list->Prev;
		}

		return false;				
	}

	Result Select(typename Template<_Kind_>::Reference kind, int index)
	{
		if (!Indices::Normalise(index,Count))
			return false;

		Entry * list = Last;
		int offset = Count;
		while (--offset > index)
		{
			list = list->Prev;
			if (!list)
				return false;

		}

		kind = list->Kind;
		return Result(true,index);
	}

	void Unique(const Callback<int,_Kind_,_Kind_> & compare)
	{

	}

	void Swap(int i,int j)
	{

		if (i != j)
		{

			Entry * ii = Last;
			Entry * jj = Last;

			int offset = Count;
			while (--offset > 0 && (offset > i || offset > j))
			{
				if (offset > i)
					ii = ii->Prev;

				if (offset > j)
					jj = jj->Prev;
			}

			if (ii && jj)
			{
				Swap(ii,jj);				
			}

		}
	}

	void Swap(Entry * left, Entry * right)
	{
		if (left && right)
		{
			if (left->Prev) left->Prev->Next = right;
			if (left->Next) left->Next->Prev = right;
			if (right->Prev) right->Prev->Next = left;
			if (right->Next) right->Next->Prev = left;

			Entry * prev = left->Prev;
			Entry * next = left->Next;
			left->Prev = right->Prev;
			left->Next = right->Next;
			right->Prev = prev;
			right->Next = next;
		}		
	}

	void Shuffle(int seed=1)
	{
		if (Count > 2)
		{
			srand(seed);
			for (int i=Count-1;i>0;--i)
				Swap(i,rand()%i);
		}
	}

	void Sort(const Callback<int,_Kind_,_Kind_> & compare)
	{
		Sort(0,Count-1,compare);
	}

	void Sort(int first, int last, const Callback<int,_Kind_,_Kind_> & compare)
	{
		if (!Indices::Normalise(first,Count) || !Indices::Normalise(last,Count))
			return;

		if (last < first) return;

	}			

	void Release()
	{
		Entry * entry = First;
		while (entry)
		{
			Entry * next = entry->Next;
			Disposer<_Kind_>::Release(Reference<_Kind_>::Template(entry->Kind));
			delete entry;
			entry = next;		
		}

		First = Last = 0;
		Count = 0;
	}

	void Destroy()
	{
		Entry * entry = First;
		while (entry)
		{
			Entry * next = entry->Next;
			Disposer<_Kind_>::Destroy(Reference<_Kind_>::Template(entry->Kind));
			delete entry;
			entry = next;		
		}

		First = Last = 0;
		Count = 0;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ArrayIndex
{
public:

	Vector<int> Keys;

	struct KeyCompare
	{
		int Key;
		Callback<int,int,int> &Compare;

		KeyCompare(int key, Callback<int,int,int> &compare):
			Key(key),Compare(compare)
		{
		}

		int operator() (int index)
		{
			#ifdef HERO_USING_TRACY
			ZoneScopedS(16);
			#endif

			return Compare(Key,index);
		}
	};

	Callback<int,int,int> Compare;

	int Size;

	int Memo[2];
	bool Memoised;

	ArrayIndex():Size(0) {}

	ArrayIndex(const ArrayIndex & array):Size(0),Memo{},Memoised(false)
	{
		Copy(array);
	}

	ArrayIndex(ArrayIndex& array):Size(0),Memo{},Memoised(false)
	{
		Move(array);
	}

	ArrayIndex(Callback<int,int,int> & compare):Size(0),Memo{},Memoised(false),Compare(compare) 
	{
	}

	int Allocated()
	{

		return Keys.Size;
	}

	ArrayIndex & Reserve(int amount)
	{
		Keys.Reserve(amount);
		return *this;
	}

	void Release()
	{
		Keys.Release();
		Size = 0;
		Memoised = false;
	}

	void Destroy()
	{
		Keys.Destroy();
		Size = 0;
		Memoised = false;
	}	

	ArrayIndex & operator = (const ArrayIndex & array)
	{
		return Copy(array);
	}

	ArrayIndex & Clone(const ArrayIndex & array) 
	{
		return Copy(array);
	}

	ArrayIndex & Copy(const ArrayIndex & array)
	{
		if(&array == this)
			return *this;

		Compare = array.Compare;

		Keys = array.Keys;
		Size = array.Size;

		return *this;
	}

	ArrayIndex & Move(ArrayIndex & array)
	{
		Keys.Move(array.Keys);

		Compare = array.Compare;

		Size = array.Size;
		array.Size = 0;

		return *this;
	}

	ArrayIndex & Swap(const ArrayIndex & array)
	{
		if(&array == this)
			return *this;

		Keys.Swap(array.Keys);

		Callback<int,int,int> compare = array.Compare;
		int size = array.Size;

		((ArrayIndex&)array).Compare = Compare;
		((ArrayIndex&)array).Size = Size;

		Compare = compare;
		Size = size;

		return *this;
	}

	int operator[] (int index)
	{
		return Keys[index];
	}

	int Index(int key, int seek=1)
	{

		if (Memoised && Memo[0] == key) return Memo[1];		

		KeyCompare comparitor(key,Compare);

		int slot = Keys.Index(comparitor,seek);

		Memo[0] = key;
		Memo[1] = slot;
		Memoised = true;
		return slot;		
	}

	int Insert(int key, int seek=1)
	{
		int slot = Index(key,seek);

		Keys.Insert(key,slot);
		++Size;

		Memoised = false;
		return key;
	}

	int Remove(int key, int seek=1)
	{
		int slot = Index(key,seek);

		if (seek > 0) --slot;

		if (Indices::IsAbsolute(slot,Size))
		{
			int index = Keys.Data[slot];
			if (Compare(key,index) == 0)
			{
				Keys.Remove(key,slot);
				--Size;

				Memoised = false;
				return key;
			}
		}

		return 0xFFFFFFFF;

	}

	int Select(int key, int seek=1)
	{
		int slot  = Index(key,seek);
		if (seek > 0) --slot;

		if (Indices::IsAbsolute(slot,Size))
		{
			int index = Keys.Data[slot];
			if (Compare(key,index) == 0)
				return index;
		}

		return 0xFFFFFFFF;		
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ArrayIndexPacked
{
public:

	struct KeyCompare
	{
		int Key;
		Callback<int,int,int> &Compare;

		KeyCompare(int key, Callback<int,int,int> &compare):
			Key(key),Compare(compare)
		{
		}

		int operator() (int index)
		{
			return Compare(Key,index);
		}
	};

	Callback<int,int,int> Compare;

	int * Data;
	int Size;

	int Power;

	int Pack;

	int Memo[2];
	bool Memoised;

	ArrayIndexPacked():
		Data(0),Size(0),Power(0),Pack(0),Memo{},Memoised(false) 
	{
	}

	ArrayIndexPacked(const ArrayIndexPacked & array):
		Data(0),Size(0),Power(0),Pack(0),Memo{},Memoised(false)
	{
		Copy(array);
	}

	ArrayIndexPacked(ArrayIndexPacked& array):
		Data(0),Size(0),Power(0),Pack(0),Memo{},Memoised(false)
	{
		Move(array);
	}

	ArrayIndexPacked(Callback<int,int,int> & compare):
		Data(0),Size(0),Power(0),Pack(0),Memo{},Memoised(false),Compare(compare) 
	{
	}

	int Allocated()
	{
		return Power/(sizeof(int)/Pack);
	}

	int Required(int amount=0)
	{

		if (amount <= 0) amount = Size;
		return (amount > 0)?((amount-1)*Pack)/sizeof(int)+1:0;

	}

	ArrayIndexPacked& Allocate(int amount=0)
	{
		if (Pack == 0) 
			return Repack(amount);

		if (Power > 0)
		{
			Assert(Power >= Size);

			if (amount == 0)
				amount = (int)(Power*Array<int>::Factor)+1;
			else
			if (amount < 0 || amount < Size)
				amount = Size;

			amount = Required(amount);

			Array<int>::Reallocate(Data,Required(Size),amount);
		}
		else
		{
			if (amount == 0)
				amount = (int)(((Size>0)?Size:1)*Array<int>::Factor)+1;			
			else
			if (amount < 0 || amount < Size)
				amount = Size;

			amount = Required(amount);

			int * data;
			Array<int>::Allocate(data,amount);

			if (Size > 0 && Data != 0)
				Array<int>::Copy(data,Data,Required(Size));		

			Data = data;
		}

		Power = amount*(sizeof(int)/Pack);

		return *this;

	}

	ArrayIndexPacked& Reserve(int amount)
	{
		if (Pack == 0) 
			return Repack(amount);

		int remaining = Power-Size;

		if (amount < 0)
		{

			amount = Size+(remaining-amount);
			if (amount < Size) amount = Size;
		}
		else
		if (amount > 0)
		{
			if (amount <= remaining) return *this;

			amount = Size + amount;
		}
		else
		{

		}

		return Allocate(amount);
	}

	ArrayIndexPacked& Repack(int amount)
	{

		if (amount == 0)
			amount = (int)(((Size>0)?Size:1)*Array<int>::Factor)+1;			
		else
		if (amount < 0 || amount < Size)
			amount = Size;

		ArrayIndexPacked array(Compare);
		array.Pack = 0;

		if (amount <= 0xFF)
			array.Pack = 1;
		else
		if (amount <= 0xFFFF)
			array.Pack = 2;
		else
			array.Pack = 4;

		if (array.Pack != Pack)
		{

			amount = (int)(amount*Array<int>::Factor)+1;
			array.Reserve(amount);			

			for (int i=0;i<Size;++i)
			{
				int key = Get(i);	

				switch (array.Pack)
				{
				case 1: ((char*)array.Data)[i] = (char)key; break;
				case 2: ((char*)array.Data)[i] = (char)key; break;
				case 4: array.Data[i] = (int)key;	
				}
			}			

			array.Size = Size;

			Move(array);
		}
		else
		if (amount > Power)
		{

			amount = (int)(amount*Array<int>::Factor)+1;
			Allocate(amount);
		}

		return *this;
	}

	int Get(int slot)
	{

		if (Pack == 1)
		{
			char index = ((char*)Data)[slot];
			return (index==(char)0xFF)?0xFFFFFFFF:(int)(unsigned char)index;
		}

		if (Pack == 2)
		{
			short index = ((short*)Data)[slot];
			return (index==(short)0xFFFF)?0xFFFFFFFF:(int)(unsigned short)index;
		}

		if (Pack == 4)
		{
			return Data[slot];
		}								

		return 0xFFFFFFFF;
	}

	int Set(int slot, int index)
	{
		Repack(slot+1);

		if (Pack == 1)
		{
			((char*)Data)[slot] = (char)index;
			return index;	
		}

		if (Pack == 2)
		{
			((short*)Data)[slot] = (short)index;
			return index;	
		}

		if (Pack == 4)
		{
			Data[slot] = (int)index;		
			return index;								
		}

		return 0xFFFFFFFF;		
	}	

	int Del(int slot)
	{
		int index=0;
		if (Pack == 1)
		{
			index = ((char*)Data)[slot];
			memmove((char*)Data+slot,(char*)Data+slot+1,sizeof(char)*(Size-slot));
			return index;
		}

		if (Pack == 2)
		{
			index = ((short*)Data)[slot];
			memmove((short*)Data+slot,(short*)Data+slot+1,sizeof(short)*(Size-slot));
			return index;	
		}

		if (Pack == 4)
		{
			index = ((int*)Data)[slot];
			memmove((int*)Data+slot,(int*)Data+slot+1,sizeof(int)*(Size-slot));
			return index;							
		}

		return 0xFFFFFFFF;
	}

	void Release()
	{
		Data = 0;
		Size = 0;
		Pack = 0;
	}

	void Destroy()
	{

		if (Data)
			Array<int>::Delete(Data);

		Size = 0;
		Pack = 0;
	}	

	ArrayIndexPacked & operator = (const ArrayIndexPacked & array)
	{
		return Copy(array);
	}

	ArrayIndexPacked & Clone(const ArrayIndexPacked & array) 
	{
		return Copy(array);
	}

	ArrayIndexPacked & Copy(const ArrayIndexPacked & array)
	{
		if(&array == this)
			return *this;

		Compare = array.Compare;

		if (Data)
			Array<int>::Delete(Data);

		int * data;
		Array<int>::Allocate(data,array.Power);

		Array<int>::Copy(data,array.Data,array.Power);
		Data = data;

		Size = array.Size;
		Power = array.Power;

		Pack = array.Pack;

		return *this;
	}

	ArrayIndexPacked & Move(ArrayIndexPacked & array)
	{		
		if(&array == this) 
			return *this;		

		Compare = array.Compare;

		if (Data)
			Array<int>::Delete(Data);

		Data = array.Data;
		Size = array.Size;
		Power = array.Power;
		Pack = array.Pack;

		array.Data = 0;
		array.Size = 0;
		array.Pack = 0;

		return *this;
	}

	ArrayIndexPacked & Swap(const ArrayIndexPacked & array)
	{
		if(&array == this) 
			return *this;

		Callback<int,int,int> compare = array.Compare;

		int * data = array.Data;
		int size = array.Size;
		int power = array.Power;
		int pack = array.Pack;

		((ArrayIndexPacked&)array).Compare = Compare;
		((ArrayIndexPacked&)array).Data = Data;
		((ArrayIndexPacked&)array).Size = Size;
		((ArrayIndexPacked&)array).Power = Power;
		((ArrayIndexPacked&)array).Pack = Pack;

		Compare = compare;

		Data = data;
		Size = size;
		Power = power;
		Pack = pack;

		return *this;
	}

	int operator[] (int index)
	{
		return Get(index);
	}

	int Index(int key, int seek=1)
	{
		if (Memoised && Memo[0] == key) return Memo[1];

		KeyCompare compare(key,Compare);

		int first	= 0;
		int last	= Size-1;
		int middle	= first + (int)((last-first+1)>>1);
		int result	= 0;

		while (first <= last)
		{

			result = compare(Get(middle));
			if (result == 0 && seek == 0) break;

			if (seek == 0)
			{

				if (result < 0)
					last = middle-1;
				else
					first = middle+1;
			}
			else
			if (seek > 0)
			{

				if (result >= 0)
					first = middle+1;
				else
					last = middle-1;
			}
			else
			if (seek < 0)
			{

				if (result <= 0)
					last = middle-1;
				else
					first = middle+1;
			}

			middle = first + (int)((last-first+1)>>1);		

			if (middle == Size && first <= last)
				--middle;
		}	

		Memo[0] = key;
		Memo[1] = middle;
		Memoised = true;

		return middle;		
	}	

	int Insert(int key, int seek=1)
	{
		int slot = Index(key,seek);

		Set(slot,key);

		++Size;
		return key;
	}

	int Remove(int key, int seek=1)
	{
		int slot = Index(key,seek);

		if (seek > 0) --slot;

		if (Indices::IsAbsolute(slot,Size))
		{
			int index = Get(slot);
			if (Compare(key,index) == 0)
			{				
				Del(slot);
				Memoised = false;

				--Size;
				return key;
			}
		}

		return 0xFFFFFFFF;

	}

	int Select(int key, int seek=1)
	{
		int slot  = Index(key,seek);
		if (seek > 0) --slot;

		if (Indices::IsAbsolute(slot,Size))
		{
			int index = Get(slot);
			if (Compare(key,index) == 0)
				return index;
		}

		return 0xFFFFFFFF;		
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class ArraySet : public Iterable<_Kind_>, public Comparable
{
public:

	using Result = Result<bool,int>;

	struct ArrayMapped
	{

		Variable<_Kind_*> Value;

		ArrayMapped() {}
		ArrayMapped(const ArrayMapped & mapped):Value(mapped.Value) {}				

		ArrayMapped & operator = (const ArrayMapped & mapped)
		{
			Value = mapped.Value;
			return *this;
		}
	};

	struct ArrayIndexer
	{		
		ArraySet & Set;

		ArrayMapped Mapped;

		ArrayMapped & Alias()
		{

			return Mapped;
		}	

		ArrayIndexer(ArraySet & set): Set(set) {}

		ArrayIndexer & operator = (const ArrayIndexer & indexer)
		{

			return *this;
		}

		bool Equals(int lhs, int rhs)
		{
			return Compare(lhs,rhs) == 0;
		}

		int Compare(int lhs, int rhs)
		{
			if (lhs == -1)
			{
				ArrayMapped & mapped = Alias();							
				return Hero::Comparer<_Kind_>::Compare(*mapped.Value(),Set.Values[rhs],Set.Cmp);
			}					

			return Hero::Comparer<_Kind_>::Compare(Set.Values[lhs],Set.Values[rhs],Set.Cmp);
		}

	};

	Array<_Kind_> Values;	

	Comparitor Cmp;
	ArrayIndex Idx;

	ArrayIndexer Idxer;

	ArraySet(int comparitor=Comparable::COMPARE_GENERAL):
		Cmp(comparitor),Idxer(*this)
	{

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
	}

	ArraySet(const ArraySet & set):
		Cmp(Comparable::COMPARE_GENERAL),Idxer(*this)
	{
		Copy(set);
	}

	ArraySet & operator = (const ArraySet & set)
	{
		return Copy(set);
	}

	ArraySet & Clone(const ArraySet & set)
	{
		return Copy(set);
	}

	ArraySet & Copy(const ArraySet & set)
	{
		if(&set == this) return *this;

		Values.Copy(set.Values);
		Idx.Copy(set.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);

		return *this;
	}

	ArraySet & Move(ArraySet & set)
	{
		if(&set == this) return *this;

		Values.Move(set.Values);
		Idx.Move(set.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
		set.Idx.Compare = 0;

		return *this;
	}

	ArraySet & Swap(const ArraySet & set)
	{
		if(&set == this) return *this;

		Values.Swap(set.Values);
		Idx.Swap(set.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
		set.Idx.Compare = Callback<int,int,int>(&set.Idxer,&ArrayIndexer::Compare);

		return *this;
	}

	_Kind_ & operator [] (const _Kind_ & value)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)value);

		int index = Idx.Select(-1,0);
		if (index != -1)
			return Values[index];

		index = Values.Size;
		Values.Append(value);

		Idx.Insert(index);
		return Values[index];

	}

	void Release()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Release(At(i));
			}

			Values.Release();
			Idx.Release();
		}
	}

	void Destroy()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Destroy(At(i));
			}

			Values.Destroy();
			Idx.Destroy();
		}
	}	

	int Length() {return Values.Size;}

	_Kind_ & At(int index) 
	{
		Indices::Normalise(index,Values.Size);

		if (index < Values.Size)
			return Values[index];

		Assert(false);		

		return Null<_Kind_>::Value();
	}	

	int Index(typename Template<_Kind_>::ConstReference kind)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1,0);

		if (index == 0xFFFFFFFF)
			return -1; 

		return index;
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind)
	{		
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1,0);
		if (index != -1)
			return Result(false,index);

		index = Values.Size;
		Values.Append(kind);

		Idx.Insert(index,0);
		return Result(true,index);
	}

	Result Remove(typename Template<_Kind_>::ConstReference kind)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Remove(-1,0);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Values.RemoveAt(offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result RemoveAt(int index)
	{
		if (!Indices::Normalise(index,Values.Size))
			return Result(false,index);

		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &Values[index];

		index = Idx.Remove(-1,0);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Values.RemoveAt(offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result Select(typename Template<_Kind_>::ConstReference kind)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1,0);
		if (index == -1)
			return Result(false,index);		

		return Result(true,index);
	}

	Result Update(typename Template<_Kind_>::ConstReference kind)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1,0);
		if (index == -1)
		{
			return Insert(kind);
		}
		else
		{
			At(index) = kind;
			return Result(true,index);
		}		
	}	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Key_, typename _Value_>
class ArrayMap : public Iterable<_Value_>, public Comparable
{
public:

	using Result = Result<bool,int>;

	struct ArrayMapped
	{

		Variable<_Key_*> Key;
		Optional<_Value_*> Value;

		ArrayMapped() {}
		ArrayMapped(const ArrayMapped & mapped):Key(mapped.Key),Value(mapped.Value) {}				

		ArrayMapped & operator = (const ArrayMapped & mapped)
		{
			Key = mapped.Key;
			Value = mapped.Value;
			return *this;
		}
	};

	struct ArrayIndexer
	{
		ArrayMap & Map;

		ArrayMapped Mapped;

		ArrayMapped & Alias()
		{

			return Mapped;
		}

		ArrayIndexer(ArrayMap & map):Map(map) {}

		bool Equals(int lhs, int rhs)
		{
			return Compare(lhs,rhs) == 0;
		}

		int Compare(int lhs, int rhs)
		{
			if (lhs == -1)
			{
				ArrayMapped & mapped = Alias();							
				return Hero::Comparer<_Key_>::Compare(*mapped.Key(),Map.Keys[rhs],Map.Cmp);
			}					

			return Hero::Comparer<_Key_>::Compare(Map.Keys[lhs],Map.Keys[rhs],Map.Cmp);
		}

	};

	Array<_Key_> Keys;
	Array<_Value_> Values;

	Comparitor Cmp;
	ArrayIndex Idx;

	ArrayIndexer Idxer;

	ArrayMap(int comparitor=Comparable::COMPARE_GENERAL):
		Cmp(comparitor),Idxer(*this)
	{

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
	}

	ArrayMap(const ArrayMap& map):
		Idxer(Values)
	{
		Copy(map);
	}

	ArrayMap& operator = (const ArrayMap& map)
	{
		Copy(map);
	}

	ArrayMap& Clone(const ArrayMap& map)
	{
		return Copy(map);
	}

	ArrayMap& Copy(const ArrayMap& map)
	{
		if(&map == this) return *this;

		Keys.Copy(map.Keys);
		Values.Copy(map.Values);
		Idx.Copy(map.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);

		return *this;
	}

	ArrayMap& Move(ArrayMap& map)
	{
		if(&map == this) return *this;

		Keys.Move(map.Keys);
		Values.Move(map.Values);
		Idx.Move(map.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
		map.Idx.Compare = 0;

		return *this;
	}

	ArrayMap& Swap(const ArrayMap& map)
	{
		if(&map == this) return *this;

		Keys.Swap(map.Keys);
		Values.Swap(map.Values);
		Idx.Swap(map.Idx);

		Idx.Compare = Callback<int,int,int>(&Idxer,&ArrayIndexer::Compare);
		map.Idx.Compare = Callback<int,int,int>(&map.Idxer,&ArrayIndexer::Compare);

		return *this;
	}

	void Release()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Key_>::Release(Keys.At(i));
				Disposer<_Value_>::Release(Values.At(i));
			}

			Keys.Release();
			Values.Release();
			Idx.Release();
		}
	}

	void Destroy()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{

				Disposer<_Key_>::Destroy(Keys.At(i));
				Disposer<_Value_>::Destroy(Values.At(i));
			}

			Keys.Destroy();
			Values.Destroy();
			Idx.Destroy();
		}
	}	

	int Length() {return Values.Size;}

	_Value_ & At(int index) 
	{
		Indices::Normalise(index,Values.Size);

		if (index < Values.Size)
			return Values[index];

		Assert(false);		

		return Null<_Value_>::Value();
	}	

	virtual _Value_ & Get(const _Key_ & key, int seek=0)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,seek);
		if (index != -1)
			return Values[index];

		index = Keys.Size;
		Values.Append(_Value_());
		Keys.Append(key);

		Idx.Insert(index);
		return Values[index];	
	}

	virtual _Value_ & Get(_Key_ && key, int seek=0)
	{
		#ifdef HERO_USING_TRACY
		ZoneScopedS(16);
		#endif

		Keys.Append((_Key_&&)key);

		int index = Idx.Select(Keys.Size-1,seek);
		if (index != -1)
		{

			Keys[Keys.Size-1] = _Key_();

			--Keys.Size;

			return Values[index];
		}

		Values.Append(_Value_());
		index = Keys.Size-1;

		Idx.Insert(index);
		return Values[index];			
	}

	_Value_ & operator [] (const _Key_ & key)
	{

		return Get(key);
	}

	_Value_ & operator [] (_Key_ && key)
	{
		return Get(key);
	}

	int Index(typename Template<_Key_>::ConstReference key)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,0);

		if (index == 0xFFFFFFFF)
			return -1; 

		return index;
	}

	Result Insert(typename Template<_Key_>::ConstReference key, typename Template<_Value_>::ConstReference value)
	{		
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,0);
		if (index != -1)
			return Result(false,index);

		index = Values.Size;
		Values.Append(value);
		Keys.Append(key);

		Idx.Insert(index,0);
		return Result(true,index);
	}

	Result Insert(typename Template<_Key_>::ConstReference key)
	{		
		return Insert(key,_Value_());
	}	

	Result Remove(typename Template<_Key_>::ConstReference key)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Remove(-1,0);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Keys.Swap(index,offset);

			Values.RemoveAt(offset);
			Keys.Swap(index,offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
			Keys.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result RemoveAt(int index)
	{
		if (!Indices::Normalise(index,Keys.Size))
			return Result(false,index);

		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &Keys[index];

		index = Idx.Remove(-1,0);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Keys.Swap(index,offset);

			Values.RemoveAt(offset);
			Keys.Swap(index,offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
			Keys.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result Select(typename Template<_Key_>::ConstReference key)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,0);
		if (index == -1)
			return Result(false,index);		

		return Result(true,index);
	}

	Result Update(typename Template<_Key_>::ConstReference key, typename Template<_Value_>::ConstReference value)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,0);
		if (index == -1)
		{
			return Insert(key,value);			
		}
		else
		{
			At(index) = value;
			return Result(true,index);
		}
	}

	Result Update(typename Template<_Key_>::ConstReference key)
	{
		ArrayMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1,0);
		if (index == -1)
		{
			return Insert(key,_Value_());			
		}

		return Result(true,index);
	}	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
using Set = ArraySet<_Kind_>;

template<typename _Key_,typename _Value_>
using Map = ArrayMap<_Key_,_Value_>;

template<typename _Key_,typename _Value_>
class Multimap : public Map<_Key_,_Value_>
{
public:

	Multimap(int comparitor=Comparable::COMPARE_GENERAL):Map<_Key_,_Value_>(comparitor) {}

	_Value_ & operator [] (const _Key_ & key)
	{
		int seek=1;
		return Map<_Key_,_Value_>::Get(key,seek);
	}

	_Value_ & operator [] (_Key_ && key)
	{
		int seek=1;
		return Map<_Key_,_Value_>::Get(key,seek);
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class HashIndex
{
public:

	static int Abs(int x)				{return (x >= 0) ? x : -x;}
	static int Max(int x, int y)		{return Abs(x) > Abs(y) ? x : y;}
	static int Min(int x, int y)		{return Abs(x) < Abs(y) ? x : y;}
	static int Compare(int x, int y)	{return Abs(x) - Abs(y);}

public:

	static const int Flow[];
	static const int Prime[];

	int * Data;
	int Size;

	int Power;

	int Pack;

	Callback<int,int> Hash;

	Callback<bool,int,int> Equals;

	struct Memoise
	{
		int Key;
		int Slot;
		int Index;
		bool Memoised;

		Memoise():Key(0),Slot(0),Index(0),Memoised(false) {}

		void Reset()
		{
			Key = Slot = Index = 0;
			Memoised = false;
		}
	};

	Memoise Memo;

	HashIndex():
		Data(0),Size(0),Power(0),Pack(0)
	{
	}

	HashIndex(const HashIndex & hash):
		Data(0),Size(0),Power(0),Pack(0)
	{
		Copy(hash);
	}

	HashIndex(HashIndex && hash):
		Data(0),Size(0),Power(0),Pack(0)
	{
		Move((HashIndex&&)hash);
	}

	HashIndex(Callback<int,int> & hash, Callback<bool,int,int> & equals):
		Data(0),Size(0),Power(0),Pack(0),Hash(hash),Equals(equals)
	{
	}

	~HashIndex()
	{
		if (Data)
			delete [] Data;
	}

	HashIndex& operator = (const HashIndex& hash)
	{
		Copy(hash);
		return *this;
	}

	HashIndex& Clone(const HashIndex& index)
	{
		return Copy(index);
	}

	HashIndex& Copy(const HashIndex& hash)
	{
		if(&hash == this)
			return *this;

		if(Data)
			delete[] Data;

		Hash = hash.Hash;
		Equals = hash.Equals;

		Power = hash.Power;
		Pack = hash.Pack;

		int amount = ((HashIndex&)hash).Required();

		Data = new int[amount];
		memcpy((char*)Data,(char*)hash.Data,amount*sizeof(int));

		Memo.Reset();
		return *this;
	}

	HashIndex& Move(HashIndex&& hash)
	{
		if(&hash == this)
			return *this;

		if(Data)
			delete[] Data;

		Hash = hash.Hash;
		Equals = hash.Equals;

		hash.Hash = 0;
		hash.Equals = 0;

		Data = hash.Data;
		Power = hash.Power;
		Pack = hash.Pack;

		hash.Data = 0;
		hash.Power = 0;
		hash.Pack = 0;

		Memo.Reset();
		return *this;
	}

	HashIndex& Swap(const HashIndex& hash)
	{
		if(&hash == this)
			return *this;

		Callback<int,int> hashcb = hash.Hash;
		Callback<bool,int,int> equalscb = hash.Equals;

		((HashIndex&)hash).Hash = Hash;
		((HashIndex&)hash).Equals = Equals;

		Hash = hashcb;
		Equals = equalscb;

		int * data  = hash.Data;
		int power = hash.Power;
		int pack = hash.Pack;

		((HashIndex&)hash).Data = Data;
		((HashIndex&)hash).Power = Power;
		((HashIndex&)hash).Pack = Pack;
		((HashIndex&)hash).Memo.Reset();

		Data = data;
		Power = power;
		Pack = pack;
		Memo.Reset();

		return *this;
	}

	void Release()
	{		
		if (Data)
		{
			delete [] Data;
			Data = 0;
		}

		Size = 0;
		Power = 0;
		Pack = 0;
		Memo.Reset();
	}

	void Destroy()
	{
		if (Data)
		{
			delete [] Data;
			Data = 0;
		}

		Size = 0;
		Power = 0;
		Pack = 0;
		Memo.Reset();
	}	

	int Allocated()
	{
		return Flow[Power];
	}

	int Factor()
	{

		return (Prime[Power]/3);
	}

	int Slots()
	{

		return Prime[Power];
	}

	int Rehash(int key)
	{

		key = ~key + (key << 15);
		key = key ^ (key >> 12);
		key = key + (key << 2);
		key = key ^ (key >> 4);
		key = key * 2057;
		key = key ^ (key >> 16);
		return key;
	}	

	int Required(int amount=0)
	{

		if (amount <= 0) amount = Flow[Power];
		return (amount > 0)?((amount-1)*Pack)/sizeof(int)+1:0;

	}

	int Get(int slot)
	{

		if (Pack == 1)
		{
			char index = ((char*)Data)[slot];
			return (index==(char)0xFF)?0xFFFFFFFF:(int)(unsigned char)index;
		}

		if (Pack == 2)
		{
			short index = ((short*)Data)[slot];
			return (index==(short)0xFFFF)?0xFFFFFFFF:(int)(unsigned short)index;
		}

		if (Pack == 4)
		{
			return Data[slot];
		}								

		return 0xFFFFFFFF;
	}

	int Set(int slot, int index)
	{

		if (Pack == 1)
		{
			((char*)Data)[slot] = (char)index;
			return index;	
		}

		if (Pack == 2)
		{
			((short*)Data)[slot] = (short)index;
			return index;	
		}

		if (Pack == 4)
		{
			Data[slot] = (int)index;		
			return index;								
		}

		return 0xFFFFFFFF;		
	}	

	int operator[] (int index)
	{

		for (int i=0;i<Allocated(),index>=0;++i)
		{		
			int reindex = Get(i);
			if (reindex != 0xFFFFFFFF)
			{
				if (index == 0)
					return reindex;
				--index;
			}
		}

		return 0xFFFFFFFF;
	}

	bool Index(int key, int & slot, int & index)
	{
		if (Memo.Memoised && Memo.Key == key)
		{
			slot = Memo.Slot; index = Memo.Index;
			return index != 0xFFFFFFFF;
		}

		Memo.Reset();
		Memo.Key = key;

		if (Power == 0)
		{
			index = 0xFFFFFFFF;

			int prev = 0;
			for (slot=0;slot<Allocated();++slot)
			{
				index = Get(slot);				
				if (index == 0xFFFFFFFF)
				{
					prev = slot;
					continue;
				}

				if (Equals(key,index))
				{
					Memo.Slot = slot; Memo.Index = index;
					Memo.Memoised = true;

					return true;
				}
			}

			slot = prev;
			index = 0xFFFFFFFF;

			Memo.Slot = slot; Memo.Index = index;
			Memo.Memoised = true;

			return false;
		}

		slot = Hash(key);

		slot = Abs(slot);

		slot %= Slots();

		int wrap=0;

		index = Get(slot);
		while(index != 0xFFFFFFFF)
		{
			if (Equals(key,index))
			{
				Memo.Slot = slot; Memo.Index = index;
				Memo.Memoised = true;

				return true;
			}

			++slot;
			if (slot >= Slots())
			{
				slot = 0;

				if (++wrap > 1)
				{

					AssertBreak();
				}
			}

			index = Get(slot);
		}

		Memo.Slot = slot; Memo.Index = index;
		Memo.Memoised = true;

		return false;	
	}

	int Insert(int key)
	{

		Reflow(1);

		int index=0;	
		int slot=0;
		if (!Index(key,slot,index))
		{					
			Set(slot,key);
			++Size;

			return key;
		}
		else
		{
			return index;
		}

		return false;		
	}

	int Remove(int key)
	{
		Memo.Reset();

		int index=0;
		int slot=0;
		Index(key,slot,index);

		if (index == 0xFFFFFFFF)
			return index;

		Set(slot,0xFFFFFFFF);
		--Size;

		if (Power == 0)
			return index;

		Vector<int,32> indexes;

		int reslot=slot;
		int reindex=0;

		while(true)
		{

			reslot = (reslot+1 == Slots())?0:reslot+1;		

			reindex = Get(reslot);
			if (reindex == 0xFFFFFFFF)
				break;

			Set(reslot,0xFFFFFFFF);
			indexes.Append(reindex);				
		}		

		for (int i=0;i<indexes.Size;++i)
		{
			int rekey = indexes[i];

			reslot = 0;
			reindex = 0;

			Index(rekey,reslot,reindex);

			Assert(reindex == 0xFFFFFFFF);

			Set(reslot,rekey);
		}

		Reflow(-1);
		return index;
	}

	int Select(int key)
	{
		int slot=0;
		int index=0;
		Index(key,slot,index);
		return index;		
	}

	void Reorder(int amount)
	{

	}

	bool Reflow(int amount)
	{		

		if (Power == 0 && Data != 0 && Size+amount <= Allocated())
			return false;

		bool overflow = Size+amount >= (Slots()/4)*3;

		bool underflow = Size+amount < (Slots()/3);	

		if (underflow || overflow)
		{		
			HashIndex hash(Hash,Equals);
			hash.Power = 0;
			while (Size+amount > hash.Factor())
			{
				++hash.Power;				

				if (hash.Factor() == 0)
					return false;
			}

			if (hash.Power == Power && Data != 0)
				return false;

			Memo.Reset(); 

			int allocated = hash.Allocated();			
			hash.Pack = 0;
			if (allocated < 0xFF)
				hash.Pack = 1;
			else
			if (allocated < 0xFFFF)
				hash.Pack = 2;
			else
				hash.Pack = 4;

			amount = hash.Required();

			hash.Data = new int[amount];
			memset((char*)hash.Data,0xFFFFFFFF,amount*sizeof(int));

			for (int i=0;i<Allocated();++i)
			{
				int key = Get(i);	
				if (key == 0xFFFFFFFF)
					continue;

				int slot=0;
				int index=0;
				if (!hash.Index(key,slot,index))
				{
					hash.Set(slot,key);
					++hash.Size;
				}
			}

			Move((HashIndex&&)hash);	
			return true;								
		}

		return false;

	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class HashSet
{
public:

	using Result = Result<bool,int>;

	struct HashMapped
	{

		Optional<_Kind_*> Value;

		HashMapped() {}
		HashMapped(const HashMapped & mapped):Value(mapped.Value) {}				

		HashMapped & operator = (const HashMapped & mapped)
		{
			Value = mapped.Value;
			return *this;
		}
	};

	struct HashIndexer
	{
		class Array<_Kind_> & Values;

		HashMapped Map;
		HashMapped & Mapped()
		{

			return Map;
		}

		HashIndexer(class Array<_Kind_> & values):
			Values(values)
		{
		}

		HashIndexer & operator = (const HashIndexer & indexer)
		{

			Values = indexer.Values;
			Map = indexer.Map;
			return *this;
		}

		int Hash(int index)
		{
			return Hero::Hasher<_Kind_>::Hash(Values[index]);
		}

		bool Equals(int lhs, int rhs)
		{
			return Compare(lhs,rhs) == 0;
		}

		int Compare(int lhs, int rhs)
		{
			if (lhs == -1)
			{
				HashMapped & mapped = Map.Get();			

				return Hero::Comparer<_Kind_>::Compare(*mapped.Value(),Values[rhs]);
			}					

			return Hero::Comparer<_Kind_>::Compare(Values[lhs],Values[rhs]);
		}

	};

	class Array<_Kind_> Values;

	HashIndex Idx;
	HashIndexer Idxer;

	HashSet():
		Idxer(Values)
	{
		Callback<bool,int,int> equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Equals = equals;

		Callback<int,int> hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);
		Idx.Hash = hash;

	}

	HashSet(HashSet&& set):
		Idxer(Values)
	{
		Copy(set);
	}

	HashSet(const HashSet& set):
		Idxer(Values)
	{
		Copy(set);
	}

	HashSet& operator = (const HashSet& set)
	{
		Copy(set);
	}

	HashSet& Clone(const HashSet& set)
	{
		return Copy(set);
	}

	HashSet& Copy(const HashSet& set)
	{
		if(&set == this) return *this;

		Values.Copy(set.Values);
		Idx.Copy(set.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		return *this;
	}

	HashSet& Move(HashSet&& set)
	{
		if(&set == this) return *this;

		Values.Move((Array<_Kind_>&&)set.Values);
		Idx.Move((HashIndex&&)set.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		set.Idx.Equals = 0;
		set.Idx.Hash = 0;

		return *this;
	}

	HashSet& Swap(const HashSet& set)
	{
		if(&set == this) return *this;

		Values.Swap(set.Values);
		Idx.Swap(set.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		set.Idx.Equals = Callback<bool,int,int>(&set.Idxer,&HashIndexer::Equals);
		set.Idx.Hash = Callback<int,int>(&set.Idxer,&HashIndexer::Hash);

		return *this;
	}

	_Kind_ & operator [] (const _Kind_ & value)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)value);

		int index = Idx.Select(-1);
		if (index != -1)
			return Values[index];

		index = Values.Size;
		Values.Append(value);

		Idx.Insert(index);
		return Values[index];

	}

	void Release()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Release(Values.At(i));
			}

			Values.Release();
			Idx.Release();
		}
	}

	void Destroy()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Kind_>::Destroy(Values.At(i));
			}

			Values.Destroy();
			Idx.Destroy();
		}
	}	

	int Length() {return Values.Size;}

	_Kind_ & At(int index) 
	{
		Indices::Normalise(index,Values.Size);

		if (index < Values.Size)
			return Values[index];

		Assert(false);		

		return Null<_Kind_>::Value();
	}	

	int Index(typename Template<_Kind_>::ConstReference kind)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index=0;
		int slot=0;
		Idx.Index(-1,slot,index);

		if (index == 0xFFFFFFFF)
			return -1; 

		return index;
	}

	Result Insert(typename Template<_Kind_>::ConstReference kind)
	{		
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1);
		if (index != -1)
			return Result(false,index);

		index = Values.Size;
		Values.Append(kind);

		Idx.Insert(index);
		return Result(true,index);
	}

	Result Remove(typename Template<_Kind_>::ConstReference kind)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Remove(-1);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Values.RemoveAt(offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result RemoveAt(int index)
	{
		if (!Indices::Normalise(index,Values.Size))
			return Result(false,index);

		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &Values[index];

		index = Idx.Remove(-1);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Values.RemoveAt(offset);

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
		}

		return Result(true,index);
	}

	Result Select(typename Template<_Kind_>::ConstReference kind)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1);
		if (index == -1)
			return Result(false,index);		

		return Result(true,index);
	}

	Result Update(typename Template<_Kind_>::ConstReference kind)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Value() = &((_Kind_&)kind);

		int index = Idx.Select(-1);
		if (index == -1)
		{
			return Insert(kind);
		}
		else
		{
			At(index) = kind;
			return Result(true,index);
		}		
	}	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Key_, typename _Value_>
class HashMap : Iterable<_Value_>
{
public:

	using Result = Result<bool,int>;

	struct HashMapped
	{

		Variable<_Key_*> Key;
		Optional<_Value_*> Value;

		HashMapped() {}
		HashMapped(const HashMapped & mapped):Key(mapped.Key),Value(mapped.Value) {}				

		HashMapped & operator = (const HashMapped & mapped)
		{
			Key = mapped.Key;
			Value = mapped.Value;
			return *this;
		}
	};

	struct HashIndexer
	{
		HashMap<_Key_,_Value_> & Map;

		HashMapped Mapped;
		HashMapped & Alias()
		{

			return Mapped;
		}

		HashIndexer(HashMap<_Key_,_Value_> & map):Map(map)
		{
		}

		int Hash(int key)
		{
			#ifdef HERO_HASH_MEMO

			if (key == -1)
			{
				HashMapped & mapped = Alias();
				int hash = Hero::Hasher<_Key_>::Hash(*mapped.Key());

				hash |= 1;

				return hash;
			}

			if (key >= Map.Hashes.Size) 
			{

				if (key >= Map.Hashes.Allocated)
				{								
					Map.Hashes.Allocate((key*Map.Hashes.Factor)+1);		
					Array<int>::Set(Map.Hashes.Data+Map.Hashes.Size,0,Map.Hashes.Allocated-Map.Hashes.Size);

					Assert(Map.Hashes.Allocated > key);
				}

				Map.Hashes.Size = key+1;
			}

			int & hash = Map.Hashes[key];
			if (hash != 0) 
				return hash;

			hash = Hero::Hasher<_Key_>::Hash(Map.Keys[key]);

			hash |= 1;

			return hash;

			#else

			if (key == -1)
			{
				HashMapped & mapped = Alias();
				return Hero::Hasher<_Key_>::Hash(*mapped.Key());			
			}

			return Hero::Hasher<_Key_>::Hash(Map.Keys[key]);

			#endif

		}

		bool Equals(int lhs, int rhs)
		{
			return Compare(lhs,rhs) == 0;
		}

		int Compare(int lhs, int rhs)
		{
			if (lhs == -1)
			{
				HashMapped & mapped = Alias();

				return Hero::Comparer<_Key_>::Compare(*mapped.Key(),Map.Keys[rhs]);
			}					

			return Hero::Comparer<_Key_>::Compare(Map.Keys[lhs],Map.Keys[rhs]);
		}

	};

	Array<_Key_> Keys;
	Array<_Value_> Values;

	Array<int> Hashes;

	HashIndex Idx;
	HashIndexer Idxer;

	HashMap():
		Idxer(*this)
	{
		Callback<bool,int,int> equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);		
		Idx.Equals = equals;

		Callback<int,int> hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);
		Idx.Hash = hash;
	}

	HashMap(HashMap&& map):
		Idxer(*this)
	{
		Move(map);
	}

	HashMap(const HashMap& map):
		Idxer(*this)
	{
		Copy(map);
	}

	HashMap& operator = (const HashMap& map)
	{
		Copy(map);
	}

	HashMap& Clone(const HashMap& map)
	{
		return Copy(map);
	}

	HashMap& Copy(const HashMap& map)
	{
		if(&map == this) return *this;

		Keys.Copy(map.Keys);
		Values.Copy(map.Values);
		#ifdef HERO_HASH_MEMO
		Hashes.Copy(map.Hashes);
		#endif

		Idx.Copy(map.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		return *this;
	}

	HashMap& Move(HashMap&& map)
	{
		if(&map == this) return *this;

		Keys.Move((Array<_Key_>&&)map.Keys);
		Values.Move((Array<_Value_>&&)map.Values);

		#ifdef HERO_HASH_MEMO
		Hashes.Move((Array<int>&&)map.Hashes);
		#endif

		Idx.Move((HashIndex&&)map.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		map.Idx.Equals = 0;
		map.Idx.Hash = 0;

		return *this;
	}

	HashMap& Swap(const HashMap& map)
	{
		if(&map == this) return *this;

		Keys.Swap(map.Keys);
		Values.Swap(map.Values);
		#ifdef HERO_HASH_MEMO
		Hashes.Swap(map.Hashes);
		#endif

		Idx.Swap(map.Idx);

		Idx.Equals = Callback<bool,int,int>(&Idxer,&HashIndexer::Equals);
		Idx.Hash = Callback<int,int>(&Idxer,&HashIndexer::Hash);

		map.Idx.Equals = Callback<bool,int,int>(&map.Idxer,&HashIndexer::Equals);
		map.Idx.Hash = Callback<int,int>(&map.Idxer,&HashIndexer::Hash);

		return *this;
	}

	_Value_ & operator [] (const _Key_ & key)
	{

		Idx.Memo.Reset();

		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1);
		if (index != -1)
		{

			return Values[index];
		}

		index = Keys.Size;
		Values.Append(_Value_());
		Keys.Append(key);

		Idx.Insert(index);
		return Values[index];
	}

	_Value_ & operator [] (_Key_ && key)
	{
		#ifdef HERO_USING_TRACY
		ZoneScopedS(16);
		#endif

		Idx.Memo.Reset();

		Keys.Append((_Key_&&)key);

		int index = Idx.Select(Keys.Size-1);
		if (index != -1)
		{

			Keys[Keys.Size-1] = _Key_();
			--Keys.Size;

			#ifdef HERO_HASH_MEMO

			--Hashes.Size;
			#endif

			return Values[index];
		}

		Values.Append(_Value_());
		index = Keys.Size-1;

		Idx.Insert(index);
		return Values[index];
	}

	void Release()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{
				Disposer<_Key_>::Release(Keys.At(i));
				Disposer<_Value_>::Release(Values.At(i));
			}

			Keys.Release();
			Values.Release();
			#ifdef HERO_HASH_MEMO
			Hashes.Release();
			#endif

			Idx.Release();
		}
	}

	void Destroy()
	{
		if (Length())
		{
			for (int i=0;i<Length();++i)
			{

				Disposer<_Key_>::Destroy(Keys.At(i));
				Disposer<_Value_>::Destroy(Values.At(i));
			}

			Keys.Destroy();
			Values.Destroy();
			#ifdef HERO_HASH_MEMO
			Hashes.Destroy();
			#endif

			Idx.Destroy();
		}
	}	

	int Length() {return Values.Size;}

	_Value_ & At(int index) 
	{
		Indices::Normalise(index,Values.Size);

		if (index < Values.Size)
			return Values[index];

		Assert(false);		

		return Null<_Value_>::Value();
	}	

	int Index(typename Template<_Key_>::ConstReference key)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index=0;
		int slot=0;
		Idx.Index(-1,slot,index);

		if (index == 0xFFFFFFFF)

			return -1; 

		return index;
	}

	Result Insert(typename Template<_Key_>::ConstReference key, typename Template<_Value_>::ConstReference value)
	{		
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1);
		if (index != -1)
			return Result(false,index);

		index = Keys.Size;
		Values.Append(value);
		Keys.Append(key);

		Idx.Insert(index);
		return Result(true,index);
	}

	Result Insert(typename Template<_Key_>::ConstReference key)
	{
		return Insert(key,_Value_());
	}

	Result Remove(typename Template<_Key_>::ConstReference key)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Remove(-1);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Keys.Swap(index,offset);
			#ifdef HERO_HASH_MEMO
			Hashes.Swap(index,offset);
			#endif

			Values.RemoveAt(offset);
			Keys.RemoveAt(offset);
			#ifdef HERO_HASH_MEMO
			Hashes.RemoveAt(offset);
			#endif

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
			Keys.RemoveAt(index);
			#ifdef HERO_HASH_MEMO
			Hashes.RemoveAt(index);
			#endif
		}

		return Result(true,index);
	}

	Result RemoveAt(int index)
	{
		if (!Indices::Normalise(index,Keys.Size))
			return Result(false,index);

		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &Keys[index];

		index = Idx.Remove(-1);
		if (index == -1)
			return Result(false,index);

		int offset = Length()-1;
		if (index != offset)
		{
			Idx.Remove(offset);

			Values.Swap(index,offset);
			Keys.Swap(index,offset);
			#ifdef HERO_HASH_MEMO
			Hashes.Swap(index,offset);
			#endif

			Values.RemoveAt(offset);
			Keys.RemoveAt(offset);
			#ifdef HERO_HASH_MEMO
			Hashes.RemoveAt(offset);
			#endif

			Idx.Insert(index);
		}
		else
		{
			Values.RemoveAt(index);
			Keys.RemoveAt(index);
			#ifdef HERO_HASH_MEMO
			Hashes.RemoveAt(index);
			#endif
		}

		return Result(true,index);
	}

	Result Select(typename Template<_Key_>::ConstReference key)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1);
		if (index == -1)
			return Result(false,index);		

		return Result(true,index);
	}

	Result Update(typename Template<_Key_>::ConstReference key, typename Template<_Value_>::ConstReference value)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1);
		if (index == -1)
		{
			return Insert(key,value);			
		}
		else
		{
			At[index] = value;
			#ifdef HERO_HASH_MEMO
			Hashes[index] = 0;
			#endif

			return Result(true,index);
		}
	}	

	Result Update(typename Template<_Key_>::ConstReference key)
	{
		HashMapped & mapped = Idxer.Alias();
		mapped.Key() = &((_Key_&)key);

		int index = Idx.Select(-1);
		if (index == -1)
		{
			return Insert(key,_Value_());			
		}

		#ifdef HERO_HASH_MEMO
		Hashes[index] = 0;
		#endif

		return Result(true,index);
	}	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_, int _Capacity_=075>
class Hashtable
{
public:

	static const float Capacity; 

	Vector< Vector<_Kind_> > Entries;

	Hashtable()
	{
	}

	int Hash(int key)
	{

		key = ~key + (key << 15);
		key = key ^ (key >> 12);
		key = key + (key << 2);
		key = key ^ (key >> 4);
		key = key * 2057;
		key = key ^ (key >> 16);
		return key;
	}

	_Kind_ & At(int index)
	{

	}

	bool Insert(typename Template<_Kind_>::Reference kind)
	{

		if (Entries.Size > Entries.Allocated*Capacity)
		{
			Vector< Vector<_Kind_> > entries;
			entries.Allocated = (Entries.Allocated*1.5)+1;
			entries.Data = new Vector<_Kind_>[entries.Allocated];

			int size = Entries.Size;
			entries.Size = entries.Allocated;

			for (int index=0;index<Entries.Allocated;++index)
			{						
				for (int chain=0;chain<Entries[index].Size;++chain)
				{
					_Kind_ & k = Entries[index][chain];

					int key = (Hash(k)&(entries.Allocated-1));

					entries[key].Insert(k);					
				}
			}

			if (entries.Data != entries.Mem)
			{
				if (Entries.Data != Entries.Mem)
					delete [] Entries.Data;

				Entries.Data = entries.Data;
				Entries.Allocated = entries.Allocated;
				entries.Data = 0;
			}
			else				
			{
				Entries = entries;
			}

			Entries.Size = size;				
		}

		int key = (Hash(kind)&(Entries.Allocated-1));
		Entries[key].Insert(kind);						

		++Entries.Size;

		return true;
	}

	bool Remove(typename Template<_Kind_>::Reference kind)
	{
		int key = (Hash(kind)&(Entries.Allocated-1));
		return Entries[key].Remove(kind);	
	}

	bool Select(typename Template<_Kind_>::Reference kind)
	{
		int key = (Hash(kind)&(Entries.Allocated-1));
		return Entries[key].Select(kind);	
	}

};

template<typename _Kind_, int _Capacity_>
const float Hashtable<_Kind_,_Capacity_>::Capacity = (float)_Capacity_/100;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, typename _Iterable_=List<_Kind_> >
class Enumeration : public _Iterable_
{
public:

	Iterator<_Kind_> It;

	bool Has() {return It.Has();}

	Iterand<_Kind_> & Move() {return It.Move();}
	Iterand<_Kind_> & Move(int amount) {return It.Move(amount);}
	Iterand<_Kind_> & Forward() 
	{
		if (!It.Iterand)
			It.Iterand = this->Iterate();
		return It.Forward();
	}

	Iterand<_Kind_> & Reverse() 
	{	
		if (!It.Iterand)
			It.Iterand = this->Iterate();	
		return It.Reverse();
	}

	int Index() {return It.Index();}

	typename Type<_Kind_>::Pointer Pointer() {return It.Pointer();}
	typename Type<_Kind_>::Reference Reference() {return It.Reference();}
	typename Template<_Kind_>::Reference operator () (void) {return It();}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Enumerable : public Iterable<_Kind_> 
{
public:

	Iterator<_Kind_> It;

	Enumerable()
	{

	}

	bool Has() {return It.Has();}

	Iterand<_Kind_> & Move() {return It.Move();}
	Iterand<_Kind_> & Move(int amount) {return It.Move(amount);}
	Iterand<_Kind_> & Forward() 
	{
		if (!It.Iterand)
			It.Iterand = this->Iterate();
		return It.Forward();
	}

	Iterand<_Kind_> & Reverse() 
	{	
		if (!It.Iterand)
			It.Iterand = this->Iterate();	
		return It.Reverse();
	}

	int Index() {return It.Index();}

	typename Type<_Kind_>::Pointer Pointer() {return It.Pointer();}
	typename Type<_Kind_>::Reference Reference() {return It.Reference();}
	typename Template<_Kind_>::Reference operator () (void) {return It();}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Generator : public Enumerable<_Kind_>
{
public:

	static Identity	Instance;
	virtual Identity & Identify() {return Instance;};

	_Kind_ * Generate;

	Hero::Callback<_Kind_&,int,void*> Callback;

	Generator():Generate(0)
	{

	}

	virtual _Kind_ & At(int index=0)
	{

		return *Generate;
	};

	virtual int Length() 
	{

		return 0;
	}

	virtual int Move(Iterand<_Kind_> & iterand, int amount)
	{

		amount *= iterand.Option;

		iterand.Index += amount;
		_Kind_ & kind = this->Callback(iterand.Index,this->Generate);
		Generate = &kind;
		return iterand.Index;
	}

};

template<typename _Kind_>
Identity Generator<_Kind_>::Instance;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_PLATFORM_POSIX
#pragma clang diagnostic pop

#endif