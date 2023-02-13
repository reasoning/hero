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

#include "hero/assert.h"
#include "hero/interface.h"
#include "hero/platform.h"
#include "hero/generic.h"
#include "hero/traits.h"

#pragma warning(disable:4522)

#if defined (HERO_PLATFORM_LINUX) || defined(HERO_PLATFORM_MINGW)
#include <new>
#else
#include <new.h>
#endif

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

class AnyPolicy
{
public:

	virtual void Del(void ** data) {}
	virtual void Copy(void ** data, void ** dest) {}
	virtual void Move(void ** data, void ** dest) {}
	virtual void Set(void * data, void ** dest) {}
	virtual void * Get(void ** data) {return 0;}

	virtual bool Cast(Types::Info & info) {return false;}

	template<typename _Kind_> 
	static AnyPolicy * Policy();

	static AnyPolicy * Empty();
};

template<typename _Kind_>
class AnyPolicyVoid : public AnyPolicy
{
public:

	void Del(void ** data) 
	{

	}

	void Copy(void ** data, void ** dest) 
	{

		_Kind_ ** kind = (_Kind_**)dest;
		*kind = *(_Kind_**)data;		

	}

	void Move(void ** data, void ** dest) {}

	void Set(void * data, void ** dest) 
	{

		_Kind_ ** kind = (_Kind_**)dest;
		*kind = *(_Kind_**)data;
	}

	void * Get(void ** data) 
	{		
		return (void*)data;		

	}

	virtual bool Cast(Types::Info & to) 
	{

		static Types::Info from = Types::Identify<_Kind_>();
		return Types::Castable(from,to);
	}
};

template<typename _Kind_>
class AnyPolicyPlace : public AnyPolicy
{
public:

	void Del(void ** data) 
	{
		_Kind_ * kind = (_Kind_*)data; 
		((_Kind_*)kind)->~_Kind_();
		*data = 0;
	}

	void Copy(void ** data, void ** dest) 
	{

		new ((char*)dest) _Kind_(*(_Kind_*)data);
	}

	void Move(void ** data, void ** dest) 
	{

	}

	void Set(void * data, void ** dest) 
	{
		new ((char*)dest) _Kind_(*(_Kind_*)data);		
	}

	void * Get(void ** data) 
	{
		return (void*)(_Kind_*)data;
	}

	virtual bool Cast(Types::Info & to) 
	{

		static Types::Info from = Types::Identify<_Kind_>();
		return Types::Castable(from,to);
	}
};

template<typename _Kind_>
class AnyPolicyNew : public AnyPolicy
{
public:

	void Del(void ** data) 
	{
		if (*data)
			delete (_Kind_*)*data;
		*data = 0;
	}

	void Copy(void ** data, void ** dest) 
	{
		*dest = (void*) new _Kind_(*(_Kind_*)*data);
	}

	void Move(void ** data, void ** dest) 
	{

		*(_Kind_*)*dest = *(_Kind_*)*data;
	}

	void Set(void * data, void ** dest) 
	{
		*dest = (void*) new _Kind_(*(_Kind_*)data);
	}

	void * Get(void ** data) 
	{
		return (void*)*data;
	}

	virtual bool Cast(Types::Info & to) 
	{

		static Types::Info from = Types::Identify<_Kind_>();
		return Types::Castable(from,to);
	}
};

class AnyPolicyEmpty : public AnyPolicy
{
public:

	void * Get(void ** data) 
	{

		return 0;
	}
};

template<typename _Kind_>
struct PolicySelector 
{
	typedef typename Traits::If< Traits::IsClass<_Kind_>::Value, AnyPolicyNew<_Kind_>, AnyPolicyPlace<_Kind_> >::Type Type;		
};

template<typename _Kind_>
struct PolicySelector<_Kind_ *> 
{
	typedef AnyPolicyVoid<_Kind_> Type;
};

template<>
struct PolicySelector<AnyPolicyEmpty>
{
	typedef AnyPolicyEmpty Type;		
};

template<typename _Kind_> 
AnyPolicy * AnyPolicy::Policy()
{
	static typename PolicySelector<_Kind_>::Type policy;
	return &policy;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Any
{
public:

	AnyPolicy * Base;
	void * Kind;

	Any():
		Base(AnyPolicy::Empty()),Kind(0)
	{

	}

	template <typename _Kind_>

	Any(const _Kind_& kind):
		Base(AnyPolicy::Empty()),Kind(0)
	{
		Assign(kind);
	}

	Any(const char * data):
		Base(AnyPolicy::Empty()),Kind(0)
	{
		Assign(data);
	}	

	~Any()
	{
		Base->Del(&Kind);
	}

	bool InstanceOf(const Any & other)
	{
		return Base == other.Base;
	}

	template<typename _Kind_>

	Any & Assign(const _Kind_& kind)
	{
		Reset();		
		Base = AnyPolicy::Policy<_Kind_>();
		Base->Set((void*)&kind,&Kind);
		return *this;

	}

	Any & Assign(const Any & any)
	{
		Reset();
		Base = any.Base;
		Base->Copy((void**)&any.Kind,&Kind);
		return *this;		
	}

	template <typename _Kind_>

	Any & operator = (const _Kind_& kind)
	{
		return Assign(kind);
	}

	Any & operator = (Any & any)
	{
		return Assign(any);
	}

	Any & operator = (const char * data)
	{
		return Assign(data);		
	}

	template<typename _Kind_>

	typename Template<_Kind_>::Reference Cast()
	{

		Types::Info info = Types::Identify<_Kind_>();

		if (Base != AnyPolicy::Policy<_Kind_>() && !Base->Cast(info))

			return Reference<_Kind_>::Template(Null<_Kind_>::Value());

		typename Template<_Kind_>::Reference ref = *(_Kind_*)Base->Get(&Kind);

		_Kind_ * kind = (_Kind_*)Base->Get(&Kind);
		return *kind;	
	}

	Any & Swap(Any & any)
	{

		return *this;
	}

	bool Empty()
	{
		return Base == AnyPolicy::Empty();
	}

	void Reset()
	{
		Base->Del(&Kind);
		Base = AnyPolicy::Empty();

		Kind = 0;
	}

};

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
