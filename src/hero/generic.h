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

#pragma warning(disable:4522)

#pragma warning(disable:4351)

#include "hero/hero.h"

#include "hero/platform.h"
#include "hero/object.h"
#include "hero/atomic.h"
#include "hero/types.h"

#if defined (HERO_PLATFORM_LINUX) || defined(HERO_PLATFORM_MINGW)
#ifdef HERO_USING_STD
#include <new>

#endif
#else
#include <new.h>

#endif

#include "hero/assert.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

template<bool _Bool_> 
struct StaticAssertHelper
{
	StaticAssertHelper(...) {}
};

template<>
struct StaticAssertHelper<false>
{
	StaticAssertHelper() {}
};

#define StaticAssert(x) {\
	StaticAssertHelper<((bool)(x) != false)> error = StaticAssertHelper<((bool)(x) != false)>(None());\
	(void)sizeof(error);}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Cast
{

	template<typename _Kind_>
	static _Kind_ & Reference(const _Kind_ & kind)
	{
		return (_Kind_&)kind;
	}

	template<typename _Kind_>
	static _Kind_ * Pointer(const _Kind_ & kind)
	{
		return &(_Kind_&)kind;
	}

	template<typename _Kind_>
	static _Kind_ Value(const _Kind_ & kind)
	{
		return (_Kind_&)kind;
	}

	template<typename _To_, typename _From_>
	static _To_ Primitive(const _From_ & from)
	{

		union PrimitiveCast
		{
			_From_ From;
			_To_ To;
		};

		PrimitiveCast cast = {from};
		return cast.To;
	}	

	template <typename _Kind_>
	_Kind_ * Dynamic(Object * object)
	{
		if (object->InstanceOf(_Kind_::Instance))
			return (_Kind_*)object;
		else
			return 0;
	};

	template <typename _Kind_>
	_Kind_ Reinterpret(_Kind_ kind)
	{
		return *(_Kind_*)((void*)&kind);
	};

	template<typename _Kind_>
	_Kind_ Reinterpret(_Kind_ * kind)
	{
		return (_Kind_)((void*)kind);
	};

	template <typename _Kind_>
	_Kind_ Static(typename Template<_Kind_>::Reference kind)
	{
		return (_Kind_)kind;
	};

	template <typename _Kind_>
	_Kind_ Const(typename Template<_Kind_>::ConstReference kind)
	{
		return (_Kind_)kind;
	};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct Null
{

	static typename Hero::Template<_Kind_>::Reference Value() 
	{

		static char mem[sizeof(_Kind_)] = {};

		return *(typename Hero::Template<_Kind_>::Pointer)(void*)&mem;
	}

	static typename Hero::Template<_Kind_>::Pointer Pointer() {return Hero::Pointer<_Kind_>::Template(0);}

	static bool Equals(typename Hero::Template<_Kind_>::ConstReference kind)
	{

		if (&kind == &Value())
			return true;

		static char mem[sizeof(_Kind_)] = {};
		return memcmp(&kind,&mem,sizeof(_Kind_)) == 0;
	}

	bool operator == (typename Hero::Template<_Kind_>::ConstReference kind)
	{
		return Null<_Kind_>::Equals(kind);
	}
};

template<>
struct Null<void>
{
	static void Value() {return (void)0;}
	static void * Pointer() {return (void*)0;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Item 
{
public:

	Item() 
	{
	}

	virtual ~Item() 
	{
	}

	virtual int Compare(typename Template<_Kind_>::ConstReference type, int comparitor = Comparer<_Kind_>::COMPARE_GENERAL)
	{
		return Comparer<_Kind_>::Compare((*this)(),type,comparitor);
	}

	virtual int Compare(Item<_Kind_> & value, int comparitor = Comparer<_Kind_>::COMPARE_GENERAL)
	{

		return Comparer<_Kind_>::Compare((*this)(),value(),comparitor);
	}

	virtual typename Type<_Kind_>::Pointer Pointer()=0;
	virtual typename Type<_Kind_>::Reference Reference()=0;
	virtual typename Template<_Kind_>::Reference operator () (void)=0;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class Variable : public Item<_Kind_>
{
public:

	Variable():Kind()
	{

	}

	Variable(const Variable & variable):Kind(((Variable &)variable).Kind)
	{
	}

	Variable(typename Template<_Kind_>::ConstReference kind):Kind((typename Template<_Kind_>::Reference)kind)
	{
	}

	~Variable()
	{
	}

	_Kind_ Kind;
	_Kind_ & operator ()(void)	
	{
		return Kind;
	}

	Variable & operator = (const Variable & variable)
	{
		Kind = ((Variable & )variable).Kind;
		return *this;
	}

	typename Type<_Kind_>::Value Value() {return Kind;}
	typename Type<_Kind_>::Reference Reference() {return Kind;}
	typename Type<_Kind_>::Pointer Pointer() {return (_Kind_*)&(char&)Kind;}

};

template <typename _Kind_>
class Variable<_Kind_&> : public Item<_Kind_&>
{
public:

	Variable()
	{

	}

	Variable(const Variable & variable):Kind(((Variable &)variable).Kind)
	{
	}

	Variable(typename Template<_Kind_&>::ConstReference kind):Kind((typename Template<_Kind_&>::Reference)kind)
	{
	}

	~Variable()
	{
	}

	_Kind_ & Kind;
	_Kind_ & operator ()(void)	
	{
		return Kind;
	}

	Variable & operator = (const Variable & variable)
	{
		Kind = variable.Kind;
		return *this;
	}

	typename Type<_Kind_&>::Reference Value()	{return Kind;}
	typename Type<_Kind_&>::Reference Reference() {return Kind;}
	typename Type<_Kind_&>::Pointer Pointer()	{return (_Kind_*)&(char&)Kind;}

};

template <typename _Kind_>
class Variable<_Kind_*> : public Item<_Kind_*>
{
public:

	Variable():Kind(0)
	{
	}

	Variable(const Variable & variable):Kind(((Variable &)variable).Kind)
	{
	}

	Variable(typename Template<_Kind_*>::ConstReference kind):Kind((typename Template<_Kind_*>::Reference)kind)
	{
	}

	~Variable()
	{
	}

	_Kind_ * Kind;
	_Kind_ *& operator ()(void)	
	{
		return Kind;
	}

	Variable & operator = (const Variable & variable)
	{
		Kind = variable.Kind;
		return *this;
	}

	typename Type<_Kind_*>::Reference Value() {return *Kind;}
	typename Type<_Kind_*>::Reference Reference() {return *Kind;}
	typename Type<_Kind_*>::Pointer Pointer() {return Kind;}

};

template <>
class Variable<void*> : public Item<void*>
{
public:

	Variable():Kind(0)
	{
	}

	Variable(const Variable & variable):Kind(((Variable &)variable).Kind)
	{
	}

	Variable(const void * kind):Kind((void*)kind)
	{
	}

	~Variable()
	{
	}

	void * Kind;
	void *& operator ()(void)	
	{
		return Kind;
	}

	Variable & operator = (const Variable & variable)
	{
		Kind = variable.Kind;
		return *this;
	}

	void Value() {}
	void *& Reference() {return Kind;}
	void * Pointer() {return Kind;}
};

template <>
class Variable<void> : public Item<void*>

{
public:

	Variable():Kind(0)
	{
	}

	Variable(const Variable & variable):Kind(((Variable &)variable).Kind)
	{
	}

	Variable(const void * kind):Kind((void*)kind)
	{
	}

	~Variable()
	{
	}

	void * Kind;
	void *& operator ()(void)	
	{
		return Kind;
	}

	Variable & operator = (const Variable & variable)
	{
		Kind = variable.Kind;
		return *this;
	}

	void Value() {}
	void *& Reference() {return Kind;}
	void * Pointer() {return Kind;}
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

template <typename _Kind_, typename TypeOfKind>
bool InstanceOf(typename Template<_Kind_>::ConstReference left, TypeOfKind type)
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
void Swap(typename Template<_Kind_>::Reference lhs, typename Template<_Kind_>::Reference rhs)
{
	_Kind_ tmp = lhs;
	lhs = rhs;
	rhs = tmp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Proxied
{
public:

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class Proxy
{
public:

	class Counted
	{
	public:

		char Mem[sizeof(_Kind_)];
		_Kind_ * Kind;
		volatile int Weak;
		volatile int Strong;

		Counted(typename Template<_Kind_>::ConstReference kind):Kind(0),Weak(0),Strong(0) 

		{

			Kind = new ((void*)Mem) _Kind_ (kind);

		}

		Counted():Kind(0),Weak(0),Strong(0)

		{

		}

		~Counted()
		{

			if (Kind == (_Kind_*)&Mem)
			{
				Kind->_Kind_::~_Kind_();
				Kind = 0;
			}
		}

		operator int ()
		{
			return Strong;
		}

		void Set(typename Template<_Kind_>::ConstReference kind)
		{
			if (!Kind)
				Kind = new ((void*)Mem) _Kind_ (kind);
			else	
				*Kind = kind;
		}

		void Zero()
		{

			if (this->Kind)
			{

				if (Kind == (_Kind_*)&Mem)
				{

					Kind->_Kind_::~_Kind_();
					Kind = 0;
				}
			}
		}
	};

	Counted * Count;	

	Proxy():Count(0)
	{
	}

	virtual ~Proxy()
	{
	}

	_Kind_ & operator () (void) const
	{

		if (!Count)
		{
			((Proxy<_Kind_>*)this)->Count = new Counted();
			((Proxy<_Kind_>*)this)->Increment();
		}

		if (!Count->Kind)
			Count->Kind = new ((void*)Count->Mem) _Kind_ ();

		return *Count->Kind;									
	}

	_Kind_ * operator & () const
	{
		_Kind_ & kind = (*this)();
		return &kind;
	}	

	_Kind_ * operator -> () const
    {

		bool null = (Count==0||Count->Kind==0);
		_Kind_ & kind = (*this)();
		if (null) Count->Set(Null<_Kind_>::Value());
		return &kind;
    }

    _Kind_ & operator * () const
    {
		bool null = (Count==0||Count->Kind==0);
    	_Kind_ & kind = (*this)();
		if (null) Count->Set(Null<_Kind_>::Value());
		return kind;
    }

	operator _Kind_ * () const
	{

		return (Count==0)?0:Count->Kind;
	}

	bool operator != (Proxy<_Kind_> & proxy) const
	{
		return Count != proxy.Count;
	}

    bool operator == (Proxy<_Kind_> & proxy) const
    {
        return Count == proxy.Count;
    }

   	bool operator ()(Proxy<_Kind_> * proxy) const
	{

		return this == proxy;
	} 

    void Copy(Proxy<_Kind_> & proxy)
	{

		Counted * counted = this->Count;
		this->Count = proxy.Count;
		Increment();
		this->Count = counted;
		counted = proxy.Count;
		Decrement();
		this->Count = counted;
	}

	void Swap(Proxy<_Kind_> & proxy)
	{

		Counted * counted = this->Count;
		proxy.Count = this->Count;
		this->Count = counted;	
	}

	virtual void Increment()=0;
	virtual void Decrement()=0;	

};

template<typename _Kind_>
class Proxy<_Kind_*>
{
public:

	class Counted
	{
	public:

		_Kind_ * Kind;
		volatile int Weak;
		volatile int Strong;

		Counted(typename Template<_Kind_*>::ConstReference kind):Kind(Pointer<_Kind_*>::Type(kind)),Weak(0),Strong(0)
		{
		}

		Counted():Kind(0),Weak(0),Strong(0)
		{
		}

		operator int ()
		{
			return Strong;
		}

		void Set(typename Template<_Kind_*>::ConstReference kind)
		{
			Kind = Hero::Pointer<_Kind_>::Type(kind);
		}

		void Zero()
		{
			if (this->Kind)
			{
				delete this->Kind;
				this->Kind = 0;
			}
		}
	};

	Counted * Count;	

	Proxy():Count(0)
	{
	}

	virtual ~Proxy()
	{
	}

	_Kind_ * operator () (void) const
	{
        Assert(Count != 0 && Count->Kind != 0);
		return (Count==0)?0:Count->Kind;	
	}

	_Kind_ ** operator & () const
	{

		if (!Count)
		{
			((Proxy<_Kind_*>*)this)->Count = new Counted();
			((Proxy<_Kind_*>*)this)->Increment();
		}

		return &Count->Kind;

	}

	_Kind_ * operator -> () const
    {
        Assert(Count != 0 && Count->Kind != 0);
		return (Count==0)?0:Count->Kind;
    }

    _Kind_ & operator * () const
    {
		Assert(Count != 0 && Count->Kind != 0);
        return *Count->Kind;
    }

	operator _Kind_ * () const
	{

		return (Count==0)?0:Count->Kind;
	}

	bool operator != (Proxy<_Kind_*> & proxy) const
	{

		return Count != proxy.Count;
	}

    bool operator == (Proxy<_Kind_*> & proxy) const
    {

        return Count == proxy.Count;
    }

	bool operator () (Proxy<_Kind_*> * proxy) const
	{

		return this == proxy;
	}

    void Copy(Proxy<_Kind_*> & proxy)
	{
		Counted * counted = this->Count;
		this->Count = proxy.Count;
		Increment();
		this->Count = counted;
		counted = proxy.Count;
		Decrement();
		this->Count = counted;
	}

	void Swap(Proxy<_Kind_*> & proxy)
	{
		Counted * counted = this->Count;
		proxy.Count = this->Count;
		this->Count = counted;	
	}

	virtual void Increment()=0;
	virtual void Decrement()=0;

};

template<>
class Proxy<void*>
{
public:

    class Counted
    {
    public:

        void * Kind;
        volatile int Weak;
        volatile int Strong;

		Counted(Template<void*>::ConstReference kind):Kind(Pointer<void*>::Type(kind)),Weak(0),Strong(0)
        {

        }

        Counted():Kind(0),Weak(0),Strong(0)
        {

        }

        operator int ()
        {
            return Strong;
        }

		void Set(Template<void*>::ConstReference kind)
        {
            Kind = Pointer<void*>::Type(kind);
        }

        void Zero()
        {
            if (this->Kind)
            {
                this->Kind = 0;
            }
        }
    };

    Counted * Count;	

    Proxy():Count(0)
    {
    }

    virtual ~Proxy()
    {
    }

    void * operator () (void) const
    {

        return (Count==0)?0:Count->Kind;	
    }

    void ** operator & () const
    {
        if (!Count)
        {
            ((Proxy<void*>*)this)->Count = new Counted();
            ((Proxy<void*>*)this)->Increment();
        }

        return &Count->Kind;
    }

    void * operator -> () const
    {

        return (Count==0)?0:Count->Kind;
    }

    operator void * () const
    {
        return (Count==0)?0:Count->Kind;
    }

    bool operator != (Proxy<void*> & proxy) const
    {
        return Count != proxy.Count;
    }

    bool operator == (Proxy<void*> & proxy) const
    {
        return Count == proxy.Count;
    }

    bool operator () (Proxy<void*> * proxy) const
    {
        return this == proxy;
    }

    void Copy(Proxy<void*> & proxy)
	{
		Counted * counted = this->Count;
		this->Count = proxy.Count;
		Increment();
		this->Count = counted;
		counted = proxy.Count;
		Decrement();
		this->Count = counted;
	}

	void Swap(Proxy<void*> & proxy)
	{
		Counted * counted = this->Count;
		proxy.Count = this->Count;
		this->Count = counted;	
	}

	virtual void Increment()=0;
	virtual void Decrement()=0;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_> 
class Owned : public Proxy<_Kind_>
{
public:

    Owned()
	{	
	}

    Owned(typename Template<_Kind_>::ConstReference kind)
    {
		if (!Null<_Kind_>::Equals(kind)) operator = (kind);
    }

	Owned(const Owned<_Kind_> & owned)
    {
		operator = ((Owned<_Kind_> &)owned);
    }

    ~Owned()
    {
		Decrement();
		this->Count=0;
    }

	void Increment()
	{
		if (this->Count) 
			Atomic::Inc(&this->Count->Strong);

	}

	void Decrement()
	{
		if (this->Count)
		{
			if (this->Count->Strong > 0)
				Atomic::Dec(&this->Count->Strong);

			if (this->Count->Strong == 0)
			{
				this->Count->Zero();

				if (this->Count->Weak == 0)
				{	
					delete this->Count;
					this->Count = 0;
				}
			}
		}
	}

    Owned<_Kind_>& operator = (typename Template<_Kind_>::ConstReference kind)
    {

		if (!Null<_Kind_>::Equals(kind))
		{
			typename Proxy<_Kind_>::Counted * counted = new typename Proxy<_Kind_>::Counted(kind);
			Decrement();
			this->Count = counted;
			Increment();
		}
		else
		{
			Decrement();
			this->Count = 0;
		}

		return *this;
    }

    Owned<_Kind_>& operator = (const Owned<_Kind_> & owned)
    {
		if (owned.Count == 0 && !owned(this))
		{
			((Owned<_Kind_> &)owned).Count = new typename Proxy<_Kind_>::Counted();
			((Owned<_Kind_> &)owned).Increment();
		}

		this->Copy(((Owned<_Kind_> &)owned));

		return *this;
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_> 
class Shared : public Proxy<_Kind_>
{
public:

    Shared()
	{	
	}

    Shared(typename Template<_Kind_>::ConstReference kind)
    {
		if (!Null<_Kind_>::Equals(kind)) operator = (kind);
    }

	Shared(const Shared<_Kind_> & shared)
    {
		operator = ((Shared<_Kind_> &)shared);
    }

    ~Shared()
    {
		Decrement();
		this->Count=0;
    }

	void Increment()
	{
		if (this->Count) 
			Atomic::Inc(&this->Count->Strong);
	}

	void Decrement()
	{
		if (this->Count)
		{
			if (this->Count->Strong > 0)
				Atomic::Dec(&this->Count->Strong);

			if (this->Count->Strong == 0)
			{
				this->Count->Zero();

				if (this->Count->Weak == 0)
				{	
					delete this->Count;
					this->Count = 0;
				}
			}
		}
	}

    Shared<_Kind_> & operator = (typename Template<_Kind_>::ConstReference kind)
    {
		if (this->Count)
		{

			this->Count->Set(kind);
		}
		else
		{
			if (!Null<_Kind_>::Equals(kind))
			{
				this->Count = new typename Proxy<_Kind_>::Counted(kind);
				Increment();
			}
		}

		return *this;
    }

    Shared<_Kind_>& operator = (const Shared<_Kind_> & shared)
    {

		if (shared.Count == 0 && !shared(this))
		{
			((Shared<_Kind_> &)shared).Count = new typename Proxy<_Kind_>::Counted();
			((Shared<_Kind_> &)shared).Increment();
		}

		this->Copy(((Shared<_Kind_> &)shared));

		return *this;
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, template<typename> class _Proxy_ = Owned> class Weak;
template <typename _Kind_, template<typename> class _Proxy_ = Owned> class Auto;
template <typename _Kind_, template<typename> class _Proxy_ = Owned> class Scoped;

template <typename _Kind_, template<typename> class _Proxy_ = Owned> 
class Strong : public _Proxy_<_Kind_>
{
public:

	Strong() {}
	Strong(typename Template<_Kind_>::ConstReference kind) {operator = (kind);}
	Strong(const Strong<_Kind_,_Proxy_> & strong) {operator = ((Strong<_Kind_,_Proxy_> &)strong);}
	Strong(const Weak<_Kind_,_Proxy_> & weak) {operator = ((Weak<_Kind_,_Proxy_>&)weak);}
	Strong(const Auto<_Kind_,_Proxy_> & automatic) {operator = ((Auto<_Kind_,_Proxy_> &)automatic);}
	Strong(const Scoped<_Kind_,_Proxy_> & scoped) {operator = ((Scoped<_Kind_,_Proxy_> &)scoped);}

	~Strong()
	{
	}

	using _Proxy_<_Kind_>::operator = ;

	Strong<_Kind_,_Proxy_>& operator = (const Auto<_Kind_,_Proxy_> & automatic)
	{
		Swap((Auto<_Kind_,_Proxy_> &)automatic);
		((Auto<_Kind_,_Proxy_> &)automatic).Count = 0;
		return *this;
	}

	Strong<_Kind_,_Proxy_>& operator = (const Scoped<_Kind_,_Proxy_> & scoped)
	{
		return *this;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, template<typename> class _Proxy_> 
class Weak : public _Proxy_<_Kind_>
{
public:

	Weak() {}
	Weak(typename Template<_Kind_>::ConstReference kind) {operator = (kind);}
	Weak(const Strong<_Kind_,_Proxy_> & strong) {operator = ((Strong<_Kind_,_Proxy_> &)strong);}
	Weak(const Weak<_Kind_,_Proxy_> & weak) {operator = ((Weak<_Kind_,_Proxy_>&)weak);}
	Weak(const Auto<_Kind_,_Proxy_> & automatic) {operator = ((Auto<_Kind_,_Proxy_> &)automatic);}
	Weak(const Scoped<_Kind_,_Proxy_> & scoped) {operator = ((Scoped<_Kind_,_Proxy_> &)scoped);}

	~Weak()
	{

		Decrement();
		this->Count=0;
	}

	void Increment()
	{
		if (this->Count) 
			Atomic::Inc(&this->Count->Weak);
	}

	void Decrement()
	{
		if (this->Count)
		{
			if (this->Count->Weak > 0)
				Atomic::Dec(&this->Count->Weak);

			if (this->Count->Strong == 0)
			{

				if (this->Count->Weak == 0)
				{	
					delete this->Count;
					this->Count = 0;
				}
			}
		}
	}

	using _Proxy_<_Kind_>::operator = ;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, template<typename> class _Proxy_> 
class Auto : public _Proxy_<_Kind_>
{
public:

	Auto() {}
	Auto(typename Template<_Kind_>::ConstReference kind) {operator = (kind);}
	Auto(const Strong<_Kind_,_Proxy_> & strong) {operator = ((Strong<_Kind_,_Proxy_> &)strong);}
	Auto(const Weak<_Kind_,_Proxy_> & weak) {operator = ((Weak<_Kind_,_Proxy_>&)weak);}
	Auto(const Auto<_Kind_,_Proxy_> & automatic) {operator = ((Auto<_Kind_,_Proxy_> &)automatic);}
	Auto(const Scoped<_Kind_,_Proxy_> & scoped) {operator = ((Scoped<_Kind_,_Proxy_> &)scoped);}

	~Auto()
	{
	}

	using _Proxy_<_Kind_>::operator = ;

	Auto<_Kind_,_Proxy_>& operator = (const Auto<_Kind_,_Proxy_> & automatic)
	{
		if (!automatic(this))
		{

			Assert(automatic.Count != this->Count);
			Assert(automatic.Count->Strong == 1);

			this->Swap((Auto<_Kind_,_Proxy_> &)automatic);
			this->Decrement();
			((Auto<_Kind_,_Proxy_> &)automatic).Count = 0;
		}
		return *this;
	}

	Auto<_Kind_,_Proxy_>& operator = (const Scoped<_Kind_,_Proxy_> & scoped)
	{
		return *this;
	}

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_, template<typename> class _Proxy_> 
class Scoped : public _Proxy_<_Kind_>
{
public:

	Scoped() {}
	Scoped(typename Template<_Kind_>::ConstReference kind) {operator = (kind);}
	Scoped(const Strong<_Kind_,_Proxy_> & strong) {operator = ((Strong<_Kind_,_Proxy_> &)strong);}
	Scoped(const Weak<_Kind_,_Proxy_> & weak) {operator = ((Weak<_Kind_,_Proxy_>&)weak);}
	Scoped(const Auto<_Kind_,_Proxy_> & automatic) {operator = ((Auto<_Kind_,_Proxy_> &)automatic);}
	Scoped(const Scoped<_Kind_,_Proxy_> & scoped) {operator = ((Scoped<_Kind_,_Proxy_> &)scoped);}

	~Scoped()
	{
	}

	using _Proxy_<_Kind_>::operator = ;

	Scoped<_Kind_,_Proxy_>& operator = (const Strong<_Kind_,_Proxy_> & strong)
	{
		return *this;
	}

	Scoped<_Kind_,_Proxy_>& operator = (const Weak<_Kind_,_Proxy_> & weak)
	{
		return *this;
	}

	Scoped<_Kind_,_Proxy_>& operator = (const Auto<_Kind_,_Proxy_> & automatic)
	{
		if (!automatic(this))
		{
			Assert(automatic.Count != this->Count);
			Assert(automatic.Count->Strong == 1);

			Swap((Auto<_Kind_,_Proxy_> &)automatic);
			this->Decrement();
			((Auto<_Kind_,_Proxy_> &)automatic).Count = 0;
		}
		return *this;
	}

	Scoped<_Kind_,_Proxy_>& operator = (const Scoped<_Kind_,_Proxy_> & scoped)
	{
		return *this;
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

template <typename _Kind_> class Nullable : public Variable<_Kind_>
{
public:

	bool Null;

	Nullable(const _Kind_ & variant):Variable<_Kind_>(variant),Null(false) {};
	Nullable():Null(false) {};
	~Nullable() {};

	_Kind_ * operator & ()
	{
		return (Null)?(_Kind_ *)(0):&this->Kind;
	}

	operator _Kind_ ()
	{
		return this->Kind;
	}

	Nullable<_Kind_> & operator = (const _Kind_ & kind)
	{
		this->Kind=kind;
		return *this;
	}

	bool IsNull() {return Null;}

	void Nullify()		{Null=true;};
	void Denullify()	{Null=false;};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
class Optional : public Item<_Kind_>
{
public:

	char Mem[sizeof(_Kind_)] = {0};
	_Kind_ * Kind;

	Optional():
		Mem(),Kind(0)
	{
	}

	Optional(typename Template<_Kind_>::ConstReference kind):
		Mem(),Kind(0)
	{
		operator = (kind);
	}

	Optional(const Optional & optional):
		Mem(),Kind(0)
	{
		operator = (optional);
	}

	~Optional()
	{
		if (Kind != 0)
		{
			Assert(Kind == (_Kind_*)&Mem);
			Kind->_Kind_::~_Kind_();
			Kind = 0;
		}
	}

	Optional & operator = (const Optional & optional)
	{	
		if (optional.Kind == 0)
		{
			if (Kind != 0)
			{
				Kind->_Kind_::~_Kind_();
				Kind = 0;		
			}
		}
		else
		if (Kind != 0)
		{
			Assert(Kind == (_Kind_*)&Mem);

			char mem[sizeof(_Kind_)];
			_Kind_ * copy = new ((void*)mem) _Kind_(*optional.Kind);
			memset(Mem,0,sizeof(_Kind_));

			Kind->_Kind_::~_Kind_();
			Kind = new ((void*)Mem) _Kind_(*copy);
		}
		else
		{
			Kind = new ((void*)Mem) _Kind_(*optional.Kind);
		}

		return *this;		
	}

	Optional & operator = (typename Template<_Kind_>::ConstReference kind)
	{
		if (Kind != 0)
		{
			Assert(Kind == (_Kind_*)&Mem);

			char mem[sizeof(_Kind_)];
			_Kind_ * copy = new ((void*)mem) _Kind_(Hero::Reference<_Kind_>::Template(kind));
			memset(Mem,0,sizeof(_Kind_));

			Kind->_Kind_::~_Kind_();
			Kind = new ((void*)Mem) _Kind_(*copy);
		}
		else
		{

			Kind = new ((void*)Mem) _Kind_(Hero::Reference<_Kind_>::Template(kind));
		}

		return *this;
	}

	typename Template<_Kind_>::Reference operator ()(void)	
	{
		if (!Kind) 
		{		
			Kind = new ((void*)Mem) _Kind_();

		}

		return Hero::Reference<_Kind_>::Template(*Kind);
	}

	typename Type<_Kind_>::Value Value() {return Hero::Reference<_Kind_>::Type((*this)());}
	typename Type<_Kind_>::Reference Reference() {return Hero::Reference<_Kind_>::Type((*this)());}
	typename Type<_Kind_>::Pointer Pointer() {return Hero::Pointer<_Kind_>::Type((*this)());}

public:

	bool Has()
	{
		return Kind != 0;
	}

	bool operator == (const Optional<_Kind_> & optional) 
	{	

		return Value() == optional.Value();
	}

	bool operator == (int value)
	{
		return (int)(bool)(operator void * ()) == value;
	}

	bool operator != (const Optional<_Kind_> & optional) {return !operator == (optional);}	
	bool operator != (int value) {return !operator == (value);}

	friend bool operator == (Optional<_Kind_> & left, Optional<_Kind_> & right) {return right.operator == (left);}
	friend bool operator != (Optional<_Kind_> & left, Optional<_Kind_> & right) {return !right.operator == (left);}

	friend bool operator == (bool left, Optional<_Kind_> & right) {return right == left;}
	friend bool operator != (bool left, Optional<_Kind_> & right) {return right != left;}

	operator void * () const
	{
		return (void*)((Optional*)this)->Has();
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

class Status
{
public:

	Status(int code):Code(code) {}

	int Code;

	operator bool () 
	{

		return Code == 0;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Status_, typename _Kind_>
class Result : Optional<_Kind_>
{
public:

	_Status_ Status;

	operator bool ()
	{
		return Status;
	}
};

template<>
struct Result<bool,int>
{
	bool Status;
	int Index;

	Result(bool value): 
		Status(value), Index(-1) 
	{}

	Result(bool value, int index):
		Status(value), Index(index)
	{}

	Result() :
		Status(false), Index(-1)
	{}

	operator void* ()
	{
		return (void*)Status;
	}

	bool operator() ()
	{
		return Status;
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

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

