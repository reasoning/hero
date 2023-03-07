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
#include "hero/types.h"

#undef _Kind_
#undef _Arg_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Hero {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Object;

struct Empty
{
};

struct None 
{
};

struct True
{
};

struct False
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Traits {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct True
{
	enum Enum {Value = true};
};

struct False
{
	enum Enum {Value = false};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<bool>
struct Not { enum Enum {Value = false}; };

template <>
struct Not<false> { enum Enum {Value = true}; };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct Primitive
{
	static const bool Is() 
	{

		return false;
	}
};

template<> struct Primitive<char> {static bool Is() {return true;}};
template<> struct Primitive<unsigned char> {static bool Is() {return true;}};
template<> struct Primitive<short> {static bool Is() {return true;}};
template<> struct Primitive<unsigned short> {static bool Is() {return true;}};
template<> struct Primitive<int> {static bool Is() {return true;}};
template<> struct Primitive<unsigned int> {static bool Is() {return true;}};
template<> struct Primitive<long> {static bool Is() {return true;}};
template<> struct Primitive<unsigned long> {static bool Is() {return true;}};
template<> struct Primitive<long long> {static bool Is() {return true;}};
template<> struct Primitive<unsigned long long> {static bool Is() {return true;}};
template<> struct Primitive<float> {static bool Is() {return true;}};
template<> struct Primitive<double> {static bool Is() {return true;}};
template<> struct Primitive<bool> {static bool Is() {return true;}};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<bool _Bool_, typename _True_, typename _False_>
struct If 
{ typedef Hero::None Type; };

template<typename _True_, typename _False_>
struct If<true,_True_,_False_> 
{ typedef _True_ Type; };

template<typename _True_, typename _False_>
struct If<false,_True_,_False_>
{ typedef _False_ Type; };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <bool _Bool_, class _Type_ = void>
struct Enable 
{

};

template <class _Type_>
struct Enable<true, _Type_> 
{

	typedef _Type_ Type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<int _Int_, typename _Case1_, typename _Case2_=None, typename _Case3_=None, typename _Case4_=None, typename _Case5_=None>
struct Switch 
{ typedef Hero::None Type; };

template<typename _Case1_>
struct Switch<1,_Case1_> 
{ typedef _Case1_ Type; };

template<typename _Case1_, typename _Case2_>
struct Switch<2,_Case1_,_Case2_> 
{ typedef _Case2_ Type; };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Left_, typename _Right_>
struct Equals { enum Enum { Value = false};};

template<typename _Left_>
struct Equals<_Left_,_Left_> { enum Enum { Value = true};};

template<typename _Kind_>
struct Empty { enum Enum { Value = false}; };

template<>
struct Empty<Hero::Empty> { enum Enum { Value = true}; };

template<typename _Kind_>
struct None { enum Enum { Value = false}; };

template<>
struct None<Hero::None> { enum Enum { Value = true}; };

template<bool _Bool1_, bool _Bool2_, bool _Bool3_=true, bool _Bool4_=true, bool _Bool5_=true>
struct And { enum Enum {Value = false}; };

template<>
struct And<true, true, true, true, true>
{ enum Enum {Value = true}; };

template<bool _Bool1_, bool _Bool2_, bool _Bool3_=false, bool _Bool4_=false, bool _Bool5_=false>
struct Or { enum Enum {Value = true}; };

template<>
struct Or<false, false, false, false, false>
{ enum Enum {Value = false}; };

template<typename _Kind_, typename _To_>
struct Convertible
{

	static typename Template<_Kind_>::Reference Kind();
	static char Conversion(typename Template<_To_>::ConstReference to);
	static int Conversion(...);
	enum Enum {Value = (sizeof(Conversion(Kind())) != sizeof(int))};

};

template<typename _Kind_, typename _To_>
struct Convertible<_Kind_,_To_*>
{
	static typename Template<_Kind_>::Reference Kind();
	static char Conversion(_To_ * to);
	static int Conversion(...);
	enum Enum {Value = (sizeof(Conversion(Kind())) != sizeof(int))};
};

template<typename _Kind_>
struct Convertible<_Kind_,void>
{
	enum Enum {Value = false};
};

template<typename _To_>
struct Convertible<void,_To_>
{
	enum Enum {Value = false};
};

template<typename _Super_, typename _Sub_>
struct Superclass
{
	enum Enum {Value = Convertible<typename Type<_Super_>::ConstPointer,typename Type<_Sub_>::ConstPointer>::Value};
};

template<typename _Sub_, typename _Super_>
struct Subclass
{
	enum Enum {Value = Convertible<typename Type<_Super_>::ConstPointer,typename Type<_Sub_>::ConstPointer>::Value};
};

template<typename _Kind_, typename _To_>
struct Castable
{
	static typename Type<_Kind_>::Pointer Kind();
	static char Cast(typename Type<_To_>::Pointer to);
	static int Cast(...);
	enum Enum {Value = (sizeof(Cast(Kind())) != sizeof(int))};
};

template<typename _Kind_>
struct Castable<_Kind_,void> 
{
	enum Enum {Value = false};
};

template<typename _To_>
struct Castable<void,_To_> 
{
	enum Enum {Value = false};
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

template<typename _Kind_>
struct ReturnOf
{

};

template<typename _Kind_>
struct TypeOf
{
	typedef typename Template<_Kind_>::Value Type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct TypeOf<_Kind_ (*)()>
{

	typedef _Kind_ Type;
};

template<typename _Return_, typename _Class_>

struct TypeOf<_Return_ (_Class_::*)(int)>

{

	typedef _Return_ Type;
};

template<typename _Return_>
struct TypeOf<_Return_ (*)(int)>
{

	typedef _Return_ Type;
};

template<typename _Return_, typename _Arg_>
struct TypeOf<_Return_ (*)(_Arg_,...)>
{

	typedef _Return_ Type;
};

template<typename _Return_, typename _Arg1_, typename _Arg2_>
struct TypeOf<_Return_ (*)(_Arg1_,_Arg2_,...)>
{

	typedef _Return_ Type;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct IsArray
{
	enum Enum {Value = false};
};

template<typename _Kind_, int _Size_>
struct IsArray<_Kind_[_Size_]>
{
	enum Enum {Value = true};
};

template<typename _Kind_>
struct IsArray<_Kind_[]>
{
	enum Enum {Value = true};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct Class
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
const int Class<_Kind_>::Value = (sizeof(Class<_Kind_>::Member<_Kind_>(0)) != sizeof(int));
#endif

template<typename _Kind_>
struct IsClass
{
	enum Enum{Value = Class<_Kind_>::Value};
};

template<typename _Kind_>
struct Pointer
{
	enum Enum{Value = false};
};

template<typename _Kind_>
struct Pointer<_Kind_*>
{
	enum Enum{Value = true};
};

template<typename _Kind_>
struct IsPointer
{
	enum Enum{Value = Pointer<_Kind_>::Value};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct FunctionPointer
{

	template<typename _Arg_>

	static short Func(void (*)());

	template<typename _Arg_>
	static int Func(...);

	enum Enum {Value = (sizeof(Func<_Kind_>(0)) == sizeof(short))};	
};

template<typename _Kind_>
struct MemberFunctionPointer
{

	template<typename _Return_, typename _Arg_>
	#ifdef HERO_PLATFORM_LINUX
	static char Func( _Return_ (_Arg_::*Member)() );
	#else
	static char Func(typename  _Return_ (_Arg_::*Member)() );
	#endif

	template<typename _Arg_>
	static int Func(...);

	enum Enum {Value = (sizeof(Func<_Kind_>(0)) == sizeof(char))};	

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct MemberPointer
{

	template<typename _Arg_>		

	static char Member(_Arg_ *);

	template<typename _Arg_>
	static short Member(void (* (_Arg_))());

	template<typename _Arg_>
	static int Member(...);

	#if defined(HERO_PLATFORM_APPLE) || defined(HERO_PLATFORM_LINUX)
	static const int Value = (sizeof(Member<_Kind_>(0)) == sizeof(char));
	#else		
	#ifdef HERO_PLATFORM_WINDOWS
	enum Enum {Value = (sizeof(Member<_Kind_>(0)) == sizeof(char))};		
	#else
	static const int Value;
	#endif		
	#endif

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct Reference
{
	enum Enum{Value = false};
};

template<typename _Kind_>
struct Reference<_Kind_&>
{
	enum Enum{Value = true};
};

template<typename _Kind_>
struct IsReference
{
	enum Enum {Value = Reference<_Kind_>::Value};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Left_, typename _Right_>
struct IsType
{
	enum Enum {Value = Traits::Equals<_Left_,_Right_>::Value};
};

template<typename _Kind_>
struct Intrinsic
{

	enum Enum {Value = (IsType<char,_Kind_>::Value || 
						IsType<unsigned char,_Kind_>::Value || IsType<signed char,_Kind_>::Value ||
						IsType<short,_Kind_>::Value || IsType<unsigned short,_Kind_>::Value ||
						IsType<int,_Kind_>::Value || IsType<unsigned int,_Kind_>::Value ||
						IsType<long,_Kind_>::Value || IsType<unsigned long,_Kind_>::Value ||
						IsType<long long,_Kind_>::Value || IsType<unsigned long long,_Kind_>::Value ||
						IsType<float,_Kind_>::Value ||
						IsType<double,_Kind_>::Value ||
						IsType<bool,_Kind_>::Value )};
};

template<typename _Kind_>
struct IsIntrinsic
{
	enum Enum {Value = Intrinsic<_Kind_>::Value};
};

template<typename _Kind_>
struct IsReal
{
	enum Enum {Value = IsType<float,_Kind_>::Value || IsType<double,_Kind_>::Value};
};

template<typename _Kind_>
struct IsInteger
{
	enum Enum {Value = (Intrinsic<_Kind_>::Value && !IsReal<_Kind_>::Value)};
};

template<typename _Kind_>
struct IsPrimitive
{
	enum Enum {Value = !Class<_Kind_>::Value};

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IS_TYPE(NAME,TYPE) \
template<typename _Kind_> struct NAME { enum Enum {Value = false};}; \
template<typename _Kind_> struct NAME<TYPE> { enum Enum {Value = true};}; \
template<typename _Kind_> struct Is##NAME { enum Enum {Value = NAME<_Kind_>::Value};}; \

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HERO_USING_STD

template<typename _Kind_,
	typename=typename _Kind_::pointer,
	typename=typename _Kind_::element_type,
	typename=typename _Kind_::deleter_type>
struct Unique
{
};

template<typename _Kind_>
struct IsUnique
{
	template<typename _Unique_>
	static char Func(typename Unique<_Unique_>::pointer *);

	template<typename _Unique_>
	static long Func(...);

	#if defined(HERO_PLATFORM_APPLE) || defined(HERO_PLATFORM_LINUX)
	static const int Value = (sizeof(Func<_Kind_>(0)) == sizeof(char));
	#else		
	#ifdef HERO_PLATFORM_WINDOWS
	enum Enum {Value = (sizeof(Func<_Kind_>(0)) == sizeof(char))};		
	#else
	static const int Value;
	#endif	
	#endif
};

template<typename _Kind_>
struct IsIterator
{

	enum Enum {Value = (IsClass<_Kind_>::Value || IsPointer<_Kind_>::Value)};
};

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct IsVoid
{
	enum Enum {Value = false};
};

template<>
struct IsVoid<void>
{
	enum Enum {Value = true};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct IsScalar
{

	enum Enum {Value = (IsIntrinsic<_Kind_>::Value || IsPointer<_Kind_>::Value)};
};

template<typename _From_, typename _To_>
struct IsAssignable
{

};

template<typename _Kind_>
struct IsCopyable : IsAssignable<_Kind_,_Kind_>
{

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct IsMovable
{

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct Const
{
	enum Enum {Value = false};
};	

template<typename _Kind_>
struct Const<const _Kind_>
{
	enum Enum {Value = true};
};	

template<typename _Kind_>
struct IsConst
{
	enum Enum {Value = Const<_Kind_>::Value};
};	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename _Kind_>
struct PointerOrPrimitive
{

	enum {Value = Traits::Or<Traits::Pointer<_Kind_>::Value,Traits::Intrinsic<_Kind_>::Value>::Value};	
};

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

