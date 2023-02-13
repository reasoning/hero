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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4180)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct Template
{
	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_   ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Template<_Kind_&>
{
	typedef _Kind_ & Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_ & ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Template<_Kind_*>
{

	typedef _Kind_ *  Value;
	typedef _Kind_ ** Pointer;
	typedef _Kind_ *& Reference;

	typedef const _Kind_ *         ConstValue;
	typedef const _Kind_ * const * ConstPointer;
	typedef const _Kind_ * const & ConstReference;

	typedef _Kind_ * && MoveReference;
};

template <typename _Kind_>
struct Template<_Kind_*&>
{

	typedef _Kind_ *  Value;
	typedef _Kind_ ** Pointer;
	typedef _Kind_ *& Reference;

	typedef const _Kind_ *         ConstValue;
	typedef const _Kind_ * const * ConstPointer;
	typedef const _Kind_ * const & ConstReference;

	typedef _Kind_ * && MoveReference;
};

template <typename _Kind_>
struct Template<_Kind_&&>
{

	typedef _Kind_ && Value;
	typedef _Kind_ && Pointer;
	typedef _Kind_ && Reference;

	typedef const _Kind_ &&  ConstValue;
	typedef const _Kind_ &&  ConstPointer;
	typedef const _Kind_ &&  ConstReference;

	typedef _Kind_ && MoveReference;

};

template<>
struct Template<void*>
{
	typedef void * Value;
	typedef void ** Pointer;
	typedef void *& Reference;

	typedef const void * ConstValue;
	typedef const void * const * ConstPointer;
	typedef const void * const & ConstReference;

	typedef void * && MoveReference;
};

template<>
struct Template<void>
{
	typedef void * Value;
	typedef void ** Pointer;
	typedef void *&  Reference;

	typedef const void * ConstValue;
	typedef const void * const * ConstPointer;
	typedef const void * const & ConstReference;

	typedef void * && MoveReference;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct Type
{

	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_   ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Type<_Kind_&>
{

	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_   ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Type<_Kind_*>
{

	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_   ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Type<_Kind_*&>
{

	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_   ConstValue;
	typedef const _Kind_ * ConstPointer;
	typedef const _Kind_ & ConstReference;

	typedef _Kind_ && MoveReference;
};

template <typename _Kind_>
struct Type<_Kind_&&>
{
	typedef _Kind_   Value;
	typedef _Kind_ * Pointer;
	typedef _Kind_ & Reference;

	typedef const _Kind_    ConstValue;
	typedef const _Kind_ *  ConstPointer;
	typedef const _Kind_ &  ConstReference;

	typedef _Kind_ && MoveReference;
};

template<>
struct Type<void*>
{
	typedef void   Value;
	typedef void * Pointer;

	typedef void *& Reference;

	typedef const void   ConstValue;
	typedef const void * ConstPointer;
	typedef const void * const & ConstReference;

	typedef void * && MoveReference;
};

template<>
struct Type<void>
{
	typedef void   Value;
	typedef void * Pointer;
	typedef void *&  Reference;

	typedef const void   ConstValue;
	typedef const void * ConstPointer;
	typedef const void * const & ConstReference;

	typedef void * && MoveReference;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct Value
{
	static typename Hero::Type<_Kind_>::Value Type(typename Hero::Template<_Kind_>::ConstReference kind) {return (typename Hero::Template<_Kind_>::Reference)kind;};
	static typename Hero::Type<_Kind_>::Value Type(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Reference)*kind;};

	static typename Hero::Template<_Kind_>::Value Template(typename Hero::Template<_Kind_>::ConstReference kind) {return (typename Hero::Template<_Kind_>::Reference)kind;};
	static typename Hero::Template<_Kind_>::Value Template(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Reference)*kind;};
};

template <typename _Kind_>
struct Value<_Kind_&>
{
	static typename Hero::Type<_Kind_&>::Value Type(typename Hero::Template<_Kind_&>::ConstReference kind) {return (typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Type<_Kind_&>::Value Type(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Reference)*kind;}

	static typename Hero::Template<_Kind_&>::Value Template(typename Hero::Template<_Kind_&>::ConstReference kind) {return (typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Template<_Kind_&>::Value Template(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Reference)*kind;}
};

template <typename _Kind_>
struct Value<_Kind_*>
{
	static typename Hero::Type<_Kind_*>::Value Type(typename Hero::Template<_Kind_*>::ConstReference kind) {return *(typename Hero::Template<_Kind_*>::Reference)kind;}	
	static typename Hero::Type<_Kind_*>::Value Type(typename Hero::Template<_Kind_*>::ConstPointer kind) {return *(typename Hero::Template<_Kind_*>::Reference)*kind;}	

	static typename Hero::Template<_Kind_*>::Value Template(typename Hero::Template<_Kind_*>::ConstReference kind) {return (typename Hero::Template<_Kind_*>::Reference)kind;}	
	static typename Hero::Template<_Kind_*>::Value Template(typename Hero::Template<_Kind_*>::ConstPointer kind) {return (typename Hero::Template<_Kind_*>::Reference)*kind;}	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct RemoveReference
{
	typedef _Kind_ Type;
};

template <typename _Kind_>
struct RemoveReference<_Kind_&>
{
	typedef _Kind_ Type;
};

template <typename _Kind_>
struct RemoveReference<_Kind_&&>
{

	typedef _Kind_ && Type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
class RemovePointer
{
public:

	typedef _Kind_ Type;
};

template <typename _Kind_>
class RemovePointer<_Kind_&>
{
public:

	typedef _Kind_ &Type;
};

template <typename _Kind_>
class RemovePointer<_Kind_*>
{
public:

	typedef _Kind_ Type;
};

template <typename _Kind_>
class RemovePointer<_Kind_&&>
{
public:

	typedef _Kind_ && Type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct Reference
{
	static typename Hero::Type<_Kind_>::Reference Type(typename Hero::Template<_Kind_>::ConstReference kind) {return (typename Hero::Template<_Kind_>::Reference)kind;}
	static typename Hero::Type<_Kind_>::Reference Type(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Reference)*kind;}

	static typename Hero::Template<_Kind_>::Reference Template(typename Hero::Template<_Kind_>::ConstReference kind) {return (typename Hero::Template<_Kind_>::Reference)kind;}
	static typename Hero::Template<_Kind_>::Reference Template(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Reference)*kind;}
};

template <typename _Kind_>
struct Reference<_Kind_&>
{
	static typename Hero::Type<_Kind_&>::Reference Type(typename Hero::Template<_Kind_&>::ConstReference kind) {return (typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Type<_Kind_&>::Reference Type(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Reference)*kind;}

	static typename Hero::Template<_Kind_&>::Reference Template(typename Hero::Template<_Kind_&>::ConstReference kind) {return (typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Template<_Kind_&>::Reference Template(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Reference)*kind;}
};

template <typename _Kind_>
struct Reference<_Kind_*>
{
	static typename Hero::Type<_Kind_*>::Reference Type(typename Hero::Template<_Kind_*>::ConstReference kind) {return *(typename Hero::Template<_Kind_*>::Reference)kind;}	
	static typename Hero::Type<_Kind_*>::Reference Type(typename Hero::Template<_Kind_*>::ConstPointer kind) {return *(typename Hero::Template<_Kind_*>::Reference)*kind;}	

	static typename Hero::Template<_Kind_*>::Reference Template(typename Hero::Template<_Kind_*>::ConstReference kind) {return (typename Hero::Template<_Kind_*>::Reference)kind;}
	static typename Hero::Template<_Kind_*>::Reference Template(typename Hero::Template<_Kind_*>::ConstPointer kind) {return (typename Hero::Template<_Kind_*>::Reference)*kind;}
};

template <>
struct Reference<void*>
{	

	static Hero::Type<void*>::Reference Type(Hero::Template<void*>::ConstReference kind) {return (Hero::Template<void*>::Reference)kind;}	
	static Hero::Type<void*>::Reference Type(Hero::Template<void*>::ConstPointer kind) {return (Hero::Template<void*>::Reference)*kind;}	

	static Hero::Template<void*>::Reference Template(Hero::Template<void*>::ConstReference kind) {return (Hero::Template<void*>::Reference)kind;}
	static Hero::Template<void*>::Reference Template(Hero::Template<void*>::ConstPointer kind) {return (Hero::Template<void*>::Reference)*kind;}
};

template <>
struct Reference<void>
{
	static Hero::Type<void>::Reference Type(Hero::Template<void>::ConstReference kind) {return (Hero::Template<void>::Reference)kind;}	

	static Hero::Type<void>::Reference Type(Hero::Template<void>::ConstPointer kind) {return (Hero::Template<void>::Reference)*kind;}	

	static Hero::Template<void>::Reference Template(Hero::Template<void>::ConstReference kind) {return (Hero::Template<void>::Reference)kind;}
	static Hero::Template<void>::Reference Template(Hero::Template<void>::ConstPointer kind) {return (Hero::Template<void>::Reference)*kind;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename _Kind_>
struct Pointer
{
	static typename Hero::Type<_Kind_>::Pointer Type(typename Hero::Template<_Kind_>::ConstReference kind) {return (_Kind_*)&(char&)(typename Hero::Template<_Kind_>::Reference)kind;};
	static typename Hero::Type<_Kind_>::Pointer Type(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Pointer)kind;};

	static typename Hero::Template<_Kind_>::Pointer Template(typename Hero::Template<_Kind_>::ConstReference kind) {return (_Kind_*)&(char&)(typename Hero::Template<_Kind_>::Reference)kind;};
	static typename Hero::Template<_Kind_>::Pointer Template(typename Hero::Template<_Kind_>::ConstPointer kind) {return (typename Hero::Template<_Kind_>::Pointer)kind;};
};

template <typename _Kind_>
struct Pointer<_Kind_&>
{
	static typename Hero::Type<_Kind_&>::Pointer Type(typename Hero::Template<_Kind_&>::ConstReference kind) {return (_Kind_*)&(char&)(typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Type<_Kind_&>::Pointer Type(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Pointer)kind;};

	static typename Hero::Template<_Kind_&>::Pointer Template(typename Hero::Template<_Kind_&>::ConstReference kind) {return (_Kind_*)&(char&)(typename Hero::Template<_Kind_&>::Reference)kind;}
	static typename Hero::Template<_Kind_&>::Pointer Template(typename Hero::Template<_Kind_&>::ConstPointer kind) {return (typename Hero::Template<_Kind_&>::Pointer)kind;}
};

template <typename _Kind_>
struct Pointer<_Kind_*>
{
	static typename Hero::Type<_Kind_*>::Pointer Type(typename Hero::Template<_Kind_*>::ConstReference kind) {return (typename Hero::Template<_Kind_*>::Reference)kind;}
	static typename Hero::Type<_Kind_*>::Pointer Type(typename Hero::Template<_Kind_*>::ConstPointer kind) {return *(typename Hero::Template<_Kind_*>::Pointer)kind;}

	static typename Hero::Template<_Kind_*>::Pointer Template(typename Hero::Template<_Kind_*>::ConstReference kind) {return &(typename Hero::Template<_Kind_*>::Reference)kind;}
	static typename Hero::Template<_Kind_*>::Pointer Template(typename Hero::Template<_Kind_*>::ConstPointer kind) {return (typename Hero::Template<_Kind_*>::Pointer)kind;}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Types {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename _Kind_>
	struct Class
	{
	public:

		static bool Is()
		{
			return IsClass<_Kind_>(0);
		}

	private:

		template<typename Parameter>
		static bool IsClass(int Parameter::*)
		{
			return true;
		}

		template<typename Parameter>
		static bool IsClass(...)
		{
			return false;
		}

	};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename _Kind_>
	struct Is
	{

		static inline bool IsValue()		{return true;};
		static inline bool IsReference()	{return false;};
		static inline bool IsPointer()		{return false;};

		static inline bool IsClass()		{return Class<_Kind_>::Value;};
		static inline bool IsPrimitive()	{return !IsClass();};
	};

	template <typename _Kind_>
	struct Is<_Kind_&>
	{

		static inline bool IsValue()		{return false;};
		static inline bool IsReference()	{return true;};
		static inline bool IsPointer()		{return false;};

		static inline bool IsClass()		{return Class<_Kind_>::Value;};
		static inline bool IsPrimitive()	{return !IsClass();};

	};

	template <typename _Kind_>
	struct Is<_Kind_*>
	{

		static inline bool IsValue()		{return false;};
		static inline bool IsReference()	{return false;};
		static inline bool IsPointer()		{return true;};

		static inline bool IsClass()		{return Class<_Kind_>::Value;};
		static inline bool IsPrimitive()	{return !IsClass();};

	};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct Enum
	{

		template<bool, int _True_, int _False_>
		struct Or
		{
			enum Enum {Value = _False_};
		};	

		template<int _True_,int _False_>

		struct Or<true,_True_,_False_>
		{
			enum Enum {Value = _True_};
		};

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

	enum Values
	{
		TYPE_NONE = 0,

		TYPE_CHAR_SIGNED		= (1)<<1,
		TYPE_CHAR_UNSIGNED		= (1)<<2,
		TYPE_CHAR = TYPE_CHAR_SIGNED|TYPE_CHAR_UNSIGNED,

		TYPE_SHORT_SIGNED		= (1)<<3,
		TYPE_SHORT_UNSIGNED		= (1)<<4,
		TYPE_SHORT = TYPE_SHORT_SIGNED|TYPE_SHORT_UNSIGNED,

		TYPE_INT_SIGNED			= (1)<<5,
		TYPE_INT_UNSIGNED		= (1)<<6,
		TYPE_INT = TYPE_INT_SIGNED|TYPE_INT_UNSIGNED,

		TYPE_LONG_SIGNED		= (1)<<7,
		TYPE_LONG_UNSIGNED		= (1)<<8,
		TYPE_LONG = TYPE_LONG_SIGNED|TYPE_LONG_UNSIGNED,

		TYPE_LONGLONG_SIGNED	= (1)<<9,
		TYPE_LONGLONG_UNSIGNED	= (1)<<10,
		TYPE_LONGLONG = TYPE_LONGLONG_SIGNED|TYPE_LONGLONG_UNSIGNED,

		TYPE_FLOAT				= (1)<<11,
		TYPE_DOUBLE				= (1)<<12,

		TYPE_BOOL				= (1)<<13,

		TYPE_VOID				= (1)<<14,

		TYPE_CLASS				= (1)<<15,

		TYPE_VALUE				= (1)<<16,
		TYPE_REFERENCE			= (1)<<17,
		TYPE_POINTER			= (1)<<18,

		TYPE_ATTRIBUTES			= TYPE_VALUE|TYPE_REFERENCE|TYPE_POINTER,
	};

	template<typename _Kind_>
	struct IdClass
	{

		template<typename _Arg_>
		static char Member(void (_Arg_::*)());

		template<typename _Arg_>
		static int Member(...);

		#if defined(HERO_PLATFORM_APPLE) || defined(HERO_PLATFORM_LINUX)
		static const int Value = (sizeof(Member<_Kind_>(0)) != sizeof(int));
		#else

		#ifdef HERO_PLATFORM_WINDOWS
		enum Enum {Value = (sizeof(Member<_Kind_>(0)) != sizeof(int))};		
		#else
		static const int Value;
		#endif		
		#endif

	};

	#if !defined(HERO_PLATFORM_APPLE) && !defined(HERO_PLATFORM_LINUX) && !defined(HERO_PLATFORM_WINDOWS)
	template<typename _Kind_>
	const int IdClass<_Kind_>::Value = (sizeof(IdClass<_Kind_>::Member<_Kind_>(0)) != sizeof(int));
	#endif

	template<typename _Kind_>
	struct IdIsClass
	{
		enum Enum{Value = IdClass<_Kind_>::Value};
	};

	template<typename _Kind_> struct Id 
	{

		static const int Value=Enum::Or<IdIsClass<typename Type<_Kind_>::Value>::Value,TYPE_CLASS,TYPE_NONE>::Value;	

	};

	template<> struct Id<char> {static const int Value=TYPE_CHAR;};
	template<> struct Id<unsigned char> {static const int Value=TYPE_CHAR_UNSIGNED;};
	template<> struct Id<signed char> {static const int Value=TYPE_CHAR_UNSIGNED;};

	template<> struct Id<short> {static const int Value=TYPE_SHORT_SIGNED;};
	template<> struct Id<unsigned short> {static const int Value=TYPE_SHORT_UNSIGNED;};
	template<> struct Id<int> {static const int Value=TYPE_INT_SIGNED;};
	template<> struct Id<unsigned int> {static const int Value=TYPE_INT_UNSIGNED;};
	template<> struct Id<long> {static const int Value=TYPE_LONG_SIGNED;};
	template<> struct Id<unsigned long> {static const int Value=TYPE_LONG_UNSIGNED;};
	template<> struct Id<long long> {static const int Value=TYPE_LONGLONG_SIGNED;};
	template<> struct Id<unsigned long long> {static const int Value=TYPE_LONGLONG_UNSIGNED;};
	template<> struct Id<float> {static const int Value=TYPE_FLOAT;};
	template<> struct Id<double> {static const int Value=TYPE_DOUBLE;};
	template<> struct Id<bool> {static const int Value=TYPE_BOOL;};
	template<> struct Id<void> {static const int Value=TYPE_VOID;};

	struct Info
	{
		int Value;
		int Size;

		Info():Value(0),Size(0) {}
		Info(int value, int size):Value(value),Size(size) {}
	};

	template<typename _Kind_>
	Info Identify()
	{

		int value = Id<typename Type<_Kind_>::Value>::Value;
		value |= (Is<_Kind_>::IsReference()?TYPE_REFERENCE:TYPE_NONE)|(Is<_Kind_>::IsPointer()?TYPE_POINTER:TYPE_NONE);
		int size = sizeof(_Kind_);

		return Info(value,size);
	}

	template<>

	Info Identify<void>();

	bool Castable(Info from, Info to);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

